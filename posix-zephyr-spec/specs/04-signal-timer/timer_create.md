# timer_create - 创建POSIX定时器

## POSIX标准定义

```c
#include <signal.h>
#include <time.h>

int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
int timer_delete(timer_t timerid);
int timer_settime(timer_t timerid, int flags,
                  const struct itimerspec *new_value,
                  struct itimerspec *old_value);
int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
int timer_getoverrun(timer_t timerid);
```

## 功能描述

- **timer_create**: 创建一个POSIX定时器
- **timer_delete**: 删除定时器
- **timer_settime**: 设置定时器的到期时间
- **timer_gettime**: 获取定时器的剩余时间
- **timer_getoverrun**: 获取定时器溢出次数

## 参数说明

### timer_create
- **clockid**: 时钟类型（CLOCK_REALTIME, CLOCK_MONOTONIC等）
- **sevp**: 通知方式（信号或线程）
- **timerid**: 输出参数，返回定时器ID

### timer_settime
- **timerid**: 定时器ID
- **flags**: 0表示相对时间，TIMER_ABSTIME表示绝对时间
- **new_value**: 新的定时器值
- **old_value**: 输出参数，返回旧值

## 返回值

- **0**: 成功
- **-1**: 失败，设置errno

## 数据结构

```c
// 定时器ID类型
typedef void *timer_t;

// 时间规格
struct itimerspec {
    struct timespec it_interval;  // 周期间隔
    struct timespec it_value;     // 初始到期时间
};

// 信号事件
struct sigevent {
    int sigev_notify;             // 通知类型
    int sigev_signo;              // 信号编号
    union sigval sigev_value;     // 传递的值
    void (*sigev_notify_function)(union sigval);  // 线程函数
    pthread_attr_t *sigev_notify_attributes;      // 线程属性
};

// 通知类型
#define SIGEV_NONE    0  // 无通知
#define SIGEV_SIGNAL  1  // 发送信号
#define SIGEV_THREAD  2  // 创建线程

// 信号值
union sigval {
    int sival_int;
    void *sival_ptr;
};
```

## Zephyr适配方案

### 核心挑战

1. **定时器精度**: 需要支持纳秒级精度
2. **信号集成**: 定时器到期时发送信号
3. **线程通知**: 支持SIGEV_THREAD方式
4. **多定时器**: 管理多个并发定时器

### 实现策略

使用Zephyr的k_timer作为底层定时器，封装POSIX接口：

```c
// POSIX定时器控制块
typedef struct posix_timer {
    struct k_timer ktimer;           // Zephyr定时器
    timer_t id;                      // 定时器ID
    clockid_t clockid;               // 时钟类型
    struct sigevent sevp;            // 通知方式
    struct itimerspec spec;          // 定时器规格
    pid_t owner_pid;                 // 所有者进程
    atomic_t overrun;                // 溢出计数
    bool active;                     // 是否激活
    struct posix_timer *next;        // 链表指针
} posix_timer_t;
```

### 实现步骤

#### 1. timer_create实现

```c
int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid)
{
    // 验证参数
    if (!timerid) {
        errno = EINVAL;
        return -1;
    }

    if (clockid != CLOCK_REALTIME && clockid != CLOCK_MONOTONIC) {
        errno = EINVAL;
        return -1;
    }

    // 分配定时器控制块
    posix_timer_t *timer = alloc_posix_timer();
    if (!timer) {
        errno = EAGAIN;
        return -1;
    }

    // 初始化字段
    timer->id = (timer_t)timer;
    timer->clockid = clockid;
    timer->owner_pid = getpid();
    atomic_set(&timer->overrun, 0);
    timer->active = false;

    // 设置通知方式
    if (sevp) {
        timer->sevp = *sevp;
    } else {
        // 默认：发送SIGALRM信号
        timer->sevp.sigev_notify = SIGEV_SIGNAL;
        timer->sevp.sigev_signo = SIGALRM;
        timer->sevp.sigev_value.sival_ptr = timer;
    }

    // 初始化Zephyr定时器
    k_timer_init(&timer->ktimer, timer_expiry_handler, NULL);
    k_timer_user_data_set(&timer->ktimer, timer);

    // 注册定时器
    register_timer(timer);

    // 返回定时器ID
    *timerid = timer->id;

    return 0;
}
```

#### 2. 定时器到期处理

```c
static void timer_expiry_handler(struct k_timer *ktimer)
{
    posix_timer_t *timer = k_timer_user_data_get(ktimer);
    if (!timer) return;

    // 增加溢出计数（如果信号还未处理）
    atomic_inc(&timer->overrun);

    // 根据通知类型处理
    switch (timer->sevp.sigev_notify) {
    case SIGEV_NONE:
        // 无通知
        break;

    case SIGEV_SIGNAL:
        // 发送信号
        send_timer_signal(timer);
        break;

    case SIGEV_THREAD:
        // 创建线程执行回调
        create_timer_thread(timer);
        break;
    }
}
```

