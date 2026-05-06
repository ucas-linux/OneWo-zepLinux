# 信号与定时器接口规范

## 概述

本模块定义8个POSIX信号和定时器接口在Zephyr RTOS上的实现规范。

## 接口列表

1. `signal` - 设置信号处理函数
2. `kill` - 发送信号
3. `alarm` - 设置闹钟
4. `timer_create` - 创建定时器
5. `timer_settime` - 设置定时器
6. `sleep` - 秒级休眠
7. `usleep` - 微秒级休眠
8. `nanosleep` - 纳秒级休眠

## 核心挑战

### 1. 异步信号机制
**问题**：Zephyr无原生信号机制，需要模拟异步通知。

**解决方案**：
- 在线程控制块中扩展信号掩码位图和处理函数表
- kill调用时修改目标线程的栈帧，插入跳转到信号处理函数
- 在取消点和系统调用返回时检查待处理信号

### 2. 定时器精度
**问题**：需要支持纳秒级精度，但Zephyr默认毫秒级。

**解决方案**：
- 使用k_timer实现毫秒级定时
- 使用k_cycle_get_32和忙等实现微秒/纳秒级补偿
- 根据系统时钟频率选择合适的实现策略

### 3. 定时器与信号集成
**问题**：POSIX定时器到期时发送信号，需要集成两个机制。

**解决方案**：
- timer_create时指定信号通知方式
- 定时器回调函数中调用信号分发机制
- 支持SIGEV_SIGNAL和SIGEV_THREAD通知方式

## 数据结构

```c
// 信号处理函数类型
typedef void (*sighandler_t)(int);

// 信号集
typedef struct {
    uint32_t bits[2];  // 支持64个信号
} sigset_t;

// 信号动作
struct sigaction {
    sighandler_t sa_handler;
    sigset_t sa_mask;
    int sa_flags;
};

// 定时器
typedef struct {
    struct k_timer ktimer;
    int signo;              // 到期时发送的信号
    pid_t target_pid;       // 目标进程
    union sigval value;     // 传递的值
    int notify_type;        // SIGEV_SIGNAL/SIGEV_THREAD
} posix_timer_t;
```

## 实现要点

### signal/kill实现

```c
// 设置信号处理函数
sighandler_t signal(int signum, sighandler_t handler)
{
    if (signum < 1 || signum > 64) return SIG_ERR;

    posix_thread_t *self = pthread_self_internal();
    sighandler_t old = self->signal_handlers[signum];
    self->signal_handlers[signum] = handler;

    return old;
}

// 发送信号
int kill(pid_t pid, int sig)
{
    pcb_t *proc = find_process(pid);
    if (!proc) return -ESRCH;

    // 设置待处理信号位
    atomic_or(&proc->pending_signals, (1ULL << sig));

    // 如果目标线程在阻塞调用中，唤醒它
    if (proc->main_thread->blocked) {
        k_wakeup(proc->main_thread);
    }

    return 0;
}
```

### sleep系列实现

```c
// 秒级休眠
unsigned int sleep(unsigned int seconds)
{
    k_sleep(K_SECONDS(seconds));
    return 0;
}

// 微秒级休眠
int usleep(useconds_t usec)
{
    if (usec >= 1000000) return -EINVAL;

    if (usec >= 1000) {
        k_sleep(K_USEC(usec));
    } else {
        // 小于1ms使用忙等
        k_busy_wait(usec);
    }

    return 0;
}

// 纳秒级休眠
int nanosleep(const struct timespec *req, struct timespec *rem)
{
    if (!req || req->tv_nsec < 0 || req->tv_nsec >= 1000000000) {
        return -EINVAL;
    }

    uint64_t nsec = req->tv_sec * 1000000000ULL + req->tv_nsec;

    if (nsec >= 1000000) {
        // >=1ms使用k_sleep
        k_sleep(K_NSEC(nsec));
    } else {
        // <1ms使用忙等
        k_busy_wait(nsec / 1000);
    }

    // 简化：不处理中断唤醒的剩余时间
    if (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }

    return 0;
}
```

## 配置选项

```kconfig
config POSIX_SIGNALS
    bool "Enable POSIX signal support"
    default y
    help
      Enable signal handling mechanism.

config POSIX_SIGNAL_MAX
    int "Maximum number of signals"
    default 64
    help
      Maximum signal number supported.

config POSIX_TIMERS
    bool "Enable POSIX timers"
    default y
    help
      Enable timer_create and related functions.

config POSIX_TIMER_MAX
    int "Maximum number of timers"
    default 32
    help
      Maximum number of concurrent timers.
```

## 测试要点

- 信号处理函数正确调用
- 信号掩码和阻塞
- 定时器到期触发信号
- sleep精度测试
- 信号中断系统调用
- 多个定时器并发

## 性能考虑

- 信号检查开销（每次系统调用返回）
- 定时器精度受系统时钟频率限制
- 忙等消耗CPU，仅用于短时间延迟
- 建议使用k_timer而非频繁的短sleep

## 限制

1. 不支持所有POSIX信号（如SIGSEGV, SIGBUS等硬件异常）
2. 信号处理函数中可调用的函数受限（异步信号安全）
3. 纳秒级精度实际受限于CPU频率
4. 不支持实时信号队列
5. sigaction的sa_flags部分特性不支持
