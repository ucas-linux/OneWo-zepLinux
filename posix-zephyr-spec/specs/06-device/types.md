# 数据类型定义

## 文件描述符类型

### fd_type
```c
enum fd_type {
    FD_TYPE_NONE,       // 未使用
    FD_TYPE_FILE,       // 文件系统文件
    FD_TYPE_DEVICE,     // 字符/块设备
    FD_TYPE_SOCKET,     // 网络socket
    FD_TYPE_PIPE,       // 管道
    FD_TYPE_EVENTFD,    // 事件fd
};
```

### fd_entry_t
```c
typedef struct {
    bool in_use;                    // 是否使用中
    enum fd_type type;              // 类型
    int flags;                      // 打开标志
    off_t offset;                   // 当前偏移

    union {
        struct fs_file_t *file;     // 文件系统文件
        const struct device *dev;   // 设备
        int sockfd;                 // socket
        struct k_pipe *pipe;        // 管道
    } handle;

    struct k_mutex lock;            // 保护并发访问
} fd_entry_t;
```

## 打开标志

```c
// 访问模式
#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002

// 创建标志
#define O_CREAT     0x0040
#define O_EXCL      0x0080
#define O_TRUNC     0x0200

// 操作标志
#define O_APPEND    0x0400
#define O_NONBLOCK  0x0800
#define O_SYNC      0x1000
```

## 全局数据结构

```c
#define MAX_FDS 64

// 全局fd表
static fd_entry_t g_fd_table[MAX_FDS];

// 全局锁
static struct k_mutex g_fd_table_lock;
```

## ioctl命令

```c
// 终端控制
#define TCGETS      0x5401
#define TCSETS      0x5402

// 文件控制
#define FIONREAD    0x541B
#define FIONBIO     0x5421
```

## 错误码

```c
#define EBADF       9   // 无效的文件描述符
#define EINVAL      22  // 无效参数
#define EMFILE      24  // 打开文件过多
#define ENOENT      2   // 文件不存在
#define ENOTTY      25  // 不支持的ioctl
```
