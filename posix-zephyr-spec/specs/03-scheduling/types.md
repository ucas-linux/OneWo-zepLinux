# 数据类型定义

## 调度策略

```c
#define SCHED_OTHER  0  // 默认策略
#define SCHED_FIFO   1  // 先进先出
#define SCHED_RR     2  // 轮转
```

## 调度参数

```c
struct sched_param {
    int sched_priority;  // 优先级（1-99）
};
```

## 优先级范围

```c
#define SCHED_PRIORITY_MIN  1
#define SCHED_PRIORITY_MAX  99
```

## 线程调度信息

```c
typedef struct {
    int posix_priority;      // POSIX优先级(1-99)
    int posix_policy;        // POSIX策略
    int zephyr_priority;     // Zephyr优先级
    int original_priority;   // 用于yield恢复
} thread_sched_info_t;
```

## 错误码

```c
#define EINVAL  22  // 无效参数
#define ESRCH   3   // 进程不存在
#define EPERM   1   // 没有权限
```
