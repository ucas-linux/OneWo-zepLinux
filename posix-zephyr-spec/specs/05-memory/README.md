# 内存管理接口规范

## 概述

本模块定义3个POSIX内存管理接口在Zephyr RTOS上的实现规范。

## 接口列表

1. `malloc` - 分配内存
2. `realloc` - 重新分配内存
3. `free` - 释放内存

## Zephyr现有支持

Zephyr提供动态内存分配API：

```c
void *k_malloc(size_t size);
void *k_calloc(size_t nmemb, size_t size);
void k_free(void *ptr);
```

**特点**：
- 基于k_heap实现
- 支持多个独立的堆
- 线程安全
- 可配置堆大小

## 适配策略

### 方案1：直接映射（推荐）

```c
#include <zephyr/kernel.h>

#define malloc(size) k_malloc(size)
#define free(ptr) k_free(ptr)

void *realloc(void *ptr, size_t size)
{
    if (!ptr) return k_malloc(size);
    if (size == 0) {
        k_free(ptr);
        return NULL;
    }

    void *new_ptr = k_malloc(size);
    if (!new_ptr) return NULL;

    // 拷贝旧数据（需要记录原始大小）
    memcpy(new_ptr, ptr, old_size);
    k_free(ptr);

    return new_ptr;
}
```

**优点**：
- 实现简单
- 性能开销小
- 充分利用Zephyr现有功能

**缺点**：
- realloc需要记录分配大小

### 方案2：封装层（完整实现）

```c
// 内存块头部
typedef struct mem_block {
    size_t size;                    // 分配大小
    uint32_t magic;                 // 魔数（检测损坏）
    struct mem_block *next;         // 链表
} mem_block_t;

#define MEM_MAGIC 0xDEADBEEF

static struct k_mutex mem_lock;
static mem_block_t *mem_list = NULL;

void *malloc(size_t size)
{
    if (size == 0) return NULL;

    // 分配额外空间存储头部
    size_t total = size + sizeof(mem_block_t);
    mem_block_t *block = k_malloc(total);
    if (!block) return NULL;

    // 初始化头部
    block->size = size;
    block->magic = MEM_MAGIC;

    // 加入链表
    k_mutex_lock(&mem_lock, K_FOREVER);
    block->next = mem_list;
    mem_list = block;
    k_mutex_unlock(&mem_lock);

    // 返回用户数据区
    return (void *)(block + 1);
}

void free(void *ptr)
{
    if (!ptr) return;

    // 获取头部
    mem_block_t *block = (mem_block_t *)ptr - 1;

    // 验证魔数
    if (block->magic != MEM_MAGIC) {
        // 检测到内存损坏
        k_panic();
        return;
    }

    // 从链表移除
    k_mutex_lock(&mem_lock, K_FOREVER);
    mem_block_t **p = &mem_list;
    while (*p) {
        if (*p == block) {
            *p = block->next;
            break;
        }
        p = &(*p)->next;
    }
    k_mutex_unlock(&mem_lock);

    // 清除魔数
    block->magic = 0;

    // 释放内存
    k_free(block);
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr) return malloc(size);

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    // 获取旧大小
    mem_block_t *block = (mem_block_t *)ptr - 1;
    if (block->magic != MEM_MAGIC) {
        return NULL;
    }

    size_t old_size = block->size;

    // 如果新大小相同或更小，直接返回
    if (size <= old_size) {
        block->size = size;
        return ptr;
    }

    // 分配新内存
    void *new_ptr = malloc(size);
    if (!new_ptr) return NULL;

    // 拷贝旧数据
    memcpy(new_ptr, ptr, old_size);

    // 释放旧内存
    free(ptr);

    return new_ptr;
}
```

**优点**：
- 完整的realloc支持
- 内存损坏检测
- 可以统计内存使用情况

**缺点**：
- 额外的内存开销（每个分配多16字节）
- 稍微复杂

## 推荐实现

