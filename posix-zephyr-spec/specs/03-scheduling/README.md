# 调度与优先级接口规范

## 概述

本模块定义3个POSIX调度接口在Zephyr RTOS上的实现规范。

## 接口列表

1. `sched_yield` - 主动让出CPU
2. `sched_setparam` - 设置调度参数
3. `sched_getparam` - 获取调度参数

## POSIX标准定义

```c
#include <sched.h>

int sched_yield(void);
int sched_setparam(pid_t pid, const struct sched_param *param);
int sched_getparam(pid_t pid, struct sched_param *param);

struct sched_param {
    int sched_priority;  // 调度优先级
};
```

## Zephyr现有支持

Zephyr提供以下调度相关API：

```c
// 让出CPU
void k_yield(void);

// 设置线程优先级
void k_thread_priority_set(k_tid_t thread, int prio);

// 获取线程优先级
int k_thread_priority_get(k_tid_t thread);
```

**差异**：
- Zephyr的k_yield只让位给同优先级线程
- Zephyr优先级数值越小优先级越高（与Linux相反）
- Zephyr没有独立的调度策略参数

## 核心挑战

### 1. sched_yield语义差异
**问题**：Linux的sched_yield会让位给所有可运行线程，Zephyr只让位给同优先级。

**解决方案**：
- 检查是否有更高优先级线程就绪
- 如果有，临时降低当前线程优先级
- 调用k_yield后恢复优先级

### 2. 优先级映射
**问题**：Linux优先级1-99（99最高），Zephyr负数到正数（负数最高）。

**解决方案**：
- 维护POSIX优先级到Zephyr优先级的映射表
- 根据调度策略选择映射范围
- 提供双向转换函数

### 3. 进程vs线程
**问题**：POSIX接口操作进程，Zephyr只有线程。

**解决方案**：
- pid映射到进程的主线程
- 修改主线程优先级影响整个"进程"

## 数据结构

```c
// POSIX调度策略
#define SCHED_OTHER 0
#define SCHED_FIFO  1
#define SCHED_RR    2

// POSIX优先级范围
#define SCHED_PRIORITY_MIN 1
#define SCHED_PRIORITY_MAX 99

// 调度参数
struct sched_param {
    int sched_priority;
};

// 线程调度信息（扩展到posix_thread_t）
typedef struct {
    int posix_priority;      // POSIX优先级(1-99)
    int posix_policy;        // POSIX策略
    int zephyr_priority;     // Zephyr优先级
    int original_priority;   // 用于yield恢复
} thread_sched_info_t;
```

## 实现要点

### sched_yield实现

```c
int sched_yield(void)
{
    // 方案1：简单实现（只让位给同优先级）
    k_yield();
    return 0;

    // 方案2：完整实现（让位给所有线程）
    posix_thread_t *self = pthread_self_internal();
    int current_prio = k_thread_priority_get(k_current_get());

    // 检查是否有其他就绪线程
    if (has_ready_threads()) {
        // 临时降低优先级到最低
        k_thread_priority_set(k_current_get(),
                              CONFIG_NUM_PREEMPT_PRIORITIES - 1);

        // 让出CPU
        k_yield();

        // 恢复原优先级
        k_thread_priority_set(k_current_get(), current_prio);
    } else {
        // 没有其他就绪线程，直接返回
        k_yield();
    }

    return 0;
}
```

### sched_setparam实现

```c
int sched_setparam(pid_t pid, const struct sched_param *param)
{
    if (!param) return -EINVAL;

    // 验证优先级范围
    if (param->sched_priority < SCHED_PRIORITY_MIN ||
        param->sched_priority > SCHED_PRIORITY_MAX) {
        return -EINVAL;
    }

    // 查找进程/线程
    posix_thread_t *thread;
    if (pid == 0) {
        // pid=0表示当前线程
        thread = pthread_self_internal();
    } else {
        pcb_t *proc = find_process(pid);
        if (!proc) return -ESRCH;
        thread = proc->main_thread_posix;
    }

    // 获取当前调度策略
    int policy = thread->sched_info.posix_policy;

    // 映射POSIX优先级到Zephyr优先级
    int zephyr_prio = map_posix_to_zephyr_priority(
        policy, param->sched_priority);

    // 设置Zephyr线程优先级
    k_thread_priority_set(&thread->zephyr_thread, zephyr_prio);

    // 更新调度信息
    thread->sched_info.posix_priority = param->sched_priority;
    thread->sched_info.zephyr_priority = zephyr_prio;

    return 0;
}
```

### sched_getparam实现

