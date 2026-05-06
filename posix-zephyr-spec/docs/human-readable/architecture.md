# POSIX接口Zephyr适配架构设计

## 1. 概述

本文档描述在Zephyr RTOS上实现**Linux syscall/ABI 兼容层**的整体架构设计，并明确在无MMU目标上的限制与退化方案。

### 1.1 目标

- 覆盖Linux syscall/ABI兼容所需的核心系统调用集合（以目标应用子集为范围）
- 允许**Linux原生ELF应用**在满足硬件/内核前提时运行
- 保持实时性和资源效率

### 1.2 目标平台

- **硬件**：ARM Cortex-M3（无MMU，有MPU）
- **操作系统**：Zephyr RTOS
- **内存模型**：单地址空间
- **兼容目标**：Linux syscall/ABI（需要ELF装载与syscall入口）

> 重要限制：Cortex-M3无MMU时**无法提供完整Linux进程语义**（如`fork`、`mmap`完整保护、进程地址空间隔离）。在该类目标上仅提供**受限兼容层**或返回`ENOSYS`。

## 2. 架构分层

```
┌─────────────────────────────────────────────────────┐
│          Linux POSIX应用层                           │
│  (pthread, fork, socket, file I/O等标准调用)         │
└─────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────┐
│          POSIX适配层 (本项目核心)                    │
│  ┌─────────────┬──────────────┬─────────────────┐  │
│  │ 线程管理    │ 进程模拟     │ 文件系统        │  │
│  │ 适配器      │ 适配器       │ 适配器          │  │
│  ├─────────────┼──────────────┼─────────────────┤  │
│  │ IPC适配器   │ 网络适配器   │ 信号/定时器     │  │
│  │             │              │ 适配器          │  │
│  └─────────────┴──────────────┴─────────────────┘  │
└─────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────┐
│          Zephyr RTOS内核                             │
│  ┌─────────────┬──────────────┬─────────────────┐  │
│  │ k_thread    │ k_mutex      │ k_timer         │  │
│  ├─────────────┼──────────────┼─────────────────┤  │
│  │ k_msgq      │ k_sem        │ device API      │  │
│  └─────────────┴──────────────┴─────────────────┘  │
└─────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────┐
│          ARM Cortex-M3硬件                           │
│  (MPU, SysTick, NVIC, 内存, 外设)                   │
└─────────────────────────────────────────────────────┘
```

## 3. 核心设计原则

### 3.1 动态vs静态内存

**挑战**：Zephyr传统使用静态内存分配（编译时确定），Linux使用动态分配。

**解决方案**：
- 引入动态内存池机制
- pthread_create使用k_malloc动态分配线程栈
- 维护资源跟踪表，确保资源正确回收
- 提供编译选项在静态/动态间切换

### 3.2 进程与ABI兼容

**挑战**：Linux ABI需要进程隔离、地址空间管理、ELF装载与系统调用入口。Cortex-M3无MMU使得这些能力不完整。

**可行方案**：
- **MMU目标**：提供进程抽象（pid/文件表/信号/资源），实现ELF装载与syscall入口；支持受限的`fork/exec/mmap`语义。
- **无MMU目标**：不实现真正`fork`与地址空间替换，相关syscall返回`ENOSYS`或`ENOEXEC`；支持线程级兼容与受限IPC。

### 3.3 调度策略映射

**挑战**：Linux调度策略（SCHED_FIFO, SCHED_RR）与Zephyr优先级模型不同。

**解决方案**：
- 建立优先级映射表：Linux优先级(1-99) → Zephyr优先级
- SCHED_FIFO → Zephyr协作式线程（负优先级）
- SCHED_RR → Zephyr抢占式线程（正优先级）
- 实现优先级对齐引擎，动态调整TCB

### 3.4 线程同步

**挑战**：pthread_join需要保留线程退出状态，Zephyr默认不保留。

**解决方案**：
- 为每个线程附加同步信号量和状态缓冲区
- 线程退出时拦截，保存退出码
- pthread_join等待信号量，读取退出码
- 确保资源在join后正确释放

### 3.5 异步终止安全

**挑战**：pthread_cancel需要安全终止线程，避免资源泄漏。

**解决方案**：
- 引入"取消点"机制
- 在阻塞调用中植入检测逻辑
- 线程运行到安全断点后释放资源再自毁
- 支持PTHREAD_CANCEL_DEFERRED延迟取消

## 4. 关键模块设计

### 4.1 线程管理模块

```c
// 线程控制块扩展
typedef struct {
    struct k_thread zephyr_thread;  // Zephyr原生线程
    pthread_t pthread_id;            // POSIX线程ID
    void *(*start_routine)(void *); // 线程函数
    void *arg;                       // 线程参数
    void *retval;                    // 返回值
    struct k_sem join_sem;           // join同步信号量
    bool detached;                   // 是否分离
    bool cancelled;                  // 取消标志
    struct k_mutex *held_mutexes;    // 持有的锁列表
} posix_thread_t;
```

