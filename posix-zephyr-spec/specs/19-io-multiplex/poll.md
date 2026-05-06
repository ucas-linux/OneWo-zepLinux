# poll()实现

## API定义

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd {
    int   fd;         // 文件描述符
    short events;     // 请求的事件
    short revents;    // 返回的事件
};

typedef unsigned long nfds_t;
```

## 事件标志

```c
// 输入事件（events字段）
#define POLLIN      0x0001  // 有数据可读
#define POLLPRI     0x0002  // 有紧急数据可读
#define POLLOUT     0x0004  // 可以写数据
#define POLLRDNORM  0x0040  // 普通数据可读
#define POLLRDBAND  0x0080  // 优先级带数据可读
#define POLLWRNORM  0x0100  // 普通数据可写
#define POLLWRBAND  0x0200  // 优先级带数据可写

// 输出事件（revents字段）
#define POLLERR     0x0008  // 发生错误
#define POLLHUP     0x0010  // 挂断
#define POLLNVAL    0x0020  // 无效的fd
```

## 实现

```c
int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    struct k_poll_event *events;
    int ret;
    int ready_count = 0;

    // 参数验证
    if (!fds && nfds > 0) {
        errno = EFAULT;
        return -1;
    }

    if (nfds == 0) {
        // 只是sleep
        if (timeout > 0) {
            k_msleep(timeout);
        } else if (timeout < 0) {
            k_sleep(K_FOREVER);
        }
        return 0;
    }

    // 分配k_poll事件数组
    events = k_malloc(nfds * sizeof(struct k_poll_event));
    if (!events) {
        errno = ENOMEM;
        return -1;
    }

    // 构建k_poll事件
    for (nfds_t i = 0; i < nfds; i++) {
        fds[i].revents = 0;

        // 检查fd有效性
        if (fds[i].fd < 0) {
            events[i].fd = -1;
            fds[i].revents = POLLNVAL;
            ready_count++;
            continue;
        }

        // 映射事件
        uint32_t k_events = 0;
        if (fds[i].events & (POLLIN | POLLRDNORM)) {
            k_events |= K_POLL_TYPE_DATA_AVAILABLE;
        }
        if (fds[i].events & (POLLOUT | POLLWRNORM)) {
            k_events |= K_POLL_TYPE_FIFO_DATA_AVAILABLE;
        }

        events[i].fd = fds[i].fd;
        events[i].events = k_events;
        events[i].revents = 0;
    }

    // 如果已有无效fd，立即返回
    if (ready_count > 0) {
        k_free(events);
        return ready_count;
    }

    // 计算超时
    k_timeout_t k_timeout;
    if (timeout < 0) {
        k_timeout = K_FOREVER;
    } else if (timeout == 0) {
        k_timeout = K_NO_WAIT;
    } else {
        k_timeout = K_MSEC(timeout);
    }

    // 调用k_poll
    ret = k_poll(events, nfds, k_timeout);
    if (ret < 0 && ret != -EAGAIN) {
        k_free(events);
        errno = -ret;
        return -1;
    }

    // 填充结果
    for (nfds_t i = 0; i < nfds; i++) {
        if (events[i].revents) {
            if (events[i].revents & K_POLL_TYPE_DATA_AVAILABLE) {
                fds[i].revents |= POLLIN;
            }
            if (events[i].revents & K_POLL_TYPE_FIFO_DATA_AVAILABLE) {
                fds[i].revents |= POLLOUT;
            }
            if (events[i].revents & K_POLL_STATE_ERROR) {
                fds[i].revents |= POLLERR;
            }
            if (events[i].revents & K_POLL_STATE_CANCELLED) {
                fds[i].revents |= POLLHUP;
            }

            ready_count++;
        }
    }

    k_free(events);
    return ready_count;
}
```

## 使用示例

```c
// 示例1：监控单个socket
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
connect(sockfd, ...);

struct pollfd pfd;
pfd.fd = sockfd;
pfd.events = POLLIN;

int ret = poll(&pfd, 1, 5000);  // 5秒超时

if (ret > 0 && (pfd.revents & POLLIN)) {
    // socket可读
    char buf[1024];
    recv(sockfd, buf, sizeof(buf), 0);
}

// 示例2：监控多个fd
struct pollfd fds[3];

fds[0].fd = sock1;
fds[0].events = POLLIN;

fds[1].fd = sock2;
fds[1].events = POLLIN | POLLOUT;

fds[2].fd = pipe_fd;
fds[2].events = POLLIN;

ret = poll(fds, 3, -1);  // 无限等待

if (ret > 0) {
    for (int i = 0; i < 3; i++) {
        if (fds[i].revents & POLLIN) {
            // fds[i]可读
        }
        if (fds[i].revents & POLLOUT) {
            // fds[i]可写
        }
        if (fds[i].revents & POLLERR) {
            // fds[i]发生错误
        }
        if (fds[i].revents & POLLHUP) {
            // fds[i]挂断
        }
    }
}

// 示例3：动态添加/删除fd
#define MAX_FDS 10
struct pollfd fds[MAX_FDS];
int nfds = 0;

// 添加fd
void add_fd(int fd, short events) {
    if (nfds < MAX_FDS) {
        fds[nfds].fd = fd;
        fds[nfds].events = events;
        nfds++;
    }
}

// 删除fd
void remove_fd(int fd) {
    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd == fd) {
            // 移动后面的元素
            memmove(&fds[i], &fds[i+1],
                    (nfds - i - 1) * sizeof(struct pollfd));
            nfds--;
            break;
        }
    }
}

// 主循环
while (1) {
    ret = poll(fds, nfds, 1000);

    if (ret > 0) {
        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents) {
                handle_event(&fds[i]);
            }
        }
    }
}
```

## 与select对比

```
poll优点：
- 没有FD_SETSIZE限制
- 不需要计算max_fd
- 不需要重新设置fd_set
- 更清晰的事件语义

poll缺点：
- 仍然是O(n)复杂度
- 需要遍历整个数组

性能：
- 少量fd：select和poll性能相近
- 大量fd：poll略优于select
- 非常大量fd：应使用epoll
```

## 错误处理

```c
int ret = poll(fds, nfds, timeout);

if (ret < 0) {
    // 错误
    switch (errno) {
    case EFAULT:
        // fds指针无效
        break;
    case EINTR:
        // 被信号中断
        break;
    case EINVAL:
        // nfds超过限制
        break;
    case ENOMEM:
        // 内存不足
        break;
    }
} else if (ret == 0) {
    // 超时，没有fd就绪
} else {
    // ret个fd就绪
    for (int i = 0; i < nfds; i++) {
        if (fds[i].revents) {
            // 处理就绪的fd
        }
    }
}
```