```c
int sched_getparam(pid_t pid, struct sched_param *param)
{
    if (!param) return -EINVAL;

    // 查找进程/线程
    posix_thread_t *thread;
    if (pid == 0) {
        thread = pthread_self_internal();
    } else {
        pcb_t *proc = find_process(pid);
        if (!proc) return -ESRCH;
        thread = proc->main_thread_posix;
    }

    // 返回POSIX优先级
    param->sched_priority = thread->sched_info.posix_priority;

    return 0;
}
```

### 优先级映射函数

```c
// POSIX优先级 → Zephyr优先级
static int map_posix_to_zephyr_priority(int policy, int posix_prio)
{
    // Linux: 1-99 (99最高)
    // Zephyr协作式: -CONFIG_NUM_COOP_PRIORITIES to -1 (-1最高)
    // Zephyr抢占式: 0 to CONFIG_NUM_PREEMPT_PRIORITIES-1 (0最高)

    if (policy == SCHED_FIFO) {
        // FIFO → 协作式（负优先级）
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        // posix_prio: 1-99 → zephyr: -coop_range to -1
        return -(coop_range - (posix_prio - 1) * coop_range / 99);
    } else if (policy == SCHED_RR) {
        // RR → 抢占式（正优先级）
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        // posix_prio: 1-99 → zephyr: preempt_range-1 to 0
        return preempt_range - 1 - (posix_prio - 1) * preempt_range / 99;
    } else {
        // SCHED_OTHER → 默认优先级
        return CONFIG_PTHREAD_DEFAULT_PRIORITY;
    }
}

// Zephyr优先级 → POSIX优先级
static int map_zephyr_to_posix_priority(int policy, int zephyr_prio)
{
    if (policy == SCHED_FIFO) {
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        // zephyr: -coop_range to -1 → posix: 1-99
        return 1 + (coop_range + zephyr_prio) * 99 / coop_range;
    } else if (policy == SCHED_RR) {
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        // zephyr: preempt_range-1 to 0 → posix: 1-99
        return 1 + (preempt_range - 1 - zephyr_prio) * 99 / preempt_range;
    } else {
        return 50;  // 默认中等优先级
    }
}
```

## 扩展接口

### sched_setscheduler / sched_getscheduler

```c
int sched_setscheduler(pid_t pid, int policy,
                       const struct sched_param *param)
{
    if (policy != SCHED_FIFO && policy != SCHED_RR &&
        policy != SCHED_OTHER) {
        return -EINVAL;
    }

    posix_thread_t *thread = find_thread_by_pid(pid);
    if (!thread) return -ESRCH;

    // 更新策略
    thread->sched_info.posix_policy = policy;

    // 设置优先级
    return sched_setparam(pid, param);
}

int sched_getscheduler(pid_t pid)
{
    posix_thread_t *thread = find_thread_by_pid(pid);
    if (!thread) return -ESRCH;

    return thread->sched_info.posix_policy;
}
```

## 配置选项

```kconfig
config POSIX_SCHED
    bool "Enable POSIX scheduling APIs"
    default y
    help
      Enable sched_yield, sched_setparam, etc.

config POSIX_SCHED_YIELD_FULL
    bool "Full sched_yield semantics"
    default n
    help
      Make sched_yield give up CPU to all threads, not just same priority.
      This has performance overhead.

config PTHREAD_DEFAULT_PRIORITY
    int "Default thread priority for SCHED_OTHER"
    default 5
    range 0 CONFIG_NUM_PREEMPT_PRIORITIES
    help
      Zephyr priority for threads with SCHED_OTHER policy.
```

## 测试用例

### 测试1：sched_yield

```c
void test_sched_yield(void)
{
    // 创建两个同优先级线程
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    // 主动让出CPU
    sched_yield();

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
```

### 测试2：优先级设置

```c
void test_priority(void)
{
    struct sched_param param;

    // 获取当前优先级
    sched_getparam(0, &param);
    printf("Current priority: %d\n", param.sched_priority);

    // 设置高优先级
    param.sched_priority = 80;
    sched_setparam(0, &param);

    // 验证
    sched_getparam(0, &param);
    assert(param.sched_priority == 80);
}
```

## 性能考虑

- sched_yield开销：约1-5微秒
- 优先级设置开销：约5-10微秒
- 完整sched_yield（临时降低优先级）开销更大
- 建议使用简单版本的sched_yield

## 注意事项

1. 优先级映射是近似的，不是精确的1:1映射
2. Zephyr的时间片轮转与Linux不完全相同
3. 实时性保证取决于系统配置和负载
4. 不支持SCHED_DEADLINE等高级策略
5. 优先级继承由Zephyr k_mutex自动处理

## 限制

1. 不支持CPU亲和性（单核MCU）
2. 不支持实时调度策略的所有细节
3. 不支持nice值（SCHED_OTHER的优先级调整）
4. 不支持调度域和组调度
