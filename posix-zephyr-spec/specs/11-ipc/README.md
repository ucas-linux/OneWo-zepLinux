# POSIX IPC接口规范

## 概述

本模块定义15个POSIX进程间通信（IPC）接口在Zephyr RTOS上的实现规范。

## 接口列表

### 消息队列（7个）
1. `mq_open` - 打开/创建消息队列
2. `mq_close` - 关闭消息队列
3. `mq_unlink` - 删除消息队列
4. `mq_send` - 发送消息
5. `mq_receive` - 接收消息
6. `mq_getattr` - 获取属性
7. `mq_setattr` - 设置属性

### 信号量（5个）
8. `sem_open` - 打开/创建信号量
9. `sem_close` - 关闭信号量
10. `sem_unlink` - 删除信号量
11. `sem_wait` - 等待信号量
12. `sem_post` - 释放信号量

### 共享内存（3个）
13. `shm_open` - 打开/创建共享内存
14. `shm_unlink` - 删除共享内存
15. `mmap/munmap` - 映射/解映射（配合shm使用）

## Zephyr现有支持

Zephyr提供类似的IPC机制：

```c
// 消息队列
struct k_msgq;
k_msgq_init(), k_msgq_put(), k_msgq_get()

// 信号量
struct k_sem;
k_sem_init(), k_sem_take(), k_sem_give()

// 共享内存（通过Memory Domain）
struct k_mem_domain;
k_mem_domain_init(), k_mem_domain_add_partition()
```

## 适配策略

### 命名空间管理

POSIX IPC对象使用字符串名称（如"/my_queue"），需要维护名称到对象的映射：

```c
// IPC对象注册表
typedef struct {
    char name[64];              // 对象名称
    enum {
        IPC_MSGQ,
        IPC_SEM,
        IPC_SHM
    } type;
    void *handle;               // 指向实际对象
    int refcount;               // 引用计数
    struct ipc_entry *next;
} ipc_entry_t;

// 全局IPC注册表
static ipc_entry_t *ipc_registry = NULL;
static struct k_mutex ipc_registry_lock;
```

### 消息队列实现

```c
// POSIX消息队列封装
typedef struct {
    struct k_msgq kmsgq;
    char *buffer;               // 消息缓冲区
    struct mq_attr attr;        // 队列属性
    int flags;                  // O_NONBLOCK等
} posix_mq_t;

mqd_t mq_open(const char *name, int oflag, ...)
{
    // 1. 查找或创建IPC对象
    ipc_entry_t *entry = find_ipc_object(name, IPC_MSGQ);

    if (!entry && (oflag & O_CREAT)) {
        // 创建新队列
        va_list args;
        va_start(args, oflag);
        mode_t mode = va_arg(args, mode_t);
        struct mq_attr *attr = va_arg(args, struct mq_attr *);
        va_end(args);

        posix_mq_t *mq = k_malloc(sizeof(*mq));
        mq->buffer = k_malloc(attr->mq_msgsize * attr->mq_maxmsg);

        k_msgq_init(&mq->kmsgq, mq->buffer,
                    attr->mq_msgsize, attr->mq_maxmsg);

        entry = register_ipc_object(name, IPC_MSGQ, mq);
    }

    if (!entry) return (mqd_t)-1;

    entry->refcount++;
    return (mqd_t)entry;
}

int mq_send(mqd_t mqdes, const char *msg_ptr,
            size_t msg_len, unsigned int msg_prio)
{
    ipc_entry_t *entry = (ipc_entry_t *)mqdes;
    posix_mq_t *mq = entry->handle;

    k_timeout_t timeout = (mq->flags & O_NONBLOCK) ? K_NO_WAIT : K_FOREVER;

    int ret = k_msgq_put(&mq->kmsgq, msg_ptr, timeout);
    return (ret == 0) ? 0 : -1;
}
```

### 信号量实现

