# posix_spawn实现

## 概述

`posix_spawn()`是fork/exec的替代方案，特别适合无MMU系统。它在单个操作中创建新进程并加载程序。

## API定义

```c
#include <spawn.h>

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);

int posix_spawnp(pid_t *pid, const char *file,
                 const posix_spawn_file_actions_t *file_actions,
                 const posix_spawnattr_t *attrp,
                 char *const argv[], char *const envp[]);
```

## 实现流程

```c
int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[])
{
    elf_handle_t elf_handle;
    pcb_t *proc;
    int ret;

    // 1. 解析并加载ELF
    ret = elf_parse(path, &elf_handle);
    if (ret < 0) {
        return ret;
    }

    ret = elf_load(&elf_handle, NULL);
    if (ret < 0) {
        elf_close(&elf_handle);
        return ret;
    }

    // 2. 分配进程控制块
    proc = alloc_process();
    if (!proc) {
        elf_close(&elf_handle);
        return EAGAIN;
    }

    // 3. 初始化进程
    proc->pid = generate_pid();
    proc->ppid = getpid();
    proc->state = PROC_RUNNING;

    // 4. 设置内存布局
    proc->code_start = elf_handle.memory.code_base;
    proc->code_size = elf_handle.memory.code_size;
    proc->data_start = elf_handle.memory.data_base;
    proc->data_size = elf_handle.memory.data_size;
    proc->heap_start = elf_handle.memory.heap_base;
    proc->heap_size = elf_handle.memory.heap_size;
    proc->stack_start = elf_handle.memory.stack_base;
    proc->stack_size = elf_handle.memory.stack_size;

    // 5. 应用spawn属性
    if (attrp) {
        apply_spawn_attributes(proc, attrp);
    }

    // 6. 应用文件操作
    if (file_actions) {
        apply_file_actions(proc, file_actions);
    }

    // 7. 配置MPU
    ret = configure_process_mpu(proc);
    if (ret < 0) {
        free_process(proc);
        elf_close(&elf_handle);
        return ret;
    }

    // 8. 准备入口环境
    ret = prepare_process_entry(proc, &elf_handle, argv, envp);
    if (ret < 0) {
        free_process(proc);
        elf_close(&elf_handle);
        return ret;
    }

    // 9. 创建主线程
    ret = create_process_main_thread(proc, elf_handle.entry);
    if (ret < 0) {
        free_process(proc);
        elf_close(&elf_handle);
        return ret;
    }

    // 10. 添加到父进程的子进程列表
    add_child_process(get_current_process(), proc);

    // 返回PID
    *pid = proc->pid;

    return 0;
}
```

## spawn属性

```c
typedef struct {
    short flags;
    pid_t pgroup;
    sigset_t sigdefault;
    sigset_t sigmask;
    struct sched_param schedparam;
    int schedpolicy;
} posix_spawnattr_t;

// 标志
#define POSIX_SPAWN_RESETIDS        0x01
#define POSIX_SPAWN_SETPGROUP       0x02
#define POSIX_SPAWN_SETSIGDEF       0x04
#define POSIX_SPAWN_SETSIGMASK      0x08
#define POSIX_SPAWN_SETSCHEDPARAM   0x10
#define POSIX_SPAWN_SETSCHEDULER    0x20

// 应用属性
static int apply_spawn_attributes(pcb_t *proc, const posix_spawnattr_t *attr)
{
    if (attr->flags & POSIX_SPAWN_SETPGROUP) {
        proc->pgid = attr->pgroup;
    }

    if (attr->flags & POSIX_SPAWN_SETSIGMASK) {
        proc->sig_blocked = attr->sigmask;
    }

    if (attr->flags & POSIX_SPAWN_SETSIGDEF) {
        // 重置信号处理器为默认
        for (int i = 0; i < NSIG; i++) {
            if (sigismember(&attr->sigdefault, i)) {
                proc->sig_handlers[i].sa_handler = SIG_DFL;
            }
        }
    }

    if (attr->flags & POSIX_SPAWN_SETSCHEDULER) {
        // 设置调度策略
        struct k_thread *thread = proc->main_thread;
        k_thread_priority_set(thread, attr->schedparam.sched_priority);
    }

    return 0;
}
```

