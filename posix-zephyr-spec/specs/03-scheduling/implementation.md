# 实现指导

## 总体架构

### 模块组织

```
posix_scheduling/
├── sched_yield.c         # sched_yield实现
├── sched_param.c         # sched_setparam/getparam实现
├── priority_map.c        # 优先级映射
└── sched_internal.h      # 内部头文件
```

## 核心实现

### 优先级映射

```c
// POSIX → Zephyr
int map_posix_to_zephyr_priority(int policy, int posix_prio)
{
    if (policy == SCHED_FIFO) {
        int coop_range = CONFIG_NUM_COOP_PRIORITIES;
        return -(coop_range - (posix_prio - 1) * coop_range / 99);
    } else if (policy == SCHED_RR) {
        int preempt_range = CONFIG_NUM_PREEMPT_PRIORITIES;
        return preempt_range - 1 - (posix_prio - 1) * preempt_range / 99;
    } else {
        return CONFIG_PTHREAD_DEFAULT_PRIORITY;
    }
}
```

## 配置选项

```kconfig
config POSIX_SCHED
    bool "POSIX scheduling APIs"
    default y

config POSIX_SCHED_YIELD_FULL
    bool "Full sched_yield semantics"
    default n

config PTHREAD_DEFAULT_PRIORITY
    int "Default priority for SCHED_OTHER"
    default 5
```

## 测试框架

```c
void test_sched_yield(void)
{
    sched_yield();
    // 应该成功返回
}

void test_priority(void)
{
    struct sched_param param = {.sched_priority = 50};
    int ret = sched_setparam(0, &param);
    zassert_equal(ret, 0, "setparam failed");
}
```

## 常见问题

### Q: 优先级映射不精确怎么办？
A: 这是正常的，POSIX到Zephyr的映射是近似的

### Q: sched_yield不让位给低优先级线程？
A: 启用CONFIG_POSIX_SCHED_YIELD_FULL使用完整实现

### Q: 如何调整默认优先级？
A: 修改CONFIG_PTHREAD_DEFAULT_PRIORITY配置
