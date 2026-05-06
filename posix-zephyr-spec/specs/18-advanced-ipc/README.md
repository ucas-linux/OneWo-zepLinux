# 高级IPC机制 - Advanced IPC

## 概述

本模块扩展基础IPC（Spec 11），提供POSIX高级进程间通信机制，包括消息队列、命名信号量、共享内存对象和UNIX域套接字。

## 目标

- 实现POSIX消息队列（mq_*）
- 实现POSIX命名信号量（sem_*）
- 实现POSIX共享内存对象（shm_*）
- 实现UNIX域套接字（AF_UNIX）
- 提供命名空间管理
- 支持进程间同步和数据交换

## 接口列表

### POSIX消息队列（7个）
```c
mqd_t mq_open(const char *name, int oflag, ...);
int mq_close(mqd_t mqdes);
int mq_unlink(const char *name);
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
int mq_getattr(mqd_t mqdes, struct mq_attr *attr);
int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr);
```

### POSIX命名信号量（5个）
```c
sem_t *sem_open(const char *name, int oflag, ...);
int sem_close(sem_t *sem);
int sem_unlink(const char *name);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
int sem_getvalue(sem_t *sem, int *sval);
```

### POSIX共享内存（2个）
```c
int shm_open(const char *name, int oflag, mode_t mode);
int shm_unlink(const char *name);
// 配合mmap/munmap使用（Spec 20）
```

### UNIX域套接字
```c
// 使用标准socket API，但地址族为AF_UNIX
int socket(AF_UNIX, SOCK_STREAM, 0);
int socket(AF_UNIX, SOCK_DGRAM, 0);
// 地址结构
struct sockaddr_un {
    sa_family_t sun_family;  // AF_UNIX
    char sun_path[108];      // 路径名
};
```

## 与基础IPC的区别

| 特性 | 基础IPC (Spec 11) | 高级IPC (本模块) |
|------|------------------|-----------------|
| 管道 | pipe() | - |
| 消息队列 | - | mq_open/mq_send/mq_receive |
| 信号量 | - | sem_open/sem_wait/sem_post |
| 共享内存 | - | shm_open + mmap |
| 套接字 | - | AF_UNIX socket |
| 命名 | 匿名 | 命名对象（字符串名称） |
| 持久性 | 进程生命周期 | 内核生命周期 |
| 优先级 | 无 | 消息队列支持优先级 |

## Zephyr适配策略

### 命名空间管理

```c
// IPC对象注册表
typedef enum {
    IPC_TYPE_MSGQ,
    IPC_TYPE_SEM,
    IPC_TYPE_SHM
} ipc_type_t;

typedef struct ipc_object {
    char name[64];              // 对象名称（如"/my_queue"）
    ipc_type_t type;            // 对象类型
    void *handle;               // 指向实际对象
    int refcount;               // 引用计数
    mode_t mode;                // 访问权限
    struct ipc_object *next;
} ipc_object_t;

// 全局IPC注册表
static ipc_object_t *g_ipc_registry = NULL;
static struct k_mutex g_ipc_lock;
```

### 消息队列实现

```c
// POSIX消息队列封装Zephyr k_msgq
typedef struct {
    struct k_msgq kmsgq;
    char *buffer;
    struct mq_attr attr;
    int flags;                  // O_NONBLOCK等
} posix_mq_t;

mqd_t mq_open(const char *name, int oflag, ...)
{
    // 1. 查找或创建IPC对象
    // 2. 初始化k_msgq
    // 3. 注册到命名空间
    // 4. 返回描述符
}
```

### 命名信号量实现

```c
// POSIX信号量封装Zephyr k_sem
typedef struct {
    struct k_sem ksem;
    unsigned int value;
} posix_sem_t;

sem_t *sem_open(const char *name, int oflag, ...)
{
    // 1. 查找或创建IPC对象
    // 2. 初始化k_sem
    // 3. 注册到命名空间
    // 4. 返回指针
}
```

### 共享内存实现

```c
// POSIX共享内存对象
typedef struct {
    void *addr;
    size_t size;
    int flags;
    struct k_mem_partition partition;  // MPU分区
} posix_shm_t;

int shm_open(const char *name, int oflag, mode_t mode)
{
    // 1. 查找或创建IPC对象
    // 2. 分配文件描述符
    // 3. 配合ftruncate设置大小
    // 4. 配合mmap映射到进程地址空间
}
```

### UNIX域套接字实现

