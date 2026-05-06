# I/O多路复用 - select/poll/epoll

## 概述

I/O多路复用允许单个线程监控多个文件描述符，等待其中任何一个变为就绪状态。这对于事件驱动的网络服务器和异步I/O至关重要。

## 目标

- 实现select()系统调用（POSIX标准）
- 实现poll()系统调用（更高效）
- 实现epoll系列（Linux特有，高性能）
- 映射到Zephyr的k_poll机制
- 支持文件、socket、管道等多种fd类型

## Linux I/O多路复用API

### select()

```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);

// fd_set操作宏
void FD_ZERO(fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
```

### poll()

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd {
    int   fd;         // 文件描述符
    short events;     // 请求的事件
    short revents;    // 返回的事件
};

// 事件标志
#define POLLIN      0x0001  // 可读
#define POLLOUT     0x0004  // 可写
#define POLLERR     0x0008  // 错误
#define POLLHUP     0x0010  // 挂断
#define POLLNVAL    0x0020  // 无效fd
```

### epoll()

```c
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);

struct epoll_event {
    uint32_t events;   // 事件掩码
    epoll_data_t data; // 用户数据
};

typedef union epoll_data {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

// 事件类型
#define EPOLLIN     0x001
#define EPOLLOUT    0x004
#define EPOLLERR    0x008
#define EPOLLHUP    0x010
#define EPOLLET     0x80000000  // 边缘触发
```

## 使用场景

```c
// 场景1：使用select监控多个socket
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(sock1, &readfds);
FD_SET(sock2, &readfds);

struct timeval tv = {.tv_sec = 5, .tv_usec = 0};
int ret = select(max_fd + 1, &readfds, NULL, NULL, &tv);

if (ret > 0) {
    if (FD_ISSET(sock1, &readfds)) {
        // sock1可读
    }
    if (FD_ISSET(sock2, &readfds)) {
        // sock2可读
    }
}

// 场景2：使用poll
struct pollfd fds[2];
fds[0].fd = sock1;
fds[0].events = POLLIN;
fds[1].fd = sock2;
fds[1].events = POLLIN | POLLOUT;

int ret = poll(fds, 2, 5000);  // 5秒超时

if (ret > 0) {
    if (fds[0].revents & POLLIN) {
        // sock1可读
    }
    if (fds[1].revents & POLLOUT) {
        // sock2可写
    }
}

// 场景3：使用epoll（高性能）
int epfd = epoll_create1(0);

struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = sock1;
epoll_ctl(epfd, EPOLL_CTL_ADD, sock1, &ev);

struct epoll_event events[10];
int nfds = epoll_wait(epfd, events, 10, 5000);

for (int i = 0; i < nfds; i++) {
    if (events[i].events & EPOLLIN) {
        int fd = events[i].data.fd;
        // 处理可读事件
    }
}
```

## 与Zephyr k_poll的映射

```c
// Zephyr的k_poll机制
struct k_poll_event events[] = {
    K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_SEM_AVAILABLE,
                             K_POLL_MODE_NOTIFY_ONLY,
                             &my_sem),
    K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_FIFO_DATA_AVAILABLE,
                             K_POLL_MODE_NOTIFY_ONLY,
                             &my_fifo),
};

k_poll(events, ARRAY_SIZE(events), K_MSEC(timeout));
```

## 架构组件

```
┌─────────────────┐
│ Linux App       │
│ select/poll/    │
│ epoll           │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ I/O Multiplex   │ ← 本模块
│ Layer           │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ FD Table        │ (Spec 06)
│ + k_poll        │
└─────────────────┘
```

## 配置选项

```kconfig
config POSIX_IO_MULTIPLEX
    bool "I/O multiplexing support"
    depends on POSIX_DEVICE_OPEN
    default n
    help
      Enable select/poll/epoll for I/O multiplexing.

config POSIX_SELECT
    bool "select() support"
    depends on POSIX_IO_MULTIPLEX
    default y

config POSIX_POLL
    bool "poll() support"
    depends on POSIX_IO_MULTIPLEX
    default y

config POSIX_EPOLL
    bool "epoll support"
    depends on POSIX_IO_MULTIPLEX
    default n
    help
      Enable Linux epoll API (more efficient than select/poll).

config EPOLL_MAX_INSTANCES
    int "Maximum epoll instances"
    depends on POSIX_EPOLL
    default 4
    range 1 16
```

## 文件组织

```
specs/19-io-multiplex/
├── README.md              # 本文件
├── select.md              # select实现
├── poll.md                # poll实现
├── epoll.md               # epoll实现
├── k_poll_mapping.md      # 映射到Zephyr k_poll
├── types.md               # 数据类型
└── implementation.md      # 实现指导
```

## 参考资料

- POSIX select: https://pubs.opengroup.org/onlinepubs/9699919799/functions/select.html
- Linux poll: https://man7.org/linux/man-pages/man2/poll.2.html
- Linux epoll: https://man7.org/linux/man-pages/man7/epoll.7.html
- Zephyr k_poll: https://docs.zephyrproject.org/latest/kernel/services/polling.html
