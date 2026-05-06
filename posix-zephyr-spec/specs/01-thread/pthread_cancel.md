# pthread_cancel - 取消线程

## POSIX标准定义

```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
void pthread_testcancel(void);
```

## 功能描述

- **pthread_cancel**: 向指定线程发送取消请求
- **pthread_setcancelstate**: 设置线程的可取消状态
- **pthread_setcanceltype**: 设置线程的取消类型
- **pthread_testcancel**: 创建取消点

## 参数说明

### pthread_cancel
- **thread**: 要取消的线程ID

### pthread_setcancelstate
- **state**: PTHREAD_CANCEL_ENABLE 或 PTHREAD_CANCEL_DISABLE
- **oldstate**: 输出参数，返回旧状态

### pthread_setcanceltype
- **type**: PTHREAD_CANCEL_DEFERRED 或 PTHREAD_CANCEL_ASYNCHRONOUS
- **oldtype**: 输出参数，返回旧类型

## 返回值

- **0**: 成功
- **ESRCH**: 找不到指定的线程

## Linux标准行为

### 取消状态
- **PTHREAD_CANCEL_ENABLE**: 允许取消（默认）
- **PTHREAD_CANCEL_DISABLE**: 禁止取消

### 取消类型
- **PTHREAD_CANCEL_DEFERRED**: 延迟取消，在取消点响应（默认）
- **PTHREAD_CANCEL_ASYNCHRONOUS**: 异步取消，立即响应（危险）

### 取消点
POSIX定义的取消点包括：
- pthread_cond_wait
- pthread_cond_timedwait
- pthread_join
- sem_wait
- read, write
- sleep, usleep, nanosleep
- 等等

## Zephyr现有支持

Zephyr提供线程终止API：

```c
void k_thread_abort(k_tid_t thread);
```

**差异**：
- k_thread_abort立即强制终止，无法延迟
- 无取消点机制
- 无取消状态和类型控制
- 被终止线程无法清理资源

## Zephyr适配方案

### 核心挑战

1. **安全终止**：避免持有锁时被终止导致死锁
2. **取消点实现**：在阻塞调用中检测取消请求
3. **资源清理**：确保线程退出前释放所有资源
4. **延迟取消**：不能立即终止，需要等待取消点

### 数据结构扩展

```c
typedef struct posix_thread {
    struct k_thread zephyr_thread;
    pthread_t pthread_id;
    void *(*start_routine)(void *);
    void *arg;
    void *retval;
    struct k_sem join_sem;
    bool detached;
    bool joined;

    // 取消相关字段
    atomic_t cancel_requested;      // 取消请求标志
    int cancelstate;                 // ENABLE/DISABLE
    int canceltype;                  // DEFERRED/ASYNCHRONOUS
    struct k_mutex cancel_lock;      // 保护取消状态

    void *stack;
    size_t stack_size;
} posix_thread_t;
```

### 实现步骤

#### 1. pthread_cancel实现

```c
int pthread_cancel(pthread_t thread)
{
    // 查找线程
    posix_thread_t *pt = find_thread_by_id(thread);
    if (!pt) {
        return ESRCH;
    }

    // 设置取消请求标志
    atomic_set(&pt->cancel_requested, 1);

    // 如果是异步取消且启用，立即终止（危险）
    k_mutex_lock(&pt->cancel_lock, K_FOREVER);
    if (pt->cancelstate == PTHREAD_CANCEL_ENABLE &&
        pt->canceltype == PTHREAD_CANCEL_ASYNCHRONOUS) {
        k_mutex_unlock(&pt->cancel_lock);
        // 强制终止（不推荐）
        k_thread_abort(&pt->zephyr_thread);
        return 0;
    }
    k_mutex_unlock(&pt->cancel_lock);

    // 延迟取消：等待线程到达取消点
    return 0;
}
```

#### 2. pthread_setcancelstate实现

```c
int pthread_setcancelstate(int state, int *oldstate)
{
    if (state != PTHREAD_CANCEL_ENABLE &&
        state != PTHREAD_CANCEL_DISABLE) {
        return EINVAL;
    }

    posix_thread_t *pt = get_current_posix_thread();
    if (!pt) {
        return EINVAL;
    }

    k_mutex_lock(&pt->cancel_lock, K_FOREVER);
    if (oldstate) {
        *oldstate = pt->cancelstate;
    }
    pt->cancelstate = state;
    k_mutex_unlock(&pt->cancel_lock);

    return 0;
}
```

#### 3. pthread_setcanceltype实现

```c
int pthread_setcanceltype(int type, int *oldtype)
{
    if (type != PTHREAD_CANCEL_DEFERRED &&
        type != PTHREAD_CANCEL_ASYNCHRONOUS) {
        return EINVAL;
    }

    posix_thread_t *pt = get_current_posix_thread();
    if (!pt) {
        return EINVAL;
    }

    k_mutex_lock(&pt->cancel_lock, K_FOREVER);
    if (oldtype) {
        *oldtype = pt->canceltype;
    }
    pt->canceltype = type;
    k_mutex_unlock(&pt->cancel_lock);

    return 0;
}
```

