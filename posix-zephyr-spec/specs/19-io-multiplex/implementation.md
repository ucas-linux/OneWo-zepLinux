# I/O多路复用实现指导

## 总体架构

```
posix_io_multiplex/
├── select.c              # select实现
├── poll.c                # poll实现
├── epoll.c               # epoll实现
├── k_poll_adapter.c      # Zephyr k_poll适配器
├── io_multiplex_internal.h  # 内部头文件
└── io_multiplex_debug.c  # 调试支持
```

## 初始化

```c
int io_multiplex_init(void)
{
    // 初始化epoll实例表
    k_mutex_init(&g_epoll_lock);
    memset(g_epoll_instances, 0, sizeof(g_epoll_instances));

    printk("I/O multiplexing initialized\n");
    return 0;
}

SYS_INIT(io_multiplex_init, POST_KERNEL, CONFIG_IO_MULTIPLEX_INIT_PRIORITY);
```

## k_poll适配器

```c
// 将POSIX事件映射到Zephyr k_poll事件
static int map_events_to_k_poll(int fd, short events,
                                 struct k_poll_event *k_event)
{
    uint32_t k_events = 0;

    if (events & (POLLIN | POLLRDNORM)) {
        k_events |= K_POLL_TYPE_DATA_AVAILABLE;
    }
    if (events & (POLLOUT | POLLWRNORM)) {
        k_events |= K_POLL_TYPE_FIFO_DATA_AVAILABLE;
    }

    if (k_events == 0) {
        return -EINVAL;
    }

    k_event->fd = fd;
    k_event->events = k_events;
    k_event->revents = 0;

    return 0;
}

// 将k_poll结果映射回POSIX事件
static short map_k_poll_to_events(uint32_t k_revents)
{
    short events = 0;

    if (k_revents & K_POLL_TYPE_DATA_AVAILABLE) {
        events |= POLLIN;
    }
    if (k_revents & K_POLL_TYPE_FIFO_DATA_AVAILABLE) {
        events |= POLLOUT;
    }
    if (k_revents & K_POLL_STATE_ERROR) {
        events |= POLLERR;
    }
    if (k_revents & K_POLL_STATE_CANCELLED) {
        events |= POLLHUP;
    }

    return events;
}
```

## 性能优化

### 1. fd_set位操作优化

```c
// 使用内建函数加速位操作
static inline int fd_set_count(fd_set *set)
{
    int count = 0;
    for (int i = 0; i < FD_SETSIZE / 32; i++) {
        count += __builtin_popcount(set->fds_bits[i]);
    }
    return count;
}

static inline int fd_set_first(fd_set *set)
{
    for (int i = 0; i < FD_SETSIZE / 32; i++) {
        if (set->fds_bits[i]) {
            int bit = __builtin_ffs(set->fds_bits[i]) - 1;
            return i * 32 + bit;
        }
    }
    return -1;
}
```

### 2. epoll红黑树优化

```c
// 使用红黑树加速epoll查找（大量fd时）
#ifdef CONFIG_EPOLL_USE_RBTREE

typedef struct epoll_rb_node {
    int fd;
    struct epoll_event event;
    struct epoll_rb_node *left;
    struct epoll_rb_node *right;
    struct epoll_rb_node *parent;
    int color;  // 0=black, 1=red
} epoll_rb_node_t;

static epoll_rb_node_t *epoll_rb_find(epoll_rb_node_t *root, int fd)
{
    while (root) {
        if (fd < root->fd) {
            root = root->left;
        } else if (fd > root->fd) {
            root = root->right;
        } else {
            return root;
        }
    }
    return NULL;
}

#endif
```

## 配置选项

```kconfig
menuconfig POSIX_IO_MULTIPLEX
    bool "I/O multiplexing support"
    depends on POSIX_DEVICE_OPEN
    default n

if POSIX_IO_MULTIPLEX

config POSIX_SELECT
    bool "select() support"
    default y

config POSIX_POLL
    bool "poll() support"
    default y

config POSIX_EPOLL
    bool "epoll support"
    default n

config EPOLL_MAX_INSTANCES
    int "Maximum epoll instances"
    depends on POSIX_EPOLL
    default 4
    range 1 16

config EPOLL_USE_RBTREE
    bool "Use red-black tree for epoll"
    depends on POSIX_EPOLL
    default n
    help
      Use red-black tree instead of linked list for large fd sets.

config IO_MULTIPLEX_INIT_PRIORITY
    int "I/O multiplex initialization priority"
    default 70

endif
```

