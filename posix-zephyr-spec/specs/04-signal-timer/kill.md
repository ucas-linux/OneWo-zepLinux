# kill - 发送信号

## POSIX标准定义

```c
#include <signal.h>
#include <sys/types.h>

int kill(pid_t pid, int sig);
int raise(int sig);
```

## 功能描述

- **kill**: 向指定进程或进程组发送信号
- **raise**: 向当前进程发送信号（等同于kill(getpid(), sig)）

## 参数说明

- **pid**: 目标进程ID
  - pid > 0: 发送给指定进程
  - pid == 0: 发送给当前进程组的所有进程
  - pid == -1: 发送给所有有权限发送的进程
  - pid < -1: 发送给进程组|pid|的所有进程
- **sig**: 信号编号（0表示检查进程是否存在）

## 返回值

- **0**: 成功
- **-1**: 失败，设置errno

## 错误码

- **EINVAL**: 无效的信号编号
- **EPERM**: 没有权限发送信号
- **ESRCH**: 进程不存在

## Zephyr适配方案

### 实现策略

由于Zephyr是单地址空间RTOS，kill的实现需要：
1. 查找目标进程/线程
2. 设置待处理信号标志
3. 如果目标在阻塞状态，唤醒它
4. 让目标在下次调度时处理信号

### 数据结构

```c
// 信号集操作
typedef struct {
    uint32_t bits[2];  // 支持64个信号
} sigset_t;

// 信号集操作宏
#define sigemptyset(set) \
    do { (set)->bits[0] = 0; (set)->bits[1] = 0; } while(0)

#define sigfillset(set) \
    do { (set)->bits[0] = ~0U; (set)->bits[1] = ~0U; } while(0)

#define sigaddset(set, sig) \
    do { \
        int __sig = (sig) - 1; \
        (set)->bits[__sig / 32] |= (1U << (__sig % 32)); \
    } while(0)

#define sigdelset(set, sig) \
    do { \
        int __sig = (sig) - 1; \
        (set)->bits[__sig / 32] &= ~(1U << (__sig % 32)); \
    } while(0)

#define sigismember(set, sig) \
    ({ \
        int __sig = (sig) - 1; \
        ((set)->bits[__sig / 32] & (1U << (__sig % 32))) != 0; \
    })
```

### 实现步骤

#### 1. kill()实现

```c
int kill(pid_t pid, int sig)
{
    // 验证信号编号
    if (sig < 0 || sig >= NSIG) {
        errno = EINVAL;
        return -1;
    }

    // sig == 0 用于检查进程是否存在
    if (sig == 0) {
        pcb_t *proc = find_process_by_pid(pid);
        if (!proc) {
            errno = ESRCH;
            return -1;
        }
        return 0;
    }

    // 根据pid值分发
    if (pid > 0) {
        return kill_process(pid, sig);
    } else if (pid == 0) {
        return kill_process_group(getpgid(0), sig);
    } else if (pid == -1) {
        return kill_all_processes(sig);
    } else {
        return kill_process_group(-pid, sig);
    }
}
```

#### 2. 向单个进程发送信号

```c
static int kill_process(pid_t pid, int sig)
{
    // 查找目标进程
    pcb_t *target = find_process_by_pid(pid);
    if (!target) {
        errno = ESRCH;
        return -1;
    }

    // 权限检查（简化：同一用户或root）
    pcb_t *sender = get_current_process();
    if (sender && sender->uid != 0 && sender->uid != target->uid) {
        errno = EPERM;
        return -1;
    }

    // 添加到待处理信号集
    k_mutex_lock(&target->signal_ctx.signal_lock, K_FOREVER);
    sigaddset(&target->signal_ctx.pending, sig);
    k_mutex_unlock(&target->signal_ctx.signal_lock);

    // 如果目标线程在阻塞状态，唤醒它
    if (target->main_thread) {
        // 检查线程是否在等待
        if (k_thread_state_get(target->main_thread) & K_PENDING) {
            k_wakeup(target->main_thread);
        }
    }

    return 0;
}
```

#### 3. 向进程组发送信号

```c
static int kill_process_group(pid_t pgid, int sig)
{
    int count = 0;
    int last_errno = 0;

    k_mutex_lock(&g_process_table_lock, K_FOREVER);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_t *proc = &g_process_table[i];

        if (proc->state != PROC_TERMINATED &&
            proc->pgid == pgid) {

            k_mutex_unlock(&g_process_table_lock);

            if (kill_process(proc->pid, sig) == 0) {
                count++;
            } else {
                last_errno = errno;
            }

            k_mutex_lock(&g_process_table_lock, K_FOREVER);
        }
    }

    k_mutex_unlock(&g_process_table_lock);

    if (count == 0) {
        errno = last_errno ? last_errno : ESRCH;
        return -1;
    }

    return 0;
}
```

#### 4. raise()实现

```c
int raise(int sig)
{
    return kill(getpid(), sig);
}
```

#### 5. 信号传递到线程

对于多线程进程，需要选择一个线程来处理信号：

```c
static int deliver_signal_to_process(pcb_t *proc, int sig)
{
    // 查找一个未阻塞该信号的线程
    posix_thread_t *target_thread = NULL;

    k_mutex_lock(&proc->thread_list_lock, K_FOREVER);

    for (posix_thread_t *t = proc->thread_list; t; t = t->next) {
        if (!sigismember(&t->signal_ctx.blocked, sig)) {
            target_thread = t;
            break;
        }
    }

    k_mutex_unlock(&proc->thread_list_lock);

    if (!target_thread) {
        // 所有线程都阻塞了该信号，添加到进程级待处理信号
        sigaddset(&proc->signal_ctx.pending, sig);
        return 0;
    }

    // 添加到目标线程的待处理信号
    k_mutex_lock(&target_thread->signal_ctx.signal_lock, K_FOREVER);
    sigaddset(&target_thread->signal_ctx.pending, sig);
    k_mutex_unlock(&target_thread->signal_ctx.signal_lock);

    // 唤醒目标线程
    if (k_thread_state_get(&target_thread->zephyr_thread) & K_PENDING) {
        k_wakeup(&target_thread->zephyr_thread);
    }

    return 0;
}
```

