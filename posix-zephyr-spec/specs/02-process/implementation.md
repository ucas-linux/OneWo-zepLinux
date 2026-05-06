# 实现指导

## 总体架构

### 模块组织

```
posix_process/
├── process_core.c          # 核心进程管理
├── fork.c                  # fork实现
├── exec.c                  # exec实现
├── exit.c                  # exit和atexit实现
├── wait.c                  # wait/waitpid实现
├── process_internal.h      # 内部数据结构和函数
├── mpu_config.c            # MPU配置管理
└── resource_tracker.c      # 资源跟踪
```

### 全局数据结构

```c
// 全局进程表
static pcb_t g_process_table[MAX_PROCESSES];
static struct k_mutex g_process_table_lock;
static atomic_t g_next_pid = ATOMIC_INIT(2);

// init进程
static pcb_t *g_init_process = NULL;

// 当前进程TLS键
static pthread_key_t g_current_process_key;
```

## 核心实现

### 1. 进程表管理

```c
// 初始化进程表
static int init_process_table(void)
{
    k_mutex_init(&g_process_table_lock);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        g_process_table[i].state = PROC_TERMINATED;
        g_process_table[i].pid = PID_INVALID;
    }

    return 0;
}

// 分配进程槽位
static pcb_t *alloc_process(void)
{
    k_mutex_lock(&g_process_table_lock, K_FOREVER);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (g_process_table[i].state == PROC_TERMINATED) {
            pcb_t *proc = &g_process_table[i];
            memset(proc, 0, sizeof(*proc));
            proc->state = PROC_RUNNING;
            k_mutex_unlock(&g_process_table_lock);
            return proc;
        }
    }

    k_mutex_unlock(&g_process_table_lock);
    return NULL;
}

// 释放进程槽位
static void free_process(pcb_t *proc)
{
    k_mutex_lock(&g_process_table_lock, K_FOREVER);
    proc->state = PROC_TERMINATED;
    proc->pid = PID_INVALID;
    k_mutex_unlock(&g_process_table_lock);
}
```

### 2. PID管理

```c
static pid_t generate_pid(void)
{
    pid_t pid;

    do {
        pid = atomic_inc(&g_next_pid);
        if (pid >= MAX_PID) {
            atomic_set(&g_next_pid, 2);
            pid = 2;
        }
    } while (find_process_by_pid(pid) != NULL);

    return pid;
}
```

### 3. MPU配置

```c
// 配置进程的MPU内存域
static int configure_process_mpu(pcb_t *proc)
{
    // 初始化内存域
    k_mem_domain_init(&proc->mem_domain, 0, NULL);

    // 添加代码段（只读执行，所有进程共享）
    extern char __text_start[], __text_end[];
    struct k_mem_partition code_part = {
        .start = (uintptr_t)__text_start,
        .size = (size_t)(__text_end - __text_start),
        .attr = K_MEM_PARTITION_P_RX_U_RX
    };
    k_mem_domain_add_partition(&proc->mem_domain, &code_part);

    // 添加只读数据段（所有进程共享）
    extern char __rodata_start[], __rodata_end[];
    struct k_mem_partition rodata_part = {
        .start = (uintptr_t)__rodata_start,
        .size = (size_t)(__rodata_end - __rodata_start),
        .attr = K_MEM_PARTITION_P_RO_U_RO
    };
    k_mem_domain_add_partition(&proc->mem_domain, &rodata_part);

    // 添加栈分区（进程私有，读写）
    if (proc->stack_start && proc->stack_size > 0) {
        struct k_mem_partition stack_part = {
            .start = (uintptr_t)proc->stack_start,
            .size = proc->stack_size,
            .attr = K_MEM_PARTITION_P_RW_U_RW
        };
        k_mem_domain_add_partition(&proc->mem_domain, &stack_part);
    }

    // 添加堆分区（进程私有，读写）
    if (proc->heap_start && proc->heap_size > 0) {
        struct k_mem_partition heap_part = {
            .start = (uintptr_t)proc->heap_start,
            .size = proc->heap_size,
            .attr = K_MEM_PARTITION_P_RW_U_RW
        };
        k_mem_domain_add_partition(&proc->mem_domain, &heap_part);
    }

    return 0;
}
```

### 4. 资源跟踪

