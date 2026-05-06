# sleep - 休眠函数

## POSIX标准定义

```c
#include <unistd.h>
#include <time.h>

unsigned int sleep(unsigned int seconds);
int usleep(useconds_t usec);
int nanosleep(const struct timespec *req, struct timespec *rem);
```

## 功能描述

- **sleep**: 秒级休眠，使当前线程休眠指定秒数
- **usleep**: 微秒级休眠，使当前线程休眠指定微秒数
- **nanosleep**: 纳秒级休眠，提供最高精度的休眠

## 参数说明

### sleep
- **seconds**: 休眠秒数

### usleep
- **usec**: 休眠微秒数（0-999999）

### nanosleep
- **req**: 请求的休眠时间
- **rem**: 输出参数，返回剩余未休眠时间（被信号中断时）

## 返回值

### sleep
- **0**: 休眠完成
- **>0**: 被信号中断，返回剩余秒数

### usleep
- **0**: 成功
- **-1**: 失败，设置errno

### nanosleep
- **0**: 成功
- **-1**: 失败（通常是被信号中断），设置errno为EINTR

## Zephyr适配方案

### 核心挑战

1. **精度问题**: Zephyr默认毫秒级精度，需要支持微秒/纳秒级
2. **信号中断**: 需要支持被信号中断并返回剩余时间
3. **忙等vs休眠**: 短时间用忙等，长时间用休眠

### 实现策略

根据休眠时间长短选择不同策略：
- **>=1ms**: 使用k_sleep()，让出CPU
- **<1ms**: 使用k_busy_wait()，忙等

### 实现步骤

#### 1. sleep()实现

```c
unsigned int sleep(unsigned int seconds)
{
    if (seconds == 0) {
        return 0;
    }

    // 记录开始时间
    int64_t start_ms = k_uptime_get();
    int64_t target_ms = start_ms + (int64_t)seconds * 1000;

    // 休眠
    k_sleep(K_SECONDS(seconds));

    // 检查是否被信号中断
    SIGNAL_CHECK_POINT();

    // 计算实际休眠时间
    int64_t now_ms = k_uptime_get();
    int64_t elapsed_ms = now_ms - start_ms;

    if (elapsed_ms >= (int64_t)seconds * 1000) {
        return 0;  // 完整休眠
    } else {
        // 被中断，返回剩余秒数
        int64_t remaining_ms = target_ms - now_ms;
        return (unsigned int)((remaining_ms + 999) / 1000);
    }
}
```

#### 2. usleep()实现

```c
int usleep(useconds_t usec)
{
    // 验证参数
    if (usec >= 1000000) {
        errno = EINVAL;
        return -1;
    }

    if (usec == 0) {
        return 0;
    }

    // 根据时间长短选择策略
    if (usec >= 1000) {
        // >=1ms，使用k_sleep
        k_sleep(K_USEC(usec));
    } else {
        // <1ms，使用忙等
        k_busy_wait(usec);
    }

    // 检查信号
    SIGNAL_CHECK_POINT();

    return 0;
}
```

#### 3. nanosleep()实现

```c
int nanosleep(const struct timespec *req, struct timespec *rem)
{
    // 验证参数
    if (!req) {
        errno = EFAULT;
        return -1;
    }

    if (req->tv_sec < 0 || req->tv_nsec < 0 || req->tv_nsec >= 1000000000) {
        errno = EINVAL;
        return -1;
    }

    // 计算总纳秒数
    uint64_t total_nsec = (uint64_t)req->tv_sec * 1000000000ULL + req->tv_nsec;

    if (total_nsec == 0) {
        return 0;
    }

    // 记录开始时间
    uint64_t start_cycles = k_cycle_get_64();
    uint32_t cycles_per_sec = sys_clock_hw_cycles_per_sec();

    // 根据时间长短选择策略
    if (total_nsec >= 1000000) {
        // >=1ms，使用k_sleep
        k_sleep(K_NSEC(total_nsec));
    } else {
        // <1ms，使用忙等
        uint64_t usec = total_nsec / 1000;
        if (usec > 0) {
            k_busy_wait(usec);
        }
    }

    // 检查信号
    posix_thread_t *thread = get_current_posix_thread();
    if (thread && atomic_get(&thread->signal_ctx.pending.bits[0])) {
        // 有待处理信号，计算剩余时间
        if (rem) {
            uint64_t end_cycles = k_cycle_get_64();
            uint64_t elapsed_cycles = end_cycles - start_cycles;
            uint64_t elapsed_nsec = (elapsed_cycles * 1000000000ULL) / cycles_per_sec;

            if (elapsed_nsec < total_nsec) {
                uint64_t remaining_nsec = total_nsec - elapsed_nsec;
                rem->tv_sec = remaining_nsec / 1000000000ULL;
                rem->tv_nsec = remaining_nsec % 1000000000ULL;
            } else {
                rem->tv_sec = 0;
                rem->tv_nsec = 0;
            }
        }

        // 处理信号
        check_pending_signals();

        errno = EINTR;
        return -1;
    }

    // 正常完成
    if (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }

    return 0;
}
```

