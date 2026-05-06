# 实现指导

## 总体架构

### 模块组织

```
posix_signal_timer/
├── signal.c              # 信号处理
├── kill.c                # 信号发送
├── timer.c               # POSIX定时器
├── sleep.c               # 休眠函数
├── signal_internal.h     # 内部头文件
└── timer_internal.h      # 定时器内部头文件
```

## 信号实现

### 全局数据结构

```c
// 全局信号表（每个进程/线程）
static struct k_mutex g_signal_lock;

// 信号名称表（用于调试）
static const char *signal_names[NSIG] = {
    [SIGHUP] = "SIGHUP",
    [SIGINT] = "SIGINT",
    [SIGQUIT] = "SIGQUIT",
    // ...
};
```

### 信号分发

```c
void check_pending_signals(void)
{
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread || thread->signal_ctx.in_signal_handler) {
        return;
    }

    k_mutex_lock(&thread->signal_ctx.signal_lock, K_FOREVER);

    for (int sig = 1; sig < NSIG; sig++) {
        if (sigismember(&thread->signal_ctx.pending, sig) &&
            !sigismember(&thread->signal_ctx.blocked, sig)) {

            sigdelset(&thread->signal_ctx.pending, sig);
            sighandler_t handler = thread->signal_ctx.handlers[sig];

            k_mutex_unlock(&thread->signal_ctx.signal_lock);

            if (handler == SIG_DFL) {
                handle_default_signal(sig);
            } else if (handler != SIG_IGN) {
                thread->signal_ctx.in_signal_handler = true;
                handler(sig);
                thread->signal_ctx.in_signal_handler = false;
            }

            return check_pending_signals();
        }
    }

    k_mutex_unlock(&thread->signal_ctx.signal_lock);
}
```

## 定时器实现

### 定时器管理

```c
// 定时器池
static posix_timer_t g_timer_pool[CONFIG_POSIX_TIMER_MAX];
static bool g_timer_used[CONFIG_POSIX_TIMER_MAX];
static struct k_mutex g_timer_pool_lock;

static posix_timer_t *alloc_posix_timer(void)
{
    k_mutex_lock(&g_timer_pool_lock, K_FOREVER);

    for (int i = 0; i < CONFIG_POSIX_TIMER_MAX; i++) {
        if (!g_timer_used[i]) {
            g_timer_used[i] = true;
            posix_timer_t *timer = &g_timer_pool[i];
            memset(timer, 0, sizeof(*timer));
            k_mutex_unlock(&g_timer_pool_lock);
            return timer;
        }
    }

    k_mutex_unlock(&g_timer_pool_lock);
    return NULL;
}

static void free_posix_timer(posix_timer_t *timer)
{
    k_mutex_lock(&g_timer_pool_lock, K_FOREVER);
    int index = timer - g_timer_pool;
    if (index >= 0 && index < CONFIG_POSIX_TIMER_MAX) {
        g_timer_used[index] = false;
    }
    k_mutex_unlock(&g_timer_pool_lock);
}
```

## 初始化

```c
int signal_timer_subsystem_init(void)
{
    // 初始化信号锁
    k_mutex_init(&g_signal_lock);

    // 初始化定时器池
    k_mutex_init(&g_timer_pool_lock);
    memset(g_timer_used, 0, sizeof(g_timer_used));

    return 0;
}

SYS_INIT(signal_timer_subsystem_init, POST_KERNEL,
         CONFIG_SIGNAL_TIMER_INIT_PRIORITY);
```

## 配置选项

```kconfig
menuconfig POSIX_SIGNALS
    bool "POSIX Signal Support"
    default n

if POSIX_SIGNALS

config POSIX_SIGNAL_MAX
    int "Maximum number of signals"
    default 64

config POSIX_TIMERS
    bool "POSIX Timers"
    default y

config POSIX_TIMER_MAX
    int "Maximum number of timers"
    default 32

config SIGNAL_TIMER_INIT_PRIORITY
    int "Initialization priority"
    default 50

endif
```

## 调试支持

```c
#ifdef CONFIG_SIGNAL_DEBUG
#define SIG_DEBUG(fmt, ...) printk("[SIG] " fmt "\n", ##__VA_ARGS__)
#else
#define SIG_DEBUG(fmt, ...)
#endif

void dump_signal_state(void)
{
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) return;

    printk("=== Signal State ===\n");
    printk("Pending: 0x%08x%08x\n",
           thread->signal_ctx.pending.bits[1],
           thread->signal_ctx.pending.bits[0]);
    printk("Blocked: 0x%08x%08x\n",
           thread->signal_ctx.blocked.bits[1],
           thread->signal_ctx.blocked.bits[0]);
}
```

## 性能优化

### 快速信号检查

```c
static inline bool has_pending_signals(posix_thread_t *thread)
{
    return (thread->signal_ctx.pending.bits[0] &
            ~thread->signal_ctx.blocked.bits[0]) ||
           (thread->signal_ctx.pending.bits[1] &
            ~thread->signal_ctx.blocked.bits[1]);
}
```

## 测试框架

```c
#ifdef CONFIG_SIGNAL_TESTS

void test_signal_basic(void)
{
    volatile int sig_count = 0;

    signal(SIGUSR1, [](int sig) { sig_count++; });
    raise(SIGUSR1);
    k_sleep(K_MSEC(100));

    zassert_equal(sig_count, 1, "Signal not received");
}

ZTEST_SUITE(signal_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(signal_tests, test_basic) { test_signal_basic(); }

#endif
```

## 常见问题

### Q: 信号处理函数中可以调用哪些函数？
A: 只能调用异步信号安全的函数，避免malloc、printf等

### Q: 定时器精度不够怎么办？
A: 增加CONFIG_SYS_CLOCK_TICKS_PER_SEC或使用忙等

### Q: 信号丢失怎么办？
A: 使用sigaction和SA_SIGINFO获取更多信息

### Q: 定时器数量不够怎么办？
A: 增加CONFIG_POSIX_TIMER_MAX配置
