# ELF格式详解

## ELF文件结构

```
┌──────────────────┐
│   ELF Header     │  固定大小，包含魔数、架构、入口点
├──────────────────┤
│ Program Headers  │  描述运行时段（loadable segments）
├──────────────────┤
│   .text段        │  代码段（可执行）
├──────────────────┤
│   .rodata段      │  只读数据
├──────────────────┤
│   .data段        │  已初始化数据
├──────────────────┤
│   .bss段         │  未初始化数据（不占文件空间）
├──────────────────┤
│ Section Headers  │  描述各个section（链接时使用）
└──────────────────┘
```

## ELF Header

```c
#define EI_NIDENT 16

typedef struct {
    unsigned char e_ident[EI_NIDENT];  // 魔数和标识
    uint16_t      e_type;              // 文件类型（ET_EXEC=2）
    uint16_t      e_machine;           // 架构（EM_ARM=40）
    uint32_t      e_version;           // 版本
    uint32_t      e_entry;             // 入口点地址
    uint32_t      e_phoff;             // Program header偏移
    uint32_t      e_shoff;             // Section header偏移
    uint32_t      e_flags;             // ARM特定标志
    uint16_t      e_ehsize;            // ELF header大小
    uint16_t      e_phentsize;         // Program header条目大小
    uint16_t      e_phnum;             // Program header数量
    uint16_t      e_shentsize;         // Section header条目大小
    uint16_t      e_shnum;             // Section header数量
    uint16_t      e_shstrndx;          // 字符串表索引
} Elf32_Ehdr;
```

### 关键字段验证

```c
// 魔数检查
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

bool is_valid_elf(Elf32_Ehdr *ehdr) {
    return ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
           ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
           ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
           ehdr->e_ident[EI_MAG3] == ELFMAG3 &&
           ehdr->e_ident[EI_CLASS] == ELFCLASS32 &&  // 32位
           ehdr->e_ident[EI_DATA] == ELFDATA2LSB &&  // 小端
           ehdr->e_machine == EM_ARM &&              // ARM架构
           ehdr->e_type == ET_EXEC;                  // 可执行文件
}
```

## Program Header

描述运行时需要加载的段。

```c
typedef struct {
    uint32_t p_type;    // 段类型（PT_LOAD=1）
    uint32_t p_offset;  // 文件中的偏移
    uint32_t p_vaddr;   // 虚拟地址（加载地址）
    uint32_t p_paddr;   // 物理地址（通常与vaddr相同）
    uint32_t p_filesz;  // 文件中的大小
    uint32_t p_memsz;   // 内存中的大小（>=filesz，差值为BSS）
    uint32_t p_flags;   // 权限标志（PF_X|PF_W|PF_R）
    uint32_t p_align;   // 对齐要求
} Elf32_Phdr;
```

### 段类型

```c
#define PT_NULL    0  // 未使用
#define PT_LOAD    1  // 可加载段
#define PT_DYNAMIC 2  // 动态链接信息（静态链接时忽略）
#define PT_INTERP  3  // 解释器路径（静态链接时无）
#define PT_NOTE    4  // 辅助信息
#define PT_PHDR    6  // Program header表本身
#define PT_GNU_STACK 0x6474e551  // 栈权限
```

### 权限标志

```c
#define PF_X 0x1  // 可执行
#define PF_W 0x2  // 可写
#define PF_R 0x4  // 可读

// 典型组合
// .text段: PF_R | PF_X (0x5)
// .rodata段: PF_R (0x4)
// .data/.bss段: PF_R | PF_W (0x6)
```

## Section Header

描述链接时的section，运行时主要用于符号表和重定位。

```c
typedef struct {
    uint32_t sh_name;       // 名称（字符串表索引）
    uint32_t sh_type;       // 类型
    uint32_t sh_flags;      // 标志
    uint32_t sh_addr;       // 运行时地址
    uint32_t sh_offset;     // 文件偏移
    uint32_t sh_size;       // 大小
    uint32_t sh_link;       // 链接到其他section
    uint32_t sh_info;       // 附加信息
    uint32_t sh_addralign;  // 对齐
    uint32_t sh_entsize;    // 条目大小（如果是表）
} Elf32_Shdr;
```

