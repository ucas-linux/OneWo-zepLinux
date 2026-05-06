# 高级文件操作

## 概述

高级文件操作提供了超越基本read/write的文件控制能力，包括文件描述符操作、文件锁、文件控制等。

## 目标

- 实现fcntl()文件控制
- 实现dup/dup2文件描述符复制
- 实现flock()文件锁
- 实现truncate/ftruncate文件截断
- 实现sendfile()零拷贝传输

## fcntl() - 文件控制

```c
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* arg */);

// 常用命令
#define F_DUPFD         0   // 复制文件描述符
#define F_GETFD         1   // 获取fd标志
#define F_SETFD         2   // 设置fd标志
#define F_GETFL         3   // 获取文件状态标志
#define F_SETFL         4   // 设置文件状态标志
#define F_GETLK         5   // 获取锁信息
#define F_SETLK         6   // 设置锁（非阻塞）
#define F_SETLKW        7   // 设置锁（阻塞）

// fd标志
#define FD_CLOEXEC      1   // 执行时关闭

// 文件状态标志
#define O_APPEND        0x0400
#define O_NONBLOCK      0x0800
#define O_SYNC          0x1000
```

### 实现

```c
int fcntl(int fd, int cmd, ...)
{
    va_list ap;
    int ret = 0;

    // 验证fd
    if (fd < 0 || fd >= MAX_FDS || !g_fd_table[fd].in_use) {
        errno = EBADF;
        return -1;
    }

    va_start(ap, cmd);

    switch (cmd) {
    case F_DUPFD: {
        int minfd = va_arg(ap, int);
        ret = dup_fd_from(fd, minfd);
        break;
    }

    case F_GETFD:
        ret = g_fd_table[fd].flags & FD_CLOEXEC;
        break;

    case F_SETFD: {
        int flags = va_arg(ap, int);
        if (flags & FD_CLOEXEC) {
            g_fd_table[fd].flags |= FD_CLOEXEC;
        } else {
            g_fd_table[fd].flags &= ~FD_CLOEXEC;
        }
        break;
    }

    case F_GETFL:
        ret = g_fd_table[fd].status_flags;
        break;

    case F_SETFL: {
        int flags = va_arg(ap, int);
        // 只允许修改某些标志
        g_fd_table[fd].status_flags &= ~(O_APPEND | O_NONBLOCK | O_SYNC);
        g_fd_table[fd].status_flags |= (flags & (O_APPEND | O_NONBLOCK | O_SYNC));
        break;
    }

    case F_GETLK:
    case F_SETLK:
    case F_SETLKW: {
        struct flock *lock = va_arg(ap, struct flock *);
        ret = handle_file_lock(fd, cmd, lock);
        break;
    }

    default:
        errno = EINVAL;
        ret = -1;
        break;
    }

    va_end(ap);
    return ret;
}
```

## dup/dup2 - 文件描述符复制

```c
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
```

### 实现

```c
int dup(int oldfd)
{
    // 验证oldfd
    if (oldfd < 0 || oldfd >= MAX_FDS || !g_fd_table[oldfd].in_use) {
        errno = EBADF;
        return -1;
    }

    // 查找最小可用fd
    for (int newfd = 0; newfd < MAX_FDS; newfd++) {
        if (!g_fd_table[newfd].in_use) {
            return dup_fd_internal(oldfd, newfd);
        }
    }

    errno = EMFILE;
    return -1;
}

int dup2(int oldfd, int newfd)
{
    // 验证oldfd
    if (oldfd < 0 || oldfd >= MAX_FDS || !g_fd_table[oldfd].in_use) {
        errno = EBADF;
        return -1;
    }

    // 验证newfd
    if (newfd < 0 || newfd >= MAX_FDS) {
        errno = EBADF;
        return -1;
    }

    // 如果相同，直接返回
    if (oldfd == newfd) {
        return newfd;
    }

    // 关闭newfd（如果打开）
    if (g_fd_table[newfd].in_use) {
        close(newfd);
    }

    return dup_fd_internal(oldfd, newfd);
}

static int dup_fd_internal(int oldfd, int newfd)
{
    // 复制fd表项
    g_fd_table[newfd] = g_fd_table[oldfd];
    g_fd_table[newfd].in_use = true;

    // 增加引用计数
    if (g_fd_table[oldfd].refcount) {
        (*g_fd_table[oldfd].refcount)++;
    }

    return newfd;
}
```