## 信号掩码操作

### sigprocmask

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) {
        errno = ESRCH;
        return -1;
    }

    k_mutex_lock(&thread->signal_ctx.signal_lock, K_FOREVER);

    // 保存旧的掩码
    if (oldset) {
        *oldset = thread->signal_ctx.blocked;
    }

    // 修改掩码
    if (set) {
        switch (how) {
        case SIG_BLOCK:
            // 添加信号到阻塞集
            thread->signal_ctx.blocked.bits[0] |= set->bits[0];
            thread->signal_ctx.blocked.bits[1] |= set->bits[1];
            break;

        case SIG_UNBLOCK:
            // 从阻塞集移除信号
            thread->signal_ctx.blocked.bits[0] &= ~set->bits[0];
            thread->signal_ctx.blocked.bits[1] &= ~set->bits[1];
            break;

        case SIG_SETMASK:
            // 设置新的阻塞集
            thread->signal_ctx.blocked = *set;
            break;

        default:
            k_mutex_unlock(&thread->signal_ctx.signal_lock);
            errno = EINVAL;
            return -1;
        }

        // SIGKILL和SIGSTOP不能被阻塞
        sigdelset(&thread->signal_ctx.blocked, SIGKILL);
        sigdelset(&thread->signal_ctx.blocked, SIGSTOP);
    }

    k_mutex_unlock(&thread->signal_ctx.signal_lock);

    // 检查是否有新的未阻塞信号需要处理
    check_pending_signals();

    return 0;
}
```

### sigsuspend

```c
int sigsuspend(const sigset_t *mask)
{
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) {
        errno = ESRCH;
        return -1;
    }

    // 保存当前掩码
    sigset_t oldmask = thread->signal_ctx.blocked;

    // 设置新掩码
    sigprocmask(SIG_SETMASK, mask, NULL);

    // 等待信号
    struct k_sem wait_sem;
    k_sem_init(&wait_sem, 0, 1);

    // 设置标志表示在sigsuspend中
    thread->signal_ctx.in_sigsuspend = true;
    thread->signal_ctx.sigsuspend_sem = &wait_sem;

    // 等待信号唤醒
    k_sem_take(&wait_sem, K_FOREVER);

    // 恢复标志
    thread->signal_ctx.in_sigsuspend = false;
    thread->signal_ctx.sigsuspend_sem = NULL;

    // 恢复旧掩码
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    // sigsuspend总是返回-1并设置errno为EINTR
    errno = EINTR;
    return -1;
}
```

## 测试用例

> 注意：进程间信号测试依赖`fork`与`waitpid`，仅在支持进程语义（MMU + 进程子系统）时运行。

### 测试1：基本kill

```c
volatile int signal_count = 0;

void sigusr1_handler(int sig)
{
    signal_count++;
}

void test_basic_kill(void)
{
    signal(SIGUSR1, sigusr1_handler);

    pid_t pid = getpid();
    kill(pid, SIGUSR1);

    k_sleep(K_MSEC(100));

    assert(signal_count == 1);
}
```

### 测试2：进程间信号

```c
void test_inter_process_signal(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        // 子进程
        signal(SIGUSR1, sigusr1_handler);

        // 等待信号
        pause();

        assert(signal_count == 1);
        exit(0);
    } else {
        // 父进程
        k_sleep(K_MSEC(100));

        // 发送信号给子进程
        kill(pid, SIGUSR1);

        int status;
        waitpid(pid, &status, 0);
    }
}
```

### 测试3：信号掩码

```c
void test_signal_mask(void)
{
    signal(SIGUSR1, sigusr1_handler);

    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    // 阻塞SIGUSR1
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    // 发送信号（应该被阻塞）
    raise(SIGUSR1);
    k_sleep(K_MSEC(100));
    assert(signal_count == 0);

    // 解除阻塞
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    // 信号应该被传递
    k_sleep(K_MSEC(100));
    assert(signal_count == 1);
}
```

### 测试4：检查进程存在

```c
void test_kill_zero(void)
{
    pid_t pid = getpid();

    // sig=0用于检查进程是否存在
    int ret = kill(pid, 0);
    assert(ret == 0);

    // 不存在的进程
    ret = kill(99999, 0);
    assert(ret == -1 && errno == ESRCH);
}
```

## 性能考虑

1. **信号发送**：约10-50微秒
2. **进程查找**：约5-20微秒
3. **线程唤醒**：约10-30微秒
4. **进程组信号**：取决于进程数量

## 注意事项

1. **权限检查**：只能向同一用户的进程发送信号
2. **SIGKILL/SIGSTOP**：不能被阻塞或忽略
3. **信号0**：用于检查进程是否存在，不实际发送信号
4. **进程组**：pid<0时发送给进程组
5. **竞态条件**：信号可能在检查和发送之间丢失

## 限制

1. 不支持完整的进程组和会话管理
2. 权限检查简化（无完整的UID/GID检查）
3. 不支持实时信号队列
4. 信号不排队（同一信号多次发送可能合并）
5. pid=-1的广播功能受限
