# getpid/wait - 进程控制

## POSIX标准定义

```c
#include <unistd.h>
#include <sys/wait.h>

pid_t getpid(void);
pid_t getppid(void);
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

## Zephyr适配

> 注意：`wait/waitpid`仅在支持进程语义（MMU + 进程子系统）时可用；无MMU目标应返回`ECHILD`或`ENOSYS`。

```c
pid_t getpid(void)
{
    pcb_t *proc = get_current_process();
    return proc ? proc->pid : 1;  // 默认返回init进程
}

pid_t getppid(void)
{
    pcb_t *proc = get_current_process();
    return proc ? proc->ppid : 0;
}

pid_t waitpid(pid_t pid, int *status, int options)
{
    pcb_t *parent = get_current_process();
    if (!parent) {
        errno = ECHILD;
        return -1;
    }

    // 查找子进程
    pcb_t *child = find_child_process(parent, pid);
    if (!child) {
        errno = ECHILD;
        return -1;
    }

    // 等待子进程退出
    if (child->state == PROC_RUNNING) {
        if (options & WNOHANG) {
            return 0;  // 非阻塞
        }
        k_sem_take(&child->wait_sem, K_FOREVER);
    }

    // 获取退出状态
    if (status) {
        *status = child->exit_status << 8;
    }

    pid_t child_pid = child->pid;
    reclaim_process(child);

    return child_pid;
}
```

## 测试

```c
void test_wait(void)
{
    pid_t pid = fork();
    if (pid == 0) {
        exit(42);
    } else {
        int status;
        waitpid(pid, &status, 0);
        assert(WEXITSTATUS(status) == 42);
    }
}
```
