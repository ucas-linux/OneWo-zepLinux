# signal - 设置信号处理函数

## POSIX标准定义

```c
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);

// 特殊处理函数
#define SIG_DFL ((sighandler_t)0)  // 默认处理
#define SIG_IGN ((sighandler_t)1)  // 忽略信号
#define SIG_ERR ((sighandler_t)-1) // 错误返回
```

## 功能描述

设置指定信号的处理函数。当进程收到该信号时，调用指定的处理函数。

## 参数说明

- **signum**: 信号编号（1-64）
- **handler**: 信号处理函数，或SIG_DFL/SIG_IGN

## 返回值

- **成功**: 返回之前的信号处理函数
- **失败**: 返回SIG_ERR，设置errno

## Linux标准行为

1. 信号处理函数异步执行
2. 处理函数执行期间，该信号被阻塞
3. 某些信号不能被捕获（SIGKILL, SIGSTOP）
4. 信号处理函数应该是异步信号安全的

## 常用信号

```c
#define SIGHUP     1   // 挂起
#define SIGINT     2   // 中断（Ctrl+C）
#define SIGQUIT    3   // 退出（Ctrl+\）
#define SIGILL     4   // 非法指令
#define SIGTRAP    5   // 跟踪陷阱
#define SIGABRT    6   // 异常终止
#define SIGBUS     7   // 总线错误
#define SIGFPE     8   // 浮点异常
#define SIGKILL    9   // 强制终止（不可捕获）
#define SIGUSR1    10  // 用户定义信号1
#define SIGSEGV    11  // 段错误
#define SIGUSR2    12  // 用户定义信号2
#define SIGPIPE    13  // 管道破裂
#define SIGALRM    14  // 闹钟
#define SIGTERM    15  // 终止
#define SIGCHLD    17  // 子进程状态改变
#define SIGCONT    18  // 继续执行
#define SIGSTOP    19  // 停止（不可捕获）
```

## Zephyr适配方案

### 核心挑战

1. **异步执行**：需要在任意时刻中断线程执行
2. **栈帧修改**：需要修改线程栈以插入信号处理函数调用
3. **信号安全**：处理函数中只能调用有限的函数

### 数据结构扩展

```c
// 信号处理函数表（添加到线程/进程控制块）
typedef struct {
    sighandler_t handlers[NSIG];     // 信号处理函数数组
    sigset_t pending;                // 待处理信号集
    sigset_t blocked;                // 阻塞信号集
    struct k_mutex signal_lock;      // 信号操作锁
    bool in_signal_handler;          // 是否在信号处理函数中
    int current_signal;              // 当前处理的信号
} signal_context_t;

// 添加到posix_thread_t或pcb_t
typedef struct posix_thread {
    // ... 其他字段
    signal_context_t signal_ctx;
} posix_thread_t;
```

### 实现步骤

#### 1. signal()实现

```c
sighandler_t signal(int signum, sighandler_t handler)
{
    // 验证信号编号
    if (signum < 1 || signum >= NSIG) {
        errno = EINVAL;
        return SIG_ERR;
    }

    // SIGKILL和SIGSTOP不能被捕获
    if (signum == SIGKILL || signum == SIGSTOP) {
        errno = EINVAL;
        return SIG_ERR;
    }

    // 获取当前线程
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) {
        errno = ESRCH;
        return SIG_ERR;
    }

    // 保存旧的处理函数
    k_mutex_lock(&thread->signal_ctx.signal_lock, K_FOREVER);
    sighandler_t old_handler = thread->signal_ctx.handlers[signum];
    thread->signal_ctx.handlers[signum] = handler;
    k_mutex_unlock(&thread->signal_ctx.signal_lock);

    return old_handler;
}
```

#### 2. 信号分发机制

```c
// 检查并处理待处理信号
void check_pending_signals(void)
{
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) return;

    k_mutex_lock(&thread->signal_ctx.signal_lock, K_FOREVER);

    // 如果已经在信号处理函数中，不递归处理
    if (thread->signal_ctx.in_signal_handler) {
        k_mutex_unlock(&thread->signal_ctx.signal_lock);
        return;
    }

    // 查找待处理且未阻塞的信号
    for (int sig = 1; sig < NSIG; sig++) {
        if (sigismember(&thread->signal_ctx.pending, sig) &&
            !sigismember(&thread->signal_ctx.blocked, sig)) {

            // 清除待处理标志
            sigdelset(&thread->signal_ctx.pending, sig);

            // 获取处理函数
            sighandler_t handler = thread->signal_ctx.handlers[sig];

            k_mutex_unlock(&thread->signal_ctx.signal_lock);

            // 调用处理函数
            if (handler == SIG_DFL) {
                handle_default_signal(sig);
            } else if (handler != SIG_IGN) {
                // 标记进入信号处理
                thread->signal_ctx.in_signal_handler = true;
                thread->signal_ctx.current_signal = sig;

                // 调用用户处理函数
                handler(sig);

                // 标记退出信号处理
                thread->signal_ctx.in_signal_handler = false;
            }

            // 处理完一个信号后重新检查
            return check_pending_signals();
        }
    }

    k_mutex_unlock(&thread->signal_ctx.signal_lock);
}
```