```c
// 注册资源
void resource_register(pcb_t *proc, resource_type_t type, void *resource)
{
    resource_entry_t *entry = k_malloc(sizeof(resource_entry_t));
    if (!entry) return;

    entry->type = type;
    entry->resource = resource;

    k_mutex_lock(&proc->resource_lock, K_FOREVER);
    entry->next = proc->resources;
    proc->resources = entry;
    k_mutex_unlock(&proc->resource_lock);
}

// 注销资源
void resource_unregister(pcb_t *proc, void *resource)
{
    k_mutex_lock(&proc->resource_lock, K_FOREVER);

    resource_entry_t **pp = &proc->resources;
    while (*pp) {
        if ((*pp)->resource == resource) {
            resource_entry_t *entry = *pp;
            *pp = entry->next;
            k_free(entry);
            break;
        }
        pp = &(*pp)->next;
    }

    k_mutex_unlock(&proc->resource_lock);
}

// 清理所有资源
void resource_cleanup_all(pcb_t *proc)
{
    k_mutex_lock(&proc->resource_lock, K_FOREVER);

    resource_entry_t *entry = proc->resources;
    while (entry) {
        resource_entry_t *next = entry->next;

        switch (entry->type) {
        case RES_THREAD:
            pthread_cancel((pthread_t)(uintptr_t)entry->resource);
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
            timer_delete((timer_t)(uintptr_t)entry->resource);
            break;
        case RES_FD:
            close((int)(uintptr_t)entry->resource);
            break;
        case RES_MEMORY:
            k_free(entry->resource);
            break;
        }

        k_free(entry);
        entry = next;
    }

    proc->resources = NULL;
    k_mutex_unlock(&proc->resource_lock);
}
```

## 初始化和清理

### 模块初始化

```c
int process_subsystem_init(void)
{
    // 初始化进程表
    init_process_table();

    // 创建TLS键
    pthread_key_create(&g_current_process_key, NULL);

    // 创建init进程
    g_init_process = alloc_process();
    if (!g_init_process) {
        return -ENOMEM;
    }

    g_init_process->pid = PID_INIT;
    g_init_process->ppid = 0;
    g_init_process->state = PROC_RUNNING;

    // init进程使用主线程
    g_init_process->main_thread = k_current_get();

    // 配置init进程的MPU
    configure_process_mpu(g_init_process);

    // 设置当前进程为init
    set_current_process(g_init_process);

    return 0;
}

// 使用Zephyr初始化系统
SYS_INIT(process_subsystem_init, POST_KERNEL,
         CONFIG_PROCESS_INIT_PRIORITY);
```

## fork实现要点

在无MMU目标上，`fork`不可实现，应直接返回`ENOSYS`。禁止通过栈拷贝/寄存器快照伪造`fork`语义。

```c
pid_t fork(void)
{
    errno = ENOSYS;
    return -1;
}
```

## exec实现要点

`exec`要求Linux ABI兼容的ELF装载器与进程隔离。LLEXT仅适用于Zephyr扩展模块，不能满足Linux ELF语义。

```c
int execve(const char *path, char *const argv[], char *const envp[])
{
    errno = ENOEXEC; // 或ENOSYS，取决于配置
    return -1;
}
```

## 配置选项

### Kconfig

```kconfig
menuconfig POSIX_PROCESS
    bool "POSIX Process Support"
    depends on PTHREAD
    depends on MMU
    default n
    help
      Enable process support for Linux ABI (fork/exec/exit).

if POSIX_PROCESS

config MAX_PROCESSES
    int "Maximum number of processes"
    default 16
    range 2 256
    help
      Maximum number of processes that can exist simultaneously.

config PROCESS_DEFAULT_HEAP
    int "Default process heap size"
    default 4096
    range 1024 65536
    help
      Default heap size for new processes.

config PROCESS_DEFAULT_STACK
    int "Default process stack size"
    default 2048
    range 1024 65536
    help
      Default stack size for new processes.

config LINUX_ABI_LOADER
    bool "Enable Linux ELF loader for exec"
    depends on MMU
    default n
    help
      Enable exec family functions using Linux ABI compatible ELF loading.

config PROCESS_INIT_PRIORITY
    int "Process subsystem initialization priority"
    default 50
    help
      Initialization priority for process subsystem.

config PROCESS_DEBUG
    bool "Enable process debugging"
    default n
    help
      Enable debug output for process operations.

endif # POSIX_PROCESS
```

