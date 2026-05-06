# epoll实现

## API定义

```c
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);

struct epoll_event {
    uint32_t events;      // 事件掩码
    epoll_data_t data;    // 用户数据
};

typedef union epoll_data {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;
```

## 事件类型

```c
#define EPOLLIN      0x001  // 可读
#define EPOLLOUT     0x004  // 可写
#define EPOLLRDHUP   0x2000 // 对端关闭连接
#define EPOLLPRI     0x002  // 紧急数据
#define EPOLLERR     0x008  // 错误
#define EPOLLHUP     0x010  // 挂断
#define EPOLLET      0x80000000  // 边缘触发
#define EPOLLONESHOT 0x40000000  // 一次性事件
```

## 操作类型

```c
#define EPOLL_CTL_ADD 1  // 添加fd
#define EPOLL_CTL_DEL 2  // 删除fd
#define EPOLL_CTL_MOD 3  // 修改fd事件
```

## 数据结构

```c
typedef struct epoll_fd_entry {
    int fd;
    struct epoll_event event;
    struct epoll_fd_entry *next;
} epoll_fd_entry_t;

typedef struct {
    int epfd;                      // epoll实例fd
    epoll_fd_entry_t *fd_list;     // 监控的fd链表
    int num_fds;                   // fd数量
    struct k_mutex lock;           // 保护链表
    struct k_poll_signal signal;   // 用于唤醒
} epoll_instance_t;

// 全局epoll实例表
static epoll_instance_t *g_epoll_instances[CONFIG_EPOLL_MAX_INSTANCES];
static struct k_mutex g_epoll_lock;
```

## 实现

### epoll_create/epoll_create1

```c
int epoll_create1(int flags)
{
    epoll_instance_t *inst;
    int epfd;

    // 分配实例
    inst = k_malloc(sizeof(epoll_instance_t));
    if (!inst) {
        errno = ENOMEM;
        return -1;
    }

    memset(inst, 0, sizeof(*inst));
    k_mutex_init(&inst->lock);
    k_poll_signal_init(&inst->signal);

    // 分配epoll fd
    k_mutex_lock(&g_epoll_lock, K_FOREVER);

    for (epfd = 0; epfd < CONFIG_EPOLL_MAX_INSTANCES; epfd++) {
        if (!g_epoll_instances[epfd]) {
            g_epoll_instances[epfd] = inst;
            inst->epfd = epfd;
            break;
        }
    }

    k_mutex_unlock(&g_epoll_lock);

    if (epfd >= CONFIG_EPOLL_MAX_INSTANCES) {
        k_free(inst);
        errno = EMFILE;
        return -1;
    }

    return epfd;
}

int epoll_create(int size)
{
    // size参数被忽略（历史遗留）
    return epoll_create1(0);
}
```

### epoll_ctl

```c
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
    epoll_instance_t *inst;
    epoll_fd_entry_t *entry, **prev;

    // 获取实例
    if (epfd < 0 || epfd >= CONFIG_EPOLL_MAX_INSTANCES) {
        errno = EBADF;
        return -1;
    }

    inst = g_epoll_instances[epfd];
    if (!inst) {
        errno = EBADF;
        return -1;
    }

    k_mutex_lock(&inst->lock, K_FOREVER);

    switch (op) {
    case EPOLL_CTL_ADD:
        // 检查fd是否已存在
        for (entry = inst->fd_list; entry; entry = entry->next) {
            if (entry->fd == fd) {
                k_mutex_unlock(&inst->lock);
                errno = EEXIST;
                return -1;
            }
        }

        // 添加新条目
        entry = k_malloc(sizeof(epoll_fd_entry_t));
        if (!entry) {
            k_mutex_unlock(&inst->lock);
            errno = ENOMEM;
            return -1;
        }

        entry->fd = fd;
        entry->event = *event;
        entry->next = inst->fd_list;
        inst->fd_list = entry;
        inst->num_fds++;
        break;

    case EPOLL_CTL_DEL:
        // 查找并删除
        prev = &inst->fd_list;
        for (entry = inst->fd_list; entry; entry = entry->next) {
            if (entry->fd == fd) {
                *prev = entry->next;
                k_free(entry);
                inst->num_fds--;
                k_mutex_unlock(&inst->lock);
                return 0;
            }
            prev = &entry->next;
        }

        k_mutex_unlock(&inst->lock);
        errno = ENOENT;
        return -1;

    case EPOLL_CTL_MOD:
        // 查找并修改
        for (entry = inst->fd_list; entry; entry = entry->next) {
            if (entry->fd == fd) {
                entry->event = *event;
                k_mutex_unlock(&inst->lock);
                return 0;
            }
        }

        k_mutex_unlock(&inst->lock);
        errno = ENOENT;
        return -1;

    default:
        k_mutex_unlock(&inst->lock);
        errno = EINVAL;
        return -1;
    }

    k_mutex_unlock(&inst->lock);
    return 0;
}
```

### epoll_wait

