# 数据类型定义

## 进程相关类型

### pid_t
```c
typedef int pid_t;
```
进程标识符，用于唯一标识一个进程。

**取值范围**：
- 0: 调度器进程（保留）
- 1: init进程
- 2-MAX_PID: 用户进程

### 进程状态枚举
```c
enum process_state {
    PROC_RUNNING,      // 运行中
    PROC_ZOMBIE,       // 僵尸状态（已退出，等待回收）
    PROC_TERMINATED    // 已终止（PCB已释放）
};
```

## 进程控制块

### pcb_t
```c
typedef struct process_control_block {
    // 进程标识
    pid_t pid;                          // 进程ID
    pid_t ppid;                         // 父进程ID
    enum process_state state;           // 进程状态
    int exit_status;                    // 退出状态码

    // 线程和内存
    struct k_thread *main_thread;       // 主线程
    struct k_thread thread_obj;         // 线程对象
    struct k_mem_domain mem_domain;     // MPU内存域

    // 内存区域
    void *heap_start;                   // 堆起始地址
    size_t heap_size;                   // 堆大小
    void *stack_start;                  // 栈起始地址
    size_t stack_size;                  // 栈大小
    void *data_start;                   // 数据段起始地址
    size_t data_size;                   // 数据段大小

    // 同步和资源
    struct k_sem wait_sem;              // wait同步信号量
    int wait_count;                     // 等待的父进程数
    struct k_mutex atexit_lock;         // atexit链表锁
    atexit_func_t *atexit_list;         // 退出函数链表
    resource_entry_t *resources;        // 资源链表

    // 文件描述符表
    struct fd_entry fd_table[MAX_FDS];  // 文件描述符表

    // 环境和参数
    int argc;                           // 参数数量
    char **argv;                        // 参数数组
    char **envp;                        // 环境变量数组

    // 链表
    struct process_control_block *next; // PCB链表
} pcb_t;
```

## CPU上下文

### cpu_context
```c
struct cpu_context {
    // ARM Cortex-M3寄存器
    uint32_t r0, r1, r2, r3;
    uint32_t r4, r5, r6, r7;
    uint32_t r8, r9, r10, r11;
    uint32_t r12;
    uint32_t sp;   // PSP (Process Stack Pointer)
    uint32_t lr;   // Link Register
    uint32_t pc;   // Program Counter
    uint32_t psr;  // Program Status Register

    // 控制标志
    bool is_child; // 是否是子进程返回路径
};
```

## 退出处理

### atexit_func_t
```c
typedef struct atexit_func {
    void (*func)(void);           // 退出处理函数
    struct atexit_func *next;     // 链表指针
} atexit_func_t;
```

## 资源跟踪

### resource_entry_t
```c
typedef enum {
    RES_THREAD,    // 线程
    RES_MUTEX,     // 互斥锁
    RES_COND,      // 条件变量
    RES_SEM,       // 信号量
    RES_TIMER,     // 定时器
    RES_FD,        // 文件描述符
    RES_MEMORY     // 动态内存
} resource_type_t;

typedef struct resource_entry {
    resource_type_t type;         // 资源类型
    void *resource;               // 资源指针
    struct resource_entry *next;  // 链表指针
} resource_entry_t;
```

## 文件描述符

### fd_entry
```c
struct fd_entry {
    bool in_use;                  // 是否使用中
    int flags;                    // 文件标志（O_RDONLY等）
    int fd_flags;                 // FD标志（FD_CLOEXEC）
    void *file_obj;               // 文件对象指针
    off_t offset;                 // 当前偏移量
};
```

## 可执行文件信息

### exec_info_t
```c
typedef struct {
    void *image_handle;           // Linux ELF装载器句柄
    void *entry_point;            // 入口函数地址
    void *load_base;              // 加载基址
    size_t load_size;             // 加载映像大小
    void *phdr_table;             // 程序头表（可选）
    size_t phdr_count;            // 程序头数量
} exec_info_t;
```

## 等待状态

### wait选项
```c
#define WNOHANG    1  // 非阻塞等待
#define WUNTRACED  2  // 报告停止的子进程
#define WCONTINUED 4  // 报告继续的子进程
```