#### 4. pthread_testcancel实现

```c
void pthread_testcancel(void)
{
    posix_thread_t *pt = get_current_posix_thread();
    if (!pt) {
        return;
    }

    // 检查是否有取消请求
    if (atomic_get(&pt->cancel_requested) == 0) {
        return;
    }

    // 检查取消状态
    k_mutex_lock(&pt->cancel_lock, K_FOREVER);
    int state = pt->cancelstate;
    k_mutex_unlock(&pt->cancel_lock);

    if (state == PTHREAD_CANCEL_ENABLE) {
        // 执行取消：调用pthread_exit
        pthread_exit(PTHREAD_CANCELED);
    }
}
```

### 取消点宏

在所有阻塞调用中插入取消点检查：

```c
#define CANCELLATION_POINT() \
    do { \
        posix_thread_t *__pt = get_current_posix_thread(); \
        if (__pt && atomic_get(&__pt->cancel_requested)) { \
            k_mutex_lock(&__pt->cancel_lock, K_FOREVER); \
            if (__pt->cancelstate == PTHREAD_CANCEL_ENABLE) { \
                k_mutex_unlock(&__pt->cancel_lock); \
                pthread_exit(PTHREAD_CANCELED); \
            } \
            k_mutex_unlock(&__pt->cancel_lock); \
        } \
    } while (0)
```

### 在阻塞函数中使用

```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    // 取消点
    CANCELLATION_POINT();

    // 执行等待
    int ret = k_condvar_wait(&cond->zcond, &mutex->zmutex, K_FOREVER);

    // 返回后再次检查
    CANCELLATION_POINT();

    return ret;
}
```

## 测试用例

### 测试1：基本取消

```c
void *cancellable_thread(void *arg) {
    for (int i = 0; i < 100; i++) {
        pthread_testcancel();  // 取消点
        k_sleep(K_MSEC(100));
    }
    return NULL;
}

void test_basic_cancel(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, cancellable_thread, NULL);

    k_sleep(K_MSEC(500));
    int ret = pthread_cancel(thread);
    assert(ret == 0);

    void *retval;
    pthread_join(thread, &retval);
    assert(retval == PTHREAD_CANCELED);
}
```

### 测试2：禁用取消

```c
void *disable_cancel_thread(void *arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    for (int i = 0; i < 10; i++) {
        pthread_testcancel();
        k_sleep(K_MSEC(100));
    }

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();  // 这里才会响应取消

    return NULL;
}

void test_disable_cancel(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, disable_cancel_thread, NULL);

    k_sleep(K_MSEC(200));
    pthread_cancel(thread);

    void *retval;
    pthread_join(thread, &retval);
    assert(retval == PTHREAD_CANCELED);
}
```

### 测试3：清理处理

```c
void cleanup_handler(void *arg) {
    int *flag = (int *)arg;
    *flag = 1;  // 标记清理已执行
}

void *cleanup_thread(void *arg) {
    int *cleanup_flag = (int *)arg;

    pthread_cleanup_push(cleanup_handler, cleanup_flag);

    for (int i = 0; i < 100; i++) {
        pthread_testcancel();
        k_sleep(K_MSEC(100));
    }

    pthread_cleanup_pop(0);
    return NULL;
}

void test_cleanup(void) {
    pthread_t thread;
    int cleanup_flag = 0;

    pthread_create(&thread, NULL, cleanup_thread, &cleanup_flag);

    k_sleep(K_MSEC(500));
    pthread_cancel(thread);

    pthread_join(thread, NULL);
    assert(cleanup_flag == 1);  // 清理函数应该被调用
}
```

## 性能考虑

1. **取消检查开销**：每个取消点约1-5微秒
2. **原子操作**：cancel_requested使用原子变量，开销极小
3. **锁竞争**：cancel_lock仅在状态变更时使用

## 注意事项

1. **避免异步取消**：PTHREAD_CANCEL_ASYNCHRONOUS非常危险，可能导致死锁
2. **使用延迟取消**：PTHREAD_CANCEL_DEFERRED是安全的默认选择
3. **添加取消点**：在长时间运行的循环中定期调用pthread_testcancel
4. **资源清理**：使用pthread_cleanup_push/pop确保资源释放
5. **不可取消区域**：关键区域使用PTHREAD_CANCEL_DISABLE保护
6. **取消后状态**：被取消线程的返回值为PTHREAD_CANCELED

## 限制

1. 不支持pthread_cleanup_push/pop的完整语义（需要编译器支持）
2. 异步取消不安全，不推荐使用
3. 取消点覆盖可能不完整，需要手动添加pthread_testcancel