```c
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout)
{
    epoll_instance_t *inst;
    struct k_poll_event *poll_events;
    epoll_fd_entry_t *entry;
    int num_events = 0;
    int ret;

    // 参数验证
    if (epfd < 0 || epfd >= CONFIG_EPOLL_MAX_INSTANCES) {
        errno = EBADF;
        return -1;
    }

    inst = g_epoll_instances[epfd];
    if (!inst) {
        errno = EBADF;
        return -1;
    }

    if (maxevents <= 0) {
        errno = EINVAL;
        return -1;
    }

    k_mutex_lock(&inst->lock, K_FOREVER);

    if (inst->num_fds == 0) {
        k_mutex_unlock(&inst->lock);
        // 没有fd，只是sleep
        if (timeout > 0) {
            k_msleep(timeout);
        } else if (timeout < 0) {
            k_sleep(K_FOREVER);
        }
        return 0;
    }

    // 构建k_poll事件数组
    poll_events = k_malloc(inst->num_fds * sizeof(struct k_poll_event));
    if (!poll_events) {
        k_mutex_unlock(&inst->lock);
        errno = ENOMEM;
        return -1;
    }

    int i = 0;
    for (entry = inst->fd_list; entry; entry = entry->next) {
        uint32_t k_events = 0;

        if (entry->event.events & EPOLLIN) {
            k_events |= K_POLL_TYPE_DATA_AVAILABLE;
        }
        if (entry->event.events & EPOLLOUT) {
            k_events |= K_POLL_TYPE_FIFO_DATA_AVAILABLE;
        }

        poll_events[i].fd = entry->fd;
        poll_events[i].events = k_events;
        poll_events[i].revents = 0;
        i++;
    }

    k_mutex_unlock(&inst->lock);

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
    ret = k_poll(poll_events, inst->num_fds, k_timeout);
    if (ret < 0 && ret != -EAGAIN) {
        k_free(poll_events);
        errno = -ret;
        return -1;
    }

    // 收集就绪事件
    k_mutex_lock(&inst->lock, K_FOREVER);

    for (i = 0; i < inst->num_fds && num_events < maxevents; i++) {
        if (poll_events[i].revents) {
            // 查找对应的entry
            for (entry = inst->fd_list; entry; entry = entry->next) {
                if (entry->fd == poll_events[i].fd) {
                    events[num_events].events = 0;
                    events[num_events].data = entry->event.data;

                    if (poll_events[i].revents & K_POLL_TYPE_DATA_AVAILABLE) {
                        events[num_events].events |= EPOLLIN;
                    }
                    if (poll_events[i].revents & K_POLL_TYPE_FIFO_DATA_AVAILABLE) {
                        events[num_events].events |= EPOLLOUT;
                    }
                    if (poll_events[i].revents & K_POLL_STATE_ERROR) {
                        events[num_events].events |= EPOLLERR;
                    }

                    // 处理EPOLLONESHOT
                    if (entry->event.events & EPOLLONESHOT) {
                        entry->event.events = 0;  // 禁用事件
                    }

                    num_events++;
                    break;
                }
            }
        }
    }

    k_mutex_unlock(&inst->lock);
    k_free(poll_events);

    return num_events;
}
```

## 使用示例

```c
// 示例1：基本用法
int epfd = epoll_create1(0);

struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = sockfd;
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

struct epoll_event events[10];
int nfds = epoll_wait(epfd, events, 10, -1);

for (int i = 0; i < nfds; i++) {
    if (events[i].events & EPOLLIN) {
        int fd = events[i].data.fd;
        // 处理可读事件
    }
}

// 示例2：边缘触发
ev.events = EPOLLIN | EPOLLET;
ev.data.fd = sockfd;
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

// 边缘触发需要循环读取直到EAGAIN
while (1) {
    int nfds = epoll_wait(epfd, events, 10, -1);
    for (int i = 0; i < nfds; i++) {
        if (events[i].events & EPOLLIN) {
            int fd = events[i].data.fd;
            while (1) {
                ssize_t n = read(fd, buf, sizeof(buf));
                if (n < 0) {
                    if (errno == EAGAIN) break;
                    // 错误处理
                }
                if (n == 0) break;  // EOF
                // 处理数据
            }
        }
    }
}

// 示例3：服务器事件循环
int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
bind(listen_fd, ...);
listen(listen_fd, 128);

int epfd = epoll_create1(0);

struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = listen_fd;
epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

while (1) {
    struct epoll_event events[64];
    int nfds = epoll_wait(epfd, events, 64, -1);

    for (int i = 0; i < nfds; i++) {
        if (events[i].data.fd == listen_fd) {
            // 新连接
            int conn_fd = accept(listen_fd, NULL, NULL);
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_fd;
            epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
        } else {
            // 数据到达
            int fd = events[i].data.fd;
            if (events[i].events & EPOLLIN) {
                handle_read(fd);
            }
            if (events[i].events & EPOLLOUT) {
                handle_write(fd);
            }
        }
    }
}
```

## 性能优势

```
epoll vs select/poll:
- O(1)复杂度（只返回就绪的fd）
- 没有fd数量限制
- 支持边缘触发（减少系统调用）
- 内核维护就绪列表

适用场景：
- 大量并发连接（>1000）
- 长连接服务器
- 高性能网络应用
```

## 边缘触发 vs 水平触发

```c
// 水平触发（默认）
// 只要fd就绪，epoll_wait就会返回
ev.events = EPOLLIN;

// 边缘触发
// 只在状态变化时返回（需要完全读取）
ev.events = EPOLLIN | EPOLLET;
```