#### 3. 默认信号处理

```c
static void handle_default_signal(int sig)
{
    switch (sig) {
    case SIGTERM:
    case SIGINT:
    case SIGQUIT:
    case SIGABRT:
        // 终止进程
        exit(128 + sig);
        break;

    case SIGCHLD:
    case SIGURG:
        // 忽略
        break;

    case SIGSTOP:
    case SIGTSTP:
    case SIGTTIN:
    case SIGTTOU:
        // 停止进程（简化实现：忽略）
        break;

    case SIGCONT:
        // 继续进程（简化实现：忽略）
        break;

    default:
        // 其他信号默认终止
        exit(128 + sig);
        break;
    }
}
```

#### 4. 信号检查点

在系统调用返回和取消点插入信号检查：

```c
#define SIGNAL_CHECK_POINT() \
    do { \
        check_pending_signals(); \
    } while (0)

// 在系统调用中使用
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    // ... 加锁操作

    // 返回前检查信号
    SIGNAL_CHECK_POINT();

    return 0;
}
```

## sigaction实现

更强大的信号处理接口：

```c
struct sigaction {
    sighandler_t sa_handler;
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
};

int sigaction(int signum, const struct sigaction *act,
              struct sigaction *oldact)
{
    if (signum < 1 || signum >= NSIG) {
        errno = EINVAL;
        return -1;
    }

    if (signum == SIGKILL || signum == SIGSTOP) {
        errno = EINVAL;
        return -1;
    }

    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) {
        errno = ESRCH;
        return -1;
    }

    k_mutex_lock(&thread->signal_ctx.signal_lock, K_FOREVER);

    // 保存旧的动作
    if (oldact) {
        oldact->sa_handler = thread->signal_ctx.handlers[signum];
        oldact->sa_mask = thread->signal_ctx.sa_masks[signum];
        oldact->sa_flags = thread->signal_ctx.sa_flags[signum];
    }

    // 设置新的动作
    if (act) {
        thread->signal_ctx.handlers[signum] = act->sa_handler;
        thread->signal_ctx.sa_masks[signum] = act->sa_mask;
        thread->signal_ctx.sa_flags[signum] = act->sa_flags;
    }

    k_mutex_unlock(&thread->signal_ctx.signal_lock);

    return 0;
}
```

## 测试用例

### 测试1：基本信号处理

```c
volatile int signal_received = 0;

void signal_handler(int sig)
{
    signal_received = sig;
}

void test_basic_signal(void)
{
    signal(SIGUSR1, signal_handler);

    kill(getpid(), SIGUSR1);

    k_sleep(K_MSEC(100));

    assert(signal_received == SIGUSR1);
}
```

### 测试2：信号忽略

```c
void test_signal_ignore(void)
{
    signal(SIGUSR1, SIG_IGN);

    kill(getpid(), SIGUSR1);

    k_sleep(K_MSEC(100));

    // 进程应该继续运行
}
```

### 测试3：信号嵌套

```c
volatile int handler_count = 0;

void nested_handler(int sig)
{
    handler_count++;
    if (handler_count == 1) {
        // 第一次调用时发送另一个信号
        kill(getpid(), SIGUSR2);
    }
}

void test_nested_signals(void)
{
    signal(SIGUSR1, nested_handler);
    signal(SIGUSR2, nested_handler);

    kill(getpid(), SIGUSR1);

    k_sleep(K_MSEC(100));

    assert(handler_count == 2);
}
```

## 性能考虑

1. **信号检查开销**：每次系统调用约1-5微秒
2. **处理函数调用**：约10-50微秒
3. **锁竞争**：多线程环境下可能有竞争

## 注意事项

1. **异步信号安全**：处理函数中只能调用异步信号安全的函数
2. **不可重入**：避免在处理函数中调用非重入函数
3. **全局变量**：使用volatile sig_atomic_t类型
4. **最小化工作**：处理函数应尽快返回
5. **不要调用**：malloc, printf, pthread_mutex_lock等非安全函数

## 限制

1. 不支持硬件异常信号（SIGSEGV, SIGBUS等）
2. 信号处理函数中可调用的函数受限
3. 不支持实时信号（SIGRTMIN-SIGRTMAX）
4. 信号不排队（同一信号多次发送可能合并）
5. sa_sigaction扩展处理函数支持有限