## 测试框架

```c
#ifdef CONFIG_IO_MULTIPLEX_TESTS

void test_select_basic(void)
{
    int pipefd[2];
    pipe(pipefd);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(pipefd[0], &readfds);

    struct timeval tv = {.tv_sec = 0, .tv_usec = 100000};
    int ret = select(pipefd[0] + 1, &readfds, NULL, NULL, &tv);

    zassert_equal(ret, 0, "select should timeout");

    write(pipefd[1], "test", 4);
    ret = select(pipefd[0] + 1, &readfds, NULL, NULL, &tv);

    zassert_equal(ret, 1, "select should return 1");
    zassert_true(FD_ISSET(pipefd[0], &readfds), "fd should be set");

    close(pipefd[0]);
    close(pipefd[1]);
}

void test_poll_basic(void)
{
    int pipefd[2];
    pipe(pipefd);

    struct pollfd pfd;
    pfd.fd = pipefd[0];
    pfd.events = POLLIN;

    int ret = poll(&pfd, 1, 100);
    zassert_equal(ret, 0, "poll should timeout");

    write(pipefd[1], "test", 4);
    ret = poll(&pfd, 1, 100);

    zassert_equal(ret, 1, "poll should return 1");
    zassert_true(pfd.revents & POLLIN, "POLLIN should be set");

    close(pipefd[0]);
    close(pipefd[1]);
}

void test_epoll_basic(void)
{
    int epfd = epoll_create1(0);
    zassert_true(epfd >= 0, "epoll_create1 failed");

    int pipefd[2];
    pipe(pipefd);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = pipefd[0];

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, pipefd[0], &ev);
    zassert_equal(ret, 0, "epoll_ctl ADD failed");

    struct epoll_event events[1];
    ret = epoll_wait(epfd, events, 1, 100);
    zassert_equal(ret, 0, "epoll_wait should timeout");

    write(pipefd[1], "test", 4);
    ret = epoll_wait(epfd, events, 1, 100);

    zassert_equal(ret, 1, "epoll_wait should return 1");
    zassert_equal(events[0].data.fd, pipefd[0], "wrong fd");

    close(pipefd[0]);
    close(pipefd[1]);
    close(epfd);
}

ZTEST_SUITE(io_multiplex_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(io_multiplex_tests, test_select) { test_select_basic(); }
ZTEST(io_multiplex_tests, test_poll) { test_poll_basic(); }
ZTEST(io_multiplex_tests, test_epoll) { test_epoll_basic(); }

#endif
```

## 调试支持

```c
#ifdef CONFIG_IO_MULTIPLEX_DEBUG

void dump_fd_set(const char *name, fd_set *set, int nfds)
{
    printk("%s: ", name);
    for (int i = 0; i < nfds; i++) {
        if (FD_ISSET(i, set)) {
            printk("%d ", i);
        }
    }
    printk("\n");
}

void dump_pollfd_array(struct pollfd *fds, nfds_t nfds)
{
    printk("poll fds:\n");
    for (nfds_t i = 0; i < nfds; i++) {
        printk("  [%u] fd=%d events=0x%x revents=0x%x\n",
               i, fds[i].fd, fds[i].events, fds[i].revents);
    }
}

void dump_epoll_instance(int epfd)
{
    epoll_instance_t *inst = g_epoll_instances[epfd];
    if (!inst) {
        printk("Invalid epoll fd: %d\n", epfd);
        return;
    }

    printk("epoll instance %d:\n", epfd);
    printk("  num_fds: %d\n", inst->num_fds);

    epoll_fd_entry_t *entry = inst->fd_list;
    while (entry) {
        printk("  fd=%d events=0x%x\n", entry->fd, entry->event.events);
        entry = entry->next;
    }
}

#endif
```

## 最佳实践

1. **选择合适的API**:
   - 少量fd（<10）: select
   - 中等数量（10-100）: poll
   - 大量fd（>100）: epoll

2. **避免常见错误**:
   - select: 记得重新设置fd_set
   - poll: 检查revents而不是events
   - epoll: 边缘触发需要循环读取

3. **性能优化**:
   - 使用非阻塞I/O
   - 合理设置超时
   - epoll使用边缘触发减少系统调用

4. **错误处理**:
   - 检查返回值
   - 处理EINTR（信号中断）
   - 验证fd有效性
