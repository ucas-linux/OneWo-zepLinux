# XIP (Execute-In-Place) 支持

## 概述

XIP允许代码直接从Flash执行，无需复制到RAM，节省宝贵的RAM资源。这对于RAM受限的Cortex-M3系统至关重要。

## 优势

- **节省RAM**：代码段保留在Flash中，仅数据段占用RAM
- **启动更快**：无需复制代码到RAM
- **支持更大程序**：Flash通常比RAM大得多

## 限制

- **性能**：Flash访问比RAM慢（通常需要等待状态）
- **只读**：代码段不能自修改
- **对齐要求**：Flash地址必须满足MPU对齐要求

## 内存布局

```
Flash (0x08000000):
├── .text段 (XIP)      ← 直接执行
├── .rodata段 (XIP)    ← 直接读取
└── ELF文件数据

RAM (0x20000000):
├── .data段            ← 从Flash复制
├── .bss段             ← 清零
├── heap
└── stack
```

## 实现

```c
#ifdef CONFIG_ELF_LOADER_XIP

typedef struct {
    bool xip_enabled;
    uint32_t flash_base;      // Flash起始地址
    uint32_t flash_size;      // Flash大小
    uint32_t ram_base;        // RAM起始地址
    uint32_t ram_size;        // RAM大小
} xip_config_t;

static xip_config_t g_xip_config = {
    .xip_enabled = true,
    .flash_base = CONFIG_FLASH_BASE_ADDRESS,
    .flash_size = CONFIG_FLASH_SIZE * 1024,
    .ram_base = CONFIG_SRAM_BASE_ADDRESS,
    .ram_size = CONFIG_SRAM_SIZE * 1024,
};

// 判断地址是否在Flash中
static bool is_flash_address(uint32_t addr)
{
    return addr >= g_xip_config.flash_base &&
           addr < g_xip_config.flash_base + g_xip_config.flash_size;
}

// XIP加载策略
static int elf_load_segment_xip(elf_handle_t *handle, int seg_idx)
{
    uint32_t flags = handle->segments[seg_idx].flags;
    uint32_t file_offset = handle->segments[seg_idx].offset;
    uint32_t file_size = handle->segments[seg_idx].filesz;
    uint32_t mem_size = handle->segments[seg_idx].memsz;
    uint32_t flash_addr = (uint32_t)(handle->elf_data + file_offset);

    // 代码段和只读数据段：XIP
    if ((flags & PF_X) || (!(flags & PF_W) && file_size > 0)) {
        // 检查是否在Flash中
        if (!is_flash_address(flash_addr)) {
            ELF_ERROR("XIP segment not in flash: 0x%08x\n", flash_addr);
            return -EINVAL;
        }

        // 直接使用Flash地址
        handle->segments[seg_idx].load_addr = flash_addr;

        ELF_DEBUG("Segment %d: XIP from flash 0x%08x (%u bytes)\n",
                  seg_idx, flash_addr, file_size);

        return 0;
    }

    // 可写段：必须复制到RAM
    void *ram_addr = k_malloc(mem_size);
    if (!ram_addr) {
        ELF_ERROR("Failed to allocate %u bytes for writable segment\n", mem_size);
        return -ENOMEM;
    }

    // 复制初始化数据
    if (file_size > 0) {
        memcpy(ram_addr, handle->elf_data + file_offset, file_size);
    }

    // 清零BSS部分
    if (mem_size > file_size) {
        memset(ram_addr + file_size, 0, mem_size - file_size);
    }

    handle->segments[seg_idx].load_addr = (uint32_t)ram_addr;
    handle->segments[seg_idx].in_ram = true;

    ELF_DEBUG("Segment %d: RAM copy at 0x%08x (%u bytes)\n",
              seg_idx, (uint32_t)ram_addr, mem_size);

    return 0;
}

#endif // CONFIG_ELF_LOADER_XIP
```

## MPU配置（XIP）

