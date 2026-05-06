# 数据类型 - 时间

```c
typedef long time_t;
typedef int clockid_t;

struct timespec {
    time_t tv_sec;   // 秒
    long tv_nsec;    // 纳秒
};

struct timeval {
    time_t tv_sec;   // 秒
    long tv_usec;    // 微秒
};

struct tm {
    int tm_sec;    // 秒 (0-59)
    int tm_min;    // 分 (0-59)
    int tm_hour;   // 时 (0-23)
    int tm_mday;   // 日 (1-31)
    int tm_mon;    // 月 (0-11)
    int tm_year;   // 年 (从1900开始)
    int tm_wday;   // 星期 (0-6)
    int tm_yday;   // 年内天数 (0-365)
    int tm_isdst;  // 夏令时标志
};
```
