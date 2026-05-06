# select()实现

## API定义

```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);

// fd_set操作宏
void FD_ZERO(fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);

#define FD_SETSIZE 64  // 最大fd数量
```

## 数据结构

```c
typedef struct {
    uint32_t fds_bits[FD_SETSIZE / 32];
} fd_set;

struct timeval {
    time_t      tv_sec;   // 秒
    suseconds_t tv_usec;  // 微秒
};
```

## 实现

```c
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
    struct k_poll_event events[FD_SETSIZE];
    int num_events = 0;
    int ret;

    // 参数验证
    if (nfds < 0 || nfds > FD_SETSIZE) {
        errno = EINVAL;
        return -1;
    }

    // 构建k_poll事件数组
    for (int fd = 0; fd < nfds; fd++) {
        uint32_t poll_events = 0;

        if (readfds && FD_ISSET(fd, readfds)) {
            poll_events |= K_POLL_TYPE_DATA_AVAILABLE;
        }
        if (writefds && FD_ISSET(fd, writefds)) {
            poll_events |= K_POLL_TYPE_FIFO_DATA_AVAILABLE;
        }

        if (poll_events) {
            events[num_events].fd = fd;
            events[num_events].events = poll_events;
            events[num_events].revents = 0;
            num_events++;
        }
    }

    if (num_events == 0) {
        // 没有fd需要监控，只是sleep
        if (timeout) {
            k_msleep(timeout->tv_sec * 1000 + timeout->tv_usec / 1000);
        }
        return 0;
    }

    // 计算超时
    k_timeout_t k_timeout;
    if (timeout == NULL) {
        k_timeout = K_FOREVER;
    } else if (timeout->tv_sec == 0 && timeout->tv_usec == 0) {
        k_timeout = K_NO_WAIT;
    } else {
        k_timeout = K_MSEC(timeout->tv_sec * 1000 + timeout->tv_usec / 1000);
    }

    // 调用k_poll
    ret = k_poll(events, num_events, k_timeout);
    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    // 清空fd_set
    if (readfds) FD_ZERO(readfds);
    if (writefds) FD_ZERO(writefds);
    if (exceptfds) FD_ZERO(exceptfds);

    // 填充结果
    int ready_count = 0;
    for (int i = 0; i < num_events; i++) {
        if (events[i].revents) {
            int fd = events[i].fd;

            if (events[i].revents & K_POLL_TYPE_DATA_AVAILABLE) {
                if (readfds) FD_SET(fd, readfds);
                ready_count++;
            }
            if (events[i].revents & K_POLL_TYPE_FIFO_DATA_AVAILABLE) {
                if (writefds) FD_SET(fd, writefds);
                ready_count++;
            }
        }
    }

    return ready_count;
}
```

## fd_set操作宏实现

```c
void FD_ZERO(fd_set *set)
{
    memset(set, 0, sizeof(fd_set));
}

void FD_SET(int fd, fd_set *set)
{
    if (fd >= 0 && fd < FD_SETSIZE) {
        set->fds_bits[fd / 32] |= (1U << (fd % 32));
    }
}

void FD_CLR(int fd, fd_set *set)
{
    if (fd >= 0 && fd < FD_SETSIZE) {
        set->fds_bits[fd / 32] &= ~(1U << (fd % 32));
    }
}

int FD_ISSET(int fd, fd_set *set)
{
    if (fd >= 0 && fd < FD_SETSIZE) {
        return (set->fds_bits[fd / 32] & (1U << (fd % 32))) != 0;
    }
    return 0;
}
```

## 使用示例

```c
// 示例1：监控单个socket读事件
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
connect(sockfd, ...);

fd_set readfds;
FD_ZERO(&readfds);
FD_SET(sockfd, &readfds);

struct timeval tv = {.tv_sec = 5, .tv_usec = 0};
int ret = select(sockfd + 1, &readfds, NULL, NULL, &tv);

if (ret > 0 && FD_ISSET(sockfd, &readfds)) {
    // socket可读
    char buf[1024];
    recv(sockfd, buf, sizeof(buf), 0);
}

// 示例2：监控多个fd
int fd1 = open("/dev/uart0", O_RDONLY);
int fd2 = open("/dev/uart1", O_RDONLY);

fd_set readfds;
FD_ZERO(&readfds);
FD_SET(fd1, &readfds);
FD_SET(fd2, &readfds);

int max_fd = (fd1 > fd2) ? fd1 : fd2;
ret = select(max_fd + 1, &readfds, NULL, NULL, NULL);

if (ret > 0) {
    if (FD_ISSET(fd1, &readfds)) {
        // fd1可读
    }
    if (FD_ISSET(fd2, &readfds)) {
        // fd2可读
    }
}

// 示例3：同时监控读写
fd_set readfds, writefds;
FD_ZERO(&readfds);
FD_ZERO(&writefds);
FD_SET(sockfd, &readfds);
FD_SET(sockfd, &writefds);

ret = select(sockfd + 1, &readfds, &writefds, NULL, NULL);

if (ret > 0) {
    if (FD_ISSET(sockfd, &readfds)) {
        // 可读
    }
    if (FD_ISSET(sockfd, &writefds)) {
        // 可写
    }
}
```

## 性能考虑

```c
// select的O(n)复杂度问题
// 每次调用都需要遍历所有fd

// 优化：使用位操作加速
static inline int find_first_set_fd(fd_set *set, int start, int end)
{
    for (int i = start / 32; i < (end + 31) / 32; i++) {
        if (set->fds_bits[i]) {
            uint32_t bits = set->fds_bits[i];
            int base = i * 32;

            // 使用__builtin_ffs查找第一个设置的位
            int bit = __builtin_ffs(bits) - 1;
            int fd = base + bit;

            if (fd >= start && fd < end) {
                return fd;
            }
        }
    }
    return -1;
}
```

## 限制

1. **FD_SETSIZE限制**：最多监控64个fd（可配置）
2. **O(n)复杂度**：每次调用都要遍历所有fd
3. **fd_set复制开销**：每次调用都要复制fd_set
4. **不支持边缘触发**：只支持水平触发

## 与poll/epoll对比

```
select优点：
- POSIX标准，可移植性好
- 简单易用

select缺点：
- FD_SETSIZE限制
- O(n)复杂度
- 每次调用都要复制fd_set

建议：
- 少量fd（<10）：使用select
- 中等数量fd（10-100）：使用poll
- 大量fd（>100）：使用epoll
```
