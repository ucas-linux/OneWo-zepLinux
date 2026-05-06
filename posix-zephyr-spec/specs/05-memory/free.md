# free - 释放内存

## POSIX标准定义

```c
#include <stdlib.h>

void free(void *ptr);
```

## 功能描述

释放之前由malloc、calloc或realloc分配的内存块。

## 参数说明

- **ptr**: 指向要释放的内存块的指针，或NULL

## 返回值

无返回值（void函数）

## 特殊情况

- **ptr为NULL**: 无操作，安全返回
- **ptr无效**: 未定义行为（可能崩溃）
- **重复释放**: 未定义行为（double free）

## Zephyr适配方案

### 实现策略

```c
void free(void *ptr)
{
    // NULL指针安全返回
    if (!ptr) {
        return;
    }

    // 获取内存块头部
    mem_block_t *block = (mem_block_t *)ptr - 1;

    // 验证魔数（检测损坏和double free）
    if (block->magic != MEM_MAGIC) {
        #ifdef CONFIG_POSIX_MALLOC_DEBUG
        printf("ERROR: Invalid free at %p (magic=0x%08x)\n",
               ptr, block->magic);
        k_panic();
        #endif
        return;
    }

    // 清除魔数（防止double free）
    uint32_t old_magic = block->magic;
    block->magic = 0;

    // 从全局链表移除
    k_mutex_lock(&g_mem_lock, K_FOREVER);

    if (block->prev) {
        block->prev->next = block->next;
    } else {
        g_mem_list = block->next;
    }

    if (block->next) {
        block->next->prev = block->prev;
    }

    // 更新统计
    #ifdef CONFIG_POSIX_MALLOC_STATS
    g_mem_stats.current_allocated -= block->size;
    g_mem_stats.num_frees++;
    #endif

    k_mutex_unlock(&g_mem_lock);

    // 释放到Zephyr堆
    k_free(block);
}
```

### 调试版本

```c
#ifdef CONFIG_POSIX_MALLOC_DEBUG

void free_debug(void *ptr, const char *file, int line)
{
    if (!ptr) {
        return;
    }

    mem_block_t *block = (mem_block_t *)ptr - 1;

    if (block->magic != MEM_MAGIC) {
        printf("ERROR: Invalid free at %p (%s:%d)\n", ptr, file, line);
        printf("  Allocated at %s:%d\n", block->file, block->line);
        printf("  Magic: 0x%08x (expected 0x%08x)\n",
               block->magic, MEM_MAGIC);
        k_panic();
        return;
    }

    printf("[FREE] %zu bytes at %p (%s:%d)\n",
           block->size, ptr, file, line);
    printf("  Originally allocated at %s:%d\n",
           block->file, block->line);

    free_internal(ptr);
}

#define free(ptr) free_debug(ptr, __FILE__, __LINE__)

#endif
```

### Double Free检测

```c
void free_with_double_check(void *ptr)
{
    if (!ptr) {
        return;
    }

    mem_block_t *block = (mem_block_t *)ptr - 1;

    // 检查魔数
    if (block->magic == 0) {
        // 魔数为0，可能是double free
        printf("ERROR: Double free detected at %p\n", ptr);
        k_panic();
        return;
    }

    if (block->magic != MEM_MAGIC) {
        // 魔数错误，内存损坏
        printf("ERROR: Memory corruption at %p (magic=0x%08x)\n",
               ptr, block->magic);
        k_panic();
        return;
    }

    // 正常释放
    free_internal(ptr);
}
```

## 测试用例

### 测试1：基本释放

```c
void test_basic_free(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    memset(ptr, 0xDD, 100);

    free(ptr);

    // ptr现在无效，不应再使用
}
```

### 测试2：NULL指针

```c
void test_free_null(void)
{
    // 应该安全返回，不崩溃
    free(NULL);
}
```

### 测试3：多次分配释放

```c
void test_multiple_alloc_free(void)
{
    for (int i = 0; i < 1000; i++) {
        void *ptr = malloc(100);
        assert(ptr != NULL);
        free(ptr);
    }
}
```

### 测试4：交错分配释放

```c
void test_interleaved_alloc_free(void)
{
    void *ptrs[10];

    // 分配
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc((i + 1) * 10);
        assert(ptrs[i] != NULL);
    }

    // 释放奇数索引
    for (int i = 1; i < 10; i += 2) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }

    // 再次分配
    for (int i = 1; i < 10; i += 2) {
        ptrs[i] = malloc((i + 1) * 10);
        assert(ptrs[i] != NULL);
    }

    // 全部释放
    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }
}
```

### 测试5：Double Free检测

