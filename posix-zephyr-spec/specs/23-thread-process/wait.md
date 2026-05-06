# wait/waitpid实现

## API定义

```c
#include <sys/wait.h>

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
pid_t wait3(int *status, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
```

## 状态宏

```c
// 检查退出状态
#define WIFEXITED(status)    (((status) & 0x7f) == 0)
#define WEXITSTATUS(status)  (((status) & 0xff00) >> 8)

// 检查信号终止
#define WIFSIGNALED(status)  (((status) & 0x7f) != 0 && ((status) & 0x7f) != 0x7f)
#define WTERMSIG(status)     ((status) & 0x7f)

// 检查停止
#define WIFSTOPPED(status)   (((status) & 0xff) == 0x7f)
#define WSTOPSIG(status)     (((status) & 0xff00) >> 8)

// 检查继续
#define WIFCONTINUED(status) ((status) == 0xffff)
```

## 选项标志

```c
#define WNOHANG    1  // 非阻塞，立即返回
#define WUNTRACED  2  // 报告停止的子进程
#define WCONTINUED 8  // 报告继续的子进程
```

## 实现

### wait()

```c
pid_t wait(int *status)
{
    return waitpid(-1, status, 0);
}
```

### waitpid()

```c
pid_t waitpid(pid_t pid, int *status, int options)
{
    pcb_t *current = get_current_process();
    pcb_t *child;
    int ret;

    while (1) {
        // 查找匹配的子进程
        child = find_child_process(current, pid);
        if (!child) {
            // 没有匹配的子进程
            errno = ECHILD;
            return -1;
        }

        // 检查子进程状态
        if (child->state == PROC_ZOMBIE) {
            // 子进程已退出
            if (status) {
                *status = child->exit_status;
            }

            pid_t child_pid = child->pid;

            // 清理子进程
            remove_child_process(current, child);
            free_process(child);

            return child_pid;
        }

        // 子进程还在运行
        if (options & WNOHANG) {
            // 非阻塞模式，立即返回
            return 0;
        }

        // 阻塞等待子进程退出
        ret = k_sem_take(&child->wait_sem, K_FOREVER);
        if (ret < 0) {
            errno = EINTR;
            return -1;
        }
    }
}
```

### 查找子进程

```c
static pcb_t *find_child_process(pcb_t *parent, pid_t pid)
{
    pcb_t *child;

    if (pid > 0) {
        // 等待特定PID
        for (child = parent->children; child; child = child->next_sibling) {
            if (child->pid == pid) {
                return child;
            }
        }
    } else if (pid == 0) {
        // 等待同一进程组的任何子进程
        for (child = parent->children; child; child = child->next_sibling) {
            if (child->pgid == parent->pgid) {
                return child;
            }
        }
    } else if (pid == -1) {
        // 等待任何子进程
        return parent->children;
    } else {
        // 等待进程组ID为|pid|的任何子进程
        pid_t pgid = -pid;
        for (child = parent->children; child; child = child->next_sibling) {
            if (child->pgid == pgid) {
                return child;
            }
        }
    }

    return NULL;
}
```

### 进程退出

```c
void exit(int status)
{
    pcb_t *proc = get_current_process();

    // 设置退出状态
    proc->exit_status = (status & 0xff) << 8;
    proc->state = PROC_ZOMBIE;
    proc->exited = true;

    // 清理资源
    resource_cleanup_all(proc);

    // 关闭所有文件描述符
    for (int i = 0; i < proc->max_fds; i++) {
        if (proc->fd_table[i].in_use) {
            close(i);
        }
    }

    // 将子进程重新父化到init
    reparent_children_to_init(proc);

    // 通知父进程
    if (proc->ppid > 0) {
        pcb_t *parent = find_process_by_pid(proc->ppid);
        if (parent) {
            k_sem_give(&proc->wait_sem);
        }
    }

    // 终止主线程
    pthread_exit(NULL);
}
```

### 重新父化子进程

```c
static void reparent_children_to_init(pcb_t *proc)
{
    pcb_t *init = get_init_process();
    pcb_t *child;

    while (proc->children) {
        child = proc->children;
        proc->children = child->next_sibling;

        // 将子进程的父进程设置为init
        child->ppid = init->pid;
        child->next_sibling = init->children;
        init->children = child;
    }
}
```

## 使用示例

```c
// 示例1：等待特定子进程
pid_t pid = posix_spawn(...);

int status;
pid_t ret = waitpid(pid, &status, 0);

if (ret > 0) {
    if (WIFEXITED(status)) {
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child killed by signal %d\n", WTERMSIG(status));
    }
}

// 示例2：非阻塞等待
ret = waitpid(pid, &status, WNOHANG);

if (ret == 0) {
    // 子进程还在运行
    printf("Child still running\n");
} else if (ret > 0) {
    // 子进程已退出
    printf("Child exited\n");
}

// 示例3：等待任何子进程
while (1) {
    ret = wait(&status);
    if (ret < 0) {
        if (errno == ECHILD) {
            // 没有子进程了
            break;
        }
    } else {
        printf("Child %d exited\n", ret);
    }
}

// 示例4：等待进程组
pid_t pgid = getpgid(0);
ret = waitpid(-pgid, &status, 0);

// 示例5：信号处理器中回收子进程
void sigchld_handler(int sig)
{
    int status;
    pid_t pid;

    // 回收所有已退出的子进程
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Reaped child %d\n", pid);
    }
}

signal(SIGCHLD, sigchld_handler);
```

## 僵尸进程

```c
// 僵尸进程：已退出但未被父进程回收的进程
// 僵尸进程占用PID和少量内存，但不占用其他资源

// 避免僵尸进程的方法：

// 方法1：父进程调用wait/waitpid
pid_t pid = posix_spawn(...);
wait(NULL);  // 回收子进程

// 方法2：忽略SIGCHLD信号（自动回收）
signal(SIGCHLD, SIG_IGN);

// 方法3：在SIGCHLD处理器中回收
void sigchld_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
signal(SIGCHLD, sigchld_handler);

// 方法4：使用双fork技巧
pid_t pid = fork();
if (pid == 0) {
    // 第一个子进程
    if (fork() == 0) {
        // 第二个子进程（孙进程）
        // 执行实际工作
        exit(0);
    }
    // 第一个子进程立即退出
    exit(0);
}
// 父进程回收第一个子进程
waitpid(pid, NULL, 0);
// 孙进程成为孤儿，由init回收
```

## 进程组和会话

```c
// 进程组：一组相关进程的集合
pid_t getpgid(pid_t pid);
int setpgid(pid_t pid, pid_t pgid);

// 会话：一组进程组的集合
pid_t getsid(pid_t pid);
pid_t setsid(void);

// 示例：创建守护进程
void daemonize(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        exit(0);  // 父进程退出
    }

    // 创建新会话
    setsid();

    // 再次fork
    pid = fork();
    if (pid > 0) {
        exit(0);
    }

    // 改变工作目录
    chdir("/");

    // 关闭标准文件描述符
    close(0);
    close(1);
    close(2);

    // 重定向到/dev/null
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}
```
