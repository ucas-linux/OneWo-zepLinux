# ELF重定位处理

## 概述

即使是静态链接的ELF，如果不是位置无关代码（PIC），也需要重定位。重定位修正代码和数据中的绝对地址引用。

## ARM重定位类型

```c
// ARM重定位类型定义
#define R_ARM_NONE              0   // 无操作
#define R_ARM_PC24              1   // 已废弃
#define R_ARM_ABS32             2   // 直接32位地址
#define R_ARM_REL32             3   // PC相对32位地址
#define R_ARM_CALL              28  // BL/BLX指令
#define R_ARM_JUMP24            29  // B/BL指令
#define R_ARM_TARGET1           38  // 数据重定位
#define R_ARM_PREL31            42  // PC相对31位
#define R_ARM_MOVW_ABS_NC       43  // MOVW指令低16位
#define R_ARM_MOVT_ABS          44  // MOVT指令高16位
#define R_ARM_THM_CALL          10  // Thumb BL
#define R_ARM_THM_JUMP24        30  // Thumb B
#define R_ARM_THM_MOVW_ABS_NC   47  // Thumb MOVW
#define R_ARM_THM_MOVT_ABS      48  // Thumb MOVT
```

## 重定位处理流程

```c
int elf_apply_relocations(elf_handle_t *handle)
{
    if (!handle->rel || handle->num_rel == 0) {
        ELF_DEBUG("No relocations to apply\n");
        return 0;
    }

    ELF_DEBUG("Applying %d relocations\n", handle->num_rel);

    for (int i = 0; i < handle->num_rel; i++) {
        Elf32_Rel *rel = &handle->rel[i];
        uint32_t type = ELF32_R_TYPE(rel->r_info);
        uint32_t sym_idx = ELF32_R_SYM(rel->r_info);

        // 计算重定位位置的实际地址
        uint32_t reloc_addr = elf_vaddr_to_load_addr(handle, rel->r_offset);
        if (reloc_addr == 0) {
            ELF_ERROR("Invalid relocation offset: 0x%08x\n", rel->r_offset);
            return -EINVAL;
        }

        // 获取符号值
        uint32_t sym_value = 0;
        if (sym_idx != 0) {
            if (sym_idx >= handle->num_syms) {
                ELF_ERROR("Invalid symbol index: %d\n", sym_idx);
                return -EINVAL;
            }
            sym_value = handle->symtab[sym_idx].st_value;
            sym_value = elf_vaddr_to_load_addr(handle, sym_value);
        }

        // 应用重定位
        int ret = elf_apply_relocation(handle, type, reloc_addr, sym_value);
        if (ret < 0) {
            ELF_ERROR("Failed to apply relocation %d (type=%d)\n", i, type);
            return ret;
        }
    }

    return 0;
}
```

## 重定位类型处理

```c
static int elf_apply_relocation(elf_handle_t *handle, uint32_t type,
                                 uint32_t reloc_addr, uint32_t sym_value)
{
    uint32_t *ptr = (uint32_t *)reloc_addr;
    uint32_t value;

    switch (type) {
    case R_ARM_NONE:
        // 无操作
        break;

    case R_ARM_ABS32:
        // 直接32位地址：S + A
        // S = 符号值，A = addend（从位置读取）
        value = *ptr;
        *ptr = sym_value + value;
        ELF_DEBUG("R_ARM_ABS32: 0x%08x -> 0x%08x\n", reloc_addr, *ptr);
        break;

    case R_ARM_REL32:
        // PC相对32位：S + A - P
        // P = 重定位位置
        value = *ptr;
        *ptr = sym_value + value - reloc_addr;
        ELF_DEBUG("R_ARM_REL32: 0x%08x -> 0x%08x\n", reloc_addr, *ptr);
        break;

    case R_ARM_CALL:
    case R_ARM_JUMP24:
        // BL/B指令：(S + A - P) >> 2
        return elf_relocate_arm_branch(reloc_addr, sym_value, type);

    case R_ARM_MOVW_ABS_NC:
        // MOVW指令：(S + A) & 0xFFFF
        return elf_relocate_arm_movw(reloc_addr, sym_value);

    case R_ARM_MOVT_ABS:
        // MOVT指令：(S + A) >> 16
        return elf_relocate_arm_movt(reloc_addr, sym_value);

    case R_ARM_THM_CALL:
    case R_ARM_THM_JUMP24:
        // Thumb BL/B指令
        return elf_relocate_thumb_branch(reloc_addr, sym_value, type);

    case R_ARM_THM_MOVW_ABS_NC:
        // Thumb MOVW指令
        return elf_relocate_thumb_movw(reloc_addr, sym_value);

    case R_ARM_THM_MOVT_ABS:
        // Thumb MOVT指令
        return elf_relocate_thumb_movt(reloc_addr, sym_value);

    default:
        ELF_ERROR("Unsupported relocation type: %d\n", type);
        return -ENOTSUP;
    }

    return 0;
}
```