### 重要的Section类型

```c
#define SHT_NULL     0   // 未使用
#define SHT_PROGBITS 1   // 程序数据（.text, .data, .rodata）
#define SHT_SYMTAB   2   // 符号表
#define SHT_STRTAB   3   // 字符串表
#define SHT_RELA     4   // 重定位表（带addend）
#define SHT_NOBITS   8   // .bss段（不占文件空间）
#define SHT_REL      9   // 重定位表（无addend）
```

## 重定位条目

静态链接的ELF可能仍需要重定位（如果不是位置无关代码）。

```c
// REL类型（ARM常用）
typedef struct {
    uint32_t r_offset;  // 需要重定位的位置
    uint32_t r_info;    // 符号索引和重定位类型
} Elf32_Rel;

// RELA类型（带显式addend）
typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    int32_t  r_addend;  // 附加值
} Elf32_Rela;

// 提取宏
#define ELF32_R_SYM(i)  ((i) >> 8)
#define ELF32_R_TYPE(i) ((i) & 0xff)
```

### ARM重定位类型

```c
#define R_ARM_NONE        0   // 无操作
#define R_ARM_ABS32       2   // 绝对32位地址
#define R_ARM_REL32       3   // 相对32位地址
#define R_ARM_CALL        28  // 函数调用（BL指令）
#define R_ARM_JUMP24      29  // 跳转（B指令）
#define R_ARM_MOVW_ABS_NC 43  // MOVW指令低16位
#define R_ARM_MOVT_ABS    44  // MOVT指令高16位
```

## 符号表

```c
typedef struct {
    uint32_t st_name;   // 符号名（字符串表索引）
    uint32_t st_value;  // 符号值（地址）
    uint32_t st_size;   // 符号大小
    uint8_t  st_info;   // 类型和绑定
    uint8_t  st_other;  // 保留
    uint16_t st_shndx;  // 所属section索引
} Elf32_Sym;

// 符号类型
#define STT_NOTYPE  0  // 未指定
#define STT_OBJECT  1  // 数据对象
#define STT_FUNC    2  // 函数
#define STT_SECTION 3  // Section
#define STT_FILE    4  // 文件名

// 符号绑定
#define STB_LOCAL  0   // 局部符号
#define STB_GLOBAL 1   // 全局符号
#define STB_WEAK   2   // 弱符号
```

## 典型静态链接ELF布局

```
地址范围          段          权限    内容
0x08000000    .text       R-X     代码
0x08010000    .rodata     R--     常量字符串、常量数据
0x20000000    .data       RW-     已初始化全局变量
0x20001000    .bss        RW-     未初始化全局变量
0x20002000    heap        RW-     动态分配
0x20010000    stack       RW-     栈（向下增长）
```

## 解析示例

```c
int parse_elf_header(const uint8_t *elf_data, elf_info_t *info) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)elf_data;

    // 验证魔数
    if (!is_valid_elf(ehdr)) {
        return -ENOEXEC;
    }

    // 提取关键信息
    info->entry = ehdr->e_entry;
    info->phoff = ehdr->e_phoff;
    info->phnum = ehdr->e_phnum;
    info->shoff = ehdr->e_shoff;
    info->shnum = ehdr->e_shnum;

    return 0;
}
```

## 注意事项

1. **字节序**：ARM Cortex-M3是小端，确保ELF也是小端
2. **对齐**：段加载地址必须满足对齐要求（通常4或8字节）
3. **权限**：使用MPU设置段的执行/读写权限
4. **BSS清零**：p_memsz > p_filesz的部分需要清零
5. **静态链接验证**：确保没有PT_INTERP和PT_DYNAMIC段