## flock() - 文件锁

```c
#include <sys/file.h>

int flock(int fd, int operation);

// 操作类型
#define LOCK_SH  1  // 共享锁
#define LOCK_EX  2  // 排他锁
#define LOCK_UN  8  // 解锁
#define LOCK_NB  4  // 非阻塞
```

### 实现

```c
typedef struct file_lock {
    int fd;
    int type;  // LOCK_SH or LOCK_EX
    pid_t owner;
    struct file_lock *next;
} file_lock_t;

static file_lock_t *g_file_locks = NULL;
static struct k_mutex g_lock_mutex;

int flock(int fd, int operation)
{
    if (fd < 0 || fd >= MAX_FDS || !g_fd_table[fd].in_use) {
        errno = EBADF;
        return -1;
    }

    int lock_type = operation & ~LOCK_NB;
    bool nonblock = (operation & LOCK_NB) != 0;

    k_mutex_lock(&g_lock_mutex, K_FOREVER);

    if (lock_type == LOCK_UN) {
        // 解锁
        remove_file_lock(fd);
        k_mutex_unlock(&g_lock_mutex);
        return 0;
    }

    // 检查冲突
    file_lock_t *lock = find_file_lock(fd);
    if (lock) {
        if (lock->type == LOCK_EX || lock_type == LOCK_EX) {
            // 有冲突
            if (nonblock) {
                k_mutex_unlock(&g_lock_mutex);
                errno = EWOULDBLOCK;
                return -1;
            }
            // 阻塞等待（简化实现）
            k_mutex_unlock(&g_lock_mutex);
            k_msleep(100);
            return flock(fd, operation);
        }
    }

    // 添加锁
    lock = k_malloc(sizeof(file_lock_t));
    lock->fd = fd;
    lock->type = lock_type;
    lock->owner = getpid();
    lock->next = g_file_locks;
    g_file_locks = lock;

    k_mutex_unlock(&g_lock_mutex);
    return 0;
}
```

## truncate/ftruncate - 文件截断

```c
#include <unistd.h>

int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);
```

### 实现

```c
int ftruncate(int fd, off_t length)
{
    if (fd < 0 || fd >= MAX_FDS || !g_fd_table[fd].in_use) {
        errno = EBADF;
        return -1;
    }

    if (length < 0) {
        errno = EINVAL;
        return -1;
    }

    // 调用底层文件系统
    struct fs_file_t *file = g_fd_table[fd].handle.file;
    int ret = fs_truncate(file, length);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

int truncate(const char *path, off_t length)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    int ret = ftruncate(fd, length);
    close(fd);

    return ret;
}
```

## 使用示例

```c
// 示例1：设置非阻塞模式
int fd = open("/dev/uart0", O_RDWR);
int flags = fcntl(fd, F_GETFL);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);

// 示例2：复制文件描述符
int fd2 = dup(fd);
int fd3 = dup2(fd, 10);

// 示例3：文件锁
int fd = open("file.txt", O_RDWR);
flock(fd, LOCK_EX);  // 排他锁
// ... 操作文件
flock(fd, LOCK_UN);  // 解锁

// 示例4：截断文件
ftruncate(fd, 1024);  // 截断到1KB
```

## 配置选项

```kconfig
config POSIX_ADVANCED_FILE
    bool "Advanced file operations"
    default n

config POSIX_FCNTL
    bool "fcntl() support"
    depends on POSIX_ADVANCED_FILE
    default y

config POSIX_FILE_LOCK
    bool "File locking support"
    depends on POSIX_ADVANCED_FILE
    default n
```