### 高精度实现

对于需要高精度的场景，可以使用硬件定时器：

```c
// 高精度纳秒级休眠
static void nanosleep_precise(uint64_t nsec)
{
    uint32_t cycles_per_sec = sys_clock_hw_cycles_per_sec();
    uint64_t target_cycles = (nsec * cycles_per_sec) / 1000000000ULL;

    uint64_t start = k_cycle_get_64();
    uint64_t end = start + target_cycles;

    // 如果时间较长，先用k_sleep休眠大部分时间
    if (nsec > 10000000) {  // >10ms
        uint64_t sleep_nsec = nsec - 1000000;  // 留1ms用忙等
        k_sleep(K_NSEC(sleep_nsec));
    }

    // 最后用忙等精确等待
    while (k_cycle_get_64() < end) {
        // 忙等
    }
}
```

### 可中断休眠

支持被信号中断的休眠：

```c
int nanosleep_interruptible(const struct timespec *req, struct timespec *rem)
{
    posix_thread_t *thread = get_current_posix_thread();
    if (!thread) {
        return nanosleep(req, rem);
    }

    // 设置休眠标志
    thread->in_sleep = true;
    thread->sleep_start = k_uptime_get();
    thread->sleep_duration_ms = timespec_to_ms(req);

    // 使用可中断的k_sleep
    int ret = k_sleep(K_MSEC(thread->sleep_duration_ms));

    // 清除休眠标志
    thread->in_sleep = false;

    // 检查是否被中断
    if (ret != 0 || atomic_get(&thread->signal_ctx.pending.bits[0])) {
        // 计算剩余时间
        if (rem) {
            int64_t elapsed_ms = k_uptime_get() - thread->sleep_start;
            int64_t remaining_ms = thread->sleep_duration_ms - elapsed_ms;

            if (remaining_ms > 0) {
                rem->tv_sec = remaining_ms / 1000;
                rem->tv_nsec = (remaining_ms % 1000) * 1000000;
            } else {
                rem->tv_sec = 0;
                rem->tv_nsec = 0;
            }
        }

        check_pending_signals();
        errno = EINTR;
        return -1;
    }

    if (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }

    return 0;
}
```

## clock_nanosleep实现

更强大的休眠接口，支持绝对时间：

```c
int clock_nanosleep(clockid_t clock_id, int flags,
                    const struct timespec *request,
                    struct timespec *remain)
{
    if (!request) {
        errno = EFAULT;
        return -1;
    }

    if (clock_id != CLOCK_REALTIME && clock_id != CLOCK_MONOTONIC) {
        errno = EINVAL;
        return -1;
    }

    struct timespec req = *request;

    // 如果是绝对时间，转换为相对时间
    if (flags & TIMER_ABSTIME) {
        struct timespec now;
        clock_gettime(clock_id, &now);

        // 计算相对时间
        req.tv_sec = request->tv_sec - now.tv_sec;
        req.tv_nsec = request->tv_nsec - now.tv_nsec;

        // 处理纳秒借位
        if (req.tv_nsec < 0) {
            req.tv_sec--;
            req.tv_nsec += 1000000000;
        }

        // 如果已经过期，立即返回
        if (req.tv_sec < 0) {
            if (remain) {
                remain->tv_sec = 0;
                remain->tv_nsec = 0;
            }
            return 0;
        }
    }

    // 调用nanosleep
    return nanosleep(&req, remain);
}
```

## 测试用例

### 测试1：基本sleep

```c
void test_basic_sleep(void)
{
    uint32_t start = k_uptime_get_32();

    sleep(2);

    uint32_t elapsed = k_uptime_get_32() - start;

    // 允许±100ms误差
    assert(elapsed >= 1900 && elapsed <= 2100);
}
```

### 测试2：usleep精度

