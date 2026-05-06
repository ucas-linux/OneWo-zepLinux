# Thread-as-Process模型

## 概述

由于ARM Cortex-M3没有MMU，无法实现真正的fork()，我们采用"Thread-as-Process"模型：每个"进程"实际上是一个隔离的线程，使用MPU提供内存保护。

## 核心概念

### 传统Linux进程 vs Thread-as-Process

```
Linux进程（有MMU）:
- 独立地址空间（通过MMU隔离）
- fork()创建完整副本
- exec()加载新程序
- 进程间完全隔离

Thread-as-Process（无MMU）:
- 共享地址空间
- 使用MPU提供有限隔离
- 通过spawn创建新"进程"
- 资源跟踪和清理
```

## 设计目标

- 提供类似进程的抽象
- 使用MPU隔离内存区域
- 支持进程ID（PID）
- 支持父子关系
- 支持wait/waitpid
- 资源跟踪和自动清理
- 支持posix_spawn()作为fork/exec替代

## 进程控制块（PCB）

```c
typedef struct pcb {
    // 进程标识
    pid_t pid;                  // 进程ID
    pid_t ppid;                 // 父进程ID
    pid_t pgid;                 // 进程组ID

    // 状态
    enum {
        PROC_RUNNING,
        PROC_SLEEPING,
        PROC_ZOMBIE,
        PROC_TERMINATED
    } state;

    // 退出状态
    int exit_status;
    bool exited;

    // 线程
    struct k_thread *main_thread;
    pthread_t main_pthread;

    // 内存布局
    void *code_start;
    size_t code_size;
    void *data_start;
    size_t data_size;
    void *heap_start;
    size_t heap_size;
    void *stack_start;
    size_t stack_size;

    // MPU内存域
    struct k_mem_domain mem_domain;

    // 文件描述符表
    struct fd_entry *fd_table;
    int max_fds;

    // 信号
    sigset_t sig_pending;
    sigset_t sig_blocked;
    struct sigaction sig_handlers[NSIG];

    // 资源跟踪
    struct resource_entry *resources;
    struct k_mutex resource_lock;

    // 子进程列表
    struct pcb *children;
    struct pcb *next_sibling;

    // 等待队列
    struct k_sem wait_sem;

} pcb_t;
```

## 进程创建：posix_spawn()

```c
#include <spawn.h>

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);

// 使用示例
pid_t pid;
char *argv[] = {"/bin/myapp", "arg1", "arg2", NULL};
char *envp[] = {"PATH=/bin", NULL};

int ret = posix_spawn(&pid, "/bin/myapp", NULL, NULL, argv, envp);
if (ret == 0) {
    // 成功创建进程
    printf("Spawned process: %d\n", pid);
}
```

## 进程生命周期

```
创建 → 运行 → 退出 → 僵尸 → 回收
  ↓      ↓      ↓      ↓      ↓
spawn  exec   exit   wait   清理
```

## MPU隔离

```c
// 为进程配置MPU区域
static int configure_process_mpu(pcb_t *proc)
{
    // 代码段：只读+可执行
    struct k_mem_partition code_part = {
        .start = (uintptr_t)proc->code_start,
        .size = proc->code_size,
        .attr = K_MEM_PARTITION_P_RX_U_RX
    };

    // 数据段：读写+不可执行
    struct k_mem_partition data_part = {
        .start = (uintptr_t)proc->data_start,
        .size = proc->data_size,
        .attr = K_MEM_PARTITION_P_RW_U_RW
    };

    // 堆：读写+不可执行
    struct k_mem_partition heap_part = {
        .start = (uintptr_t)proc->heap_start,
        .size = proc->heap_size,
        .attr = K_MEM_PARTITION_P_RW_U_RW
    };

    // 栈：读写+不可执行
    struct k_mem_partition stack_part = {
        .start = (uintptr_t)proc->stack_start,
        .size = proc->stack_size,
        .attr = K_MEM_PARTITION_P_RW_U_RW
    };

    // 初始化内存域
    k_mem_domain_init(&proc->mem_domain, 0, NULL);
    k_mem_domain_add_partition(&proc->mem_domain, &code_part);
    k_mem_domain_add_partition(&proc->mem_domain, &data_part);
    k_mem_domain_add_partition(&proc->mem_domain, &heap_part);
    k_mem_domain_add_partition(&proc->mem_domain, &stack_part);

    // 将线程添加到内存域
    k_mem_domain_add_thread(&proc->mem_domain, proc->main_thread);

    return 0;
}
```

## 与其他模块的关系

```
┌─────────────────┐
│  ELF Loader     │ (Spec 16)
│  加载二进制      │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Thread-Process  │ ← 本模块
│  spawn/wait     │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Syscall Layer   │ (Spec 17)
│  系统调用        │
└─────────────────┘
```

## 配置选项

```kconfig
config THREAD_PROCESS_MODEL
    bool "Thread-as-process model"
    depends on !MMU
    default n
    help
      Enable thread-as-process model for no-MMU targets.

config MAX_PROCESSES
    int "Maximum number of processes"
    depends on THREAD_PROCESS_MODEL
    default 16
    range 2 256

config PROCESS_DEFAULT_HEAP
    int "Default process heap size"
    depends on THREAD_PROCESS_MODEL
    default 4096

config PROCESS_DEFAULT_STACK
    int "Default process stack size"
    depends on THREAD_PROCESS_MODEL
    default 2048
```

## 文件组织

```
specs/23-thread-process/
├── README.md              # 本文件
├── pcb.md                 # 进程控制块
├── spawn.md               # posix_spawn实现
├── wait.md                # wait/waitpid实现
├── mpu_isolation.md       # MPU隔离
├── resource_tracking.md   # 资源跟踪
├── types.md               # 数据类型
└── implementation.md      # 实现指导
```

## 参考资料

- POSIX spawn: https://pubs.opengroup.org/onlinepubs/9699919799/functions/posix_spawn.html
- Zephyr MPU: https://docs.zephyrproject.org/latest/kernel/memory_protection/index.html
- uClinux process model: http://www.uclinux.org/
