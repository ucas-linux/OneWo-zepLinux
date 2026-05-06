# 实现指导

## 总体架构

### 模块组织

```
posix_memory/
├── malloc.c              # malloc/calloc实现
├── free.c                # free实现
├── realloc.c             # realloc实现
├── memory_internal.h     # 内部头文件
└── memory_stats.c        # 统计和调试
```

## 核心实现

### 全局数据结构

```c
// 全局内存链表
static mem_block_t *g_mem_list = NULL;

// 全局锁
static struct k_mutex g_mem_lock;

// 统计信息
#ifdef CONFIG_POSIX_MALLOC_STATS
static malloc_stats_t g_mem_stats = {
    .total_allocated = 0,
    .current_allocated = 0,
    .peak_allocated = 0,
    .num_allocations = 0,
    .num_frees = 0,
    .num_reallocs = 0,
    .failed_allocations = 0
};
#endif
```

### 初始化

```c
int memory_subsystem_init(void)
{
    // 初始化全局锁
    k_mutex_init(&g_mem_lock);

    // 清空链表
    g_mem_list = NULL;

    // 初始化统计
    #ifdef CONFIG_POSIX_MALLOC_STATS
    memset(&g_mem_stats, 0, sizeof(g_mem_stats));
    #endif

    return 0;
}

SYS_INIT(memory_subsystem_init, POST_KERNEL,
         CONFIG_MEMORY_INIT_PRIORITY);
```

## 配置选项

### Kconfig

```kconfig
menuconfig POSIX_MALLOC
    bool "POSIX malloc/free support"
    default y
    help
      Enable POSIX-compliant dynamic memory allocation.

if POSIX_MALLOC

config HEAP_MEM_POOL_SIZE
    int "System heap size"
    default 16384
    range 1024 1048576
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
      Track memory allocation statistics.

config POSIX_MALLOC_FILL
    bool "Fill allocated/freed memory"
    default n
    help
      Fill memory with patterns to help detect use-after-free.

config MEMORY_INIT_PRIORITY
    int "Memory subsystem initialization priority"
    default 40
    help
      Initialization priority for memory subsystem.

endif
```

## 调试支持

### 内存统计

```c
void malloc_stats_print(void)
{
    #ifdef CONFIG_POSIX_MALLOC_STATS
    k_mutex_lock(&g_mem_lock, K_FOREVER);

    printf("=== Memory Statistics ===\n");
    printf("Total allocated:   %zu bytes\n", g_mem_stats.total_allocated);
    printf("Current allocated: %zu bytes\n", g_mem_stats.current_allocated);
    printf("Peak allocated:    %zu bytes\n", g_mem_stats.peak_allocated);
    printf("Allocations:       %zu\n", g_mem_stats.num_allocations);
    printf("Frees:             %zu\n", g_mem_stats.num_frees);
    printf("Reallocs:          %zu\n", g_mem_stats.num_reallocs);
    printf("Failed allocs:     %zu\n", g_mem_stats.failed_allocations);

    k_mutex_unlock(&g_mem_lock);
    #else
    printf("Memory statistics not enabled\n");
    #endif
}
```

### 泄漏检测

```c
void malloc_dump_leaks(void)
{
    k_mutex_lock(&g_mem_lock, K_FOREVER);

    printf("=== Memory Leak Report ===\n");

    mem_block_t *block = g_mem_list;
    int count = 0;
    size_t total_leaked = 0;

    while (block) {
        count++;
        total_leaked += block->size;

        printf("Leak #%d: %zu bytes at %p\n",
               count, block->size, BLOCK_TO_PTR(block));

        #ifdef CONFIG_POSIX_MALLOC_DEBUG
        printf("  Allocated at %s:%d\n", block->file, block->line);
        #endif

        block = block->next;
    }

    if (count == 0) {
        printf("No memory leaks detected!\n");
    } else {
        printf("Total: %d leaks, %zu bytes\n", count, total_leaked);
    }

    k_mutex_unlock(&g_mem_lock);
}
```

## 性能优化

### 快速路径

```c
// 小对象快速分配（使用内存池）
#ifdef CONFIG_POSIX_MALLOC_FAST_PATH
K_MEM_SLAB_DEFINE(small_slab, 32, 64, 4);
K_MEM_SLAB_DEFINE(medium_slab, 128, 32, 4);

void *malloc_fast(size_t size)
{
    if (size <= 32) {
        void *ptr;
        if (k_mem_slab_alloc(&small_slab, &ptr, K_NO_WAIT) == 0) {
            return ptr;
        }
    } else if (size <= 128) {
        void *ptr;
        if (k_mem_slab_alloc(&medium_slab, &ptr, K_NO_WAIT) == 0) {
            return ptr;
        }
    }

    // 回退到普通malloc
    return malloc_slow(size);
}
#endif
```

## 测试框架

```c
#ifdef CONFIG_POSIX_MALLOC_TESTS

void test_malloc_basic(void)
{
    void *ptr = malloc(100);
    zassert_not_null(ptr, "malloc failed");
    free(ptr);
}

void test_realloc_expand(void)
{
    int *arr = malloc(10 * sizeof(int));
    zassert_not_null(arr, "malloc failed");

    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    arr = realloc(arr, 20 * sizeof(int));
    zassert_not_null(arr, "realloc failed");

    for (int i = 0; i < 10; i++) {
        zassert_equal(arr[i], i, "data corrupted");
    }

    free(arr);
}

ZTEST_SUITE(memory_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(memory_tests, test_basic) { test_malloc_basic(); }
ZTEST(memory_tests, test_realloc) { test_realloc_expand(); }

#endif
```

## 常见问题

### Q: 如何增加堆大小？
A: 修改CONFIG_HEAP_MEM_POOL_SIZE配置

### Q: 如何检测内存泄漏？
A: 启用CONFIG_POSIX_MALLOC_DEBUG并调用malloc_dump_leaks()

### Q: malloc失败怎么办？
A: 检查堆大小配置，减少内存使用，或使用内存池

### Q: 如何优化性能？
A: 对于频繁的固定大小分配，使用k_mem_slab

## 最佳实践

1. **配置合适的堆大小**: 根据应用需求设置CONFIG_HEAP_MEM_POOL_SIZE
2. **启用调试**: 开发时启用CONFIG_POSIX_MALLOC_DEBUG
3. **检查返回值**: 总是检查malloc返回值
4. **及时释放**: 不再使用的内存立即释放
5. **使用内存池**: 频繁分配固定大小时使用k_mem_slab
6. **定期检查**: 定期调用malloc_stats_print()和malloc_dump_leaks()
