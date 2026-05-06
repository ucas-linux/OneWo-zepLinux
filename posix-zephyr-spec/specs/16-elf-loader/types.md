# ELF加载器类型定义

## 基本类型

```c
#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>

// ELF魔数
#define EI_NIDENT 16
#define ELFMAG0   0x7f
#define ELFMAG1   'E'
#define ELFMAG2   'L'
#define ELFMAG3   'F'

// ELF类别
#define ELFCLASSNONE 0
#define ELFCLASS32   1
#define ELFCLASS64   2

// 字节序
#define ELFDATANONE 0
#define ELFDATA2LSB 1  // 小端
#define ELFDATA2MSB 2  // 大端

// ELF版本
#define EV_NONE    0
#define EV_CURRENT 1

// 文件类型
#define ET_NONE   0  // 未知
#define ET_REL    1  // 可重定位
#define ET_EXEC   2  // 可执行
#define ET_DYN    3  // 共享对象
#define ET_CORE   4  // Core文件

// 架构
#define EM_NONE  0
#define EM_ARM   40
#define EM_AARCH64 183

// ELF标识索引
#define EI_MAG0       0
#define EI_MAG1       1
#define EI_MAG2       2
#define EI_MAG3       3
#define EI_CLASS      4
#define EI_DATA       5
#define EI_VERSION    6
#define EI_OSABI      7
#define EI_ABIVERSION 8
#define EI_PAD        9
```

## ELF Header

```c
typedef struct {
    unsigned char e_ident[EI_NIDENT];  // 魔数和标识
    uint16_t      e_type;              // 文件类型
    uint16_t      e_machine;           // 架构
    uint32_t      e_version;           // 版本
    uint32_t      e_entry;             // 入口点地址
    uint32_t      e_phoff;             // Program header偏移
    uint32_t      e_shoff;             // Section header偏移
    uint32_t      e_flags;             // 处理器特定标志
    uint16_t      e_ehsize;            // ELF header大小
    uint16_t      e_phentsize;         // Program header条目大小
    uint16_t      e_phnum;             // Program header数量
    uint16_t      e_shentsize;         // Section header条目大小
    uint16_t      e_shnum;             // Section header数量
    uint16_t      e_shstrndx;          // 字符串表索引
} Elf32_Ehdr;
```

## Program Header

```c
// 段类型
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7
#define PT_GNU_STACK 0x6474e551

// 段标志
#define PF_X 0x1  // 可执行
#define PF_W 0x2  // 可写
#define PF_R 0x4  // 可读

typedef struct {
    uint32_t p_type;    // 段类型
    uint32_t p_offset;  // 文件偏移
    uint32_t p_vaddr;   // 虚拟地址
    uint32_t p_paddr;   // 物理地址
    uint32_t p_filesz;  // 文件中的大小
    uint32_t p_memsz;   // 内存中的大小
    uint32_t p_flags;   // 段标志
    uint32_t p_align;   // 对齐
} Elf32_Phdr;
```

## Section Header

```c
// Section类型
#define SHT_NULL     0
#define SHT_PROGBITS 1
#define SHT_SYMTAB   2
#define SHT_STRTAB   3
#define SHT_RELA     4
#define SHT_HASH     5
#define SHT_DYNAMIC  6
#define SHT_NOTE     7
#define SHT_NOBITS   8
#define SHT_REL      9
#define SHT_SHLIB    10
#define SHT_DYNSYM   11

// Section标志
#define SHF_WRITE     0x1
#define SHF_ALLOC     0x2
#define SHF_EXECINSTR 0x4

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
    uint32_t sh_entsize;    // 条目大小
} Elf32_Shdr;
```

## 符号表

```c
// 符号绑定
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

// 符号类型
#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4

// 符号可见性
#define STV_DEFAULT   0
#define STV_INTERNAL  1
#define STV_HIDDEN    2
#define STV_PROTECTED 3

typedef struct {
    uint32_t st_name;   // 符号名（字符串表索引）
    uint32_t st_value;  // 符号值
    uint32_t st_size;   // 符号大小
    uint8_t  st_info;   // 类型和绑定
    uint8_t  st_other;  // 可见性
    uint16_t st_shndx;  // Section索引
} Elf32_Sym;

#define ELF32_ST_BIND(i)   ((i) >> 4)
#define ELF32_ST_TYPE(i)   ((i) & 0xf)
#define ELF32_ST_INFO(b,t) (((b) << 4) + ((t) & 0xf))
```

## 重定位

```c
typedef struct {
    uint32_t r_offset;  // 重定位位置
    uint32_t r_info;    // 符号索引和类型
} Elf32_Rel;

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    int32_t  r_addend;  // 附加值
} Elf32_Rela;

#define ELF32_R_SYM(i)  ((i) >> 8)
#define ELF32_R_TYPE(i) ((i) & 0xff)
#define ELF32_R_INFO(s,t) (((s) << 8) + ((t) & 0xff))
```

## ELF加载器句柄

```c
typedef struct {
    // 段信息
    uint32_t vaddr;      // 虚拟地址
    uint32_t offset;     // 文件偏移
    uint32_t filesz;     // 文件大小
    uint32_t memsz;      // 内存大小
    uint32_t flags;      // 权限标志
    uint32_t load_addr;  // 实际加载地址
    bool in_ram;         // 是否在RAM中
    bool lazy;           // 延迟加载
} elf_segment_info_t;

typedef struct {
    void *code_base;
    size_t code_size;
    void *rodata_base;
    size_t rodata_size;
    void *data_base;
    size_t data_size;
    void *bss_base;
    size_t bss_size;
    void *heap_base;
    size_t heap_size;
    void *stack_base;
    size_t stack_size;
} elf_memory_layout_t;

typedef struct {
    // ELF文件数据
    const uint8_t *elf_data;
    size_t elf_size;

    // ELF结构
    Elf32_Ehdr *ehdr;
    Elf32_Phdr *phdr;
    Elf32_Shdr *shdr;

    // 入口点
    uint32_t entry;

    // 段信息
    elf_segment_info_t segments[CONFIG_ELF_MAX_SEGMENTS];
    int num_segments;

    // 重定位
    Elf32_Rel *rel;
    int num_rel;

    // 符号表
    Elf32_Sym *symtab;
    int num_syms;
    const char *strtab;

    // 内存布局
    elf_memory_layout_t memory;
    void *memory_base;
    size_t memory_size;

    // 状态
    bool loaded;
    bool relocated;

} elf_handle_t;
```

## 错误码

```c
// ELF加载器特定错误码
#define ELF_SUCCESS       0
#define ELF_ERROR_NOMEM   -1
#define ELF_ERROR_NOEXEC  -2
#define ELF_ERROR_INVAL   -3
#define ELF_ERROR_IO      -4
#define ELF_ERROR_NOENT   -5
#define ELF_ERROR_NOSYS   -6
#define ELF_ERROR_RANGE   -7
```

## 配置常量

```c
#ifndef CONFIG_ELF_MAX_SEGMENTS
#define CONFIG_ELF_MAX_SEGMENTS 8
#endif

#ifndef CONFIG_PROCESS_DEFAULT_HEAP
#define CONFIG_PROCESS_DEFAULT_HEAP 4096
#endif

#ifndef CONFIG_PROCESS_DEFAULT_STACK
#define CONFIG_PROCESS_DEFAULT_STACK 2048
#endif
```
