# sched_yield - 主动让出CPU

## POSIX标准定义

```c
#include <sched.h>

int sched_yield(void);
```

## 功能描述

使调用线程主动让出CPU，允许其他线程运行。

## 返回值

- **0**: 成功
- **-1**: 失败（极少发生）

## Linux标准行为

1. 将当前线程移到就绪队列末尾
2. 允许任何优先级的线程运行
3. 如果没有其他就绪线程，立即返回
4. 不改变线程优先级

## Zephyr现有支持

```c
void k_yield(void);
```

**差异**：
- k_yield只让位给同优先级或更高优先级线程
- 不会让位给低优先级线程

## Zephyr适配方案

### 方案1：简单实现（推荐）

```c
int sched_yield(void)
{
    k_yield();
    return 0;
}
```

**优点**：
- 实现简单
- 性能开销小
- 符合大多数使用场景

**缺点**：
- 不完全符合POSIX语义（不让位给低优先级）

### 方案2：完整实现

```c
int sched_yield(void)
{
    // 获取当前优先级
    int current_prio = k_thread_priority_get(k_current_get());

    // 检查是否有其他就绪线程
    if (has_other_ready_threads()) {
        // 临时降低到最低优先级
        k_thread_priority_set(k_current_get(),
                              CONFIG_NUM_PREEMPT_PRIORITIES - 1);

        // 让出CPU
        k_yield();

        // 恢复原优先级
        k_thread_priority_set(k_current_get(), current_prio);
    } else {
        // 没有其他线程，直接返回
        k_yield();
    }

    return 0;
}
```

**优点**：
- 完全符合POSIX语义
- 可以让位给所有线程

**缺点**：
- 性能开销较大（两次优先级设置）
- 可能导致优先级反转问题

### 辅助函数

```c
static bool has_other_ready_threads(void)
{
    // 简化实现：假设总有其他线程
    return true;

    // 完整实现需要遍历就绪队列
    // 这在Zephyr中没有直接API
}
```

## 测试用例

### 测试1：基本yield

```c
volatile int counter = 0;

void *worker_thread(void *arg)
{
    for (int i = 0; i < 100; i++) {
        counter++;
        sched_yield();
    }
    return NULL;
}

void test_basic_yield(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, worker_thread, NULL);
    pthread_create(&t2, NULL, worker_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(counter == 200);
}
```

### 测试2：yield在循环中

```c
void test_yield_in_loop(void)
{
    for (int i = 0; i < 1000; i++) {
        // 执行一些工作
        volatile int x = i * i;

        // 主动让出CPU
        sched_yield();
    }
}
```

### 测试3：不同优先级

```c
void test_yield_priority(void)
{
    pthread_t high, low;
    pthread_attr_t attr_high, attr_low;
    struct sched_param param;

    // 创建高优先级线程
    pthread_attr_init(&attr_high);
    param.sched_priority = 80;
    pthread_attr_setschedparam(&attr_high, &param);
    pthread_create(&high, &attr_high, worker_thread, NULL);

    // 创建低优先级线程
    pthread_attr_init(&attr_low);
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr_low, &param);
    pthread_create(&low, &attr_low, worker_thread, NULL);

    // 主线程yield
    sched_yield();

    pthread_join(high, NULL);
    pthread_join(low, NULL);
}
```

## 性能考虑

1. **简单实现**: 约1-5微秒
2. **完整实现**: 约10-30微秒（包括优先级设置）
3. **上下文切换**: 取决于是否有其他就绪线程

## 使用场景

### 1. 协作式多任务

```c
void cooperative_task(void)
{
    while (running) {
        // 执行一小块工作
        do_work_chunk();

        // 主动让出CPU
        sched_yield();
    }
}
```

### 2. 自旋锁优化

```c
void spin_lock_with_yield(atomic_t *lock)
{
    while (atomic_test_and_set(lock)) {
        // 让出CPU而不是忙等
        sched_yield();
    }
}
```

### 3. 等待条件

```c
void wait_for_condition(void)
{
    while (!condition_met()) {
        sched_yield();
    }
}
```

## 注意事项

1. **不保证公平性**: yield不保证下次运行哪个线程
2. **不是休眠**: yield不会休眠，只是让出当前时间片
3. **可能立即返回**: 如果没有其他就绪线程
4. **不改变优先级**: yield不影响线程优先级
5. **避免过度使用**: 频繁yield可能降低性能

## 最佳实践

1. **适度使用**: 仅在必要时使用yield
2. **配合条件**: 通常与条件检查一起使用
3. **避免忙等**: 长时间等待应使用sleep或条件变量
4. **考虑替代方案**:
   - 短时间等待：使用k_busy_wait
   - 长时间等待：使用k_sleep或条件变量
   - 同步：使用互斥锁或信号量

## 限制

1. 简单实现不完全符合POSIX语义
2. 完整实现有性能开销
3. 不支持CPU亲和性（单核MCU）
4. 调度行为取决于Zephyr调度器配置