```c
void test_usleep_precision(void)
{
    uint64_t start = k_cycle_get_64();

    usleep(10000);  // 10ms

    uint64_t end = k_cycle_get_64();
    uint32_t freq = sys_clock_hw_cycles_per_sec();
    uint64_t elapsed_us = ((end - start) * 1000000ULL) / freq;

    // 允许±1ms误差
    assert(elapsed_us >= 9000 && elapsed_us <= 11000);
}
```

### 测试3：nanosleep中断

```c
volatile int signal_received = 0;

void alarm_handler(int sig)
{
    signal_received = 1;
}

void test_nanosleep_interrupt(void)
{
    signal(SIGALRM, alarm_handler);

    // 设置1秒后的闹钟
    alarm(1);

    struct timespec req = {5, 0};  // 请求休眠5秒
    struct timespec rem;

    int ret = nanosleep(&req, &rem);

    // 应该被中断
    assert(ret == -1 && errno == EINTR);
    assert(signal_received == 1);

    // 剩余时间应该约为4秒
    assert(rem.tv_sec >= 3 && rem.tv_sec <= 5);
}
```

### 测试4：高精度休眠

```c
void test_nanosleep_precision(void)
{
    struct timespec req = {0, 100000};  // 100微秒
    struct timespec rem;

    uint64_t start = k_cycle_get_64();
    nanosleep(&req, &rem);
    uint64_t end = k_cycle_get_64();

    uint32_t freq = sys_clock_hw_cycles_per_sec();
    uint64_t elapsed_ns = ((end - start) * 1000000000ULL) / freq;

    // 允许±50微秒误差
    assert(elapsed_ns >= 50000 && elapsed_ns <= 150000);
}
```

### 测试5：零时间休眠

```c
void test_zero_sleep(void)
{
    // 零时间休眠应该立即返回
    unsigned int ret = sleep(0);
    assert(ret == 0);

    ret = usleep(0);
    assert(ret == 0);

    struct timespec req = {0, 0};
    ret = nanosleep(&req, NULL);
    assert(ret == 0);
}
```

## 性能考虑

### 精度vs功耗

| 方法 | 精度 | 功耗 | 适用场景 |
|------|------|------|----------|
| k_sleep | 1-10ms | 低 | 长时间休眠 |
| k_busy_wait | 微秒级 | 高 | 短时间精确延迟 |
| 混合方式 | 微秒级 | 中 | 中等时间休眠 |

### 时间开销

1. **sleep(1)**: 约1000ms ± 10ms
2. **usleep(1000)**: 约1ms ± 0.1ms
3. **nanosleep({0, 1000000})**: 约1ms ± 0.1ms
4. **k_busy_wait(100)**: 约100μs ± 1μs

## 注意事项

1. **精度限制**: 实际精度受系统时钟频率限制
2. **忙等功耗**: 短时间休眠使用忙等，会消耗CPU
3. **信号中断**: 休眠可能被信号中断，需要检查返回值
4. **剩余时间**: nanosleep被中断时，rem参数返回剩余时间
5. **时钟源**: 不同时钟源可能有不同精度
6. **上下文切换**: k_sleep会触发上下文切换，有额外开销

## 限制

1. 纳秒级精度实际受限于CPU频率（通常MHz级）
2. 系统时钟频率限制最小休眠时间（通常1-10ms）
3. 忙等会阻塞CPU，不适合长时间休眠
4. 信号中断处理可能不完全符合POSIX标准
5. 不支持CLOCK_PROCESS_CPUTIME_ID等特殊时钟

## 配置选项

```kconfig
config POSIX_SLEEP_PRECISION
    int "Sleep precision mode"
    default 1
    range 0 2
    help
      0: Low precision (use k_sleep only)
      1: Medium precision (k_sleep + k_busy_wait)
      2: High precision (cycle-accurate busy wait)

config POSIX_SLEEP_MIN_BUSY_WAIT_US
    int "Minimum time for busy wait (microseconds)"
    default 1000
    help
      Times below this threshold use busy wait instead of k_sleep.
```

## 最佳实践

1. **长时间休眠**: 使用sleep()或k_sleep()
2. **短时间精确延迟**: 使用usleep()或k_busy_wait()
3. **可中断休眠**: 使用nanosleep()并检查返回值
4. **绝对时间**: 使用clock_nanosleep()与TIMER_ABSTIME
5. **低功耗**: 避免频繁的短时间休眠，合并为较长休眠
