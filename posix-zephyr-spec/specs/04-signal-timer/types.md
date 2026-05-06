# 数据类型定义

## 信号相关类型

### sighandler_t
```c
typedef void (*sighandler_t)(int);
```
信号处理函数类型。

### 特殊处理函数
```c
#define SIG_DFL ((sighandler_t)0)   // 默认处理
#define SIG_IGN ((sighandler_t)1)   // 忽略信号
#define SIG_ERR ((sighandler_t)-1)  // 错误返回
```

### sigset_t
```c
typedef struct {
    uint32_t bits[2];  // 支持64个信号
} sigset_t;
```
信号集，用于表示一组信号。

### siginfo_t
```c
typedef struct {
    int si_signo;      // 信号编号
    int si_code;       // 信号代码
    int si_errno;      // 错误码
    pid_t si_pid;      // 发送进程ID
    uid_t si_uid;      // 发送进程用户ID
    void *si_addr;     // 故障地址
    int si_status;     // 退出状态
    union sigval si_value;  // 信号值
    timer_t si_timerid;     // 定时器ID
    int si_overrun;         // 定时器溢出次数
} siginfo_t;
```

### sigaction
```c
struct sigaction {
    sighandler_t sa_handler;                        // 简单处理函数
    void (*sa_sigaction)(int, siginfo_t *, void *); // 扩展处理函数
    sigset_t sa_mask;                               // 处理期间阻塞的信号
    int sa_flags;                                   // 标志
};
```

**sa_flags值**：
```c
#define SA_NOCLDSTOP  1   // 子进程停止时不发送SIGCHLD
#define SA_NOCLDWAIT  2   // 子进程退出时不产生僵尸进程
#define SA_SIGINFO    4   // 使用sa_sigaction而非sa_handler
#define SA_RESTART    8   // 自动重启被中断的系统调用
#define SA_NODEFER    16  // 处理期间不阻塞该信号
#define SA_RESETHAND  32  // 处理后重置为SIG_DFL
```

### sigval
```c
union sigval {
    int sival_int;     // 整数值
    void *sival_ptr;   // 指针值
};
```

## 信号编号

```c
#define NSIG          64   // 最大信号数

// 标准信号
#define SIGHUP        1    // 挂起
#define SIGINT        2    // 中断（Ctrl+C）
#define SIGQUIT       3    // 退出（Ctrl+\）
#define SIGILL        4    // 非法指令
#define SIGTRAP       5    // 跟踪陷阱
#define SIGABRT       6    // 异常终止
#define SIGBUS        7    // 总线错误
#define SIGFPE        8    // 浮点异常
#define SIGKILL       9    // 强制终止（不可捕获）
#define SIGUSR1       10   // 用户定义信号1
#define SIGSEGV       11   // 段错误
#define SIGUSR2       12   // 用户定义信号2
#define SIGPIPE       13   // 管道破裂
#define SIGALRM       14   // 闹钟
#define SIGTERM       15   // 终止
#define SIGSTKFLT     16   // 栈错误
#define SIGCHLD       17   // 子进程状态改变
#define SIGCONT       18   // 继续执行
#define SIGSTOP       19   // 停止（不可捕获）
#define SIGTSTP       20   // 终端停止
#define SIGTTIN       21   // 后台读终端
#define SIGTTOU       22   // 后台写终端
#define SIGURG        23   // 紧急数据
#define SIGXCPU       24   // CPU时间超限
#define SIGXFSZ       25   // 文件大小超限
#define SIGVTALRM     26   // 虚拟定时器
#define SIGPROF       27   // 性能分析定时器
#define SIGWINCH      28   // 窗口大小改变
#define SIGIO         29   // I/O可用
#define SIGPWR        30   // 电源故障
#define SIGSYS        31   // 非法系统调用
```

## 信号代码

```c
// si_code值
#define SI_USER       0    // 用户发送
#define SI_KERNEL     128  // 内核发送
#define SI_QUEUE      -1   // sigqueue发送
#define SI_TIMER      -2   // 定时器到期
#define SI_MESGQ      -3   // 消息队列
#define SI_ASYNCIO    -4   // 异步I/O
#define SI_SIGIO      -5   // 排队的SIGIO
#define SI_TKILL      -6   // tkill发送
```

## 定时器相关类型

### timer_t
```c
typedef void *timer_t;
```
定时器标识符。

### clockid_t
```c
typedef int clockid_t;
```
时钟类型标识符。

**时钟类型**：
```c
#define CLOCK_REALTIME           0  // 系统实时时钟
#define CLOCK_MONOTONIC          1  // 单调递增时钟
#define CLOCK_PROCESS_CPUTIME_ID 2  // 进程CPU时间
#define CLOCK_THREAD_CPUTIME_ID  3  // 线程CPU时间
```

### timespec
```c
struct timespec {
    time_t tv_sec;   // 秒
    long tv_nsec;    // 纳秒（0-999999999）
};
```

### itimerspec
```c
struct itimerspec {
    struct timespec it_interval;  // 周期间隔
    struct timespec it_value;     // 初始到期时间
};
```

### sigevent
```c
struct sigevent {
    int sigev_notify;                           // 通知类型
    int sigev_signo;                            // 信号编号
    union sigval sigev_value;                   // 传递的值
    void (*sigev_notify_function)(union sigval); // 线程函数
    pthread_attr_t *sigev_notify_attributes;    // 线程属性
};
```

**通知类型**：
```c
#define SIGEV_NONE    0  // 无通知
#define SIGEV_SIGNAL  1  // 发送信号
#define SIGEV_THREAD  2  // 创建线程
```