```c
// 使用Zephyr的k_pipe或k_msgq模拟
typedef struct {
    int type;                   // SOCK_STREAM或SOCK_DGRAM
    struct sockaddr_un addr;    // 绑定地址
    struct k_pipe pipe;         // 用于SOCK_STREAM
    struct k_msgq msgq;         // 用于SOCK_DGRAM
    bool connected;
} unix_socket_t;
```

## 使用场景

```c
// 场景1：生产者-消费者（消息队列）
// 生产者
mqd_t mq = mq_open("/work_queue", O_CREAT | O_WRONLY, 0644, &attr);
mq_send(mq, "task1", 5, 0);
mq_close(mq);

// 消费者
mqd_t mq = mq_open("/work_queue", O_RDONLY);
char buf[256];
mq_receive(mq, buf, sizeof(buf), NULL);
mq_close(mq);

// 场景2：互斥访问（信号量）
sem_t *sem = sem_open("/resource_lock", O_CREAT, 0644, 1);
sem_wait(sem);
// 临界区
sem_post(sem);
sem_close(sem);

// 场景3：共享内存
int fd = shm_open("/shared_data", O_CREAT | O_RDWR, 0644);
ftruncate(fd, 4096);
void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
// 读写共享内存
munmap(ptr, 4096);
close(fd);

// 场景4：本地进程通信（UNIX套接字）
int sock = socket(AF_UNIX, SOCK_STREAM, 0);
struct sockaddr_un addr = {.sun_family = AF_UNIX, .sun_path = "/tmp/my.sock"};
bind(sock, (struct sockaddr *)&addr, sizeof(addr));
listen(sock, 5);
```

## 与其他模块的关系

- **基础IPC (Spec 11)**：提供pipe等基础机制
- **内存映射 (Spec 20)**：共享内存需要mmap支持
- **网络 (Spec 10)**：UNIX域套接字使用socket API
- **文件系统 (Spec 07)**：命名对象可能需要文件系统支持
- **线程-进程 (Spec 23)**：提供进程隔离和MPU配置

## 配置选项

```kconfig
config POSIX_ADVANCED_IPC
    bool "Advanced IPC support"
    default n
    help
      Enable POSIX message queues, named semaphores, and shared memory.

config POSIX_MQ
    bool "POSIX message queues"
    depends on POSIX_ADVANCED_IPC
    default y

config POSIX_MQ_MAX
    int "Maximum message queues"
    depends on POSIX_MQ
    default 16
    range 1 64

config POSIX_NAMED_SEM
    bool "POSIX named semaphores"
    depends on POSIX_ADVANCED_IPC
    default y

config POSIX_SEM_MAX
    int "Maximum named semaphores"
    depends on POSIX_NAMED_SEM
    default 32
    range 1 128

config POSIX_SHM
    bool "POSIX shared memory objects"
    depends on POSIX_ADVANCED_IPC && POSIX_MMAP
    default n

config POSIX_SHM_MAX
    int "Maximum shared memory objects"
    depends on POSIX_SHM
    default 8
    range 1 32

config UNIX_SOCKET
    bool "UNIX domain sockets"
    depends on POSIX_ADVANCED_IPC
    default n
```

## 文件组织

```
specs/18-advanced-ipc/
├── README.md              # 本文件
├── mq.md                  # 消息队列实现
├── named_sem.md           # 命名信号量实现
├── shm.md                 # 共享内存实现
├── unix_socket.md         # UNIX域套接字实现
├── namespace.md           # 命名空间管理
├── types.md               # 数据类型定义
└── implementation.md      # 实现指导
```

## 性能考虑

- 消息队列：零拷贝优化（传递指针而非数据）
- 信号量：最低开销的同步原语
- 共享内存：最高效的数据交换（无拷贝）
- UNIX套接字：比网络套接字快，但比共享内存慢
- 命名查找：使用哈希表加速

## 限制

1. 命名对象不持久化（重启后丢失）
2. 不支持跨设备IPC（仅单MCU内）
3. 共享内存依赖MPU，某些平台可能不支持
4. 消息队列不支持优先级（Zephyr k_msgq限制）
5. UNIX套接字功能受限（无SCM_RIGHTS等高级特性）

## 参考资料

- POSIX Message Queues: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
- POSIX Semaphores: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html
- POSIX Shared Memory: https://pubs.opengroup.org/onlinepubs/9699919799/functions/shm_open.html
- UNIX Domain Sockets: https://man7.org/linux/man-pages/man7/unix.7.html