#### 3. 发送定时器信号

```c
static void send_timer_signal(posix_timer_t *timer)
{
    // 构造siginfo
    siginfo_t info;
    memset(&info, 0, sizeof(info));
    info.si_signo = timer->sevp.sigev_signo;
    info.si_code = SI_TIMER;
    info.si_value = timer->sevp.sigev_value;
    info.si_timerid = timer->id;
    info.si_overrun = atomic_get(&timer->overrun);

    // 重置溢出计数
    atomic_set(&timer->overrun, 0);

    // 发送信号到所有者进程
    kill(timer->owner_pid, timer->sevp.sigev_signo);
}
```

#### 4. 创建定时器线程

```c
static void timer_thread_entry(void *p1, void *p2, void *p3)
{
    posix_timer_t *timer = (posix_timer_t *)p1;

    // 调用用户回调函数
    if (timer->sevp.sigev_notify_function) {
        timer->sevp.sigev_notify_function(timer->sevp.sigev_value);
    }
}

static void create_timer_thread(posix_timer_t *timer)
{
    pthread_t thread;
    pthread_attr_t attr;

    // 使用用户提供的属性或默认属性
    if (timer->sevp.sigev_notify_attributes) {
        attr = *timer->sevp.sigev_notify_attributes;
    } else {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }

    // 创建线程
    pthread_create(&thread, &attr,
                   (void *(*)(void *))timer_thread_entry,
                   timer);
}
```

#### 5. timer_settime实现

```c
int timer_settime(timer_t timerid, int flags,
                  const struct itimerspec *new_value,
                  struct itimerspec *old_value)
{
    if (!timerid || !new_value) {
        errno = EINVAL;
        return -1;
    }

    posix_timer_t *timer = (posix_timer_t *)timerid;

    // 验证时间值
    if (new_value->it_value.tv_nsec < 0 ||
        new_value->it_value.tv_nsec >= 1000000000 ||
        new_value->it_interval.tv_nsec < 0 ||
        new_value->it_interval.tv_nsec >= 1000000000) {
        errno = EINVAL;
        return -1;
    }

    // 保存旧值
    if (old_value) {
        *old_value = timer->spec;
    }

    // 停止当前定时器
    k_timer_stop(&timer->ktimer);

    // 保存新规格
    timer->spec = *new_value;

    // 如果it_value为0，表示停止定时器
    if (new_value->it_value.tv_sec == 0 &&
        new_value->it_value.tv_nsec == 0) {
        timer->active = false;
        return 0;
    }

    // 计算超时时间
    uint64_t initial_ms = timespec_to_ms(&new_value->it_value);
    uint64_t period_ms = timespec_to_ms(&new_value->it_interval);

    // 如果是绝对时间，需要转换为相对时间
    if (flags & TIMER_ABSTIME) {
        struct timespec now;
        clock_gettime(timer->clockid, &now);
        uint64_t now_ms = timespec_to_ms(&now);

        if (initial_ms > now_ms) {
            initial_ms -= now_ms;
        } else {
            initial_ms = 0;
        }
    }

    // 启动定时器
    if (period_ms > 0) {
        // 周期定时器
        k_timer_start(&timer->ktimer, K_MSEC(initial_ms), K_MSEC(period_ms));
    } else {
        // 一次性定时器
        k_timer_start(&timer->ktimer, K_MSEC(initial_ms), K_NO_WAIT);
    }

    timer->active = true;
    atomic_set(&timer->overrun, 0);

    return 0;
}
```

#### 6. timer_gettime实现

```c
int timer_gettime(timer_t timerid, struct itimerspec *curr_value)
{
    if (!timerid || !curr_value) {
        errno = EINVAL;
        return -1;
    }

    posix_timer_t *timer = (posix_timer_t *)timerid;

    if (!timer->active) {
        // 定时器未激活，返回0
        memset(curr_value, 0, sizeof(*curr_value));
        return 0;
    }

    // 获取剩余时间
    uint32_t remaining_ms = k_timer_remaining_get(&timer->ktimer);

    // 转换为timespec
    curr_value->it_value.tv_sec = remaining_ms / 1000;
    curr_value->it_value.tv_nsec = (remaining_ms % 1000) * 1000000;

    // 返回周期
    curr_value->it_interval = timer->spec.it_interval;

    return 0;
}
```

#### 7. timer_delete实现

```c
int timer_delete(timer_t timerid)
{
    if (!timerid) {
        errno = EINVAL;
        return -1;
    }

    posix_timer_t *timer = (posix_timer_t *)timerid;

    // 停止定时器
    k_timer_stop(&timer->ktimer);

    // 从定时器表移除
    unregister_timer(timer);

    // 释放定时器控制块
    free_posix_timer(timer);

    return 0;
}
```

