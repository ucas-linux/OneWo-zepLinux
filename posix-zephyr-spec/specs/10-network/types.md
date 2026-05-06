# 数据类型 - 网络

```c
typedef unsigned int socklen_t;
typedef unsigned short sa_family_t;

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};

struct sockaddr_in {
    sa_family_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
};

// 地址族
#define AF_INET   2
#define AF_INET6  10

// 套接字类型
#define SOCK_STREAM  1
#define SOCK_DGRAM   2
```