采用**方案2（封装层）**，理由：
1. 提供完整的POSIX语义
2. 内存调试功能（魔数检测）
3. 可以扩展统计功能
4. 额外开销可接受

## 配置选项

```kconfig
config POSIX_MALLOC
    bool "Enable POSIX malloc/free"
    default y
    select HEAP_MEM_POOL_SIZE
    help
      Enable POSIX-compliant dynamic memory allocation.

config HEAP_MEM_POOL_SIZE
    int "System heap size"
    default 16384
    help
      Size of the system heap in bytes.

config POSIX_MALLOC_DEBUG
    bool "Enable malloc debugging"
    default n
    help
      Add magic numbers and validation for memory corruption detection.

config POSIX_MALLOC_STATS
    bool "Enable malloc statistics"
    default n
    help
      Track memory allocation statistics (total allocated, peak usage, etc).
```

## 扩展功能

### 内存统计

```c
typedef struct {
    size_t total_allocated;     // 总分配字节数
    size_t current_allocated;   // 当前已分配
    size_t peak_allocated;      // 峰值
    size_t num_allocations;     // 分配次数
    size_t num_frees;           // 释放次数
} malloc_stats_t;

static malloc_stats_t mem_stats;

void malloc_stats(malloc_stats_t *stats)
{
    k_mutex_lock(&mem_lock, K_FOREVER);
    memcpy(stats, &mem_stats, sizeof(*stats));
    k_mutex_unlock(&mem_lock);
}
```

### 内存泄漏检测

```c
void malloc_dump_leaks(void)
{
    k_mutex_lock(&mem_lock, K_FOREVER);

    printf("Memory leak report:\n");
    mem_block_t *block = mem_list;
    int count = 0;

    while (block) {
        printf("  Leak %d: %zu bytes at %p\n",
               ++count, block->size, (void *)(block + 1));
        block = block->next;
    }

    if (count == 0) {
        printf("  No leaks detected!\n");
    }

    k_mutex_unlock(&mem_lock);
}
```

## 测试用例

### 测试1：基本分配和释放

```c
void test_basic_malloc(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    memset(ptr, 0xAA, 100);

    free(ptr);
}
```

### 测试2：realloc扩展

```c
void test_realloc(void)
{
    int *arr = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    arr = realloc(arr, 20 * sizeof(int));
    assert(arr != NULL);

    // 验证旧数据保留
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == i);
    }

    free(arr);
}
```

### 测试3：内存泄漏检测

```c
void test_leak_detection(void)
{
    void *leak1 = malloc(100);
    void *leak2 = malloc(200);

    // 故意不释放

    malloc_dump_leaks();
    // 应该报告2个泄漏
}
```

## 性能考虑

- malloc/free开销：约10-50微秒（取决于堆大小）
- realloc可能涉及内存拷贝，大块内存较慢
- 建议使用内存池（k_mem_slab）处理固定大小的频繁分配
- 避免频繁的小内存分配（<32字节）

## 注意事项

1. 堆大小需要在编译时配置（CONFIG_HEAP_MEM_POOL_SIZE）
2. 内存碎片可能导致分配失败，即使总空闲内存足够
3. 不支持多个独立堆（所有进程共享一个堆）
4. 在中断上下文中不能调用malloc/free
5. 线程安全由k_mutex保证，但有性能开销

## 与进程隔离集成

如果需要每个进程有独立的堆：

```c
// 进程私有堆
typedef struct {
    void *heap_start;
    size_t heap_size;
    struct k_heap kheap;
} process_heap_t;

void *process_malloc(size_t size)
{
    pcb_t *proc = current_process();
    return k_heap_alloc(&proc->heap.kheap, size, K_NO_WAIT);
}
```

## 限制

1. 不支持完整mmap/munmap语义（无MMU）；仅在IPC模块提供**受限的shm映射**。
2. 不支持brk/sbrk（堆大小固定）
3. 不支持malloc_trim等GNU扩展
4. 内存对齐保证取决于k_malloc实现
