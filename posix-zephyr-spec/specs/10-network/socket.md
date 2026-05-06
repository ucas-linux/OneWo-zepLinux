# socket - 网络套接字

## POSIX标准定义

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

## Zephyr适配

```c
// Zephyr已提供zsock_*函数，直接映射
int socket(int domain, int type, int protocol)
{
    int sockfd = zsock_socket(domain, type, protocol);
    if (sockfd < 0) {
        errno = -sockfd;
        return -1;
    }

    // 分配fd并映射
    int fd = alloc_fd();
    if (fd < 0) {
        zsock_close(sockfd);
        return -1;
    }

    g_fd_table[fd].type = FD_TYPE_SOCKET;
    g_fd_table[fd].handle.sockfd = sockfd;

    return fd;
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    fd_entry_t *entry = &g_fd_table[sockfd];
    if (entry->type != FD_TYPE_SOCKET) {
        errno = ENOTSOCK;
        return -1;
    }

    return zsock_bind(entry->handle.sockfd, addr, addrlen);
}

// 其他函数类似映射...
```

## 测试

```c
void test_socket(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr.s_addr = INADDR_ANY
    };

    int ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret == 0);

    close(sock);
}
```