#### 8. timer_getoverrun实现

```c
int timer_getoverrun(timer_t timerid)
{
    if (!timerid) {
        errno = EINVAL;
        return -1;
    }

    posix_timer_t *timer = (posix_timer_t *)timerid;

    return atomic_get(&timer->overrun);
}
```

### 辅助函数

```c
// timespec转毫秒
static uint64_t timespec_to_ms(const struct timespec *ts)
{
    return (uint64_t)ts->tv_sec * 1000 + ts->tv_nsec / 1000000;
}

// 毫秒转timespec
static void ms_to_timespec(uint64_t ms, struct timespec *ts)
{
    ts->tv_sec = ms / 1000;
    ts->tv_nsec = (ms % 1000) * 1000000;
}
```

## alarm实现

alarm是timer_create的简化版本：

```c
unsigned int alarm(unsigned int seconds)
{
    static timer_t alarm_timer = NULL;
    struct itimerspec new_value, old_value;

    // 如果是第一次调用，创建定时器
    if (!alarm_timer) {
        struct sigevent sevp;
        sevp.sigev_notify = SIGEV_SIGNAL;
        sevp.sigev_signo = SIGALRM;

        if (timer_create(CLOCK_REALTIME, &sevp, &alarm_timer) < 0) {
            return 0;
        }
    }

    // 获取剩余时间
    unsigned int remaining = 0;
    if (timer_gettime(alarm_timer, &old_value) == 0) {
        remaining = old_value.it_value.tv_sec;
    }

    // 设置新的定时器
    if (seconds > 0) {
        memset(&new_value, 0, sizeof(new_value));
        new_value.it_value.tv_sec = seconds;
        timer_settime(alarm_timer, 0, &new_value, NULL);
    } else {
        // seconds=0表示取消定时器
        memset(&new_value, 0, sizeof(new_value));
        timer_settime(alarm_timer, 0, &new_value, NULL);
    }

    return remaining;
}
```

## 测试用例

### 测试1：一次性定时器

```c
volatile int timer_fired = 0;

void timer_handler(int sig)
{
    timer_fired = 1;
}

void test_oneshot_timer(void)
{
    signal(SIGALRM, timer_handler);

    timer_t timerid;
    struct sigevent sevp;
    sevp.sigev_notify = SIGEV_SIGNAL;
    sevp.sigev_signo = SIGALRM;

    timer_create(CLOCK_REALTIME, &sevp, &timerid);

    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);

    sleep(2);

    assert(timer_fired == 1);

    timer_delete(timerid);
}
```

### 测试2：周期定时器

```c
volatile int timer_count = 0;

void periodic_handler(int sig)
{
    timer_count++;
}

void test_periodic_timer(void)
{
    signal(SIGALRM, periodic_handler);

    timer_t timerid;
    struct sigevent sevp;
    sevp.sigev_notify = SIGEV_SIGNAL;
    sevp.sigev_signo = SIGALRM;

    timer_create(CLOCK_REALTIME, &sevp, &timerid);

    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 500000000;  // 500ms
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 500000000;

    timer_settime(timerid, 0, &its, NULL);

    sleep(2);

    assert(timer_count >= 3 && timer_count <= 5);

    timer_delete(timerid);
}
```

### 测试3：线程通知

```c
volatile int thread_called = 0;

void timer_thread_func(union sigval val)
{
    thread_called = 1;
}

void test_thread_timer(void)
{
    timer_t timerid;
    struct sigevent sevp;
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_notify_function = timer_thread_func;
    sevp.sigev_notify_attributes = NULL;

    timer_create(CLOCK_REALTIME, &sevp, &timerid);

    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);

    sleep(2);

    assert(thread_called == 1);

    timer_delete(timerid);
}
```

## 性能考虑

1. **定时器精度**: 受Zephyr系统时钟频率限制（通常1-10ms）
2. **定时器数量**: 每个定时器占用约100-200字节
3. **信号开销**: 每次到期约50-200微秒
4. **线程创建**: SIGEV_THREAD方式开销较大（约1-5ms）

## 注意事项

1. **精度限制**: 纳秒级精度实际受限于系统时钟
2. **信号安全**: 信号处理函数中只能调用异步信号安全的函数
3. **资源清理**: 必须调用timer_delete释放定时器
4. **溢出处理**: 高频定时器可能导致信号溢出
5. **线程属性**: SIGEV_THREAD创建的线程是分离的

## 限制

1. 定时器精度受限于CONFIG_SYS_CLOCK_TICKS_PER_SEC
2. 最大定时器数量受CONFIG_POSIX_TIMER_MAX限制
3. SIGEV_THREAD方式有额外开销
4. 不支持CLOCK_PROCESS_CPUTIME_ID等特殊时钟
5. timer_getoverrun可能不准确（信号合并）