## ARM分支指令重定位

```c
static int elf_relocate_arm_branch(uint32_t reloc_addr, uint32_t sym_value,
                                    uint32_t type)
{
    uint32_t *insn = (uint32_t *)reloc_addr;
    int32_t offset;

    // 计算偏移：(S + A - P) >> 2
    // A = 当前指令中的偏移（符号扩展）
    int32_t addend = (*insn & 0x00FFFFFF) << 2;
    if (addend & 0x02000000) {
        addend |= 0xFC000000;  // 符号扩展
    }

    offset = (sym_value + addend - reloc_addr) >> 2;

    // 检查范围（±32MB）
    if (offset < -0x800000 || offset > 0x7FFFFF) {
        ELF_ERROR("Branch offset out of range: %d\n", offset);
        return -ERANGE;
    }

    // 更新指令
    *insn = (*insn & 0xFF000000) | (offset & 0x00FFFFFF);

    ELF_DEBUG("ARM branch: 0x%08x -> target 0x%08x (offset=%d)\n",
              reloc_addr, sym_value, offset);

    return 0;
}
```

## ARM MOVW/MOVT重定位

```c
static int elf_relocate_arm_movw(uint32_t reloc_addr, uint32_t sym_value)
{
    uint32_t *insn = (uint32_t *)reloc_addr;
    uint32_t addend;
    uint32_t value;

    // 提取addend
    addend = ((*insn & 0x000F0000) >> 4) | (*insn & 0x00000FFF);

    // 计算值：(S + A) & 0xFFFF
    value = (sym_value + addend) & 0xFFFF;

    // 更新指令
    *insn = (*insn & 0xFFF0F000) |
            ((value & 0xF000) << 4) |
            (value & 0x0FFF);

    ELF_DEBUG("ARM MOVW: 0x%08x -> value 0x%04x\n", reloc_addr, value);

    return 0;
}

static int elf_relocate_arm_movt(uint32_t reloc_addr, uint32_t sym_value)
{
    uint32_t *insn = (uint32_t *)reloc_addr;
    uint32_t addend;
    uint32_t value;

    // 提取addend
    addend = ((*insn & 0x000F0000) >> 4) | (*insn & 0x00000FFF);

    // 计算值：(S + A) >> 16
    value = ((sym_value + addend) >> 16) & 0xFFFF;

    // 更新指令
    *insn = (*insn & 0xFFF0F000) |
            ((value & 0xF000) << 4) |
            (value & 0x0FFF);

    ELF_DEBUG("ARM MOVT: 0x%08x -> value 0x%04x\n", reloc_addr, value);

    return 0;
}
```

## Thumb分支指令重定位

```c
static int elf_relocate_thumb_branch(uint32_t reloc_addr, uint32_t sym_value,
                                      uint32_t type)
{
    uint16_t *insn = (uint16_t *)reloc_addr;
    int32_t offset;

    // Thumb BL/BLX是32位指令（两个16位半字）
    uint32_t upper = insn[0];
    uint32_t lower = insn[1];

    // 提取偏移
    int32_t addend = ((upper & 0x07FF) << 12) | ((lower & 0x07FF) << 1);
    if (addend & 0x00400000) {
        addend |= 0xFF800000;  // 符号扩展
    }

    // 计算新偏移
    offset = sym_value + addend - reloc_addr;

    // 检查范围（±16MB）
    if (offset < -0x1000000 || offset > 0xFFFFFF) {
        ELF_ERROR("Thumb branch offset out of range: %d\n", offset);
        return -ERANGE;
    }

    // 更新指令
    insn[0] = (upper & 0xF800) | ((offset >> 12) & 0x07FF);
    insn[1] = (lower & 0xF800) | ((offset >> 1) & 0x07FF);

    ELF_DEBUG("Thumb branch: 0x%08x -> target 0x%08x\n", reloc_addr, sym_value);

    return 0;
}
```

## Thumb MOVW/MOVT重定位

