# clock_gettime - 获取时间

## POSIX标准定义

```c
#include <time.h>

int clock_gettime(clockid_t clock_id, struct timespec *tp);
int clock_settime(clockid_t clock_id, const struct timespec *tp);
int clock_getres(clockid_t clock_id, struct timespec *res);
```

## 时钟类型

```c
#define CLOCK_REALTIME           0  // 系统实时时钟
#define CLOCK_MONOTONIC          1  // 单调递增时钟
#define CLOCK_PROCESS_CPUTIME_ID 2  // 进程CPU时间
#define CLOCK_THREAD_CPUTIME_ID  3  // 线程CPU时间
```

## Zephyr适配

```c
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    if (!tp) {
        errno = EINVAL;
        return -1;
    }

    switch (clock_id) {
    case CLOCK_REALTIME:
        // 使用RTC或系统时间
        return get_realtime(tp);

    case CLOCK_MONOTONIC:
        // 使用k_uptime_get
        uint64_t ms = k_uptime_get();
        tp->tv_sec = ms / 1000;
        tp->tv_nsec = (ms % 1000) * 1000000;
        return 0;

    case CLOCK_PROCESS_CPUTIME_ID:
    case CLOCK_THREAD_CPUTIME_ID:
        // 使用k_cycle_get_64
        uint64_t cycles = k_cycle_get_64();
        uint32_t freq = sys_clock_hw_cycles_per_sec();
        tp->tv_sec = cycles / freq;
        tp->tv_nsec = ((cycles % freq) * 1000000000ULL) / freq;
        return 0;

    default:
        errno = EINVAL;
        return -1;
    }
}
```

## 测试

```c
void test_clock_gettime(void)
{
    struct timespec ts;
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    assert(ret == 0);
    printf("Time: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
}
```
