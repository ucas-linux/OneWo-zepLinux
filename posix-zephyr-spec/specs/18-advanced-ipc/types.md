# 数据类型定义

## POSIX消息队列类型

```c
#include <mqueue.h>

// 消息队列描述符
typedef int mqd_t;

// 消息队列属性
struct mq_attr {
    long mq_flags;      // 标志：0或O_NONBLOCK
    long mq_maxmsg;     // 队列中最大消息数
    long mq_msgsize;    // 每条消息的最大字节数
    long mq_curmsgs;    // 当前队列中的消息数
};
```

## POSIX命名信号量类型

```c
#include <semaphore.h>

// 信号量类型（不透明）
typedef struct {
    void *__opaque;
} sem_t;

#define SEM_FAILED ((sem_t *)0)
```

## POSIX共享内存类型

```c
#include <sys/mman.h>

// 使用标准文件描述符
// 配合mmap使用
```

## UNIX域套接字类型

```c
#include <sys/un.h>

// UNIX域套接字地址
struct sockaddr_un {
    sa_family_t sun_family;  // AF_UNIX
    char sun_path[108];      // 路径名
};
```

## 内部实现类型

```c
// IPC对象类型枚举
typedef enum {
    IPC_TYPE_MSGQ,
    IPC_TYPE_SEM,
    IPC_TYPE_SHM
} ipc_type_t;

// IPC对象注册表条目
typedef struct ipc_object {
    char name[64];
    ipc_type_t type;
    void *handle;
    int refcount;
    mode_t mode;
    struct ipc_object *next;
} ipc_object_t;

// POSIX消息队列内部结构
typedef struct {
    struct k_msgq kmsgq;
    char *buffer;
    struct mq_attr attr;
    int flags;
} posix_mq_t;

// POSIX信号量内部结构
typedef struct {
    struct k_sem ksem;
    unsigned int value;
} posix_sem_t;

// POSIX共享内存内部结构
typedef struct {
    void *addr;
    size_t size;
    int flags;
    struct k_mem_partition partition;
} posix_shm_t;
```