```c
static int elf_relocate_thumb_movw(uint32_t reloc_addr, uint32_t sym_value)
{
    uint16_t *insn = (uint16_t *)reloc_addr;
    uint32_t upper = insn[0];
    uint32_t lower = insn[1];
    uint32_t addend;
    uint32_t value;

    // 提取addend
    addend = ((upper & 0x000F) << 12) |
             ((upper & 0x0400) << 1) |
             ((lower & 0x7000) >> 4) |
             (lower & 0x00FF);

    // 计算值
    value = (sym_value + addend) & 0xFFFF;

    // 更新指令
    insn[0] = (upper & 0xFBF0) |
              ((value & 0xF000) >> 12) |
              ((value & 0x0800) >> 1);
    insn[1] = (lower & 0x8F00) |
              ((value & 0x0700) << 4) |
              (value & 0x00FF);

    ELF_DEBUG("Thumb MOVW: 0x%08x -> value 0x%04x\n", reloc_addr, value);

    return 0;
}

static int elf_relocate_thumb_movt(uint32_t reloc_addr, uint32_t sym_value)
{
    uint16_t *insn = (uint16_t *)reloc_addr;
    uint32_t upper = insn[0];
    uint32_t lower = insn[1];
    uint32_t addend;
    uint32_t value;

    // 提取addend
    addend = ((upper & 0x000F) << 12) |
             ((upper & 0x0400) << 1) |
             ((lower & 0x7000) >> 4) |
             (lower & 0x00FF);

    // 计算值
    value = ((sym_value + addend) >> 16) & 0xFFFF;

    // 更新指令
    insn[0] = (upper & 0xFBF0) |
              ((value & 0xF000) >> 12) |
              ((value & 0x0800) >> 1);
    insn[1] = (lower & 0x8F00) |
              ((value & 0x0700) << 4) |
              (value & 0x00FF);

    ELF_DEBUG("Thumb MOVT: 0x%08x -> value 0x%04x\n", reloc_addr, value);

    return 0;
}
```

## 缓存一致性

重定位修改了代码段，需要确保指令缓存一致性。

```c
static void elf_flush_icache(uint32_t start, uint32_t end)
{
#ifdef CONFIG_ARM_MPU
    // 清空数据缓存
    SCB_CleanDCache_by_Addr((uint32_t *)start, end - start);

    // 使指令缓存无效
    SCB_InvalidateICache_by_Addr((uint32_t *)start, end - start);
#endif

    // 内存屏障
    __DSB();
    __ISB();
}

int elf_apply_relocations(elf_handle_t *handle)
{
    // ... 应用重定位 ...

    // 刷新指令缓存
    for (int i = 0; i < handle->num_segments; i++) {
        if (handle->segments[i].flags & PF_X) {
            uint32_t start = handle->segments[i].load_addr;
            uint32_t end = start + handle->segments[i].memsz;
            elf_flush_icache(start, end);
        }
    }

    return 0;
}
```

## 调试支持

```c
void elf_dump_relocations(elf_handle_t *handle)
{
    if (!handle->rel) {
        printk("No relocations\n");
        return;
    }

    printk("=== Relocations ===\n");
    for (int i = 0; i < handle->num_rel; i++) {
        Elf32_Rel *rel = &handle->rel[i];
        uint32_t type = ELF32_R_TYPE(rel->r_info);
        uint32_t sym_idx = ELF32_R_SYM(rel->r_info);

        const char *type_name = elf_reloc_type_name(type);
        const char *sym_name = "";

        if (sym_idx < handle->num_syms && handle->strtab) {
            sym_name = handle->strtab + handle->symtab[sym_idx].st_name;
        }

        printk("  [%3d] offset=0x%08x type=%-20s sym=%s\n",
               i, rel->r_offset, type_name, sym_name);
    }
}

static const char *elf_reloc_type_name(uint32_t type)
{
    switch (type) {
    case R_ARM_NONE:            return "R_ARM_NONE";
    case R_ARM_ABS32:           return "R_ARM_ABS32";
    case R_ARM_REL32:           return "R_ARM_REL32";
    case R_ARM_CALL:            return "R_ARM_CALL";
    case R_ARM_JUMP24:          return "R_ARM_JUMP24";
    case R_ARM_MOVW_ABS_NC:     return "R_ARM_MOVW_ABS_NC";
    case R_ARM_MOVT_ABS:        return "R_ARM_MOVT_ABS";
    case R_ARM_THM_CALL:        return "R_ARM_THM_CALL";
    case R_ARM_THM_JUMP24:      return "R_ARM_THM_JUMP24";
    case R_ARM_THM_MOVW_ABS_NC: return "R_ARM_THM_MOVW_ABS_NC";
    case R_ARM_THM_MOVT_ABS:    return "R_ARM_THM_MOVT_ABS";
    default:                    return "UNKNOWN";
    }
}
```
