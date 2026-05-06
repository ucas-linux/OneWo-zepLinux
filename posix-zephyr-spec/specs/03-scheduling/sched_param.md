# sched_setparam / sched_getparam - 调度参数管理

## POSIX标准定义

```c
#include <sched.h>

int sched_setparam(pid_t pid, const struct sched_param *param);
int sched_getparam(pid_t pid, struct sched_param *param);

struct sched_param {
    int sched_priority;  // 调度优先级（1-99）
};
```

## 功能描述

- **sched_setparam**: 设置进程/线程的调度参数（主要是优先级）
- **sched_getparam**: 获取进程/线程的调度参数

## 参数说明

- **pid**: 进程ID（0表示当前进程）
- **param**: 调度参数结构体

## 返回值

- **0**: 成功
- **-1**: 失败，设置errno

## 错误码

- **EINVAL**: 无效的优先级或参数
- **ESRCH**: 进程不存在
- **EPERM**: 没有权限修改优先级

## POSIX优先级范围

```c
#define SCHED_PRIORITY_MIN  1
#define SCHED_PRIORITY_MAX  99
```

Linux优先级：1-99，99最高

## Zephyr适配方案

### 核心挑战

1. **优先级方向相反**: Linux 99最高，Zephyr负数最高
2. **进程vs线程**: POSIX操作进程，Zephyr只有线程
3. **调度策略**: 需要根据策略选择映射范围

### 数据结构扩展

```c
// 添加到posix_thread_t
typedef struct {
    int posix_priority;      // POSIX优先级(1-99)
    int posix_policy;        // POSIX策略
    int zephyr_priority;     // Zephyr优先级
} thread_sched_info_t;
```

### sched_setparam实现

```c
int sched_setparam(pid_t pid, const struct sched_param *param)
{
    if (!param) {
        errno = EINVAL;
        return -1;
    }

    // 验证优先级范围
    if (param->sched_priority < SCHED_PRIORITY_MIN ||
        param->sched_priority > SCHED_PRIORITY_MAX) {
        errno = EINVAL;
        return -1;
    }

    // 查找线程
    posix_thread_t *thread;
    if (pid == 0) {
        thread = get_current_posix_thread();
    } else {
        pcb_t *proc = find_process_by_pid(pid);
        if (!proc) {
            errno = ESRCH;
            return -1;
        }
        thread = proc->main_thread_posix;
    }

    if (!thread) {
        errno = ESRCH;
        return -1;
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
    if (!param) {
        errno = EINVAL;
        return -1;
    }

    // 查找线程
    posix_thread_t *thread;
    if (pid == 0) {
        thread = get_current_posix_thread();
    } else {
        pcb_t *proc = find_process_by_pid(pid);
        if (!proc) {
            errno = ESRCH;
            return -1;
        }
        thread = proc->main_thread_posix;
    }

    if (!thread) {
        errno = ESRCH;
        return -1;
    }

    // 返回POSIX优先级
    param->sched_priority = thread->sched_info.posix_priority;

    return 0;
}
```

### 优先级映射

```c
// POSIX → Zephyr
static int map_posix_to_zephyr_priority(int policy, int posix_prio)
{
    // Linux: 1-99 (99最高)
    // Zephyr协作式: -CONFIG_NUM_COOP_PRIORITIES to -1 (-1最高)
    // Zephyr抢占式: 0 to CONFIG_NUM_PREEMPT_PRIORITIES-1 (0最高)

    if (policy == SCHED_FIFO) {
        // FIFO → 协作式
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        return -(coop_range - (posix_prio - 1) * coop_range / 99);
    } else if (policy == SCHED_RR) {
        // RR → 抢占式
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        return preempt_range - 1 - (posix_prio - 1) * preempt_range / 99;
    } else {
        // SCHED_OTHER → 默认
        return CONFIG_PTHREAD_DEFAULT_PRIORITY;
    }
}

// Zephyr → POSIX
static int map_zephyr_to_posix_priority(int policy, int zephyr_prio)
{
    if (policy == SCHED_FIFO) {
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        return 1 + (coop_range + zephyr_prio) * 99 / coop_range;
    } else if (policy == SCHED_RR) {
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        return 1 + (preempt_range - 1 - zephyr_prio) * 99 / preempt_range;
    } else {
        return 50;  // 默认中等优先级
    }
}
```

## 测试用例

### 测试1：设置和获取优先级

```c
void test_setget_param(void)
{
    struct sched_param param;

    // 获取当前优先级
    int ret = sched_getparam(0, &param);
    assert(ret == 0);
    printf("Current priority: %d\n", param.sched_priority);

    // 设置新优先级
    param.sched_priority = 80;
    ret = sched_setparam(0, &param);
    assert(ret == 0);

    // 验证
    struct sched_param verify;
    sched_getparam(0, &verify);
    assert(verify.sched_priority == 80);
}
```

### 测试2：优先级范围验证

```c
void test_priority_range(void)
{
    struct sched_param param;

    // 测试最小值
    param.sched_priority = SCHED_PRIORITY_MIN;
    assert(sched_setparam(0, &param) == 0);

    // 测试最大值
    param.sched_priority = SCHED_PRIORITY_MAX;
    assert(sched_setparam(0, &param) == 0);

    // 测试无效值
    param.sched_priority = 0;
    assert(sched_setparam(0, &param) == -1);
    assert(errno == EINVAL);

    param.sched_priority = 100;
    assert(sched_setparam(0, &param) == -1);
    assert(errno == EINVAL);
}
```

### 测试3：优先级抢占

```c
volatile int high_ran = 0;
volatile int low_ran = 0;

void *high_priority_thread(void *arg)
{
    high_ran = 1;
    return NULL;
}

void *low_priority_thread(void *arg)
{
    low_ran = 1;
    k_sleep(K_MSEC(100));
    return NULL;
}

void test_priority_preemption(void)
{
    pthread_t high, low;
    struct sched_param param;

    // 创建低优先级线程
    pthread_create(&low, NULL, low_priority_thread, NULL);
    param.sched_priority = 20;
    sched_setparam(pthread_to_pid(low), &param);

    // 创建高优先级线程（应该抢占）
    pthread_create(&high, NULL, high_priority_thread, NULL);
    param.sched_priority = 80;
    sched_setparam(pthread_to_pid(high), &param);

    pthread_join(high, NULL);
    pthread_join(low, NULL);

    // 高优先级应该先运行
    assert(high_ran == 1);
}
```

## 性能考虑

1. **设置开销**: 约5-10微秒
2. **获取开销**: 约1-5微秒
3. **优先级切换**: 可能触发调度

## 注意事项

1. **映射不精确**: POSIX到Zephyr的映射是近似的
2. **策略依赖**: 优先级映射取决于调度策略
3. **权限检查**: 简化实现，不完全检查权限
4. **实时性**: 取决于系统配置和负载
5. **优先级继承**: 由k_mutex自动处理

## 限制

1. 不支持nice值（SCHED_OTHER的优先级调整）
2. 不支持实时调度的所有细节
3. 优先级范围受Zephyr配置限制
4. 不支持CPU亲和性
