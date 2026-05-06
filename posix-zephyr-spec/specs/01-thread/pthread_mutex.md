# pthread_mutex - 互斥锁

## POSIX标准定义

```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex,
                       const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

// 属性操作
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
```

## 功能描述

互斥锁用于保护共享资源，确保同一时间只有一个线程访问。

## 互斥锁类型

- **PTHREAD_MUTEX_NORMAL**: 普通锁，不检测死锁
- **PTHREAD_MUTEX_RECURSIVE**: 递归锁，同一线程可多次加锁
- **PTHREAD_MUTEX_ERRORCHECK**: 错误检查锁，检测死锁和非法解锁

## 返回值

- **0**: 成功
- **EINVAL**: 无效参数
- **EBUSY**: 锁正在使用中（destroy时）
- **EDEADLK**: 死锁检测（errorcheck类型）
- **EPERM**: 非锁持有者尝试解锁（errorcheck类型）

## Zephyr现有支持

```c
struct k_mutex {
    _wait_q_t wait_q;
    struct k_thread *owner;
    uint32_t lock_count;
    int owner_orig_prio;
};

void k_mutex_init(struct k_mutex *mutex);
int k_mutex_lock(struct k_mutex *mutex, k_timeout_t timeout);
int k_mutex_unlock(struct k_mutex *mutex);
```

**特性**：
- 支持优先级继承
- 支持递归加锁
- 不支持错误检查类型
- 不支持属性配置

## Zephyr适配方案

### 数据结构设计

```c
// 互斥锁属性
typedef struct {
    int type;  // NORMAL, RECURSIVE, ERRORCHECK
    int protocol;  // PTHREAD_PRIO_NONE, PTHREAD_PRIO_INHERIT
} pthread_mutexattr_t;

// 互斥锁
typedef struct {
    struct k_mutex zmutex;           // Zephyr原生互斥锁
    int type;                        // 锁类型
    pthread_t owner;                 // 持有者线程ID（用于错误检查）
    atomic_t lock_count;             // 递归计数
    bool initialized;                // 初始化标志
} pthread_mutex_t;

// 静态初始化宏
#define PTHREAD_MUTEX_INITIALIZER \
    { .initialized = false, .type = PTHREAD_MUTEX_NORMAL }
```

### 实现步骤

#### 1. pthread_mutex_init

```c
int pthread_mutex_init(pthread_mutex_t *mutex,
                       const pthread_mutexattr_t *attr)
{
    if (!mutex) {
        return EINVAL;
    }

    // 初始化Zephyr互斥锁
    k_mutex_init(&mutex->zmutex);

    // 设置类型
    if (attr) {
        mutex->type = attr->type;
    } else {
        mutex->type = PTHREAD_MUTEX_NORMAL;
    }

    // 初始化字段
    mutex->owner = 0;
    atomic_set(&mutex->lock_count, 0);
    mutex->initialized = true;

    return 0;
}
```

#### 2. pthread_mutex_lock

```c
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    if (!mutex) {
        return EINVAL;
    }

    // 延迟初始化
    if (!mutex->initialized) {
        pthread_mutex_init(mutex, NULL);
    }

    pthread_t self = pthread_self();

    // 错误检查：检测死锁
    if (mutex->type == PTHREAD_MUTEX_ERRORCHECK) {
        if (mutex->owner == self) {
            return EDEADLK;  // 同一线程重复加锁
        }
    }

    // 递归锁：允许重复加锁
    if (mutex->type == PTHREAD_MUTEX_RECURSIVE) {
        if (mutex->owner == self) {
            atomic_inc(&mutex->lock_count);
            return 0;
        }
    }

    // 加锁
    int ret = k_mutex_lock(&mutex->zmutex, K_FOREVER);
    if (ret != 0) {
        return EINVAL;
    }

    // 记录持有者
    mutex->owner = self;
    atomic_set(&mutex->lock_count, 1);

    // 取消点
    CANCELLATION_POINT();

    return 0;
}
```

#### 3. pthread_mutex_trylock

