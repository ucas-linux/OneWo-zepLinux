# 动态链接支持 - Dynamic Linking

## 概述

在无MMU的Cortex-M3上实现有限的动态链接功能，允许运行时加载共享库（.so文件）。这是对静态ELF加载器（Spec 16）的扩展。

## 目标

- 实现dlopen/dlsym/dlclose API
- 支持位置无关代码（PIC）
- 处理符号解析和重定位
- 提供简化的动态链接器（ld.so）
- 支持延迟绑定（可选）

## 限制与挑战

### 无MMU环境的限制

1. **无虚拟地址空间**：所有库共享同一物理地址空间
2. **无地址随机化**：ASLR不可用
3. **有限的内存保护**：依赖MPU而非页表
4. **加载地址冲突**：需要仔细管理加载地址

### 简化策略

- **仅支持PIC/PIE代码**：所有共享库必须编译为位置无关
- **预分配地址池**：为动态库预留固定的地址区域
- **符号表缓存**：维护全局符号表加速查找
- **无版本控制**：不支持符号版本（GLIBC_2.x等）

## Linux动态链接API

```c
#include <dlfcn.h>

// 打开共享库
void *dlopen(const char *filename, int flags);

// 查找符号
void *dlsym(void *handle, const char *symbol);

// 关闭共享库
int dlclose(void *handle);

// 获取错误信息
char *dlerror(void);

// 获取符号信息
int dladdr(void *addr, Dl_info *info);

// 标志
#define RTLD_LAZY     0x00001  // 延迟绑定
#define RTLD_NOW      0x00002  // 立即绑定
#define RTLD_GLOBAL   0x00100  // 符号全局可见
#define RTLD_LOCAL    0x00000  // 符号局部可见
```

## 使用场景

```c
// 场景1：加载插件
void *handle = dlopen("/lib/plugin.so", RTLD_NOW | RTLD_LOCAL);
if (!handle) {
    fprintf(stderr, "dlopen failed: %s\n", dlerror());
    return -1;
}

// 查找函数
typedef int (*plugin_init_t)(void);
plugin_init_t init = dlsym(handle, "plugin_init");
if (!init) {
    fprintf(stderr, "dlsym failed: %s\n", dlerror());
    dlclose(handle);
    return -1;
}

// 调用函数
int ret = init();

// 卸载
dlclose(handle);

// 场景2：可选功能
void *libssl = dlopen("libssl.so", RTLD_LAZY);
if (libssl) {
    // SSL可用
    void *ssl_init = dlsym(libssl, "SSL_library_init");
    // 使用SSL
} else {
    // 降级到非SSL模式
}
```

## 架构组件

```
┌─────────────────┐
│ Linux App       │
│ dlopen/dlsym    │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Dynamic Linker  │ ← 本模块
│ - Symbol Table  │
│ - Relocation    │
│ - Lazy Binding  │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ ELF Loader      │ (Spec 16)
│ - Parse SO      │
│ - Load Segments │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Memory Manager  │
│ - Address Pool  │
│ - MPU Setup     │
└─────────────────┘
```

## 实现策略

### 1. 地址空间布局

```
0x20000000 ┌─────────────────┐
           │  Main Program   │
           │  (Static)       │
0x20100000 ├─────────────────┤
           │  Dynamic Lib 1  │
0x20200000 ├─────────────────┤
           │  Dynamic Lib 2  │
0x20300000 ├─────────────────┤
           │  Dynamic Lib 3  │
           │  ...            │
0x20800000 ├─────────────────┤
           │  Heap           │
           └─────────────────┘
```

### 2. 共享库结构

```c
typedef struct {
    char path[256];             // 库路径
    void *base_addr;            // 加载基地址
    size_t size;                // 库大小
    Elf32_Ehdr *ehdr;           // ELF头
    Elf32_Sym *symtab;          // 符号表
    char *strtab;               // 字符串表
    int refcount;               // 引用计数
    bool global;                // 全局符号可见性
    struct dl_handle *next;
} dl_handle_t;
```

### 3. 全局符号表

```c
typedef struct {
    const char *name;
    void *addr;
    dl_handle_t *owner;
} symbol_entry_t;

static symbol_entry_t *g_symbol_table = NULL;
static int g_symbol_count = 0;
```

## 与其他模块的关系

- **ELF Loader (Spec 16)**：复用ELF解析和加载逻辑
- **内存管理 (Spec 05)**：分配动态库加载地址
- **文件系统 (Spec 07)**：从文件系统加载.so文件
- **系统调用 (Spec 17)**：提供dlopen等系统调用

## 配置选项

```kconfig
config DYNAMIC_LINKING
    bool "Dynamic linking support"
    depends on ELF_LOADER
    default n
    help
      Enable runtime loading of shared libraries (.so files).

config DYNAMIC_LIB_MAX
    int "Maximum number of dynamic libraries"
    depends on DYNAMIC_LINKING
    default 8
    range 1 32

config DYNAMIC_LIB_BASE
    hex "Base address for dynamic libraries"
    depends on DYNAMIC_LINKING
    default 0x20100000

config DYNAMIC_LIB_SIZE
    hex "Size of each dynamic library slot"
    depends on DYNAMIC_LINKING
    default 0x100000

config RTLD_LAZY_BINDING
    bool "Support lazy binding"
    depends on DYNAMIC_LINKING
    default n
    help
      Enable RTLD_LAZY flag for deferred symbol resolution.
```

## 文件组织

```
specs/24-dynamic-link/
├── README.md              # 本文件
├── dlopen.md              # dlopen实现
├── dlsym.md               # 符号查找
├── dlclose.md             # 库卸载
├── relocation.md          # 动态重定位
├── lazy_binding.md        # 延迟绑定
├── types.md               # 数据类型
└── implementation.md      # 实现指导
```

## 性能考虑

- **符号查找**：使用哈希表加速
- **延迟绑定**：减少启动时间（可选）
- **符号缓存**：避免重复查找
- **预加载**：常用库可以预加载

## 安全考虑

1. **路径验证**：防止路径遍历攻击
2. **签名验证**：验证.so文件完整性（可选）
3. **权限检查**：限制可加载的库
4. **地址空间隔离**：使用MPU保护

## 限制

1. **无C++异常**：不支持跨库异常传播
2. **无TLS**：不支持线程局部存储
3. **无构造/析构**：不支持.init/.fini节
4. **符号冲突**：同名符号可能冲突
5. **内存碎片**：频繁加载/卸载可能导致碎片

## 替代方案

如果完整的动态链接过于复杂，可以考虑：

1. **静态链接**：所有代码编译到单一二进制
2. **插件系统**：简化的函数表注册机制
3. **脚本语言**：使用Lua/Python等嵌入式脚本

## 参考资料

- ELF Dynamic Linking: https://refspecs.linuxfoundation.org/elf/elf.pdf
- Linux ld.so: https://man7.org/linux/man-pages/man8/ld.so.8.html
- Position Independent Code: https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
- ARM PIC/PIE: https://developer.arm.com/documentation