## 调试支持

### 调试宏

```c
#ifdef CONFIG_PROCESS_DEBUG
#define PROC_DEBUG(fmt, ...) \
    printk("[PROC] " fmt "\n", ##__VA_ARGS__)
#else
#define PROC_DEBUG(fmt, ...)
#endif

#define PROC_ERROR(fmt, ...) \
    printk("[PROC ERROR] " fmt "\n", ##__VA_ARGS__)
```

### 进程状态转储

```c
void dump_process_table(void)
{
    printk("=== Process Table ===\n");

    k_mutex_lock(&g_process_table_lock, K_FOREVER);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_t *proc = &g_process_table[i];

        if (proc->state != PROC_TERMINATED) {
            printk("Process %d:\n", proc->pid);
            printk("  PPID: %d\n", proc->ppid);
            printk("  State: %d\n", proc->state);
            printk("  Exit Status: %d\n", proc->exit_status);
            printk("  Stack: %p (%zu bytes)\n",
                   proc->stack_start, proc->stack_size);
            printk("  Heap: %p (%zu bytes)\n",
                   proc->heap_start, proc->heap_size);
        }
    }

    k_mutex_unlock(&g_process_table_lock);
}
```

## 性能优化

### 1. 内存池

```c
// 预分配内存池减少动态分配
#ifdef CONFIG_PROCESS_MEMORY_POOL
K_MEM_POOL_DEFINE(process_pool,
                  CONFIG_PROCESS_DEFAULT_HEAP,
                  CONFIG_PROCESS_DEFAULT_HEAP * MAX_PROCESSES,
                  MAX_PROCESSES,
                  4);

static void *process_alloc(size_t size)
{
    return k_mem_pool_malloc(&process_pool, size);
}

static void process_free(void *ptr)
{
    k_mem_pool_free(ptr);
}
#else
#define process_alloc k_malloc
#define process_free k_free
#endif
```

### 2. 快速路径

```c
// getpid快速路径
pid_t getpid(void)
{
    pcb_t *proc = get_current_process();
    return proc ? proc->pid : PID_INIT;
}

// getppid快速路径
pid_t getppid(void)
{
    pcb_t *proc = get_current_process();
    return proc ? proc->ppid : 0;
}
```

## 测试框架

### 单元测试

```c
#ifdef CONFIG_PROCESS_TESTS

void test_fork_basic(void)
{
    pid_t pid = fork();
    zassert_true(pid >= 0, "fork failed");

    if (pid == 0) {
        exit(42);
    } else {
        int status;
        waitpid(pid, &status, 0);
        zassert_equal(WEXITSTATUS(status), 42, "wrong exit status");
    }
}

void test_exec_basic(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        char *argv[] = {"/bin/test", NULL};
        execv("/bin/test", argv);
        exit(1);  // 不应该到达
    } else {
        int status;
        waitpid(pid, &status, 0);
        zassert_equal(WEXITSTATUS(status), 0, "exec failed");
    }
}

ZTEST_SUITE(process_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(process_tests, test_fork) { test_fork_basic(); }
ZTEST(process_tests, test_exec) { test_exec_basic(); }

#endif
```

## 移植注意事项

### MMU目标特定

1. **用户态/内核态**：需要明确syscall入口与返回路径
2. **地址空间**：确保进程隔离与ELF装载地址布局一致
3. **栈对齐**：栈地址必须满足ABI对齐要求
4. **中断处理**：进程切换时保存/恢复中断状态

> Cortex-M3无MMU：本模块中的`fork/exec`相关能力应禁用。

### 内存限制

1. **最小RAM**：建议至少64KB
2. **进程数量**：根据可用RAM调整MAX_PROCESSES
3. **栈/堆大小**：根据应用需求调整默认值

## 常见问题

### Q: fork失败，返回EAGAIN
A: 若无MMU目标应返回ENOSYS；若支持MMU，检查MAX_PROCESSES配置和可用RAM

### Q: exec失败，返回ENOEXEC
A: 确保CONFIG_LINUX_ABI_LOADER=y且ELF文件为Linux ABI兼容格式

### Q: 进程退出后内存泄漏
A: 确保父进程调用wait/waitpid回收子进程

### Q: MPU配置失败
A: 检查内存区域大小和地址是否满足MPU对齐要求
