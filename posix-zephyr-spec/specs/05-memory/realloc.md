# realloc - 重新分配内存

## POSIX标准定义

```c
#include <stdlib.h>

void *realloc(void *ptr, size_t size);
```

## 功能描述

改变已分配内存块的大小。可以扩大或缩小内存块，必要时会移动内存块。

## 参数说明

- **ptr**: 之前由malloc/calloc/realloc分配的指针，或NULL
- **size**: 新的大小（字节）

## 返回值

- **成功**: 返回指向重新分配内存的指针
- **失败**: 返回NULL，原内存块保持不变

## 特殊情况

| ptr | size | 行为 |
|-----|------|------|
| NULL | >0 | 等同于malloc(size) |
| 非NULL | 0 | 等同于free(ptr)，返回NULL |
| 非NULL | >0 | 重新分配 |

## Zephyr适配方案

### 核心挑战

Zephyr的k_malloc不支持原地扩展，realloc必须：
1. 分配新内存
2. 拷贝旧数据
3. 释放旧内存

### 实现策略

```c
void *realloc(void *ptr, size_t size)
{
    // 情况1：ptr为NULL，等同于malloc
    if (!ptr) {
        return malloc(size);
    }

    // 情况2：size为0，等同于free
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    // 获取旧内存块头部
    mem_block_t *old_block = (mem_block_t *)ptr - 1;

    // 验证魔数
    if (old_block->magic != MEM_MAGIC) {
        errno = EINVAL;
        return NULL;
    }

    size_t old_size = old_block->size;

    // 情况3：新大小与旧大小相同或更小
    if (size <= old_size) {
        // 简单实现：直接返回原指针
        // 更新记录的大小
        k_mutex_lock(&g_mem_lock, K_FOREVER);
        old_block->size = size;

        #ifdef CONFIG_POSIX_MALLOC_STATS
        g_mem_stats.current_allocated -= (old_size - size);
        #endif

        k_mutex_unlock(&g_mem_lock);

        return ptr;
    }

    // 情况4：需要扩展内存
    // 分配新内存
    void *new_ptr = malloc(size);
    if (!new_ptr) {
        // 分配失败，原内存保持不变
        return NULL;
    }

    // 拷贝旧数据
    memcpy(new_ptr, ptr, old_size);

    // 释放旧内存
    free(ptr);

    return new_ptr;
}
```

### 优化版本

尝试原地扩展（如果可能）：

```c
void *realloc_optimized(void *ptr, size_t size)
{
    if (!ptr) return malloc(size);
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    mem_block_t *old_block = (mem_block_t *)ptr - 1;
    if (old_block->magic != MEM_MAGIC) {
        errno = EINVAL;
        return NULL;
    }

    size_t old_size = old_block->size;

    // 如果缩小，直接返回
    if (size <= old_size) {
        old_block->size = size;
        return ptr;
    }

    // 尝试原地扩展（检查后续内存是否空闲）
    size_t needed = size - old_size;

    k_mutex_lock(&g_mem_lock, K_FOREVER);

    // 检查下一个块是否空闲且足够大
    mem_block_t *next = old_block->next;
    if (next && is_free_block(next) && next->size >= needed) {
        // 可以原地扩展
        merge_blocks(old_block, next);
        old_block->size = size;
        k_mutex_unlock(&g_mem_lock);
        return ptr;
    }

    k_mutex_unlock(&g_mem_lock);

    // 无法原地扩展，分配新内存
    void *new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, old_size);
    free(ptr);

    return new_ptr;
}
```

## 测试用例

### 测试1：扩展内存

```c
void test_realloc_expand(void)
{
    int *arr = malloc(10 * sizeof(int));

    // 初始化数据
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    // 扩展到20个元素
    arr = realloc(arr, 20 * sizeof(int));
    assert(arr != NULL);

    // 验证旧数据保留
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == i);
    }

    // 使用新空间
    for (int i = 10; i < 20; i++) {
        arr[i] = i;
    }

    free(arr);
}
```

