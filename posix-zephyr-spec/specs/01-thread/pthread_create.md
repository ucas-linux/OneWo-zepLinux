# pthread_create - 创建线程

## POSIX标准定义

```c
#include <pthread.h>

int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
```

## 功能描述

创建一个新线程，新线程从`start_routine`函数开始执行，参数为`arg`。

## 参数说明

- **thread**: 输出参数，返回新线程的ID
- **attr**: 线程属性，NULL表示使用默认属性
- **start_routine**: 线程入口函数
- **arg**: 传递给线程函数的参数

## 返回值

- **0**: 成功
- **EAGAIN**: 系统资源不足，无法创建线程
- **EINVAL**: 无效的属性设置
- **EPERM**: 没有权限设置调度策略或优先级

## Linux标准行为

1. 新线程继承创建者的信号掩码
2. 新线程的挂起信号集为空
3. 新线程不继承创建者的CPU亲和性
4. 线程栈大小由attr指定，默认通常为2MB
5. 线程调度策略和优先级由attr指定
6. 线程可以是joinable或detached状态

## Zephyr现有支持

Zephyr提供以下线程创建API：

```c
k_tid_t k_thread_create(struct k_thread *new_thread,
                        k_thread_stack_t *stack,
                        size_t stack_size,
                        k_thread_entry_t entry,
                        void *p1, void *p2, void *p3,
                        int prio, uint32_t options,
                        k_timeout_t delay);
```

**差异**：
- Zephyr需要预先定义的栈空间（通常静态分配）
- Zephyr线程函数接受3个void*参数
- Zephyr使用整数优先级（数值越小优先级越高）
- Zephyr没有线程属性结构体

## Zephyr适配方案

### 数据结构设计

```c
// 线程控制块
typedef struct {
    struct k_thread zephyr_thread;      // Zephyr原生线程
    pthread_t pthread_id;                // POSIX线程ID
    void *(*start_routine)(void *);     // 线程入口函数
    void *arg;                           // 线程参数
    void *retval;                        // 返回值
    struct k_sem join_sem;               // join同步信号量
    bool detached;                       // 是否分离
    bool joined;                         // 是否已被join
    atomic_t cancel_requested;           // 取消请求标志
    int cancelstate;                     // 取消状态
    int canceltype;                      // 取消类型
    void *stack;                         // 动态分配的栈
    size_t stack_size;                   // 栈大小
    struct posix_thread *next;           // 链表指针
} posix_thread_t;

// 线程属性
typedef struct {
    int detachstate;                     // PTHREAD_CREATE_JOINABLE/DETACHED
    size_t stacksize;                    // 栈大小
    void *stackaddr;                     // 栈地址（通常不用）
    int schedpolicy;                     // SCHED_FIFO/SCHED_RR/SCHED_OTHER
    struct sched_param schedparam;       // 调度参数（优先级）
    int inheritsched;                    // 继承调度属性
    int scope;                           // 竞争范围
} pthread_attr_t;
```

### 实现步骤

#### 1. 参数验证

```c
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg)
{
    // 验证必需参数
    if (!thread || !start_routine) {
        return EINVAL;
    }

    // 验证属性
    if (attr) {
        if (attr->stacksize < PTHREAD_STACK_MIN) {
            return EINVAL;
        }
        if (attr->schedpolicy != SCHED_FIFO &&
            attr->schedpolicy != SCHED_RR &&
            attr->schedpolicy != SCHED_OTHER) {
            return EINVAL;
        }
    }

    // 继续实现...
}
```

#### 2. 分配线程控制块

```c
// 从线程池分配或动态分配
posix_thread_t *pt = alloc_posix_thread();
if (!pt) {
    return EAGAIN;
}

// 初始化字段
memset(pt, 0, sizeof(*pt));
pt->start_routine = start_routine;
pt->arg = arg;
pt->pthread_id = generate_thread_id();
```

#### 3. 动态分配栈

```c
// 获取栈大小
size_t stack_size = attr ? attr->stacksize : CONFIG_PTHREAD_DEFAULT_STACK_SIZE;

// 动态分配栈（关键改进点）
pt->stack = k_malloc(stack_size);
if (!pt->stack) {
    free_posix_thread(pt);
    return ENOMEM;
}
pt->stack_size = stack_size;
```

#### 4. 优先级映射

```c
// 获取调度策略和优先级
int policy = attr ? attr->schedpolicy : SCHED_OTHER;
int priority = attr ? attr->schedparam.sched_priority : 0;

// 映射到Zephyr优先级
int zephyr_prio = map_posix_priority(policy, priority);

// 映射规则：
// SCHED_FIFO + 高优先级 → 负优先级（协作式）
// SCHED_RR + 中优先级 → 低正优先级（抢占式）
// SCHED_OTHER → 默认优先级
```

#### 5. 创建线程包装器