**关键功能**：
- pthread_create：动态分配栈，创建k_thread
- pthread_join：等待信号量，获取返回值
- pthread_cancel：设置取消标志，在取消点检查
- 互斥锁：封装k_mutex，支持递归锁和错误检查

### 4.2 进程管理模块

```c
// 进程控制块
typedef struct {
    pid_t pid;                       // 进程ID
    pid_t ppid;                      // 父进程ID
    struct k_thread *main_thread;    // 主线程
    struct k_mem_domain mem_domain;  // MPU内存域
    void *heap_start;                // 堆起始地址
    size_t heap_size;                // 堆大小
    int exit_status;                 // 退出状态
    struct k_sem wait_sem;           // wait同步
} posix_process_t;
```

**关键功能**：
- fork：仅在具备进程隔离与ELF加载的目标上支持；无MMU时返回`ENOSYS`
- exec：需要Linux ABI兼容的ELF装载器（LLEXT不等价于Linux ELF）
- exit：遍历资源跟踪表，释放所有资源
- wait/waitpid：仅在支持子进程语义时可用

### 4.3 文件系统模块

```c
// 文件描述符表
typedef struct {
    int fd;                          // 文件描述符
    enum fd_type {
        FD_FILE,
        FD_DEVICE,
        FD_SOCKET,
        FD_PIPE
    } type;
    union {
        struct fs_file_t *file;      // 文件
        const struct device *dev;    // 设备
        int socket;                  // Socket
        struct k_pipe *pipe;         // 管道
    } handle;
    int flags;                       // 打开标志
} fd_entry_t;
```

**关键功能**：
- open：查找设备/文件，分配fd
- read/write：根据fd类型分发到具体实现
- ioctl：命令转换器，调用设备control回调
- close：释放fd和底层资源

### 4.4 网络Socket模块

```c
// Socket描述符
typedef struct {
    int sockfd;                      // Socket文件描述符
    int domain;                      // AF_INET, AF_INET6
    int type;                        // SOCK_STREAM, SOCK_DGRAM
    int protocol;                    // IPPROTO_TCP, IPPROTO_UDP
    struct zsock_pollfd *pollfd;     // Zephyr socket
    struct sockaddr_storage addr;    // 绑定地址
} posix_socket_t;
```

**关键功能**：
- socket：创建zsock，分配fd
- bind/listen/accept：映射到zsock API
- send/recv：封装zsock_send/zsock_recv
- setsockopt/getsockopt：选项转换

### 4.5 IPC模块

**消息队列**：
- mq_open：创建k_msgq
- mq_send/mq_receive：封装k_msgq_put/k_msgq_get

**信号量**：
- sem_open：创建k_sem
- sem_wait/sem_post：封装k_sem_take/k_sem_give

**共享内存**：
- shm_open：分配内存区域，配置MPU
- mmap：无MMU时仅支持受限匿名共享映射；文件映射与保护标志不完整

## 5. 资源管理

### 5.1 资源跟踪表

```c
typedef struct {
    pid_t owner_pid;                 // 所属进程
    enum resource_type {
        RES_THREAD,
        RES_MUTEX,
        RES_SEM,
        RES_MSGQ,
        RES_TIMER,
        RES_FD,
        RES_MEMORY
    } type;
    void *handle;                    // 资源句柄
    struct resource_entry *next;     // 链表
} resource_entry_t;
```

**功能**：
- 进程创建资源时自动注册
- 进程退出时自动遍历并释放
- 防止内存泄漏和资源耗尽

### 5.2 内存管理

- 使用k_heap实现malloc/free
- 每个进程有独立的堆区域
- MPU保护堆边界，防止越界访问
- 支持realloc的原地扩展和移动

## 6. 性能优化

### 6.1 快速路径优化

- 常用操作（mutex_lock, sem_wait）直接调用内核API
- 避免不必要的参数检查和转换
- 使用内联函数减少调用开销

### 6.2 内存优化

- 共享只读数据（代码段、常量）
- 延迟分配（按需创建资源）
- 资源池复用（避免频繁分配释放）

### 6.3 实时性保证

- 关键路径禁用抢占
- 优先级继承防止优先级反转
- 中断处理与线程分离

## 7. 测试策略

### 7.1 单元测试

- 每个接口独立测试
- 覆盖正常路径和错误路径
- 使用Zephyr测试框架

### 7.2 集成测试

- 多线程并发测试
- 进程间通信测试
- 资源泄漏检测

### 7.3 兼容性测试

- 运行标准POSIX测试套件
- 移植真实Linux应用验证

## 8. 未来扩展

- 支持更多POSIX接口
- 优化性能和内存占用
- 支持其他架构（Cortex-M4, M7, RISC-V）
- 完善调试和诊断工具
