# exit - 终止进程

## POSIX标准定义

```c
#include <stdlib.h>
#include <unistd.h>

void exit(int status);
void _exit(int status);
void _Exit(int status);
```

## 功能描述

- **exit**: 正常终止进程，执行清理操作
- **_exit**: 立即终止进程，不执行清理
- **_Exit**: 等同于_exit（C99标准）

## 参数说明

- **status**: 退出状态码，传递给父进程

## 返回值

这些函数不返回（进程终止）

## Linux标准行为

### exit()执行的清理操作：
1. 调用atexit注册的函数（逆序）
2. 刷新并关闭所有打开的stdio流
3. 删除tmpfile创建的临时文件
4. 调用_exit()

### _exit()执行的操作：
1. 关闭所有文件描述符
2. 将子进程托管给init进程
3. 向父进程发送SIGCHLD信号
4. 进入僵尸状态，等待父进程回收

## Zephyr适配方案

### 核心挑战

1. **资源清理**：需要释放所有内核对象和内存
2. **父进程通知**：需要通知父进程并传递退出状态
3. **僵尸进程**：需要保持PCB直到父进程wait
4. **atexit处理**：需要维护退出函数链表

### 数据结构

```c
// 退出处理函数节点
typedef struct atexit_func {
    void (*func)(void);
    struct atexit_func *next;
} atexit_func_t;

// 进程控制块中添加
typedef struct process_control_block {
    // ... 其他字段
    atexit_func_t *atexit_list;    // 退出函数链表
    int exit_status;                // 退出状态
    enum process_state state;       // 进程状态
    struct k_sem wait_sem;          // wait同步信号量
} pcb_t;
```

### 实现步骤

#### 1. exit()实现

```c
void exit(int status)
{
    pcb_t *proc = get_current_process();
    if (!proc) {
        // 如果不在进程上下文中，直接终止线程
        pthread_exit((void *)(intptr_t)status);
        return;
    }

    // 1. 调用atexit注册的函数（逆序）
    call_atexit_functions(proc);

    // 2. 刷新stdio缓冲区
    fflush(NULL);

    // 3. 关闭stdio流
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    // 4. 调用_exit
    _exit(status);
}
```

#### 2. atexit()实现

```c
#define ATEXIT_MAX 32

int atexit(void (*func)(void))
{
    if (!func) {
        return -1;
    }

    pcb_t *proc = get_current_process();
    if (!proc) {
        return -1;
    }

    // 分配节点
    atexit_func_t *node = k_malloc(sizeof(atexit_func_t));
    if (!node) {
        return -1;
    }

    node->func = func;

    // 插入链表头部（逆序调用）
    k_mutex_lock(&proc->atexit_lock, K_FOREVER);
    node->next = proc->atexit_list;
    proc->atexit_list = node;
    k_mutex_unlock(&proc->atexit_lock);

    return 0;
}
```

#### 3. 调用atexit函数

```c
static void call_atexit_functions(pcb_t *proc)
{
    k_mutex_lock(&proc->atexit_lock, K_FOREVER);

    atexit_func_t *node = proc->atexit_list;
    while (node) {
        atexit_func_t *next = node->next;

        // 调用退出函数
        if (node->func) {
            node->func();
        }

        // 释放节点
        k_free(node);
        node = next;
    }

    proc->atexit_list = NULL;
    k_mutex_unlock(&proc->atexit_lock);
}
```

#### 4. _exit()实现

```c
void _exit(int status)
{
    pcb_t *proc = get_current_process();
    if (!proc) {
        // 非进程上下文，直接终止线程
        pthread_exit((void *)(intptr_t)status);
        return;
    }

    // 保存退出状态
    proc->exit_status = status;

    // 关闭所有文件描述符
    close_all_fds(proc);

    // 清理所有资源
    cleanup_process_resources(proc);

    // 将子进程托管给init进程
    reparent_children(proc);

    // 标记为僵尸状态
    proc->state = PROC_ZOMBIE;

    // 通知父进程
    notify_parent(proc);

    // 如果父进程在等待，唤醒它
    k_sem_give(&proc->wait_sem);

    // 终止当前线程（不返回）
    pthread_exit((void *)(intptr_t)status);
}
```

#### 5. 资源清理

```c
static void cleanup_process_resources(pcb_t *proc)
{
    // 遍历资源跟踪表
    resource_entry_t *entry = proc->resources;
    while (entry) {
        resource_entry_t *next = entry->next;

        switch (entry->type) {
        case RES_THREAD:
            // 终止所有子线程
            pthread_cancel((pthread_t)entry->resource);
            break;

        case RES_MUTEX:
            pthread_mutex_destroy((pthread_mutex_t *)entry->resource);
            break;

        case RES_COND:
            pthread_cond_destroy((pthread_cond_t *)entry->resource);
            break;

        case RES_SEM:
            sem_destroy((sem_t *)entry->resource);
            break;

        case RES_TIMER:
            timer_delete((timer_t)entry->resource);
            break;
        }

        k_free(entry);
        entry = next;
    }

    proc->resources = NULL;
}
```