### 测试2：缩小内存

```c
void test_realloc_shrink(void)
{
    int *arr = malloc(20 * sizeof(int));

    for (int i = 0; i < 20; i++) {
        arr[i] = i;
    }

    // 缩小到10个元素
    arr = realloc(arr, 10 * sizeof(int));
    assert(arr != NULL);

    // 验证前10个元素保留
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == i);
    }

    free(arr);
}
```

### 测试3：realloc(NULL, size)

```c
void test_realloc_null(void)
{
    // 应该等同于malloc
    void *ptr = realloc(NULL, 100);
    assert(ptr != NULL);

    memset(ptr, 0xBB, 100);

    free(ptr);
}
```

### 测试4：realloc(ptr, 0)

```c
void test_realloc_zero(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    // 应该等同于free
    void *result = realloc(ptr, 0);
    assert(result == NULL);

    // ptr已被释放，不应再使用
}
```

### 测试5：realloc失败

```c
void test_realloc_failure(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    memset(ptr, 0xCC, 100);

    // 尝试分配巨大内存（应该失败）
    void *new_ptr = realloc(ptr, SIZE_MAX);

    // 失败时，原指针应该仍然有效
    assert(new_ptr == NULL);

    // 验证原数据未被破坏
    uint8_t *bytes = (uint8_t *)ptr;
    for (int i = 0; i < 100; i++) {
        assert(bytes[i] == 0xCC);
    }

    free(ptr);
}
```

### 测试6：多次realloc

```c
void test_multiple_realloc(void)
{
    int *arr = malloc(sizeof(int));
    arr[0] = 0;

    // 逐步扩展
    for (int i = 1; i < 100; i++) {
        arr = realloc(arr, (i + 1) * sizeof(int));
        assert(arr != NULL);

        // 验证旧数据
        for (int j = 0; j < i; j++) {
            assert(arr[j] == j);
        }

        arr[i] = i;
    }

    // 验证所有数据
    for (int i = 0; i < 100; i++) {
        assert(arr[i] == i);
    }

    free(arr);
}
```

## 性能考虑

### 最坏情况

```c
// 每次realloc都需要拷贝所有数据
for (int i = 1; i <= 1000; i++) {
    arr = realloc(arr, i * sizeof(int));
}
// 总拷贝量：O(n²)
```

### 优化策略

```c
// 使用倍增策略减少realloc次数
size_t capacity = 10;
size_t size = 0;
int *arr = malloc(capacity * sizeof(int));

void add_element(int value)
{
    if (size >= capacity) {
        // 容量翻倍
        capacity *= 2;
        arr = realloc(arr, capacity * sizeof(int));
    }
    arr[size++] = value;
}
// 总拷贝量：O(n)
```

### 时间复杂度

- **缩小**: O(1)
- **扩展（原地）**: O(1)（如果支持）
- **扩展（移动）**: O(n)，n为旧大小

## 注意事项

1. **检查返回值**: realloc失败时原指针仍有效
2. **避免丢失指针**:
   ```c
   // 错误：如果realloc失败，ptr丢失
   ptr = realloc(ptr, new_size);

   // 正确：使用临时变量
   void *new_ptr = realloc(ptr, new_size);
   if (new_ptr) {
       ptr = new_ptr;
   }
   ```
3. **数据拷贝**: 扩展时会拷贝所有旧数据
4. **指针失效**: realloc可能移动内存，旧指针失效
5. **对齐保持**: 新指针保持适当对齐

## 限制

1. 不支持原地扩展（总是分配新内存）
2. 大块内存realloc开销大（需要拷贝）
3. 频繁realloc可能导致内存碎片
4. 不支持mremap等高级功能

## 最佳实践

1. **预分配**: 如果知道最终大小，一次性分配
2. **倍增策略**: 动态增长时使用容量翻倍
3. **错误处理**: 总是检查realloc返回值
4. **避免频繁调用**: 批量增长而非逐个增长
5. **考虑替代方案**: 对于频繁调整大小，考虑使用链表或其他数据结构
