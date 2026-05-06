# ioctl - 设备控制

## POSIX标准定义

```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```

## 功能描述

对设备执行控制操作，提供统一的设备配置接口。

## 参数说明

- **fd**: 文件描述符
- **request**: 控制命令
- **...**: 可选参数（通常是指针）

## 返回值

- **0**: 成功
- **-1**: 失败，设置errno

## 常用命令

```c
// 终端控制
#define TCGETS      0x5401  // 获取终端属性
#define TCSETS      0x5402  // 设置终端属性
#define TIOCGWINSZ  0x5413  // 获取窗口大小

// 文件控制
#define FIONREAD    0x541B  // 获取可读字节数
#define FIONBIO     0x5421  // 设置非阻塞模式

// 设备特定
#define GPIO_SET    0x1000  // GPIO设置
#define SPI_MODE    0x2000  // SPI模式
```

## Zephyr适配方案

### 实现策略

```c
int ioctl(int fd, unsigned long request, ...)
{
    if (fd < 0 || fd >= MAX_FDS) {
        errno = EBADF;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];
    if (!entry->in_use) {
        errno = EBADF;
        return -1;
    }

    va_list args;
    va_start(args, request);
    void *argp = va_arg(args, void *);
    va_end(args);

    int ret = 0;

    switch (entry->type) {
    case FD_TYPE_DEVICE:
        ret = device_ioctl(entry->handle.dev, request, argp);
        break;

    case FD_TYPE_FILE:
        ret = file_ioctl(entry, request, argp);
        break;

    case FD_TYPE_SOCKET:
        ret = socket_ioctl(entry->handle.sockfd, request, argp);
        break;

    default:
        errno = ENOTTY;
        return -1;
    }

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}
```

### 设备ioctl分发

```c
static int device_ioctl(const struct device *dev,
                       unsigned long request, void *argp)
{
    // UART设备
    if (device_is_uart(dev)) {
        return uart_ioctl(dev, request, argp);
    }

    // GPIO设备
    if (device_is_gpio(dev)) {
        return gpio_ioctl(dev, request, argp);
    }

    // SPI设备
    if (device_is_spi(dev)) {
        return spi_ioctl(dev, request, argp);
    }

    errno = ENOTTY;
    return -1;
}
```

### UART ioctl示例

```c
static int uart_ioctl(const struct device *dev,
                     unsigned long request, void *argp)
{
    switch (request) {
    case TCGETS: {
        struct termios *term = (struct termios *)argp;
        // 获取UART配置
        return uart_get_config(dev, term);
    }

    case TCSETS: {
        struct termios *term = (struct termios *)argp;
        // 设置UART配置
        return uart_set_config(dev, term);
    }

    case FIONREAD: {
        int *count = (int *)argp;
        // 获取可读字节数
        *count = uart_fifo_read_available(dev);
        return 0;
    }

    default:
        errno = EINVAL;
        return -1;
    }
}
```

## 测试用例

### 测试1：获取可读字节数

```c
void test_fionread(void)
{
    int fd = open("/dev/uart0", O_RDWR);
    assert(fd >= 0);

    int count;
    int ret = ioctl(fd, FIONREAD, &count);
    assert(ret == 0);
    printf("Available bytes: %d\n", count);

    close(fd);
}
```

### 测试2：设置非阻塞模式

```c
void test_nonblock(void)
{
    int fd = open("/dev/uart0", O_RDWR);

    int flags = 1;
    int ret = ioctl(fd, FIONBIO, &flags);
    assert(ret == 0);

    close(fd);
}
```

## 注意事项

1. **命令编号**: 不同系统可能不同
2. **参数类型**: 根据命令确定参数类型
3. **设备支持**: 不是所有设备都支持所有命令
4. **线程安全**: ioctl操作需要加锁保护
