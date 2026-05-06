# 数据类型定义

## 线程相关类型

### pthread_t
```c
typedef uint32_t pthread_t;
```
线程标识符，用于唯一标识一个线程。

### pthread_attr_t
```c
typedef struct {
    int detachstate;        // PTHREAD_CREATE_JOINABLE/DETACHED
    size_t stacksize;       // 栈大小
    void *stackaddr;        // 栈地址（通常不用）
    int schedpolicy;        // SCHED_FIFO/SCHED_RR/SCHED_OTHER
    struct sched_param schedparam;  // 调度参数
    int inheritsched;       // 继承调度属性
    int scope;              // 竞争范围
} pthread_attr_t;
```

线程属性结构体。

**字段说明**：
- **detachstate**: 分离状态
  - `PTHREAD_CREATE_JOINABLE` (0): 可join（默认）
  - `PTHREAD_CREATE_DETACHED` (1): 分离状态
- **stacksize**: 线程栈大小（字节）
- **schedpolicy**: 调度策略
  - `SCHED_FIFO` (1): 先进先出
  - `SCHED_RR` (2): 轮转
  - `SCHED_OTHER` (0): 默认
- **schedparam**: 包含优先级信息

## 互斥锁相关类型

### pthread_mutex_t
```c
typedef struct {
    struct k_mutex zmutex;   // Zephyr原生互斥锁
    int type;                // 锁类型
    pthread_t owner;         // 持有者线程ID
    atomic_t lock_count;     // 递归计数
    bool initialized;        // 初始化标志
} pthread_mutex_t;
```

互斥锁结构体。

### pthread_mutexattr_t
```c
typedef struct {
    int type;       // NORMAL/RECURSIVE/ERRORCHECK
    int protocol;   // PTHREAD_PRIO_NONE/PTHREAD_PRIO_INHERIT
} pthread_mutexattr_t;
```

互斥锁属性。

**type值**：
- `PTHREAD_MUTEX_NORMAL` (0): 普通锁
- `PTHREAD_MUTEX_RECURSIVE` (1): 递归锁
- `PTHREAD_MUTEX_ERRORCHECK` (2): 错误检查锁

**protocol值**：
- `PTHREAD_PRIO_NONE` (0): 无优先级继承
- `PTHREAD_PRIO_INHERIT` (1): 优先级继承

## 条件变量相关类型

### pthread_cond_t
```c
typedef struct {
    struct k_condvar zcond;
    bool initialized;
} pthread_cond_t;
```

条件变量结构体。

### pthread_condattr_t
```c
typedef struct {
    int pshared;  // 进程共享属性（未使用）
} pthread_condattr_t;
```

条件变量属性。

## 调度相关类型

### sched_param
```c
struct sched_param {
    int sched_priority;  // 优先级（1-99）
};
```

调度参数结构体。

## 内部数据结构

### posix_thread_t
```c
typedef struct posix_thread {
    struct k_thread zephyr_thread;      // Zephyr原生线程
    pthread_t pthread_id;                // POSIX线程ID
    void *(*start_routine)(void *);     // 线程入口函数
    void *arg;                           // 线程参数
    void *retval;                        // 返回值
    struct k_sem join_sem;               // join同步信号量
    bool detached;                       // 是否分离
    bool joined;                         // 是否已被join
    bool terminated;                     // 是否已终止
    atomic_t join_count;                 // join计数

    // 取消相关
    atomic_t cancel_requested;           // 取消请求标志
    int cancelstate;                     // ENABLE/DISABLE
    int canceltype;                      // DEFERRED/ASYNCHRONOUS
    struct k_mutex cancel_lock;          // 保护取消状态

    // 资源管理
    void *stack;                         // 动态分配的栈
    size_t stack_size;                   // 栈大小
    struct posix_thread *next;           // 链表指针
} posix_thread_t;
```

内部线程控制块，封装Zephyr线程并添加POSIX语义。

## 常量定义