```c
void test_double_free_detection(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    free(ptr);

    // 第二次释放应该被检测到
    #ifdef CONFIG_POSIX_MALLOC_DEBUG
    // 应该触发panic或错误
    free(ptr);  // 不应该到达这里
    #endif
}
```

### 测试6：内存损坏检测

```c
void test_corruption_detection(void)
{
    void *ptr = malloc(100);
    assert(ptr != NULL);

    // 故意破坏头部
    mem_block_t *block = (mem_block_t *)ptr - 1;
    block->magic = 0xBADBAD;

    #ifdef CONFIG_POSIX_MALLOC_DEBUG
    // 应该检测到损坏
    free(ptr);  // 应该触发panic
    #endif
}
```

## 常见错误

### 1. Double Free

```c
// 错误示例
void *ptr = malloc(100);
free(ptr);
free(ptr);  // 错误：重复释放
```

**解决方案**：
```c
void *ptr = malloc(100);
free(ptr);
ptr = NULL;  // 设置为NULL防止误用

if (ptr) {
    free(ptr);  // 安全：不会执行
}
```

### 2. 使用已释放的内存

```c
// 错误示例
void *ptr = malloc(100);
free(ptr);
memset(ptr, 0, 100);  // 错误：使用已释放的内存
```

### 3. 释放栈内存

```c
// 错误示例
int arr[10];
free(arr);  // 错误：不能释放栈内存
```

### 4. 释放部分内存

```c
// 错误示例
void *ptr = malloc(100);
free(ptr + 10);  // 错误：必须释放原始指针
```

### 5. 内存泄漏

```c
// 错误示例
void leak_memory(void)
{
    void *ptr = malloc(100);
    // 忘记调用free
}  // 内存泄漏
```

**解决方案**：
```c
void no_leak(void)
{
    void *ptr = malloc(100);
    if (!ptr) return;

    // 使用内存...

    free(ptr);  // 确保释放
}
```

## 性能考虑

1. **释放开销**: 约5-30微秒
2. **锁竞争**: 多线程环境下可能有竞争
3. **内存合并**: 某些实现会合并相邻空闲块（Zephyr不一定）

## 注意事项

1. **NULL安全**: free(NULL)是安全的，无需检查
2. **只释放一次**: 每个指针只能释放一次
3. **原始指针**: 必须释放malloc/calloc/realloc返回的原始指针
4. **不要使用**: 释放后不要再使用该指针
5. **设置NULL**: 释放后建议设置指针为NULL
6. **栈内存**: 不能释放栈上的变量
7. **静态内存**: 不能释放静态分配的内存

## 最佳实践

### 1. 配对使用

```c
void *ptr = malloc(size);
if (ptr) {
    // 使用ptr
    free(ptr);
}
```

### 2. 释放后置NULL

```c
free(ptr);
ptr = NULL;
```

### 3. RAII模式（使用宏）

```c
#define AUTO_FREE __attribute__((cleanup(auto_free_func)))

static void auto_free_func(void **ptr)
{
    free(*ptr);
}

void example(void)
{
    AUTO_FREE void *ptr = malloc(100);
    // 函数退出时自动释放
}
```

### 4. 错误处理

```c
void *ptr = malloc(size);
if (!ptr) {
    // 处理分配失败
    return -ENOMEM;
}

// 使用ptr...

free(ptr);
```

### 5. 资源管理

```c
typedef struct {
    void *data;
    size_t size;
} buffer_t;

void buffer_init(buffer_t *buf, size_t size)
{
    buf->data = malloc(size);
    buf->size = buf->data ? size : 0;
}

void buffer_destroy(buffer_t *buf)
{
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
}
```

## 调试技巧

### 1. 内存泄漏检测

```c
void check_leaks(void)
{
    k_mutex_lock(&g_mem_lock, K_FOREVER);

    if (g_mem_list) {
        printf("Memory leaks detected:\n");
        mem_block_t *block = g_mem_list;
        while (block) {
            printf("  %zu bytes at %p\n",
                   block->size, (void *)(block + 1));
            block = block->next;
        }
    } else {
        printf("No memory leaks!\n");
    }

    k_mutex_unlock(&g_mem_lock);
}
```

### 2. 填充模式

```c
#ifdef CONFIG_POSIX_MALLOC_DEBUG
void free_debug(void *ptr)
{
    if (!ptr) return;

    mem_block_t *block = (mem_block_t *)ptr - 1;

    // 填充已释放内存（帮助检测use-after-free）
    memset(ptr, 0xDD, block->size);

    free_internal(ptr);
}
#endif
```

## 限制

1. 不支持free_sized（C23新特性）
2. 不支持自定义释放器
3. 无法查询内存是否已释放
4. 不支持内存池的部分释放
