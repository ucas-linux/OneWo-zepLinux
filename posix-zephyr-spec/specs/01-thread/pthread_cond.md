# pthread_cond - 条件变量

## POSIX标准定义

```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond,
                      const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                           const struct timespec *abstime);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```

## 功能描述

条件变量用于线程间同步，允许线程等待特定条件成立。

## 参数说明

- **cond**: 条件变量指针
- **attr**: 条件变量属性（通常为NULL）
- **mutex**: 关联的互斥锁
- **abstime**: 绝对超时时间

## 返回值

- **0**: 成功
- **EINVAL**: 无效参数
- **ETIMEDOUT**: 超时（timedwait）
- **EPERM**: 互斥锁未被持有

## Linux标准行为

1. **原子性**：pthread_cond_wait原子地释放锁并进入等待
2. **虚假唤醒**：wait可能在没有signal的情况下返回
3. **唤醒顺序**：signal唤醒一个等待线程，broadcast唤醒所有
4. **锁重新获取**：wait返回前自动重新获取互斥锁

## Zephyr现有支持

```c
struct k_condvar {
    _wait_q_t wait_q;
};

void k_condvar_init(struct k_condvar *condvar);
int k_condvar_signal(struct k_condvar *condvar);
int k_condvar_broadcast(struct k_condvar *condvar);
int k_condvar_wait(struct k_condvar *condvar, struct k_mutex *mutex,
                   k_timeout_t timeout);
```

**特性**：
- 支持基本的条件变量操作
- 支持超时等待
- 与k_mutex配合使用

## Zephyr适配方案

### 数据结构设计

```c
typedef struct {
    struct k_condvar zcond;
    bool initialized;
} pthread_cond_t;

typedef struct {
    int pshared;  // 进程共享属性（未使用）
} pthread_condattr_t;

#define PTHREAD_COND_INITIALIZER \
    { .initialized = false }
```

### 实现步骤

#### 1. pthread_cond_init

```c
int pthread_cond_init(pthread_cond_t *cond,
                      const pthread_condattr_t *attr)
{
    if (!cond) {
        return EINVAL;
    }

    k_condvar_init(&cond->zcond);
    cond->initialized = true;

    return 0;
}
```

#### 2. pthread_cond_wait

```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    if (!cond || !mutex) {
        return EINVAL;
    }

    // 延迟初始化
    if (!cond->initialized) {
        pthread_cond_init(cond, NULL);
    }

    // 取消点
    CANCELLATION_POINT();

    // 调用Zephyr条件变量等待
    // k_condvar_wait会原子地释放锁并等待
    int ret = k_condvar_wait(&cond->zcond, &mutex->zmutex, K_FOREVER);

    // 返回后再次检查取消
    CANCELLATION_POINT();

    return (ret == 0) ? 0 : EINVAL;
}
```

#### 3. pthread_cond_timedwait

```c
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                           const struct timespec *abstime)
{
    if (!cond || !mutex || !abstime) {
        return EINVAL;
    }

    if (!cond->initialized) {
        pthread_cond_init(cond, NULL);
    }

    // 计算相对超时时间
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    int64_t timeout_ms = (abstime->tv_sec - now.tv_sec) * 1000 +
                         (abstime->tv_nsec - now.tv_nsec) / 1000000;

    if (timeout_ms < 0) {
        return ETIMEDOUT;
    }

    // 取消点
    CANCELLATION_POINT();

    // 等待
    int ret = k_condvar_wait(&cond->zcond, &mutex->zmutex,
                             K_MSEC(timeout_ms));

    CANCELLATION_POINT();

    if (ret == -EAGAIN) {
        return ETIMEDOUT;
    }

    return (ret == 0) ? 0 : EINVAL;
}
```

#### 4. pthread_cond_signal

```c
int pthread_cond_signal(pthread_cond_t *cond)
{
    if (!cond) {
        return EINVAL;
    }

    if (!cond->initialized) {
        return 0;  // 未初始化的条件变量，无操作
    }

    int ret = k_condvar_signal(&cond->zcond);
    return (ret == 0) ? 0 : EINVAL;
}
```

#### 5. pthread_cond_broadcast

```c
int pthread_cond_broadcast(pthread_cond_t *cond)
{
    if (!cond) {
        return EINVAL;
    }

    if (!cond->initialized) {
        return 0;
    }

    int ret = k_condvar_broadcast(&cond->zcond);
    return (ret == 0) ? 0 : EINVAL;
}
```

#### 6. pthread_cond_destroy

```c
int pthread_cond_destroy(pthread_cond_t *cond)
{
    if (!cond || !cond->initialized) {
        return EINVAL;
    }

    // Zephyr条件变量无需显式销毁
    cond->initialized = false;
    return 0;
}
```

## 测试用例

### 测试1：生产者-消费者模型

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int queue[10];
int count = 0;

void *producer(void *arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);

        while (count == 10) {
            pthread_cond_wait(&cond, &mutex);
        }

        queue[count++] = i;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        int value = queue[--count];
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);

        printf("Consumed: %d\n", value);
    }
    return NULL;
}

void test_producer_consumer(void) {
    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
}
```

### 测试2：broadcast唤醒多个线程

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;
int wakeup_count = 0;

void *waiter(void *arg) {
    pthread_mutex_lock(&mutex);

    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }

    wakeup_count++;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void test_broadcast(void) {
    pthread_t threads[5];

    // 创建5个等待线程
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, waiter, NULL);
    }

    k_sleep(K_MSEC(100));  // 确保所有线程都在等待

    // 广播唤醒所有线程
    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    // 等待所有线程
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    assert(wakeup_count == 5);
}
```

### 测试3：超时等待

```c
void test_timedwait(void) {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1;  // 1秒超时

    pthread_mutex_lock(&mutex);
    int ret = pthread_cond_timedwait(&cond, &mutex, &timeout);
    pthread_mutex_unlock(&mutex);

    assert(ret == ETIMEDOUT);
}
```

## 使用模式

### 标准等待模式

```c
pthread_mutex_lock(&mutex);
while (!condition) {
    pthread_cond_wait(&cond, &mutex);
}
// 条件满足，执行操作
pthread_mutex_unlock(&mutex);
```

### 标准信号模式

```c
pthread_mutex_lock(&mutex);
// 修改条件
condition = true;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&mutex);
```

## 性能考虑

1. **等待开销**：约10-50微秒（无竞争时）
2. **唤醒开销**：约5-20微秒
3. **上下文切换**：取决于调度器

## 注意事项

1. **必须持有锁**：调用wait前必须持有关联的互斥锁
2. **使用while循环**：总是在while循环中检查条件，防止虚假唤醒
3. **原子性保证**：wait原子地释放锁并等待
4. **锁重新获取**：wait返回时自动重新获取锁
5. **signal vs broadcast**：signal唤醒一个，broadcast唤醒所有
6. **避免死锁**：确保signal/broadcast在持有锁时调用