```c
static int elf_setup_mpu_xip(elf_handle_t *handle)
{
#ifdef CONFIG_ARM_MPU
    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t base = handle->segments[i].load_addr;
        uint32_t size = handle->segments[i].memsz;
        uint32_t flags = handle->segments[i].flags;

        struct arm_mpu_region region;
        region.base = base;
        region.size = size;

        // 根据段类型设置权限
        if (flags & PF_X) {
            // 代码段：只读+可执行
            region.attr.rw = MPU_REGION_RO;
            region.attr.xn = 0;
        } else if (flags & PF_W) {
            // 数据段：读写+不可执行
            region.attr.rw = MPU_REGION_RW;
            region.attr.xn = 1;
        } else {
            // 只读数据段：只读+不可执行
            region.attr.rw = MPU_REGION_RO;
            region.attr.xn = 1;
        }

        // Flash段使用缓存
        if (is_flash_address(base)) {
            region.attr.cacheable = 1;
            region.attr.bufferable = 1;
        } else {
            // RAM段
            region.attr.cacheable = 1;
            region.attr.bufferable = 0;
        }

        arm_mpu_config_region(&region);

        ELF_DEBUG("MPU region %d: base=0x%08x size=%u flags=0x%x\n",
                  i, base, size, flags);
    }
#endif

    return 0;
}
```

## Flash对齐

```c
// 检查Flash对齐
static int elf_check_flash_alignment(elf_handle_t *handle)
{
    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t flags = handle->segments[i].flags;

        // 只检查XIP段
        if ((flags & PF_X) || !(flags & PF_W)) {
            uint32_t flash_addr = (uint32_t)(handle->elf_data +
                                             handle->segments[i].offset);

            // MPU要求地址对齐到区域大小
            // 对于小区域，至少32字节对齐
            if (flash_addr & 0x1F) {
                ELF_ERROR("Flash segment not aligned: 0x%08x\n", flash_addr);
                return -EINVAL;
            }
        }
    }

    return 0;
}
```

## 性能优化

### 1. Flash预取

```c
static void elf_enable_flash_prefetch(void)
{
#ifdef STM32
    // 启用Flash预取缓冲
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    // 设置等待状态（根据系统时钟）
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2WS;
#endif
}
```

### 2. 指令缓存

```c
static void elf_enable_icache(void)
{
#ifdef CONFIG_ARM_MPU
    // 启用指令缓存
    SCB_EnableICache();

    // 启用数据缓存
    SCB_EnableDCache();
#endif
}
```

## 调试支持

```c
void elf_dump_xip_info(elf_handle_t *handle)
{
    printk("=== XIP Information ===\n");

    size_t flash_usage = 0;
    size_t ram_usage = 0;

    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t addr = handle->segments[i].load_addr;
        uint32_t size = handle->segments[i].memsz;
        bool in_flash = is_flash_address(addr);

        printk("Segment %d: 0x%08x (%u bytes) - %s\n",
               i, addr, size, in_flash ? "FLASH" : "RAM");

        if (in_flash) {
            flash_usage += size;
        } else {
            ram_usage += size;
        }
    }

    printk("Total Flash usage: %zu bytes\n", flash_usage);
    printk("Total RAM usage: %zu bytes\n", ram_usage);
    printk("RAM saved by XIP: %zu bytes\n", flash_usage);
}
```

## 配置选项

```kconfig
config ELF_LOADER_XIP
    bool "Execute-in-place from flash"
    depends on ELF_LOADER
    default y
    help
      Allow executing code directly from flash without copying to RAM.

config FLASH_BASE_ADDRESS
    hex "Flash base address"
    depends on ELF_LOADER_XIP
    default 0x08000000
    help
      Base address of flash memory.

config FLASH_SIZE
    int "Flash size (KB)"
    depends on ELF_LOADER_XIP
    default 512
    help
      Size of flash memory in kilobytes.

config ELF_XIP_ALIGNMENT
    int "XIP segment alignment"
    depends on ELF_LOADER_XIP
    default 32
    help
      Required alignment for XIP segments (must match MPU requirements).
```

## 最佳实践

1. **链接脚本**：确保代码段对齐到Flash边界
2. **编译选项**：使用`-ffunction-sections -fdata-sections`优化
3. **只读数据**：将常量数据放在`.rodata`段
4. **性能关键代码**：考虑复制到RAM执行
5. **Flash寿命**：避免频繁重写Flash
