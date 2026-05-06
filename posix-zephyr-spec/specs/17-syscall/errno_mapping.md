# 错误码映射

## Linux vs Zephyr错误码

Linux和Zephyr使用不同的错误码定义。系统调用层需要在两者之间转换。

## Linux错误码（标准POSIX）

```c
// Linux errno.h
#define EPERM           1   /* Operation not permitted */
#define ENOENT          2   /* No such file or directory */
#define ESRCH           3   /* No such process */
#define EINTR           4   /* Interrupted system call */
#define EIO             5   /* I/O error */
#define ENXIO           6   /* No such device or address */
#define E2BIG           7   /* Argument list too long */
#define ENOEXEC         8   /* Exec format error */
#define EBADF           9   /* Bad file number */
#define ECHILD          10  /* No child processes */
#define EAGAIN          11  /* Try again */
#define ENOMEM          12  /* Out of memory */
#define EACCES          13  /* Permission denied */
#define EFAULT          14  /* Bad address */
#define ENOTBLK         15  /* Block device required */
#define EBUSY           16  /* Device or resource busy */
#define EEXIST          17  /* File exists */
#define EXDEV           18  /* Cross-device link */
#define ENODEV          19  /* No such device */
#define ENOTDIR         20  /* Not a directory */
#define EISDIR          21  /* Is a directory */
#define EINVAL          22  /* Invalid argument */
#define ENFILE          23  /* File table overflow */
#define EMFILE          24  /* Too many open files */
#define ENOTTY          25  /* Not a typewriter */
#define ETXTBSY         26  /* Text file busy */
#define EFBIG           27  /* File too large */
#define ENOSPC          28  /* No space left on device */
#define ESPIPE          29  /* Illegal seek */
#define EROFS           30  /* Read-only file system */
#define EMLINK          31  /* Too many links */
#define EPIPE           32  /* Broken pipe */
#define EDOM            33  /* Math argument out of domain */
#define ERANGE          34  /* Math result not representable */
// ... 更多
```

## Zephyr错误码

```c
// Zephyr errno.h
#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
// ... 大部分与Linux相同
```

## 错误码转换表

大多数情况下，Linux和Zephyr的错误码值相同，但需要处理特殊情况：

```c
// 错误码映射表
static const struct {
    int linux_errno;
    int zephyr_errno;
} errno_map[] = {
    // 大部分直接映射
    { EPERM,    EPERM },
    { ENOENT,   ENOENT },
    { ESRCH,    ESRCH },
    { EINTR,    EINTR },
    { EIO,      EIO },
    { ENXIO,    ENXIO },
    { E2BIG,    E2BIG },
    { ENOEXEC,  ENOEXEC },
    { EBADF,    EBADF },
    { ECHILD,   ECHILD },
    { EAGAIN,   EAGAIN },
    { ENOMEM,   ENOMEM },
    { EACCES,   EACCES },
    { EFAULT,   EFAULT },
    { EBUSY,    EBUSY },
    { EEXIST,   EEXIST },
    { EXDEV,    EXDEV },
    { ENODEV,   ENODEV },
    { ENOTDIR,  ENOTDIR },
    { EISDIR,   EISDIR },
    { EINVAL,   EINVAL },
    { ENFILE,   ENFILE },
    { EMFILE,   EMFILE },
    { ENOTTY,   ENOTTY },
    { EFBIG,    EFBIG },
    { ENOSPC,   ENOSPC },
    { ESPIPE,   ESPIPE },
    { EROFS,    EROFS },
    { EMLINK,   EMLINK },
    { EPIPE,    EPIPE },
    { EDOM,     EDOM },
    { ERANGE,   ERANGE },
    { ENOSYS,   ENOSYS },
    { ETIMEDOUT,ETIMEDOUT },
    { 0, 0 }  // 结束标记
};
```

## 转换函数

### Zephyr错误码 → Linux错误码

```c
// 将Zephyr错误码转换为Linux错误码
static int zephyr_to_linux_errno(int zephyr_err)
{
    // 大多数情况下值相同
    if (zephyr_err >= 0) {
        return zephyr_err;
    }

    // 负值：取绝对值
    int err = -zephyr_err;

    // 查找映射表
    for (int i = 0; errno_map[i].linux_errno != 0; i++) {
        if (errno_map[i].zephyr_errno == err) {
            return errno_map[i].linux_errno;
        }
    }

    // 未找到映射，返回原值
    return err;
}
```

### Linux错误码 → Zephyr错误码

```c
// 将Linux错误码转换为Zephyr错误码
static int linux_to_zephyr_errno(int linux_err)
{
    // 大多数情况下值相同
    if (linux_err >= 0) {
        return linux_err;
    }

    // 负值：取绝对值
    int err = -linux_err;

    // 查找映射表
    for (int i = 0; errno_map[i].linux_errno != 0; i++) {
        if (errno_map[i].linux_errno == err) {
            return errno_map[i].zephyr_errno;
        }
    }

    // 未找到映射，返回原值
    return err;
}
```

## 系统调用返回值处理

