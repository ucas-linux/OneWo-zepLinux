# ELF加载器实现

## 加载流程

```c
int elf_load(elf_handle_t *handle, int *entry_fd)
{
    int ret;

    // 1. 分配内存区域
    ret = elf_allocate_memory(handle);
    if (ret < 0) {
        return ret;
    }

    // 2. 加载段到内存
    ret = elf_load_segments(handle);
    if (ret < 0) {
        goto cleanup;
    }

    // 3. 处理重定位
    ret = elf_apply_relocations(handle);
    if (ret < 0) {
        goto cleanup;
    }

    // 4. 配置MPU保护
    ret = elf_setup_mpu(handle);
    if (ret < 0) {
        goto cleanup;
    }

    // 5. 清零BSS段
    ret = elf_clear_bss(handle);
    if (ret < 0) {
        goto cleanup;
    }

    return 0;

cleanup:
    elf_free_memory(handle);
    return ret;
}
```

## 1. 内存分配

```c
typedef struct {
    void *code_base;      // 代码段基址
    size_t code_size;
    void *rodata_base;    // 只读数据基址
    size_t rodata_size;
    void *data_base;      // 数据段基址
    size_t data_size;
    void *bss_base;       // BSS段基址
    size_t bss_size;
    void *heap_base;      // 堆基址
    size_t heap_size;
    void *stack_base;     // 栈基址
    size_t stack_size;
} elf_memory_layout_t;

static int elf_allocate_memory(elf_handle_t *handle)
{
    elf_memory_layout_t *layout = &handle->memory;
    size_t total_size = 0;

    // 计算每个段所需的内存
    for (int i = 0; i < handle->num_segments; i++) {
        total_size += ALIGN_UP(handle->segments[i].memsz, 4096);
    }

    // 添加堆和栈空间
    total_size += CONFIG_PROCESS_DEFAULT_HEAP;
    total_size += CONFIG_PROCESS_DEFAULT_STACK;

    ELF_DEBUG("Total memory required: %zu bytes\n", total_size);

    // 分配连续内存块
    void *base = k_malloc(total_size);
    if (!base) {
        ELF_ERROR("Failed to allocate %zu bytes\n", total_size);
        return -ENOMEM;
    }

    handle->memory_base = base;
    handle->memory_size = total_size;

    // 分配各段
    void *current = base;

    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t flags = handle->segments[i].flags;
        size_t size = ALIGN_UP(handle->segments[i].memsz, 4096);

        if (flags & PF_X) {
            // 代码段
            layout->code_base = current;
            layout->code_size = size;
        } else if (flags & PF_W) {
            // 数据段
            layout->data_base = current;
            layout->data_size = size;
        } else {
            // 只读数据段
            layout->rodata_base = current;
            layout->rodata_size = size;
        }

        handle->segments[i].load_addr = (uint32_t)current;
        current += size;
    }

    // 分配堆
    layout->heap_base = current;
    layout->heap_size = CONFIG_PROCESS_DEFAULT_HEAP;
    current += layout->heap_size;

    // 分配栈
    layout->stack_base = current;
    layout->stack_size = CONFIG_PROCESS_DEFAULT_STACK;

    ELF_DEBUG("Memory layout:\n");
    ELF_DEBUG("  Code:   %p - %p (%zu bytes)\n",
              layout->code_base,
              layout->code_base + layout->code_size,
              layout->code_size);
    ELF_DEBUG("  RO Data: %p - %p (%zu bytes)\n",
              layout->rodata_base,
              layout->rodata_base + layout->rodata_size,
              layout->rodata_size);
    ELF_DEBUG("  Data:   %p - %p (%zu bytes)\n",
              layout->data_base,
              layout->data_base + layout->data_size,
              layout->data_size);
    ELF_DEBUG("  Heap:   %p - %p (%zu bytes)\n",
              layout->heap_base,
              layout->heap_base + layout->heap_size,
              layout->heap_size);
    ELF_DEBUG("  Stack:  %p - %p (%zu bytes)\n",
              layout->stack_base,
              layout->stack_base + layout->stack_size,
              layout->stack_size);

    return 0;
}
```

## 2. 加载段

```c
static int elf_load_segments(elf_handle_t *handle)
{
    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t load_addr = handle->segments[i].load_addr;
        uint32_t file_offset = handle->segments[i].offset;
        uint32_t file_size = handle->segments[i].filesz;
        uint32_t mem_size = handle->segments[i].memsz;

        ELF_DEBUG("Loading segment %d: vaddr=0x%08x -> load_addr=0x%08x\n",
                  i, handle->segments[i].vaddr, load_addr);

        // 复制文件数据到内存
        if (file_size > 0) {
            memcpy((void *)load_addr,
                   handle->elf_data + file_offset,
                   file_size);
        }

        // BSS部分清零（memsz > filesz）
        if (mem_size > file_size) {
            memset((void *)(load_addr + file_size), 0, mem_size - file_size);
        }
    }

    return 0;
}
```

## 3. XIP支持（从Flash执行）

