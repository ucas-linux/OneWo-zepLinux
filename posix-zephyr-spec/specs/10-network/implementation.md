# 实现指导 - 网络

## 核心实现

```c
// 直接使用Zephyr的zsock_*函数
int socket(int domain, int type, int protocol)
{
    int sockfd = zsock_socket(domain, type, protocol);

    // 映射到fd表
    int fd = alloc_fd();
    g_fd_table[fd].type = FD_TYPE_SOCKET;
    g_fd_table[fd].handle.sockfd = sockfd;

    return fd;
}

// 其他函数直接转发到zsock_*
int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    return zsock_bind(g_fd_table[fd].handle.sockfd, addr, len);
}
```

## 配置

```kconfig
config POSIX_SOCKET
    bool "POSIX socket support"
    depends on NETWORKING
    default y
```