## 文件操作

```c
typedef struct {
    enum {
        SPAWN_FILE_ACTION_OPEN,
        SPAWN_FILE_ACTION_CLOSE,
        SPAWN_FILE_ACTION_DUP2
    } action;
    int fd;
    union {
        struct {
            char *path;
            int oflag;
            mode_t mode;
        } open;
        struct {
            int newfd;
        } dup2;
    };
} spawn_file_action_t;

typedef struct {
    spawn_file_action_t *actions;
    int count;
} posix_spawn_file_actions_t;

// 应用文件操作
static int apply_file_actions(pcb_t *proc,
                               const posix_spawn_file_actions_t *actions)
{
    for (int i = 0; i < actions->count; i++) {
        spawn_file_action_t *act = &actions->actions[i];

        switch (act->action) {
        case SPAWN_FILE_ACTION_OPEN:
            // 打开文件并分配到指定fd
            int fd = open(act->open.path, act->open.oflag, act->open.mode);
            if (fd < 0) {
                return -errno;
            }
            if (fd != act->fd) {
                dup2(fd, act->fd);
                close(fd);
            }
            break;

        case SPAWN_FILE_ACTION_CLOSE:
            close(act->fd);
            break;

        case SPAWN_FILE_ACTION_DUP2:
            dup2(act->fd, act->dup2.newfd);
            break;
        }
    }

    return 0;
}
```

## 创建主线程

```c
static int create_process_main_thread(pcb_t *proc, uint32_t entry)
{
    // 线程入口包装器
    void *thread_entry(void *arg) {
        pcb_t *proc = (pcb_t *)arg;

        // 设置当前进程
        set_current_process(proc);

        // 跳转到ELF入口点
        typedef int (*entry_func_t)(int, char **, char **);
        entry_func_t entry = (entry_func_t)proc->entry_point;

        int ret = entry(proc->entry_argc,
                       proc->entry_argv,
                       proc->entry_envp);

        // 进程退出
        exit(ret);

        return NULL;
    }

    // 创建线程
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstackaddr(&attr, proc->stack_start);
    pthread_attr_setstacksize(&attr, proc->stack_size);

    int ret = pthread_create(&proc->main_pthread, &attr,
                            thread_entry, proc);
    if (ret != 0) {
        return -ret;
    }

    proc->main_thread = pthread_to_k_thread(proc->main_pthread);

    return 0;
}
```

## 使用示例

```c
// 示例1：简单spawn
pid_t pid;
char *argv[] = {"/bin/ls", "-l", NULL};
char *envp[] = {"PATH=/bin", NULL};

int ret = posix_spawn(&pid, "/bin/ls", NULL, NULL, argv, envp);
if (ret == 0) {
    printf("Spawned process %d\n", pid);

    int status;
    waitpid(pid, &status, 0);
    printf("Process exited with status %d\n", WEXITSTATUS(status));
}

// 示例2：重定向标准输出
posix_spawn_file_actions_t actions;
posix_spawn_file_actions_init(&actions);
posix_spawn_file_actions_addopen(&actions, 1, "/tmp/output.txt",
                                  O_WRONLY | O_CREAT | O_TRUNC, 0644);

ret = posix_spawn(&pid, "/bin/myapp", &actions, NULL, argv, envp);

posix_spawn_file_actions_destroy(&actions);

// 示例3：设置进程组
posix_spawnattr_t attr;
posix_spawnattr_init(&attr);
posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETPGROUP);
posix_spawnattr_setpgroup(&attr, 0);  // 新进程组

ret = posix_spawn(&pid, "/bin/daemon", NULL, &attr, argv, envp);

posix_spawnattr_destroy(&attr);
```