```c
#ifdef CONFIG_ELF_LOADER_XIP

static int elf_load_segments_xip(elf_handle_t *handle)
{
    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t flags = handle->segments[i].flags;
        uint32_t file_offset = handle->segments[i].offset;
        uint32_t file_size = handle->segments[i].filesz;
        uint32_t mem_size = handle->segments[i].memsz;

        // 代码段和只读数据段可以直接从Flash执行
        if ((flags & PF_X) || !(flags & PF_W)) {
            // 直接使用Flash地址
            handle->segments[i].load_addr =
                (uint32_t)(handle->elf_data + file_offset);

            ELF_DEBUG("Segment %d: XIP from 0x%08x\n",
                      i, handle->segments[i].load_addr);
        } else {
            // 可写段必须复制到RAM
            void *ram_addr = k_malloc(mem_size);
            if (!ram_addr) {
                return -ENOMEM;
            }

            if (file_size > 0) {
                memcpy(ram_addr, handle->elf_data + file_offset, file_size);
            }

            if (mem_size > file_size) {
                memset(ram_addr + file_size, 0, mem_size - file_size);
            }

            handle->segments[i].load_addr = (uint32_t)ram_addr;

            ELF_DEBUG("Segment %d: RAM copy at 0x%08x\n",
                      i, handle->segments[i].load_addr);
        }
    }

    return 0;
}

#endif
```

## 4. MPU配置

```c
static int elf_setup_mpu(elf_handle_t *handle)
{
    elf_memory_layout_t *layout = &handle->memory;

#ifdef CONFIG_ARM_MPU
    // 配置代码段（只读+执行）
    if (layout->code_base && layout->code_size > 0) {
        struct arm_mpu_region code_region = {
            .base = (uint32_t)layout->code_base,
            .size = layout->code_size,
            .attr = {
                .rw = MPU_REGION_RO,
                .xn = 0,  // 可执行
            }
        };
        arm_mpu_config_region(&code_region);
    }

    // 配置只读数据段（只读）
    if (layout->rodata_base && layout->rodata_size > 0) {
        struct arm_mpu_region rodata_region = {
            .base = (uint32_t)layout->rodata_base,
            .size = layout->rodata_size,
            .attr = {
                .rw = MPU_REGION_RO,
                .xn = 1,  // 不可执行
            }
        };
        arm_mpu_config_region(&rodata_region);
    }

    // 配置数据段（读写）
    if (layout->data_base && layout->data_size > 0) {
        struct arm_mpu_region data_region = {
            .base = (uint32_t)layout->data_base,
            .size = layout->data_size,
            .attr = {
                .rw = MPU_REGION_RW,
                .xn = 1,  // 不可执行
            }
        };
        arm_mpu_config_region(&data_region);
    }

    // 配置堆（读写）
    if (layout->heap_base && layout->heap_size > 0) {
        struct arm_mpu_region heap_region = {
            .base = (uint32_t)layout->heap_base,
            .size = layout->heap_size,
            .attr = {
                .rw = MPU_REGION_RW,
                .xn = 1,
            }
        };
        arm_mpu_config_region(&heap_region);
    }

    // 配置栈（读写）
    if (layout->stack_base && layout->stack_size > 0) {
        struct arm_mpu_region stack_region = {
            .base = (uint32_t)layout->stack_base,
            .size = layout->stack_size,
            .attr = {
                .rw = MPU_REGION_RW,
                .xn = 1,
            }
        };
        arm_mpu_config_region(&stack_region);
    }

    ELF_DEBUG("MPU configured for process\n");
#endif

    return 0;
}
```

## 5. 清零BSS

```c
static int elf_clear_bss(elf_handle_t *handle)
{
    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t file_size = handle->segments[i].filesz;
        uint32_t mem_size = handle->segments[i].memsz;

        if (mem_size > file_size) {
            uint32_t bss_start = handle->segments[i].load_addr + file_size;
            uint32_t bss_size = mem_size - file_size;

            ELF_DEBUG("Clearing BSS: 0x%08x - 0x%08x (%u bytes)\n",
                      bss_start, bss_start + bss_size, bss_size);

            memset((void *)bss_start, 0, bss_size);
        }
    }

    return 0;
}
```

## 6. 内存释放

```c
static void elf_free_memory(elf_handle_t *handle)
{
    if (handle->memory_base) {
        k_free(handle->memory_base);
        handle->memory_base = NULL;
    }

    handle->memory_size = 0;
}
```

## 7. 地址转换

加载后，ELF中的虚拟地址需要转换为实际加载地址。

```c
static uint32_t elf_vaddr_to_load_addr(elf_handle_t *handle, uint32_t vaddr)
{
    for (int i = 0; i < handle->num_segments; i++) {
        uint32_t seg_vaddr = handle->segments[i].vaddr;
        uint32_t seg_size = handle->segments[i].memsz;

        if (vaddr >= seg_vaddr && vaddr < seg_vaddr + seg_size) {
            uint32_t offset = vaddr - seg_vaddr;
            return handle->segments[i].load_addr + offset;
        }
    }

    return 0;  // 地址不在任何段中
}
```

## 性能优化

### 1. 延迟加载

```c
// 仅加载必要的段，其他段按需加载
static int elf_load_on_demand(elf_handle_t *handle)
{
    // 只加载代码段和初始化数据
    for (int i = 0; i < handle->num_segments; i++) {
        if (handle->segments[i].flags & (PF_X | PF_W)) {
            // 加载这个段
        } else {
            // 标记为延迟加载
            handle->segments[i].lazy = true;
        }
    }

    return 0;
}
```

### 2. 内存对齐优化

```c
// 确保段对齐到缓存行边界
#define CACHE_LINE_SIZE 32

static size_t align_to_cache_line(size_t size)
{
    return (size + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1);
}
```

## 错误处理

```c
static const char *elf_load_error_str(int error)
{
    switch (error) {
    case -ENOMEM:
        return "Out of memory";
    case -ENOEXEC:
        return "Invalid executable format";
    case -EINVAL:
        return "Invalid argument";
    case -EIO:
        return "I/O error";
    default:
        return "Unknown error";
    }
}
```