```c
// POSIX信号量封装
typedef struct {
    struct k_sem ksem;
    unsigned int value;
} posix_sem_t;

sem_t *sem_open(const char *name, int oflag, ...)
{
    ipc_entry_t *entry = find_ipc_object(name, IPC_SEM);

    if (!entry && (oflag & O_CREAT)) {
        va_list args;
        va_start(args, oflag);
        mode_t mode = va_arg(args, mode_t);
        unsigned int value = va_arg(args, unsigned int);
        va_end(args);

        posix_sem_t *sem = k_malloc(sizeof(*sem));
        k_sem_init(&sem->ksem, value, UINT_MAX);
        sem->value = value;

        entry = register_ipc_object(name, IPC_SEM, sem);
    }

    if (!entry) return SEM_FAILED;

    entry->refcount++;
    return (sem_t *)entry;
}

int sem_wait(sem_t *sem)
{
    ipc_entry_t *entry = (ipc_entry_t *)sem;
    posix_sem_t *psem = entry->handle;

    int ret = k_sem_take(&psem->ksem, K_FOREVER);
    if (ret == 0) {
        psem->value--;
    }
    return ret;
}

int sem_post(sem_t *sem)
{
    ipc_entry_t *entry = (ipc_entry_t *)sem;
    posix_sem_t *psem = entry->handle;

    k_sem_give(&psem->ksem);
    psem->value++;
    return 0;
}
```

### 共享内存实现

```c
// POSIX共享内存对象
typedef struct {
    void *addr;                 // 内存地址
    size_t size;                // 大小
    int flags;                  // 访问标志
    struct k_mem_partition partition;  // MPU分区
} posix_shm_t;

int shm_open(const char *name, int oflag, mode_t mode)
{
    ipc_entry_t *entry = find_ipc_object(name, IPC_SHM);

    if (!entry && (oflag & O_CREAT)) {
        posix_shm_t *shm = k_malloc(sizeof(*shm));
        // 初始大小为0，通过ftruncate设置
        shm->addr = NULL;
        shm->size = 0;

        entry = register_ipc_object(name, IPC_SHM, shm);
    }

    if (!entry) return -1;

    // 分配fd
    int fd = alloc_fd();
    fd_table[fd].type = FD_TYPE_SHM;
    fd_table[fd].handle.shm = entry;

    return fd;
}

void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset)
{
    // 仅支持shm对象的匿名共享映射（无MMU目标的受限语义）
    if (fd_table[fd].type != FD_TYPE_SHM) {
        errno = ENODEV;
        return MAP_FAILED;
    }
    if (addr != NULL || (flags & MAP_FIXED)) {
        errno = ENOSYS;
        return MAP_FAILED;
    }
    if ((flags & MAP_PRIVATE) || !(flags & MAP_SHARED)) {
        errno = ENOSYS;
        return MAP_FAILED;
    }
    if (prot & PROT_EXEC) {
        errno = ENOSYS;
        return MAP_FAILED;
    }

    ipc_entry_t *entry = fd_table[fd].handle.shm;
    posix_shm_t *shm = entry->handle;

    // 如果是第一次映射，分配内存
    if (!shm->addr) {
        shm->addr = k_malloc(length);
        shm->size = length;

        // 配置MPU允许多进程访问
        shm->partition.start = (uintptr_t)shm->addr;
        shm->partition.size = length;
        shm->partition.attr = K_MEM_PARTITION_P_RW_U_RW;
    }

    // 将共享内存添加到当前进程的Memory Domain
    pcb_t *proc = current_process();
    k_mem_domain_add_partition(&proc->mem_domain, &shm->partition);

    return shm->addr;
}
```

**限制与退化**：
- 无MMU目标不支持文件映射、MAP_PRIVATE、MAP_FIXED、PROT_EXEC。
- 仅支持基于`shm_open`的匿名共享映射；地址选择由实现决定。
- 若需要Linux ABI完整`mmap`语义，必须依赖MMU与虚拟内存子系统。

## 配置选项

```kconfig
config POSIX_IPC
    bool "Enable POSIX IPC support"
    default y
    help
      Enable message queues, semaphores, and shared memory.

config POSIX_MQ_MAX
    int "Maximum number of message queues"
    default 16

config POSIX_SEM_MAX
    int "Maximum number of semaphores"
    default 32

config POSIX_SHM_MAX
    int "Maximum number of shared memory objects"
    default 8
```

## 测试要点

- 消息队列生产者-消费者模型
- 信号量互斥和同步
- 共享内存多进程访问
- 命名对象的创建和查找
- 引用计数和资源清理
- 非阻塞模式
- 优先级消息队列

## 性能考虑

- 命名查找有哈希表优化空间
- 消息队列零拷贝（直接传递指针）
- 共享内存最高效（无拷贝）
- 信号量开销最小

## 限制

1. 不支持跨设备的IPC（仅单MCU内）
2. 共享内存依赖MPU，某些平台可能不支持
3. 消息队列不支持优先级（Zephyr k_msgq限制）
4. 不支持System V IPC（msgget, semget, shmget）
5. 命名对象不持久化（重启后丢失）
