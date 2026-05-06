# malloc/calloc - 动态内存分配

## POSIX标准定义

```c
#include <stdlib.h>

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
```

## 功能描述

- **malloc**: 分配指定大小的内存块
- **calloc**: 分配数组内存并初始化为0

## 参数说明

### malloc
- **size**: 要分配的字节数

### calloc
- **nmemb**: 数组元素数量
- **size**: 每个元素的字节数

## 返回值

- **成功**: 返回指向分配内存的指针
- **失败**: 返回NULL

## Zephyr适配方案

### 实现策略

使用Zephyr的k_malloc作为底层，添加元数据头部用于支持realloc和调试：

```c
// 内存块头部
typedef struct mem_block {
    size_t size;                    // 用户请求的大小
    uint32_t magic;                 // 魔数（用于检测损坏）
    #ifdef CONFIG_POSIX_MALLOC_DEBUG
    const char *file;               // 分配位置（文件）
    int line;                       // 分配位置（行号）
    #endif
    struct mem_block *next;         // 全局链表
    struct mem_block *prev;         // 双向链表
} mem_block_t;

#define MEM_MAGIC 0xDEADBEEF
```

### malloc实现

```c
void *malloc(size_t size)
{
    if (size == 0) {
        return NULL;  // POSIX允许返回NULL或唯一指针
    }

    // 计算总大小（头部 + 用户数据）
    size_t total_size = sizeof(mem_block_t) + size;

    // 从Zephyr堆分配
    mem_block_t *block = k_malloc(total_size);
    if (!block) {
        errno = ENOMEM;
        return NULL;
    }

    // 初始化头部
    block->size = size;
    block->magic = MEM_MAGIC;
    block->next = NULL;
    block->prev = NULL;

    #ifdef CONFIG_POSIX_MALLOC_DEBUG
    block->file = __FILE__;
    block->line = __LINE__;
    #endif

    // 加入全局链表
    k_mutex_lock(&g_mem_lock, K_FOREVER);

    if (g_mem_list) {
        g_mem_list->prev = block;
    }
    block->next = g_mem_list;
    g_mem_list = block;

    // 更新统计
    #ifdef CONFIG_POSIX_MALLOC_STATS
    g_mem_stats.total_allocated += size;
    g_mem_stats.current_allocated += size;
    g_mem_stats.num_allocations++;

    if (g_mem_stats.current_allocated > g_mem_stats.peak_allocated) {
        g_mem_stats.peak_allocated = g_mem_stats.current_allocated;
    }
    #endif

    k_mutex_unlock(&g_mem_lock);

    // 返回用户数据区（跳过头部）
    return (void *)(block + 1);
}
```

### calloc实现

```c
void *calloc(size_t nmemb, size_t size)
{
    // 检查溢出
    if (nmemb != 0 && size > SIZE_MAX / nmemb) {
        errno = ENOMEM;
        return NULL;
    }

    size_t total = nmemb * size;

    // 分配内存
    void *ptr = malloc(total);
    if (!ptr) {
        return NULL;
    }

    // 初始化为0
    memset(ptr, 0, total);

    return ptr;
}
```

### 调试版本

```c
#ifdef CONFIG_POSIX_MALLOC_DEBUG

#define malloc(size) malloc_debug(size, __FILE__, __LINE__)
#define calloc(n, s) calloc_debug(n, s, __FILE__, __LINE__)

void *malloc_debug(size_t size, const char *file, int line)
{
    void *ptr = malloc_internal(size);

    if (ptr) {
        mem_block_t *block = (mem_block_t *)ptr - 1;
        block->file = file;
        block->line = line;

        printf("[MALLOC] %zu bytes at %p (%s:%d)\n",
               size, ptr, file, line);
    }

    return ptr;
}

#endif
```

## 内存对齐

Zephyr的k_malloc保证对齐，通常为：
- 32位系统：4字节对齐
- 64位系统：8字节对齐

如需特定对齐，使用aligned_alloc：

```c
void *aligned_alloc(size_t alignment, size_t size)
{
    if (!is_power_of_two(alignment) || size % alignment != 0) {
        errno = EINVAL;
        return NULL;
    }

    // Zephyr支持对齐分配
    return k_aligned_alloc(alignment, size);
}
```

## 测试用例

### 测试1：基本分配

```c
void test_basic_malloc(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    // 写入数据
    memset(ptr, 0xAA, 100);

    // 验证数据
    uint8_t *bytes = (uint8_t *)ptr;
    for (int i = 0; i < 100; i++) {
        assert(bytes[i] == 0xAA);
    }

    free(ptr);
}
```

### 测试2：calloc初始化

```c
void test_calloc_zero(void)
{
    int *arr = calloc(10, sizeof(int));
    assert(arr != NULL);

    // 验证全部为0
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == 0);
    }

    free(arr);
}
```

### 测试3：大量分配

```c
void test_many_allocations(void)
{
    #define NUM_ALLOCS 100
    void *ptrs[NUM_ALLOCS];

    // 分配
    for (int i = 0; i < NUM_ALLOCS; i++) {
        ptrs[i] = malloc(i + 1);
        assert(ptrs[i] != NULL);
    }

    // 释放
    for (int i = 0; i < NUM_ALLOCS; i++) {
        free(ptrs[i]);
    }
}
```

### 测试4：零大小分配

```c
void test_zero_size(void)
{
    void *ptr = malloc(0);
    // POSIX允许返回NULL或唯一指针

    if (ptr) {
        free(ptr);  // 必须能安全释放
    }
}
```

### 测试5：内存损坏检测

```c
void test_corruption_detection(void)
{
    void *ptr = malloc(100);

    // 故意破坏头部
    mem_block_t *block = (mem_block_t *)ptr - 1;
    block->magic = 0xBADBAD;

    // free应该检测到损坏
    free(ptr);  // 应该触发panic或错误
}
```

## 性能考虑

1. **分配开销**: 约10-50微秒（取决于堆大小和碎片）
2. **头部开销**: 每个分配额外16-32字节
3. **锁竞争**: 多线程环境下可能有竞争
4. **碎片问题**: 频繁分配/释放可能导致碎片

### 优化建议

```c
// 对于频繁的固定大小分配，使用内存池
K_MEM_SLAB_DEFINE(my_slab, 64, 100, 4);

void *fast_alloc(void)
{
    void *ptr;
    k_mem_slab_alloc(&my_slab, &ptr, K_NO_WAIT);
    return ptr;
}

void fast_free(void *ptr)
{
    k_mem_slab_free(&my_slab, ptr);
}
```

## 注意事项

1. **堆大小**: 需要配置CONFIG_HEAP_MEM_POOL_SIZE
2. **内存碎片**: 长时间运行可能导致碎片
3. **线程安全**: malloc/free是线程安全的
4. **中断上下文**: 不能在中断中调用
5. **对齐保证**: 返回的指针适合任何基本类型
6. **NULL检查**: 总是检查返回值

## 限制

1. 不支持malloc_usable_size（无法查询实际分配大小）
2. 不支持malloc_trim（无法释放未使用的堆内存）
3. 堆大小固定，不能动态增长
4. 不支持多个独立堆（所有分配共享一个堆）
5. 内存碎片无法完全避免