```c
// 线程入口包装函数
static void pthread_entry_wrapper(void *p1, void *p2, void *p3)
{
    posix_thread_t *pt = (posix_thread_t *)p1;

    // 调用用户线程函数
    void *retval = pt->start_routine(pt->arg);

    // 保存返回值
    pt->retval = retval;

    // 如果是joinable，触发join信号量
    if (!pt->detached) {
        k_sem_give(&pt->join_sem);
        // 等待被join，不立即销毁
        while (!pt->joined) {
            k_sleep(K_MSEC(10));
        }
    }

    // 清理资源
    cleanup_thread(pt);
}
```

#### 6. 创建Zephyr线程

```c
// 初始化join信号量
k_sem_init(&pt->join_sem, 0, 1);

// 设置detached状态
pt->detached = attr ? (attr->detachstate == PTHREAD_CREATE_DETACHED) : false;

// 创建Zephyr线程
k_tid_t tid = k_thread_create(
    &pt->zephyr_thread,
    (k_thread_stack_t *)pt->stack,
    pt->stack_size,
    pthread_entry_wrapper,
    pt, NULL, NULL,
    zephyr_prio,
    0,  // options
    K_NO_WAIT
);

if (!tid) {
    k_free(pt->stack);
    free_posix_thread(pt);
    return EAGAIN;
}
```

#### 7. 注册资源并返回

```c
// 注册到全局线程表
register_thread(pt);

// 注册到资源跟踪表
resource_register(current_pid(), RES_THREAD, pt);

// 返回线程ID
*thread = pt->pthread_id;

return 0;
```

### 优先级映射算法

```c
static int map_posix_priority(int policy, int priority)
{
    // Linux优先级范围：1-99（99最高）
    // Zephyr协作式：-CONFIG_NUM_COOP_PRIORITIES to -1
    // Zephyr抢占式：0 to CONFIG_NUM_PREEMPT_PRIORITIES-1

    if (policy == SCHED_FIFO) {
        // FIFO映射到协作式（负优先级）
        // Linux 99 → Zephyr -1（最高）
        // Linux 1 → Zephyr -CONFIG_NUM_COOP_PRIORITIES
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        return -(coop_range - (priority * coop_range / 99));
    } else if (policy == SCHED_RR) {
        // RR映射到抢占式（正优先级）
        // Linux 99 → Zephyr 0（最高）
        // Linux 1 → Zephyr CONFIG_NUM_PREEMPT_PRIORITIES-1
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        return (99 - priority) * preempt_range / 99;
    } else {
        // SCHED_OTHER使用默认优先级
        return CONFIG_PTHREAD_DEFAULT_PRIORITY;
    }
}
```

## 配置选项

```kconfig
# Kconfig配置
config PTHREAD_DYNAMIC_STACK
    bool "Enable dynamic thread stack allocation"
    default y
    help
      Use k_malloc to dynamically allocate thread stacks.
      Disable to use static allocation for memory-constrained systems.

config PTHREAD_DEFAULT_STACK_SIZE
    int "Default pthread stack size"
    default 2048
    help
      Default stack size for threads created without attributes.

config PTHREAD_MAX_THREADS
    int "Maximum number of concurrent pthreads"
    default 32
    help
      Maximum number of POSIX threads that can exist simultaneously.

config PTHREAD_DEFAULT_PRIORITY
    int "Default pthread priority"
    default 5
    help
      Default Zephyr priority for SCHED_OTHER threads.
```

## 测试用例

### 测试1：基本创建和退出

```c
void *thread_func(void *arg) {
    int *value = (int *)arg;
    printf("Thread received: %d\n", *value);
    return (void *)(intptr_t)(*value * 2);
}

void test_basic_create(void) {
    pthread_t thread;
    int arg = 42;

    int ret = pthread_create(&thread, NULL, thread_func, &arg);
    assert(ret == 0);

    void *retval;
    ret = pthread_join(thread, &retval);
    assert(ret == 0);
    assert((intptr_t)retval == 84);
}
```

### 测试2：自定义栈大小

```c
void test_custom_stack(void) {
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 4096);

    int ret = pthread_create(&thread, &attr, thread_func, NULL);
    assert(ret == 0);

    pthread_attr_destroy(&attr);
    pthread_join(thread, NULL);
}
```

### 测试3：优先级设置

```c
void test_priority(void) {
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = 50;
    pthread_attr_setschedparam(&attr, &param);

    int ret = pthread_create(&thread, &attr, thread_func, NULL);
    assert(ret == 0);

    pthread_attr_destroy(&attr);
    pthread_join(thread, NULL);
}
```

## 性能考虑

1. **栈分配开销**：动态分配比静态分配慢，但更灵活
2. **线程创建时间**：约100-500微秒（取决于栈大小）
3. **内存占用**：每个线程约2-8KB（栈 + 控制块）
4. **最大线程数**：受限于可用RAM，建议<32个

## 注意事项

1. 必须调用pthread_join或pthread_detach，否则资源泄漏
2. 线程函数返回等同于调用pthread_exit
3. 不支持线程取消清理处理程序（cleanup handlers）
4. 不支持线程特定数据（thread-specific data）的完整语义
5. 信号处理与Linux不完全兼容
