# 网络Socket接口规范

## 概述

本模块定义12个POSIX Socket网络接口在Zephyr RTOS上的实现规范。

## 接口列表

1. `socket` - 创建socket
2. `bind` - 绑定地址
3. `listen` - 监听连接
4. `accept` - 接受连接
5. `connect` - 连接服务器
6. `send` - 发送数据
7. `recv` - 接收数据
8. `sendto` - 发送数据报
9. `recvfrom` - 接收数据报
10. `setsockopt` - 设置socket选项
11. `getsockopt` - 获取socket选项
12. `getaddrinfo` - 域名解析

## Zephyr现有支持

Zephyr提供BSD Socket兼容的API：

```c
// Zephyr已有的socket API
int zsock_socket(int family, int type, int proto);
int zsock_bind(int sock, const struct sockaddr *addr, socklen_t addrlen);
int zsock_listen(int sock, int backlog);
int zsock_accept(int sock, struct sockaddr *addr, socklen_t *addrlen);
int zsock_connect(int sock, const struct sockaddr *addr, socklen_t addrlen);
ssize_t zsock_send(int sock, const void *buf, size_t len, int flags);
ssize_t zsock_recv(int sock, void *buf, size_t max_len, int flags);
// ... 等等
```

## 适配策略

### 方案1：直接映射（推荐）

```c
// 简单封装Zephyr的zsock API
#define socket(family, type, proto) zsock_socket(family, type, proto)
#define bind(sock, addr, addrlen) zsock_bind(sock, addr, addrlen)
#define listen(sock, backlog) zsock_listen(sock, backlog)
// ... 其他接口类似
```

**优点**：
- 实现简单，开销小
- 充分利用Zephyr现有功能
- 兼容性好

**缺点**：
- 依赖Zephyr网络栈配置

### 方案2：集成到fd表

```c
// 将socket集成到统一的文件描述符表
int socket(int family, int type, int proto)
{
    // 创建Zephyr socket
    int zsock = zsock_socket(family, type, proto);
    if (zsock < 0) return zsock;

    // 分配POSIX fd
    int fd = alloc_fd();
    if (fd < 0) {
        zsock_close(zsock);
        return -EMFILE;
    }

    // 注册到fd表
    fd_table[fd].type = FD_TYPE_SOCKET;
    fd_table[fd].handle.sockfd = zsock;

    return fd;
}
```

**优点**：
- 统一的fd管理
- 可以对socket使用read/write/close等通用接口
- 更符合POSIX语义

**缺点**：
- 额外的fd映射开销
- 实现复杂度增加

## 推荐实现

采用**方案1（直接映射）**，理由：
1. Zephyr的zsock API已经高度兼容POSIX
2. 性能开销最小
3. 实现简单，维护成本低
4. 大多数Linux网络应用可以直接使用

## 配置要求

```kconfig
# 需要启用的Zephyr配置
CONFIG_NETWORKING=y
CONFIG_NET_SOCKETS=y
CONFIG_NET_SOCKETS_POSIX_NAMES=y  # 自动定义POSIX名称

# TCP支持
CONFIG_NET_TCP=y

# UDP支持
CONFIG_NET_UDP=y

# IPv4/IPv6
CONFIG_NET_IPV4=y
CONFIG_NET_IPV6=y

# DNS解析
CONFIG_DNS_RESOLVER=y
CONFIG_POSIX_API=y
```

## 实现示例

```c
// 如果Zephyr已启用POSIX_NAMES，则无需额外封装
// 否则提供简单的宏定义

#ifndef CONFIG_NET_SOCKETS_POSIX_NAMES

#include <zephyr/net/socket.h>

#define socket(family, type, proto) zsock_socket(family, type, proto)
#define bind(sock, addr, len) zsock_bind(sock, addr, len)
#define listen(sock, backlog) zsock_listen(sock, backlog)
#define accept(sock, addr, len) zsock_accept(sock, addr, len)
#define connect(sock, addr, len) zsock_connect(sock, addr, len)
#define send(sock, buf, len, flags) zsock_send(sock, buf, len, flags)
#define recv(sock, buf, len, flags) zsock_recv(sock, buf, len, flags)
#define sendto(sock, buf, len, flags, addr, alen) \
    zsock_sendto(sock, buf, len, flags, addr, alen)
#define recvfrom(sock, buf, len, flags, addr, alen) \
    zsock_recvfrom(sock, buf, len, flags, addr, alen)
#define setsockopt(sock, level, optname, optval, optlen) \
    zsock_setsockopt(sock, level, optname, optval, optlen)
#define getsockopt(sock, level, optname, optval, optlen) \
    zsock_getsockopt(sock, level, optname, optval, optlen)
#define close(sock) zsock_close(sock)

#endif /* CONFIG_NET_SOCKETS_POSIX_NAMES */
```

## getaddrinfo实现

```c
#include <zephyr/net/dns_resolve.h>

int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res)
{
    // 使用Zephyr DNS解析器
    struct zsock_addrinfo zhints;
    struct zsock_addrinfo *zres;

    // 转换hints
    if (hints) {
        zhints.ai_family = hints->ai_family;
        zhints.ai_socktype = hints->ai_socktype;
        zhints.ai_protocol = hints->ai_protocol;
        zhints.ai_flags = hints->ai_flags;
    }

    // 调用Zephyr API
    int ret = zsock_getaddrinfo(node, service,
                                hints ? &zhints : NULL, &zres);
    if (ret != 0) return ret;

    // 转换结果
    *res = convert_addrinfo(zres);

    return 0;
}

void freeaddrinfo(struct addrinfo *res)
{
    zsock_freeaddrinfo((struct zsock_addrinfo *)res);
}
```

## 测试要点

- TCP客户端/服务器通信
- UDP数据报收发
- 非阻塞socket
- select/poll多路复用
- socket选项设置
- DNS域名解析
- 并发连接处理
- 错误处理

## 性能考虑

- Zephyr网络栈针对嵌入式优化
- 内存占用较小（数十KB）
- 支持零拷贝（取决于网卡驱动）
- 建议限制并发连接数（<10个）

## 限制

1. 不支持所有socket选项（如SO_LINGER部分特性）
2. 性能不如Linux（但对嵌入式足够）
3. 依赖网络硬件和驱动支持
4. IPv6支持可能不完整
