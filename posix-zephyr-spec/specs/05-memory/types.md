# 数据类型定义

## 基本类型

### size_t
```c
typedef unsigned long size_t;
```
用于表示对象大小的无符号整数类型。

## 内存块头部

### mem_block_t
```c
typedef struct mem_block {
    size_t size;                    // 用户请求的大小
    uint32_t magic;                 // 魔数（用于检测损坏）
    #ifdef CONFIG_POSIX_MALLOC_DEBUG
    const char *file;               // 分配位置（文件）
    int line;                       // 分配位置（行号）
    uint64_t timestamp;             // 分配时间戳
    #endif
    struct mem_block *next;         // 全局链表（下一个）
    struct mem_block *prev;         // 全局链表（上一个）
} mem_block_t;
```

内存块头部结构，存储在每个分配块之前。

**字段说明**：
- **size**: 用户请求的字节数（不包括头部）
- **magic**: 魔数0xDEADBEEF，用于检测内存损坏和double free
- **file/line**: 调试模式下记录分配位置
- **timestamp**: 调试模式下记录分配时间
- **next/prev**: 双向链表指针，用于跟踪所有分配

### 魔数定义
```c
#define MEM_MAGIC      0xDEADBEEF  // 有效内存块
#define MEM_FREED      0x00000000  // 已释放（用于检测double free）
#define MEM_CORRUPTED  0xBADBAD00  // 损坏标记
```

## 内存统计

### malloc_stats_t
```c
typedef struct {
    size_t total_allocated;     // 累计分配字节数
    size_t current_allocated;   // 当前已分配字节数
    size_t peak_allocated;      // 峰值分配字节数
    size_t num_allocations;     // 分配次数
    size_t num_frees;           // 释放次数
    size_t num_reallocs;        // realloc次数
    size_t failed_allocations;  // 失败的分配次数
} malloc_stats_t;
```

内存分配统计信息。

## 全局数据结构

### 内存链表
```c
// 全局内存块链表头
static mem_block_t *g_mem_list = NULL;

// 全局锁
static struct k_mutex g_mem_lock;

// 统计信息
#ifdef CONFIG_POSIX_MALLOC_STATS
static malloc_stats_t g_mem_stats = {0};
#endif
```

## 配置常量

```c
// 最小分配大小
#define MIN_ALLOC_SIZE  16

// 对齐要求
#define MEM_ALIGN       8

// 头部大小
#define HEADER_SIZE     sizeof(mem_block_t)

// 最大分配大小
#define MAX_ALLOC_SIZE  (SIZE_MAX - HEADER_SIZE)
```

## 辅助宏

### 对齐宏
```c
// 向上对齐到指定边界
#define ALIGN_UP(x, align) \
    (((x) + (align) - 1) & ~((align) - 1))

// 检查是否对齐
#define IS_ALIGNED(x, align) \
    (((uintptr_t)(x) & ((align) - 1)) == 0)

// 检查是否是2的幂
#define IS_POWER_OF_TWO(x) \
    (((x) != 0) && (((x) & ((x) - 1)) == 0))
```

### 指针转换宏
```c
// 从用户指针获取头部
#define PTR_TO_BLOCK(ptr) \
    ((mem_block_t *)(ptr) - 1)

// 从头部获取用户指针
#define BLOCK_TO_PTR(block) \
    ((void *)((block) + 1))
```

### 调试宏
```c
#ifdef CONFIG_POSIX_MALLOC_DEBUG
#define MEM_DEBUG(fmt, ...) \
    printk("[MEM] " fmt "\n", ##__VA_ARGS__)
#else
#define MEM_DEBUG(fmt, ...)
#endif

#define MEM_ERROR(fmt, ...) \
    printk("[MEM ERROR] " fmt "\n", ##__VA_ARGS__)
```

## 内存布局

```
分配的内存块布局：

+------------------+
| mem_block_t      |  <- 头部（malloc返回地址 - sizeof(mem_block_t)）
|   size           |
|   magic          |
|   file/line      |  (调试模式)
|   next/prev      |
+------------------+
| 用户数据区       |  <- malloc返回的地址
|                  |
|   (size bytes)   |
|                  |
+------------------+
```

## 错误码

```c
#define ENOMEM  12  // 内存不足
#define EINVAL  22  // 无效参数
```

## 函数指针类型

### 内存分配器类型
```c
typedef void *(*malloc_func_t)(size_t size);
typedef void (*free_func_t)(void *ptr);
typedef void *(*realloc_func_t)(void *ptr, size_t size);
```

可用于自定义内存分配器。

## 配置选项类型

```c
// Kconfig生成的配置
#define CONFIG_POSIX_MALLOC              1
#define CONFIG_HEAP_MEM_POOL_SIZE        16384
#define CONFIG_POSIX_MALLOC_DEBUG        0
#define CONFIG_POSIX_MALLOC_STATS        0
#define CONFIG_POSIX_MALLOC_FILL_PATTERN 0xAA
```

## 内存池类型（可选）

如果使用进程私有堆：

```c
typedef struct {
    void *heap_start;           // 堆起始地址
    size_t heap_size;           // 堆大小
    struct k_heap kheap;        // Zephyr堆对象
    mem_block_t *block_list;    // 该堆的分配链表
    struct k_mutex lock;        // 堆锁
} process_heap_t;
```

## 内存区域类型

```c
typedef enum {
    MEM_REGION_HEAP,    // 堆内存
    MEM_REGION_STACK,   // 栈内存
    MEM_REGION_STATIC,  // 静态内存
    MEM_REGION_UNKNOWN  // 未知
} mem_region_t;
```

## 内存标签（调试用）

```c
#ifdef CONFIG_POSIX_MALLOC_TAGS
typedef struct {
    const char *tag;            // 标签名称
    size_t allocated;           // 该标签分配的字节数
    size_t count;               // 分配次数
} mem_tag_t;

#define malloc_tagged(size, tag) \
    malloc_with_tag(size, tag)
#endif
```

## 内存钩子类型

```c
typedef void (*malloc_hook_t)(void *ptr, size_t size);
typedef void (*free_hook_t)(void *ptr);

#ifdef CONFIG_POSIX_MALLOC_HOOKS
extern malloc_hook_t __malloc_hook;
extern free_hook_t __free_hook;
#endif
```

## 常量定义

```c
// 特殊值
#define SIZE_MAX  ((size_t)-1)

// 填充模式
#define ALLOC_FILL_PATTERN  0xAA  // 新分配内存填充
#define FREE_FILL_PATTERN   0xDD  // 已释放内存填充
#define GUARD_PATTERN       0xCC  // 保护字节填充
```

## 注意事项

1. **头部大小**: 每个分配额外占用16-32字节（取决于配置）
2. **对齐要求**: 所有分配至少8字节对齐
3. **魔数检查**: 用于检测内存损坏和double free
4. **链表维护**: 所有分配通过双向链表跟踪
5. **线程安全**: 通过g_mem_lock保证线程安全
6. **调试开销**: 调试模式下每个分配额外占用约16字节
