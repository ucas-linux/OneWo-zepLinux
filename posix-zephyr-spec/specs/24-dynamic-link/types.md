# 数据类型定义

## 动态链接API类型

```c
#include <dlfcn.h>

// 库句柄（不透明指针）
typedef void *dl_handle;

// 符号信息
typedef struct {
    const char *dli_fname;   // 库文件名
    void *dli_fbase;         // 库加载基地址
    const char *dli_sname;   // 符号名称
    void *dli_saddr;         // 符号地址
} Dl_info;

// 标志位
#define RTLD_LAZY     0x00001
#define RTLD_NOW      0x00002
#define RTLD_GLOBAL   0x00100
#define RTLD_LOCAL    0x00000
#define RTLD_NODELETE 0x01000
#define RTLD_NOLOAD   0x00004
#define RTLD_DEEPBIND 0x00008
```

## ELF动态链接类型

```c
// ELF动态节条目
typedef struct {
    Elf32_Sword d_tag;
    union {
        Elf32_Word d_val;
        Elf32_Addr d_ptr;
    } d_un;
} Elf32_Dyn;

// 动态节标签
#define DT_NULL         0   // 结束标记
#define DT_NEEDED       1   // 依赖库名称
#define DT_PLTRELSZ     2   // PLT重定位大小
#define DT_PLTGOT       3   // PLT/GOT地址
#define DT_HASH         4   // 符号哈希表
#define DT_STRTAB       5   // 字符串表
#define DT_SYMTAB       6   // 符号表
#define DT_RELA         7   // 重定位表（带加数）
#define DT_RELASZ       8   // 重定位表大小
#define DT_RELAENT      9   // 重定位条目大小
#define DT_STRSZ        10  // 字符串表大小
#define DT_SYMENT       11  // 符号表条目大小
#define DT_INIT         12  // 初始化函数地址
#define DT_FINI         13  // 终止函数地址
#define DT_SONAME       14  // 共享库名称
#define DT_RPATH        15  // 库搜索路径
#define DT_SYMBOLIC     16  // 符号优先级
#define DT_REL          17  // 重定位表（无加数）
#define DT_RELSZ        18  // REL表大小
#define DT_RELENT       19  // REL条目大小
#define DT_PLTREL       20  // PLT重定位类型
#define DT_DEBUG        21  // 调试信息
#define DT_TEXTREL      22  // 文本段重定位
#define DT_JMPREL       23  // PLT重定位表
```

## 内部实现类型

```c
// 动态库句柄
typedef struct dl_handle {
    char path[256];             // 库路径
    void *base_addr;            // 加载基地址
    size_t size;                // 库大小

    // ELF信息
    Elf32_Ehdr *ehdr;           // ELF头
    Elf32_Phdr *phdr;           // 程序头表
    Elf32_Dyn *dynamic;         // 动态节

    // 符号表
    Elf32_Sym *symtab;          // 符号表
    char *strtab;               // 字符串表
    uint32_t *hash;             // 符号哈希表

    // 重定位
    Elf32_Rel *rel;             // REL重定位
    size_t rel_count;
    Elf32_Rela *rela;           // RELA重定位
    size_t rela_count;

    // PLT/GOT
    void *pltgot;               // PLT/GOT地址
    Elf32_Rel *plt_rel;         // PLT重定位
    size_t plt_rel_count;

    // 状态
    int refcount;               // 引用计数
    int flags;                  // RTLD_*标志
    bool relocated;             // 是否已重定位
    bool initialized;           // 是否已初始化

    // 依赖
    struct dl_handle **deps;    // 依赖库列表
    int dep_count;

    struct dl_handle *next;
} dl_handle_t;

// 符号表条目
typedef struct {
    const char *name;           // 符号名称
    void *addr;                 // 符号地址
    dl_handle_t *owner;         // 所属库
    int type;                   // 符号类型
    int binding;                // 符号绑定
} symbol_entry_t;

// 全局符号表
typedef struct {
    symbol_entry_t *entries;
    int count;
    int capacity;
    struct k_mutex lock;
} symbol_table_t;

// 地址池管理
typedef struct {
    void *base;                 // 起始地址
    size_t slot_size;           // 每个槽大小
    int slot_count;             // 槽数量
    bool *used;                 // 使用标记
    struct k_mutex lock;
} addr_pool_t;
```

## 符号类型和绑定

```c
// 符号类型（ELF32_ST_TYPE）
#define STT_NOTYPE  0   // 未指定
#define STT_OBJECT  1   // 数据对象
#define STT_FUNC    2   // 函数
#define STT_SECTION 3   // 节
#define STT_FILE    4   // 文件名
#define STT_COMMON  5   // 公共数据
#define STT_TLS     6   // 线程局部存储

// 符号绑定（ELF32_ST_BIND）
#define STB_LOCAL   0   // 局部符号
#define STB_GLOBAL  1   // 全局符号
#define STB_WEAK    2   // 弱符号
```

## 重定位类型（ARM）

```c
#define R_ARM_NONE              0
#define R_ARM_ABS32             2   // 绝对32位地址
#define R_ARM_REL32             3   // 相对32位地址
#define R_ARM_GLOB_DAT          21  // GOT条目
#define R_ARM_JUMP_SLOT         22  // PLT条目
#define R_ARM_RELATIVE          23  // 相对基地址
#define R_ARM_GOT_BREL          26  // GOT相对
#define R_ARM_PLT32             27  // PLT相对
#define R_ARM_CALL              28  // 函数调用
#define R_ARM_JUMP24            29  // 跳转
```