### 状态宏
```c
// 提取退出状态
#define WEXITSTATUS(status) (((status) >> 8) & 0xFF)

// 检查是否正常退出
#define WIFEXITED(status) (((status) & 0x7F) == 0)

// 检查是否被信号终止
#define WIFSIGNALED(status) (((status) & 0x7F) != 0)

// 提取终止信号
#define WTERMSIG(status) ((status) & 0x7F)

// 检查是否停止
#define WIFSTOPPED(status) (((status) & 0xFF) == 0x7F)

// 提取停止信号
#define WSTOPSIG(status) (((status) >> 8) & 0xFF)
```

## 常量定义

### 进程限制
```c
#define MAX_PROCESSES  16    // 最大进程数
#define MAX_PID        1024  // 最大PID值
#define MAX_FDS        32    // 每进程最大文件描述符数
#define ATEXIT_MAX     32    // 最大atexit函数数
```

### MPU对齐
```c
#define MPU_ALIGN      32    // MPU对齐要求（2的幂次方）
#define ALIGN_MPU(x)   (((x) + MPU_ALIGN - 1) & ~(MPU_ALIGN - 1))
```

### 特殊PID
```c
#define PID_INIT       1     // init进程PID
#define PID_INVALID    -1    // 无效PID
```

## 全局数据结构

### 进程表
```c
// 全局进程表
static pcb_t g_process_table[MAX_PROCESSES];
static struct k_mutex g_process_table_lock;
static atomic_t g_next_pid = ATOMIC_INIT(2);  // 从2开始（1是init）
```

### 当前进程
```c
// 线程本地存储键
static pthread_key_t g_current_process_key;

// 获取当前进程
static inline pcb_t *get_current_process(void)
{
    return (pcb_t *)pthread_getspecific(g_current_process_key);
}

// 设置当前进程
static inline void set_current_process(pcb_t *proc)
{
    pthread_setspecific(g_current_process_key, proc);
}
```

## 辅助宏

### PID生成
```c
static inline pid_t generate_pid(void)
{
    pid_t pid = atomic_inc(&g_next_pid);
    if (pid >= MAX_PID) {
        atomic_set(&g_next_pid, 2);
        pid = 2;
    }
    return pid;
}
```

### 进程查找
```c
static inline pcb_t *find_process_by_pid(pid_t pid)
{
    k_mutex_lock(&g_process_table_lock, K_FOREVER);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (g_process_table[i].state != PROC_TERMINATED &&
            g_process_table[i].pid == pid) {
            pcb_t *proc = &g_process_table[i];
            k_mutex_unlock(&g_process_table_lock);
            return proc;
        }
    }

    k_mutex_unlock(&g_process_table_lock);
    return NULL;
}
```

### 子进程查找
```c
static inline pcb_t *find_child_process(pcb_t *parent, pid_t pid)
{
    k_mutex_lock(&g_process_table_lock, K_FOREVER);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_t *proc = &g_process_table[i];

        if (proc->state != PROC_TERMINATED &&
            proc->ppid == parent->pid) {
            // 如果指定了pid，检查是否匹配
            if (pid > 0 && proc->pid != pid) {
                continue;
            }
            // 如果pid=-1，返回任意子进程
            // 如果pid<-1，检查进程组（未实现）

            k_mutex_unlock(&g_process_table_lock);
            return proc;
        }
    }

    k_mutex_unlock(&g_process_table_lock);
    return NULL;
}
```

## 错误码

```c
#define ECHILD     10  // 无子进程
#define EAGAIN     11  // 资源暂时不可用
#define ENOMEM     12  // 内存不足
#define EINVAL     22  // 无效参数
#define ESRCH      3   // 无此进程
#define ENOENT     2   // 文件或目录不存在
#define ENOEXEC    8   // 可执行文件格式错误
```

## 配置选项

```c
// Kconfig生成的配置
#define CONFIG_POSIX_PROCESS           1
#define CONFIG_MAX_PROCESSES           16
#define CONFIG_PROCESS_DEFAULT_HEAP    4096
#define CONFIG_PROCESS_DEFAULT_STACK   2048
#define CONFIG_LINUX_ABI_LOADER        1
#define CONFIG_MMU                     1
```

## 注意事项

1. **PID唯一性**：确保PID在系统中唯一
2. **内存对齐**：所有内存区域必须满足MPU对齐要求
3. **资源限制**：进程数量受限于MAX_PROCESSES
4. **线程安全**：所有全局数据结构访问需要加锁
5. **僵尸进程**：必须由父进程wait回收，否则泄漏PCB
