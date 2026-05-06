# pthread_join - 等待线程结束

## POSIX标准定义

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

## 功能描述

等待指定线程终止，并获取其返回值。调用线程将阻塞直到目标线程退出。

## 参数说明

- **thread**: 要等待的线程ID
- **retval**: 输出参数，接收线程返回值。如果为NULL，则忽略返回值

## 返回值

- **0**: 成功
- **EDEADLK**: 检测到死锁（线程等待自己）
- **EINVAL**: 线程不是joinable状态或无效的线程ID
- **ESRCH**: 找不到指定的线程

## Linux标准行为

1. 只能对joinable线程调用pthread_join
2. 每个线程只能被join一次
3. 多个线程不能同时join同一个线程
4. 线程不能join自己
5. join成功后，线程资源被回收
6. 如果目标线程已经退出，join立即返回

## Zephyr现有支持

Zephyr提供线程终止等待API：

```c
int k_thread_join(struct k_thread *thread, k_timeout_t timeout);
```

**差异**：
- Zephyr的k_thread_join不返回线程退出值
- Zephyr支持超时等待
- Zephyr线程退出后立即释放资源

## Zephyr适配方案

### 核心挑战

1. **保存退出值**：Zephyr线程退出后不保留返回值
2. **防止重复join**：需要跟踪join状态
3. **资源延迟释放**：线程退出后需要保持控制块直到被join

### 数据结构扩展

```c
typedef struct posix_thread {
    struct k_thread zephyr_thread;
    pthread_t pthread_id;
    void *(*start_routine)(void *);
    void *arg;
    void *retval;                    // 保存返回值
    struct k_sem join_sem;           // join同步信号量
    bool detached;                   // 是否分离
    bool joined;                     // 是否已被join
    bool terminated;                 // 是否已终止
    atomic_t join_count;             // join计数（防止多次join）
    void *stack;
    size_t stack_size;
} posix_thread_t;
```

### 实现步骤

#### 1. 参数验证

```c
int pthread_join(pthread_t thread, void **retval)
{
    // 查找线程控制块
    posix_thread_t *pt = find_thread_by_id(thread);
    if (!pt) {
        return ESRCH;  // 线程不存在
    }

    // 检查是否是detached状态
    if (pt->detached) {
        return EINVAL;  // 不能join detached线程
    }

    // 检查是否是自己
    pthread_t self = pthread_self();
    if (thread == self) {
        return EDEADLK;  // 不能join自己
    }

    // 检查是否已经被join
    if (atomic_inc(&pt->join_count) > 1) {
        atomic_dec(&pt->join_count);
        return EINVAL;  // 已经被join过
    }

    // 继续实现...
}
```

#### 2. 等待线程退出

```c
// 等待线程退出信号量
int ret = k_sem_take(&pt->join_sem, K_FOREVER);
if (ret != 0) {
    atomic_dec(&pt->join_count);
    return EINVAL;
}
```

#### 3. 获取返回值

```c
// 获取线程返回值
if (retval) {
    *retval = pt->retval;
}
```

#### 4. 标记已join并清理

```c
// 标记为已join
pt->joined = true;

// 通知线程包装器可以清理
// 线程包装器会检查joined标志并释放资源

return 0;
```

### 线程包装器配合

线程入口包装函数需要配合实现：

```c
static void pthread_entry_wrapper(void *p1, void *p2, void *p3)
{
    posix_thread_t *pt = (posix_thread_t *)p1;

    // 调用用户线程函数
    void *retval = pt->start_routine(pt->arg);

    // 保存返回值
    pt->retval = retval;
    pt->terminated = true;

    // 如果是joinable，触发join信号量
    if (!pt->detached) {
        k_sem_give(&pt->join_sem);

        // 等待被join
        while (!pt->joined) {
            k_sleep(K_MSEC(10));
        }
    }

    // 清理资源
    cleanup_thread_resources(pt);
}
```

### 资源清理函数

```c
static void cleanup_thread_resources(posix_thread_t *pt)
{
    // 从全局线程表移除
    unregister_thread(pt);

    // 从资源跟踪表移除
    resource_unregister(pt);

    // 释放栈内存
    if (pt->stack) {
        k_free(pt->stack);
        pt->stack = NULL;
    }

    // 释放线程控制块
    free_posix_thread(pt);
}
```

## 完整实现

```c
int pthread_join(pthread_t thread, void **retval)
{
    // 1. 查找线程
    posix_thread_t *pt = find_thread_by_id(thread);
    if (!pt) {
        return ESRCH;
    }

    // 2. 验证状态
    if (pt->detached) {
        return EINVAL;
    }

    pthread_t self = pthread_self();
    if (thread == self) {
        return EDEADLK;
    }

    // 3. 防止重复join
    if (atomic_inc(&pt->join_count) > 1) {
        atomic_dec(&pt->join_count);
        return EINVAL;
    }

    // 4. 等待线程退出
    int ret = k_sem_take(&pt->join_sem, K_FOREVER);
    if (ret != 0) {
        atomic_dec(&pt->join_count);
        return EINVAL;
    }

    // 5. 获取返回值
    if (retval) {
        *retval = pt->retval;
    }

    // 6. 标记已join
    pt->joined = true;

    return 0;
}
```

## 测试用例

### 测试1：基本join

```c
void *thread_func(void *arg) {
    int value = *(int *)arg;
    return (void *)(intptr_t)(value * 2);
}

void test_basic_join(void) {
    pthread_t thread;
    int arg = 21;

    pthread_create(&thread, NULL, thread_func, &arg);

    void *retval;
    int ret = pthread_join(thread, &retval);

    assert(ret == 0);
    assert((intptr_t)retval == 42);
}
```

### 测试2：忽略返回值

```c
void test_join_no_retval(void) {
    pthread_t thread;

    pthread_create(&thread, NULL, thread_func, NULL);

    int ret = pthread_join(thread, NULL);
    assert(ret == 0);
}
```

### 测试3：join detached线程（应失败）

```c
void test_join_detached(void) {
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, thread_func, NULL);
    pthread_attr_destroy(&attr);

    void *retval;
    int ret = pthread_join(thread, &retval);

    assert(ret == EINVAL);  // 不能join detached线程
}
```

### 测试4：重复join（应失败）

```c
void test_double_join(void) {
    pthread_t thread;

    pthread_create(&thread, NULL, thread_func, NULL);

    void *retval;
    int ret1 = pthread_join(thread, &retval);
    assert(ret1 == 0);

    int ret2 = pthread_join(thread, &retval);
    assert(ret2 == EINVAL);  // 不能重复join
}
```

### 测试5：join自己（应失败）

```c
void *self_join_thread(void *arg) {
    pthread_t self = pthread_self();
    int ret = pthread_join(self, NULL);
    assert(ret == EDEADLK);
    return NULL;
}

void test_self_join(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, self_join_thread, NULL);
    pthread_join(thread, NULL);
}
```

## 性能考虑

1. **阻塞时间**：取决于目标线程执行时间
2. **信号量开销**：约1-10微秒
3. **资源清理**：约50-200微秒

## 注意事项

1. 必须对每个joinable线程调用pthread_join或pthread_detach
2. 不调用会导致资源泄漏（线程控制块和栈内存）
3. join操作会阻塞调用线程
4. 不支持超时join（与POSIX标准一致）
5. 线程退出后，其ID可能被重用