### 定时器标志
```c
#define TIMER_ABSTIME  1  // 绝对时间
```

## 内部数据结构

### signal_context_t
```c
typedef struct {
    sighandler_t handlers[NSIG];     // 信号处理函数数组
    sigset_t pending;                // 待处理信号集
    sigset_t blocked;                // 阻塞信号集
    sigset_t sa_masks[NSIG];         // 每个信号的掩码
    int sa_flags[NSIG];              // 每个信号的标志
    struct k_mutex signal_lock;      // 信号操作锁
    bool in_signal_handler;          // 是否在信号处理函数中
    int current_signal;              // 当前处理的信号
    bool in_sigsuspend;              // 是否在sigsuspend中
    struct k_sem *sigsuspend_sem;    // sigsuspend等待信号量
} signal_context_t;
```

### posix_timer_t
```c
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

## 信号集操作宏

```c
// 清空信号集
#define sigemptyset(set) \
    do { (set)->bits[0] = 0; (set)->bits[1] = 0; } while(0)

// 填充信号集
#define sigfillset(set) \
    do { (set)->bits[0] = ~0U; (set)->bits[1] = ~0U; } while(0)

// 添加信号到集合
#define sigaddset(set, sig) \
    do { \
        int __sig = (sig) - 1; \
        (set)->bits[__sig / 32] |= (1U << (__sig % 32)); \
    } while(0)

// 从集合移除信号
#define sigdelset(set, sig) \
    do { \
        int __sig = (sig) - 1; \
        (set)->bits[__sig / 32] &= ~(1U << (__sig % 32)); \
    } while(0)

// 检查信号是否在集合中
#define sigismember(set, sig) \
    ({ \
        int __sig = (sig) - 1; \
        ((set)->bits[__sig / 32] & (1U << (__sig % 32))) != 0; \
    })
```

## sigprocmask操作

```c
#define SIG_BLOCK    0  // 添加信号到阻塞集
#define SIG_UNBLOCK  1  // 从阻塞集移除信号
#define SIG_SETMASK  2  // 设置新的阻塞集
```

## 时间类型

### time_t
```c
typedef long time_t;
```

### useconds_t
```c
typedef unsigned int useconds_t;
```

## 全局数据结构

### 定时器表
```c
// 全局定时器表
static posix_timer_t *g_timer_table[CONFIG_POSIX_TIMER_MAX];
static struct k_mutex g_timer_table_lock;
static atomic_t g_next_timer_id = ATOMIC_INIT(1);
```

### 信号检查点宏
```c
#define SIGNAL_CHECK_POINT() \
    do { \
        check_pending_signals(); \
    } while (0)

#define CANCELLATION_POINT() \
    do { \
        pthread_testcancel(); \
        SIGNAL_CHECK_POINT(); \
    } while (0)
```

## 辅助函数类型

```c
// 时间转换
static inline uint64_t timespec_to_ms(const struct timespec *ts)
{
    return (uint64_t)ts->tv_sec * 1000 + ts->tv_nsec / 1000000;
}

static inline uint64_t timespec_to_us(const struct timespec *ts)
{
    return (uint64_t)ts->tv_sec * 1000000 + ts->tv_nsec / 1000;
}

static inline uint64_t timespec_to_ns(const struct timespec *ts)
{
    return (uint64_t)ts->tv_sec * 1000000000ULL + ts->tv_nsec;
}

static inline void ms_to_timespec(uint64_t ms, struct timespec *ts)
{
    ts->tv_sec = ms / 1000;
    ts->tv_nsec = (ms % 1000) * 1000000;
}

static inline void us_to_timespec(uint64_t us, struct timespec *ts)
{
    ts->tv_sec = us / 1000000;
    ts->tv_nsec = (us % 1000000) * 1000;
}

static inline void ns_to_timespec(uint64_t ns, struct timespec *ts)
{
    ts->tv_sec = ns / 1000000000ULL;
    ts->tv_nsec = ns % 1000000000ULL;
}
```

## 错误码

```c
#define EINTR      4   // 被信号中断
#define EINVAL     22  // 无效参数
#define EAGAIN     11  // 资源暂时不可用
#define ESRCH      3   // 无此进程
#define EPERM      1   // 操作不允许
#define EFAULT     14  // 地址错误
#define ETIMEDOUT  110 // 超时
```

## 配置选项

```c
// Kconfig生成的配置
#define CONFIG_POSIX_SIGNALS           1
#define CONFIG_POSIX_SIGNAL_MAX        64
#define CONFIG_POSIX_TIMERS            1
#define CONFIG_POSIX_TIMER_MAX         32
#define CONFIG_POSIX_SLEEP_PRECISION   1
```

## 常量定义

```c
// 最大值
#define TIMER_MAX      CONFIG_POSIX_TIMER_MAX
#define DELAYTIMER_MAX 32

// 时间限制
#define USEC_PER_SEC   1000000
#define NSEC_PER_SEC   1000000000ULL
#define NSEC_PER_MSEC  1000000
#define NSEC_PER_USEC  1000
```

## 注意事项

1. **信号编号**: 从1开始，0不是有效信号
2. **信号集**: 使用位图表示，支持64个信号
3. **SIGKILL/SIGSTOP**: 不能被捕获、阻塞或忽略
4. **时间精度**: timespec的tv_nsec必须在0-999999999范围内
5. **定时器ID**: 使用指针作为ID，确保唯一性
6. **原子操作**: 信号相关操作需要原子性保证