### 从POSIX函数返回

```c
// POSIX函数通常返回-1并设置errno
int posix_function(void)
{
    // ... 操作失败
    errno = ENOENT;
    return -1;
}

// 系统调用包装器需要转换为负errno
static int32_t sys_wrapper(uint32_t *args)
{
    int ret = posix_function();

    if (ret < 0) {
        // 返回负errno给Linux应用
        return -errno;
    }

    return ret;
}
```

### 从Zephyr API返回

```c
// Zephyr API通常直接返回负错误码
int zephyr_function(void)
{
    // ... 操作失败
    return -ENOMEM;
}

// 系统调用包装器直接返回
static int32_t sys_wrapper(uint32_t *args)
{
    int ret = zephyr_function();

    // Zephyr已经返回负errno，直接传递
    return ret;
}
```

## 特殊错误码处理

### EWOULDBLOCK vs EAGAIN

```c
// Linux中EWOULDBLOCK == EAGAIN
#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

// 统一处理
static int normalize_errno(int err)
{
    if (err == EWOULDBLOCK) {
        return EAGAIN;
    }
    return err;
}
```

### ERESTARTSYS等内核内部错误码

```c
// 这些错误码不应该暴露给用户空间
#define ERESTARTSYS     512
#define ERESTARTNOINTR  513
#define ERESTARTNOHAND  514

static int filter_kernel_errno(int err)
{
    switch (err) {
    case ERESTARTSYS:
    case ERESTARTNOINTR:
    case ERESTARTNOHAND:
        // 转换为EINTR
        return EINTR;
    default:
        return err;
    }
}
```

## 错误字符串

```c
// 错误码描述
static const char *errno_strings[] = {
    [0]         = "Success",
    [EPERM]     = "Operation not permitted",
    [ENOENT]    = "No such file or directory",
    [ESRCH]     = "No such process",
    [EINTR]     = "Interrupted system call",
    [EIO]       = "I/O error",
    [ENXIO]     = "No such device or address",
    [E2BIG]     = "Argument list too long",
    [ENOEXEC]   = "Exec format error",
    [EBADF]     = "Bad file descriptor",
    [ECHILD]    = "No child processes",
    [EAGAIN]    = "Resource temporarily unavailable",
    [ENOMEM]    = "Out of memory",
    [EACCES]    = "Permission denied",
    [EFAULT]    = "Bad address",
    [EBUSY]     = "Device or resource busy",
    [EEXIST]    = "File exists",
    [EXDEV]     = "Cross-device link",
    [ENODEV]    = "No such device",
    [ENOTDIR]   = "Not a directory",
    [EISDIR]    = "Is a directory",
    [EINVAL]    = "Invalid argument",
    [ENFILE]    = "File table overflow",
    [EMFILE]    = "Too many open files",
    [ENOTTY]    = "Inappropriate ioctl for device",
    [EFBIG]     = "File too large",
    [ENOSPC]    = "No space left on device",
    [ESPIPE]    = "Illegal seek",
    [EROFS]     = "Read-only file system",
    [EMLINK]    = "Too many links",
    [EPIPE]     = "Broken pipe",
    [EDOM]      = "Math argument out of domain",
    [ERANGE]    = "Math result not representable",
    [ENOSYS]    = "Function not implemented",
    [ETIMEDOUT] = "Connection timed out",
};

const char *strerror(int errnum)
{
    if (errnum < 0) {
        errnum = -errnum;
    }

    if (errnum < ARRAY_SIZE(errno_strings) && errno_strings[errnum]) {
        return errno_strings[errnum];
    }

    return "Unknown error";
}
```

## 调试支持

```c
#ifdef CONFIG_SYSCALL_DEBUG

void syscall_print_errno(int err)
{
    if (err >= 0) {
        printk("Success\n");
        return;
    }

    int errnum = -err;
    printk("Error: %s (%d)\n", strerror(errnum), errnum);
}

#endif
```

## 完整示例

```c
// 系统调用包装器完整示例
static int32_t sys_open(uint32_t *args)
{
    const char *pathname = (const char *)args[0];
    int flags = (int)args[1];
    mode_t mode = (mode_t)args[2];
    int ret;

    // 调用POSIX实现
    ret = open(pathname, flags, mode);

    if (ret < 0) {
        // 获取errno
        int err = errno;

        // 转换为Linux错误码（如果需要）
        err = zephyr_to_linux_errno(err);

        // 过滤内核内部错误码
        err = filter_kernel_errno(err);

        // 标准化
        err = normalize_errno(err);

#ifdef CONFIG_SYSCALL_DEBUG
        printk("[SYSCALL] open(\"%s\", 0x%x, 0%o) = %d (%s)\n",
               pathname, flags, mode, -err, strerror(err));
#endif

        // 返回负errno
        return -err;
    }

#ifdef CONFIG_SYSCALL_DEBUG
    printk("[SYSCALL] open(\"%s\", 0x%x, 0%o) = %d\n",
           pathname, flags, mode, ret);
#endif

    return ret;
}
```
