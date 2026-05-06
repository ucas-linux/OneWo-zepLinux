# 完整信号支持

## 概述

扩展基础信号支持，提供完整的POSIX信号处理能力，包括高级信号处理、信号屏蔽、实时信号等。

## 目标

- 实现sigaction()高级信号处理
- 实现sigprocmask()信号屏蔽
- 实现sigsuspend()原子信号等待
- 实现sigaltstack()备用信号栈
- 支持实时信号（SIGRTMIN-SIGRTMAX）

## sigaction() - 高级信号处理

```c
#include <signal.h>

int sigaction(int signum, const struct sigaction *act,
              struct sigaction *oldact);

struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};

// 标志
#define SA_NOCLDSTOP  1
#define SA_NOCLDWAIT  2
#define SA_SIGINFO    4
#define SA_ONSTACK    0x08000000
#define SA_RESTART    0x10000000
#define SA_NODEFER    0x40000000
#define SA_RESETHAND  0x80000000
```

### 实现

```c
int sigaction(int signum, const struct sigaction *act,
              struct sigaction *oldact)
{
    pcb_t *proc = get_current_process();

    if (signum < 1 || signum >= NSIG) {
        errno = EINVAL;
        return -1;
    }

    // 不能捕获SIGKILL和SIGSTOP
    if (signum == SIGKILL || signum == SIGSTOP) {
        errno = EINVAL;
        return -1;
    }

    // 返回旧的处理器
    if (oldact) {
        *oldact = proc->sig_handlers[signum];
    }

    // 设置新的处理器
    if (act) {
        proc->sig_handlers[signum] = *act;
    }

    return 0;
}
```

## sigprocmask() - 信号屏蔽

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

#define SIG_BLOCK    0  // 添加到屏蔽集
#define SIG_UNBLOCK  1  // 从屏蔽集移除
#define SIG_SETMASK  2  // 设置屏蔽集
```

### 实现

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    pcb_t *proc = get_current_process();

    // 返回旧的屏蔽集
    if (oldset) {
        *oldset = proc->sig_blocked;
    }

    if (set) {
        switch (how) {
        case SIG_BLOCK:
            // 添加信号到屏蔽集
            for (int i = 1; i < NSIG; i++) {
                if (sigismember(set, i)) {
                    sigaddset(&proc->sig_blocked, i);
                }
            }
            break;

        case SIG_UNBLOCK:
            // 从屏蔽集移除信号
            for (int i = 1; i < NSIG; i++) {
                if (sigismember(set, i)) {
                    sigdelset(&proc->sig_blocked, i);
                }
            }
            // 检查是否有待处理的信号
            check_pending_signals(proc);
            break;

        case SIG_SETMASK:
            // 设置屏蔽集
            proc->sig_blocked = *set;
            // SIGKILL和SIGSTOP不能被屏蔽
            sigdelset(&proc->sig_blocked, SIGKILL);
            sigdelset(&proc->sig_blocked, SIGSTOP);
            check_pending_signals(proc);
            break;

        default:
            errno = EINVAL;
            return -1;
        }
    }

    return 0;
}
```

## sigsuspend() - 原子信号等待

```c
int sigsuspend(const sigset_t *mask);
```

### 实现

```c
int sigsuspend(const sigset_t *mask)
{
    pcb_t *proc = get_current_process();
    sigset_t oldmask;

    // 保存旧的屏蔽集
    oldmask = proc->sig_blocked;

    // 原子地设置新的屏蔽集并等待信号
    proc->sig_blocked = *mask;
    sigdelset(&proc->sig_blocked, SIGKILL);
    sigdelset(&proc->sig_blocked, SIGSTOP);

    // 等待信号
    while (1) {
        // 检查是否有未屏蔽的待处理信号
        for (int i = 1; i < NSIG; i++) {
            if (sigismember(&proc->sig_pending, i) &&
                !sigismember(&proc->sig_blocked, i)) {
                // 有信号待处理
                goto out;
            }
        }

        // 睡眠等待信号
        k_sem_take(&proc->sig_sem, K_FOREVER);
    }

out:
    // 恢复旧的屏蔽集
    proc->sig_blocked = oldmask;

    // sigsuspend总是返回-1并设置errno为EINTR
    errno = EINTR;
    return -1;
}
```

## 实时信号

```c
// 实时信号范围
#define SIGRTMIN  32
#define SIGRTMAX  64

// 发送实时信号
int sigqueue(pid_t pid, int sig, const union sigval value);

union sigval {
    int sival_int;
    void *sival_ptr;
};
```

### 实现

```c
int sigqueue(pid_t pid, int sig, const union sigval value)
{
    pcb_t *proc = find_process_by_pid(pid);

    if (!proc) {
        errno = ESRCH;
        return -1;
    }

    if (sig < SIGRTMIN || sig > SIGRTMAX) {
        errno = EINVAL;
        return -1;
    }

    // 创建siginfo
    siginfo_t info;
    memset(&info, 0, sizeof(info));
    info.si_signo = sig;
    info.si_code = SI_QUEUE;
    info.si_pid = getpid();
    info.si_uid = getuid();
    info.si_value = value;

    // 排队信号
    return queue_signal(proc, &info);
}
```

## 使用示例

```c
// 示例1：使用sigaction
void signal_handler(int sig, siginfo_t *info, void *context)
{
    printf("Received signal %d from PID %d\n", sig, info->si_pid);
}

struct sigaction sa;
sa.sa_sigaction = signal_handler;
sa.sa_flags = SA_SIGINFO;
sigemptyset(&sa.sa_mask);
sigaction(SIGUSR1, &sa, NULL);

// 示例2：屏蔽信号
sigset_t mask;
sigemptyset(&mask);
sigaddset(&mask, SIGINT);
sigprocmask(SIG_BLOCK, &mask, NULL);

// 临界区代码

sigprocmask(SIG_UNBLOCK, &mask, NULL);

// 示例3：等待信号
sigset_t mask;
sigfillset(&mask);
sigdelset(&mask, SIGUSR1);
sigsuspend(&mask);  // 等待SIGUSR1

// 示例4：实时信号
union sigval value;
value.sival_int = 42;
sigqueue(pid, SIGRTMIN, value);
```

## 配置选项

```kconfig
config POSIX_SIGNALS_ADVANCED
    bool "Advanced signal support"
    depends on POSIX_SIGNAL_TIMER
    default n

config POSIX_REALTIME_SIGNALS
    bool "Real-time signals"
    depends on POSIX_SIGNALS_ADVANCED
    default n

config SIGRTMAX
    int "Maximum real-time signal"
    depends on POSIX_REALTIME_SIGNALS
    default 64
    range 32 128
```
