# 实现指导 - 时间管理

## 核心实现

```c
// 使用Zephyr时钟
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    switch (clock_id) {
    case CLOCK_MONOTONIC:
        uint64_t ms = k_uptime_get();
        tp->tv_sec = ms / 1000;
        tp->tv_nsec = (ms % 1000) * 1000000;
        return 0;
    default:
        errno = EINVAL;
        return -1;
    }
}

int gettimeofday(struct timeval *tv, void *tz)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    tv->tv_sec = ts.tv_sec;
    tv->tv_usec = ts.tv_nsec / 1000;
    return 0;
}
```

## 配置

```kconfig
config POSIX_CLOCK
    bool "POSIX clock support"
    default y

config POSIX_CLOCK_REALTIME
    bool "Support CLOCK_REALTIME"
    depends on RTC
    default n
```