```c
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    if (!mutex) {
        return EINVAL;
    }

    if (!mutex->initialized) {
        pthread_mutex_init(mutex, NULL);
    }

    pthread_t self = pthread_self();

    // 递归锁：允许重复加锁
    if (mutex->type == PTHREAD_MUTEX_RECURSIVE) {
        if (mutex->owner == self) {
            atomic_inc(&mutex->lock_count);
            return 0;
        }
    }

    // 尝试加锁
    int ret = k_mutex_lock(&mutex->zmutex, K_NO_WAIT);
    if (ret != 0) {
        return EBUSY;  // 锁已被持有
    }

    mutex->owner = self;
    atomic_set(&mutex->lock_count, 1);

    return 0;
}
```

#### 4. pthread_mutex_unlock

```c
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    if (!mutex || !mutex->initialized) {
        return EINVAL;
    }

    pthread_t self = pthread_self();

    // 错误检查：验证持有者
    if (mutex->type == PTHREAD_MUTEX_ERRORCHECK) {
        if (mutex->owner != self) {
            return EPERM;  // 非持有者尝试解锁
        }
    }

    // 递归锁：递减计数
    if (mutex->type == PTHREAD_MUTEX_RECURSIVE) {
        if (mutex->owner == self) {
            int count = atomic_dec(&mutex->lock_count);
            if (count > 0) {
                return 0;  // 仍持有锁
            }
        }
    }

    // 清除持有者
    mutex->owner = 0;
    atomic_set(&mutex->lock_count, 0);

    // 解锁
    int ret = k_mutex_unlock(&mutex->zmutex);
    if (ret != 0) {
        return EINVAL;
    }

    return 0;
}
```

#### 5. pthread_mutex_destroy

```c
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    if (!mutex || !mutex->initialized) {
        return EINVAL;
    }

    // 检查是否被持有
    if (mutex->owner != 0) {
        return EBUSY;
    }

    mutex->initialized = false;
    return 0;
}
```

### 属性操作

```c
int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    if (!attr) {
        return EINVAL;
    }

    attr->type = PTHREAD_MUTEX_NORMAL;
    attr->protocol = PTHREAD_PRIO_INHERIT;
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if (!attr) {
        return EINVAL;
    }

    if (type != PTHREAD_MUTEX_NORMAL &&
        type != PTHREAD_MUTEX_RECURSIVE &&
        type != PTHREAD_MUTEX_ERRORCHECK) {
        return EINVAL;
    }

    attr->type = type;
    return 0;
}
```

## 测试用例

### 测试1：基本加锁解锁

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_counter = 0;

void *increment_thread(void *arg) {
    for (int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);
        shared_counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void test_basic_mutex(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment_thread, NULL);
    pthread_create(&t2, NULL, increment_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(shared_counter == 2000);
}
```

### 测试2：递归锁

```c
void test_recursive_mutex(void) {
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);

    // 多次加锁
    assert(pthread_mutex_lock(&mutex) == 0);
    assert(pthread_mutex_lock(&mutex) == 0);
    assert(pthread_mutex_lock(&mutex) == 0);

    // 多次解锁
    assert(pthread_mutex_unlock(&mutex) == 0);
    assert(pthread_mutex_unlock(&mutex) == 0);
    assert(pthread_mutex_unlock(&mutex) == 0);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);
}
```

### 测试3：错误检查锁

```c
void test_errorcheck_mutex(void) {
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &attr);

    // 加锁
    assert(pthread_mutex_lock(&mutex) == 0);

    // 重复加锁应失败
    assert(pthread_mutex_lock(&mutex) == EDEADLK);

    // 解锁
    assert(pthread_mutex_unlock(&mutex) == 0);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);
}
```

## 性能考虑

1. **加锁开销**：约5-20微秒（无竞争时）
2. **竞争开销**：取决于等待时间
3. **优先级继承**：Zephyr自动支持，防止优先级反转

## 注意事项

1. 必须配对使用lock和unlock
2. 不要在持有锁时调用可能阻塞的函数
3. 避免死锁：按固定顺序获取多个锁
4. 使用ERRORCHECK类型帮助调试
5. 递归锁有性能开销，仅在必要时使用
