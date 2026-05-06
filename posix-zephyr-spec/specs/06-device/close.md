# close - 关闭文件描述符

## POSIX标准定义

```c
#include <unistd.h>

int close(int fd);
```

## 功能描述

关闭文件描述符，释放相关资源。

## 参数说明

- **fd**: 要关闭的文件描述符

## 返回值

- **0**: 成功
- **-1**: 失败，设置errno

## 错误码

- **EBADF**: 无效的文件描述符
- **EINTR**: 被信号中断
- **EIO**: I/O错误

## Zephyr适配方案

### 实现策略

```c
int close(int fd)
{
    // 验证fd
    if (fd < 0 || fd >= MAX_FDS) {
        errno = EBADF;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];

    k_mutex_lock(&entry->lock, K_FOREVER);

    if (!entry->in_use) {
        k_mutex_unlock(&entry->lock);
        errno = EBADF;
        return -1;
    }

    // 根据类型关闭
    int ret = 0;
    switch (entry->type) {
    case FD_TYPE_FILE:
        ret = fs_close(entry->handle.file);
        k_free(entry->handle.file);
        break;

    case FD_TYPE_DEVICE:
        // 设备通常不需要显式关闭
        break;

    case FD_TYPE_SOCKET:
        ret = zsock_close(entry->handle.sockfd);
        break;

    case FD_TYPE_PIPE:
        // 管道关闭
        break;

    default:
        break;
    }

    // 清除fd表项
    entry->in_use = false;
    entry->type = FD_TYPE_NONE;
    entry->handle.file = NULL;

    k_mutex_unlock(&entry->lock);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}
```

### 标准fd处理

```c
// 不允许关闭标准fd
if (fd <= 2) {
    errno = EBADF;
    return -1;
}
```

## 测试用例

### 测试1：基本关闭

```c
void test_basic_close(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
    assert(fd >= 0);

    int ret = close(fd);
    assert(ret == 0);

    // 再次关闭应该失败
    ret = close(fd);
    assert(ret == -1);
    assert(errno == EBADF);
}
```

### 测试2：关闭无效fd

```c
void test_close_invalid(void)
{
    int ret = close(999);
    assert(ret == -1);
    assert(errno == EBADF);

    ret = close(-1);
    assert(ret == -1);
    assert(errno == EBADF);
}
```

## 注意事项

1. **不要关闭标准fd**: stdin/stdout/stderr
2. **只关闭一次**: 重复关闭是错误
3. **检查返回值**: 某些情况下close可能失败
4. **资源释放**: close后fd可被重用
5. **并发安全**: 多线程不应同时操作同一fd
