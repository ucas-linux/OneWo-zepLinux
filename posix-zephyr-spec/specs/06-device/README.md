# 设备管理接口规范

## 概述

本模块定义5个POSIX设备管理接口在Zephyr RTOS上的实现规范。

## 接口列表

1. `open` - 打开设备/文件
2. `close` - 关闭设备/文件
3. `read` - 读取数据
4. `write` - 写入数据
5. `ioctl` - 设备控制

## 核心挑战

### 1. 文件描述符抽象
**问题**：Zephyr使用设备指针，Linux使用整数文件描述符。

**解决方案**：
- 维护全局文件描述符表
- 将设备指针、文件指针、socket等映射到整数fd
- 标准fd：0=stdin, 1=stdout, 2=stderr

### 2. 统一的read/write接口
**问题**：不同设备类型有不同的API（uart_fifo_read, i2c_write等）。

**解决方案**：
- 实现通用分发器
- 根据fd类型调用对应的底层驱动API
- 处理阻塞/非阻塞模式

### 3. ioctl命令转换
**问题**：Zephyr驱动使用特定API，Linux使用ioctl统一接口。

**解决方案**：
- 定义标准ioctl命令号
- 在设备驱动中扩展control回调
- 将ioctl命令转换为驱动特定操作

## 数据结构

```c
// 文件描述符类型
enum fd_type {
    FD_TYPE_NONE,
    FD_TYPE_FILE,       // 文件系统文件
    FD_TYPE_DEVICE,     // 字符/块设备
    FD_TYPE_SOCKET,     // 网络socket
    FD_TYPE_PIPE,       // 管道
    FD_TYPE_EVENTFD,    // 事件fd
};

// 文件描述符表项
typedef struct {
    int fd;                         // 文件描述符
    enum fd_type type;              // 类型
    int flags;                      // 打开标志(O_RDONLY, O_WRONLY等)
    off_t offset;                   // 当前偏移

    union {
        struct fs_file_t *file;     // 文件系统文件
        const struct device *dev;   // 设备
        int sockfd;                 // socket
        struct k_pipe *pipe;        // 管道
    } handle;

    struct k_mutex lock;            // 保护并发访问
} fd_entry_t;

// 全局fd表
#define MAX_FDS 64
static fd_entry_t fd_table[MAX_FDS];
static struct k_mutex fd_table_lock;
```

## 实现要点

### open实现

```c
int open(const char *pathname, int flags, ...)
{
    // 1. 参数验证
    if (!pathname) return -EINVAL;

    // 2. 分配fd
    int fd = alloc_fd();
    if (fd < 0) return -EMFILE;

    // 3. 判断路径类型
    if (strncmp(pathname, "/dev/", 5) == 0) {
        // 设备文件
        const char *devname = pathname + 5;
        const struct device *dev = device_get_binding(devname);
        if (!dev) {
            free_fd(fd);
            return -ENOENT;
        }

        fd_table[fd].type = FD_TYPE_DEVICE;
        fd_table[fd].handle.dev = dev;
    } else {
        // 普通文件
        struct fs_file_t *file = k_malloc(sizeof(*file));
        fs_file_t_init(file);

        int ret = fs_open(file, pathname, flags);
        if (ret < 0) {
            k_free(file);
            free_fd(fd);
            return ret;
        }

        fd_table[fd].type = FD_TYPE_FILE;
        fd_table[fd].handle.file = file;
    }

    fd_table[fd].flags = flags;
    fd_table[fd].offset = 0;

    return fd;
}
```

### read/write分发

```c
ssize_t read(int fd, void *buf, size_t count)
{
    if (fd < 0 || fd >= MAX_FDS) return -EBADF;
    if (!buf) return -EINVAL;

    fd_entry_t *entry = &fd_table[fd];

    switch (entry->type) {
    case FD_TYPE_FILE:
        return fs_read(entry->handle.file, buf, count);

    case FD_TYPE_DEVICE:
        // 根据设备类型调用不同API
        if (device_is_uart(entry->handle.dev)) {
            return uart_read(entry->handle.dev, buf, count);
        } else if (device_is_i2c(entry->handle.dev)) {
            return i2c_read(entry->handle.dev, buf, count);
        }
        return -ENOTSUP;

    case FD_TYPE_SOCKET:
        return zsock_recv(entry->handle.sockfd, buf, count, 0);

    case FD_TYPE_PIPE:
        return k_pipe_get(entry->handle.pipe, buf, count, NULL, K_FOREVER);

    default:
        return -EINVAL;
    }
}
```

## 配置选项

```kconfig
config POSIX_MAX_FDS
    int "Maximum number of file descriptors"
    default 64
    help
      Maximum number of open file descriptors per process.

config POSIX_DEVICE_OPEN
    bool "Support /dev/ device files"
    default y
    help
      Enable opening devices through /dev/ paths.

config POSIX_IOCTL
    bool "Support ioctl system call"
    default y
    help
      Enable ioctl for device control operations.
```

## 测试要点

- 打开/关闭文件和设备
- 读写数据正确性
- 文件偏移管理
- 并发访问保护
- 错误处理（无效fd、权限等）
- ioctl设备控制
- 资源泄漏检测