### 线程分离状态
```c
#define PTHREAD_CREATE_JOINABLE  0
#define PTHREAD_CREATE_DETACHED  1
```

### 调度策略
```c
#define SCHED_OTHER  0
#define SCHED_FIFO   1
#define SCHED_RR     2
```

### 取消状态
```c
#define PTHREAD_CANCEL_ENABLE   0
#define PTHREAD_CANCEL_DISABLE  1
```

### 取消类型
```c
#define PTHREAD_CANCEL_DEFERRED      0
#define PTHREAD_CANCEL_ASYNCHRONOUS  1
```

### 特殊返回值
```c
#define PTHREAD_CANCELED  ((void *)-1)
```

### 栈大小
```c
#define PTHREAD_STACK_MIN  2048
```

### 静态初始化宏
```c
#define PTHREAD_MUTEX_INITIALIZER \
    { .initialized = false, .type = PTHREAD_MUTEX_NORMAL }

#define PTHREAD_COND_INITIALIZER \
    { .initialized = false }
```

## 错误码

```c
#define EAGAIN      11  // 资源暂时不可用
#define EINVAL      22  // 无效参数
#define EDEADLK     35  // 死锁
#define EBUSY       16  // 设备或资源忙
#define EPERM       1   // 操作不允许
#define ESRCH       3   // 无此进程
#define ETIMEDOUT   110 // 连接超时
```

## 类型转换宏

### 线程ID转换
```c
#define PTHREAD_TO_ZEPHYR_THREAD(pt) (&(pt)->zephyr_thread)
#define ZEPHYR_TO_PTHREAD_ID(zt) ((pthread_t)(uintptr_t)(zt))
```

### 优先级转换
```c
// Linux优先级（1-99）到Zephyr优先级
static inline int posix_to_zephyr_priority(int policy, int priority)
{
    if (policy == SCHED_FIFO) {
        // 映射到协作式（负优先级）
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        return -(coop_range - (priority * coop_range / 99));
    } else if (policy == SCHED_RR) {
        // 映射到抢占式（正优先级）
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        return (99 - priority) * preempt_range / 99;
    } else {
        return CONFIG_PTHREAD_DEFAULT_PRIORITY;
    }
}

// Zephyr优先级到Linux优先级
static inline int zephyr_to_posix_priority(int zephyr_prio)
{
    if (zephyr_prio < 0) {
        // 协作式到FIFO
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        return 99 - ((-zephyr_prio) * 99 / coop_range);
    } else {
        // 抢占式到RR
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        return 99 - (zephyr_prio * 99 / preempt_range);
    }
}
```

## 配置选项类型

```c
// Kconfig生成的配置
#define CONFIG_PTHREAD_DYNAMIC_STACK        1
#define CONFIG_PTHREAD_DEFAULT_STACK_SIZE   2048
#define CONFIG_PTHREAD_MAX_THREADS          32
#define CONFIG_PTHREAD_DEFAULT_PRIORITY     5
#define CONFIG_NUM_COOP_PRIORITIES          16
#define CONFIG_NUM_PREEMPT_PRIORITIES       15
```

## 时间相关类型

### timespec
```c
struct timespec {
    time_t tv_sec;   // 秒
    long tv_nsec;    // 纳秒
};
```

用于高精度时间表示。

## 原子类型

```c
typedef struct {
    int counter;
} atomic_t;

#define ATOMIC_INIT(i)  { (i) }

static inline int atomic_get(const atomic_t *v);
static inline void atomic_set(atomic_t *v, int i);
static inline int atomic_inc(atomic_t *v);
static inline int atomic_dec(atomic_t *v);
```

## 注意事项

1. **对齐要求**：某些结构体需要特定对齐（如栈地址）
2. **大小限制**：栈大小必须满足MPU对齐要求（2的幂次方）
3. **ID唯一性**：pthread_t必须在系统中唯一
4. **初始化**：静态初始化宏仅用于全局变量
5. **移植性**：类型定义可能因平台而异
