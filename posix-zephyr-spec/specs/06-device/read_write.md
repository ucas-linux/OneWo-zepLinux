# read/write - 读写数据

## POSIX标准定义

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

## 功能描述

- **read**: 从文件描述符读取数据
- **write**: 向文件描述符写入数据

## 参数说明

- **fd**: 文件描述符
- **buf**: 数据缓冲区
- **count**: 要读写的字节数

## 返回值

- **>0**: 实际读写的字节数
- **0**: EOF（read时）
- **-1**: 失败，设置errno

## Zephyr适配方案

### read实现

```c
ssize_t read(int fd, void *buf, size_t count)
{
    if (fd < 0 || fd >= MAX_FDS || !buf) {
        errno = EINVAL;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];

    if (!entry->in_use) {
        errno = EBADF;
        return -1;
    }

    // 检查读权限
    if (!(entry->flags & (O_RDONLY | O_RDWR))) {
        errno = EBADF;
        return -1;
    }

    ssize_t ret = 0;

    switch (entry->type) {
    case FD_TYPE_FILE:
        ret = fs_read(entry->handle.file, buf, count);
        if (ret > 0) {
            entry->offset += ret;
        }
        break;

    case FD_TYPE_DEVICE:
        ret = device_read(entry->handle.dev, buf, count);
        break;

    case FD_TYPE_SOCKET:
        ret = zsock_recv(entry->handle.sockfd, buf, count, 0);
        break;

    case FD_TYPE_PIPE:
        size_t bytes_read;
        int r = k_pipe_get(entry->handle.pipe, buf, count,
                          &bytes_read, K_FOREVER);
        ret = (r == 0) ? bytes_read : -1;
        break;

    default:
        errno = EINVAL;
        return -1;
    }

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}
```

### write实现

```c
ssize_t write(int fd, const void *buf, size_t count)
{
    if (fd < 0 || fd >= MAX_FDS || !buf) {
        errno = EINVAL;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];

    if (!entry->in_use) {
        errno = EBADF;
        return -1;
    }

    // 检查写权限
    if (!(entry->flags & (O_WRONLY | O_RDWR))) {
        errno = EBADF;
        return -1;
    }

    ssize_t ret = 0;

    switch (entry->type) {
    case FD_TYPE_FILE:
        // 处理O_APPEND
        if (entry->flags & O_APPEND) {
            fs_seek(entry->handle.file, 0, FS_SEEK_END);
        }
        ret = fs_write(entry->handle.file, buf, count);
        if (ret > 0) {
            entry->offset += ret;
        }
        break;

    case FD_TYPE_DEVICE:
        ret = device_write(entry->handle.dev, buf, count);
        break;

    case FD_TYPE_SOCKET:
        ret = zsock_send(entry->handle.sockfd, buf, count, 0);
        break;

    case FD_TYPE_PIPE:
        size_t bytes_written;
        int r = k_pipe_put(entry->handle.pipe, (void *)buf, count,
                          &bytes_written, K_FOREVER);
        ret = (r == 0) ? bytes_written : -1;
        break;

    default:
        errno = EINVAL;
        return -1;
    }

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}
```

### 设备读写分发

```c
static ssize_t device_read(const struct device *dev, void *buf, size_t count)
{
    // 根据设备类型调用不同API
    if (device_is_uart(dev)) {
        return uart_fifo_read(dev, buf, count);
    } else if (device_is_i2c(dev)) {
        return i2c_read(dev, buf, count);
    } else if (device_is_spi(dev)) {
        return spi_read(dev, buf, count);
    }

    errno = ENOTSUP;
    return -1;
}

static ssize_t device_write(const struct device *dev,
                           const void *buf, size_t count)
{
    if (device_is_uart(dev)) {
        return uart_fifo_fill(dev, buf, count);
    } else if (device_is_i2c(dev)) {
        return i2c_write(dev, buf, count);
    } else if (device_is_spi(dev)) {
        return spi_write(dev, buf, count);
    }

    errno = ENOTSUP;
    return -1;
}
```

## 测试用例

### 测试1：文件读写

```c
void test_file_read_write(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    assert(fd >= 0);

    const char *data = "Hello, World!";
    ssize_t written = write(fd, data, strlen(data));
    assert(written == strlen(data));

    // 回到开头
    lseek(fd, 0, SEEK_SET);

    char buf[100];
    ssize_t nread = read(fd, buf, sizeof(buf));
    assert(nread == strlen(data));
    assert(memcmp(buf, data, nread) == 0);

    close(fd);
}
```

### 测试2：部分读写

```c
void test_partial_io(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

    char data[1000];
    memset(data, 'A', sizeof(data));

    // 写入
    ssize_t written = write(fd, data, sizeof(data));
    assert(written == sizeof(data));

    // 回到开头
    lseek(fd, 0, SEEK_SET);

    // 分块读取
    char buf[100];
    size_t total = 0;
    while (total < sizeof(data)) {
        ssize_t n = read(fd, buf, sizeof(buf));
        assert(n > 0);
        total += n;
    }

    assert(total == sizeof(data));
    close(fd);
}
```

## 性能考虑

1. **缓冲**: 大块读写比小块高效
2. **对齐**: 某些设备要求对齐访问
3. **阻塞**: 默认阻塞，可用O_NONBLOCK设置非阻塞

## 注意事项

1. **返回值**: 可能小于请求的字节数
2. **EOF**: read返回0表示文件结束
3. **EINTR**: 可能被信号中断
4. **原子性**: write不保证原子性
5. **O_APPEND**: write自动追加到文件末尾