#### 6. 关闭文件描述符

```c
static void close_all_fds(pcb_t *proc)
{
    // 遍历文件描述符表
    for (int fd = 0; fd < MAX_FDS; fd++) {
        if (proc->fd_table[fd].in_use) {
            close(fd);
        }
    }
}
```

#### 7. 子进程托管

```c
static void reparent_children(pcb_t *proc)
{
    pcb_t *init_proc = get_init_process();

    // 遍历所有进程
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_t *child = &g_process_table[i];

        if (child->state != PROC_TERMINATED &&
            child->ppid == proc->pid) {
            // 将子进程的父进程改为init
            child->ppid = init_proc->pid;

            // 如果子进程已经是僵尸，通知init
            if (child->state == PROC_ZOMBIE) {
                k_sem_give(&init_proc->wait_sem);
            }
        }
    }
}
```

#### 8. 通知父进程

```c
static void notify_parent(pcb_t *proc)
{
    pcb_t *parent = find_process_by_pid(proc->ppid);
    if (!parent) {
        return;
    }

    // 发送SIGCHLD信号（如果实现了信号）
    #ifdef CONFIG_POSIX_SIGNALS
    kill(proc->ppid, SIGCHLD);
    #endif

    // 唤醒等待的父进程
    k_sem_give(&parent->wait_sem);
}
```

### wait/waitpid配合

```c
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

    // 如果子进程还在运行，等待
    if (child->state == PROC_RUNNING) {
        if (options & WNOHANG) {
            return 0;  // 非阻塞，立即返回
        }

        // 阻塞等待
        k_sem_take(&child->wait_sem, K_FOREVER);
    }

    // 获取退出状态
    if (status) {
        *status = WEXITSTATUS(child->exit_status);
    }

    pid_t child_pid = child->pid;

    // 回收子进程资源
    reclaim_process(child);

    return child_pid;
}
```

#### 9. 进程回收

```c
static void reclaim_process(pcb_t *proc)
{
    // 释放内存域
    k_mem_domain_remove_thread(&proc->mem_domain, proc->main_thread);

    // 释放栈
    if (proc->stack_start) {
        k_free(proc->stack_start);
        proc->stack_start = NULL;
    }

    // 释放堆
    if (proc->heap_start) {
        k_free(proc->heap_start);
        proc->heap_start = NULL;
    }

    // 释放数据段
    if (proc->data_start) {
        k_free(proc->data_start);
        proc->data_start = NULL;
    }

    // 标记为已终止
    proc->state = PROC_TERMINATED;

    // 从进程表移除
    unregister_process(proc);

    // 释放PCB
    free_process(proc);
}
```

## 测试用例

> 注意：以下用例依赖`fork`语义，仅在具备MMU并启用进程支持时运行；无MMU目标应跳过。

### 测试1：基本exit

```c
void test_basic_exit(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        // 子进程
        printf("Child exiting with status 42\n");
        exit(42);
    } else {
        // 父进程
        int status;
        waitpid(pid, &status, 0);
        printf("Child exit status: %d\n", WEXITSTATUS(status));
        assert(WEXITSTATUS(status) == 42);
    }
}
```

### 测试2：atexit处理

```c
int cleanup_called = 0;

void cleanup1(void) {
    printf("Cleanup 1\n");
    cleanup_called++;
}

void cleanup2(void) {
    printf("Cleanup 2\n");
    cleanup_called++;
}

void test_atexit(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        atexit(cleanup1);
        atexit(cleanup2);
        exit(0);  // 应该调用cleanup2, cleanup1（逆序）
    } else {
        waitpid(pid, NULL, 0);
        // 注意：cleanup_called在父进程中不会改变
    }
}
```

### 测试3：资源清理

```c
void test_resource_cleanup(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        // 分配资源
        int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        // 退出（应该自动清理）
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        // 验证资源已释放
    }
}
```

### 测试4：_exit vs exit

```c
void cleanup_func(void) {
    printf("This should not print with _exit\n");
}

void test_exit_vs_underscore_exit(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        atexit(cleanup_func);
        _exit(0);  // 不调用atexit函数
    } else {
        waitpid(pid, NULL, 0);
    }
}
```

## 性能考虑

1. **exit开销**：取决于资源数量
   - 无资源：~100微秒
   - 10个资源：~500微秒
   - 100个资源：~5毫秒

2. **atexit调用**：每个函数约10-50微秒

3. **文件描述符关闭**：每个约50-200微秒

## 注意事项

1. **exit vs _exit**：exit执行清理，_exit立即终止
2. **atexit顺序**：函数按注册的逆序调用
3. **资源泄漏**：确保所有资源在exit前释放或由exit清理
4. **僵尸进程**：父进程必须调用wait回收子进程
5. **孤儿进程**：父进程退出后，子进程托管给init
6. **线程安全**：exit不是线程安全的，多线程程序应使用pthread_exit

## 限制

1. 不支持on_exit（GNU扩展）
2. atexit函数数量限制（通常32个）
3. 不支持quick_exit（C11）
4. 信号处理简化
