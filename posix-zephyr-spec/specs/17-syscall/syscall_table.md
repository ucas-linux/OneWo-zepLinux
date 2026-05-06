# 系统调用表

## 系统调用表结构

```c
typedef int32_t (*syscall_handler_t)(uint32_t *args);

typedef struct {
    uint32_t nr;                // 系统调用号
    syscall_handler_t handler;  // 处理函数
    const char *name;           // 名称（调试用）
    uint8_t num_args;           // 参数数量
} syscall_entry_t;

// 系统调用表
static const syscall_entry_t syscall_table[] = {
    [__NR_exit]         = { __NR_exit,         sys_exit,         "exit",         1 },
    [__NR_fork]         = { __NR_fork,         sys_fork,         "fork",         0 },
    [__NR_read]         = { __NR_read,         sys_read,         "read",         3 },
    [__NR_write]        = { __NR_write,        sys_write,        "write",        3 },
    [__NR_open]         = { __NR_open,         sys_open,         "open",         3 },
    [__NR_close]        = { __NR_close,        sys_close,        "close",        1 },
    [__NR_waitpid]      = { __NR_waitpid,      sys_waitpid,      "waitpid",      3 },
    [__NR_execve]       = { __NR_execve,       sys_execve,       "execve",       3 },
    [__NR_getpid]       = { __NR_getpid,       sys_getpid,       "getpid",       0 },
    [__NR_getppid]      = { __NR_getppid,      sys_getppid,      "getppid",      0 },
    // ... 更多系统调用
};

#define SYSCALL_TABLE_SIZE ARRAY_SIZE(syscall_table)
```

## 系统调用分发

```c
int32_t syscall_invoke(uint32_t nr, uint32_t *args)
{
    /* 检查系统调用号有效性 */
    if (nr >= SYSCALL_TABLE_SIZE) {
        return -ENOSYS;
    }

    const syscall_entry_t *entry = &syscall_table[nr];
    if (!entry->handler) {
        return -ENOSYS;
    }

    /* 调用处理函数 */
    return entry->handler(args);
}
```

## 系统调用包装器

每个系统调用需要一个包装器函数，从参数数组提取参数并调用实际实现。

```c
/* 示例：read系统调用 */
static int32_t sys_read(uint32_t *args)
{
    int fd = (int)args[0];
    void *buf = (void *)args[1];
    size_t count = (size_t)args[2];

    ssize_t ret = read(fd, buf, count);

    /* 转换返回值：成功返回字节数，失败返回负errno */
    if (ret < 0) {
        return -errno;
    }
    return ret;
}

/* 示例：write系统调用 */
static int32_t sys_write(uint32_t *args)
{
    int fd = (int)args[0];
    const void *buf = (const void *)args[1];
    size_t count = (size_t)args[2];

    ssize_t ret = write(fd, buf, count);

    if (ret < 0) {
        return -errno;
    }
    return ret;
}

/* 示例：open系统调用 */
static int32_t sys_open(uint32_t *args)
{
    const char *pathname = (const char *)args[0];
    int flags = (int)args[1];
    mode_t mode = (mode_t)args[2];

    int ret = open(pathname, flags, mode);

    if (ret < 0) {
        return -errno;
    }
    return ret;
}
```

## 完整系统调用列表

### 进程管理

```c
[__NR_exit]         = { __NR_exit,         sys_exit,         "exit",         1 },
[__NR_fork]         = { __NR_fork,         sys_fork,         "fork",         0 },
[__NR_execve]       = { __NR_execve,       sys_execve,       "execve",       3 },
[__NR_waitpid]      = { __NR_waitpid,      sys_waitpid,      "waitpid",      3 },
[__NR_getpid]       = { __NR_getpid,       sys_getpid,       "getpid",       0 },
[__NR_getppid]      = { __NR_getppid,      sys_getppid,      "getppid",      0 },
[__NR_kill]         = { __NR_kill,         sys_kill,         "kill",         2 },
[__NR_clone]        = { __NR_clone,        sys_clone,        "clone",        5 },
```

### 文件I/O

```c
[__NR_read]         = { __NR_read,         sys_read,         "read",         3 },
[__NR_write]        = { __NR_write,        sys_write,        "write",        3 },
[__NR_open]         = { __NR_open,         sys_open,         "open",         3 },
[__NR_close]        = { __NR_close,        sys_close,        "close",        1 },
[__NR_lseek]        = { __NR_lseek,        sys_lseek,        "lseek",        3 },
[__NR_dup]          = { __NR_dup,          sys_dup,          "dup",          1 },
[__NR_dup2]         = { __NR_dup2,         sys_dup2,         "dup2",         2 },
[__NR_fcntl]        = { __NR_fcntl,        sys_fcntl,        "fcntl",        3 },
[__NR_ioctl]        = { __NR_ioctl,        sys_ioctl,        "ioctl",        3 },
```

### 文件系统

```c
[__NR_stat]         = { __NR_stat,         sys_stat,         "stat",         2 },
[__NR_fstat]        = { __NR_fstat,        sys_fstat,        "fstat",        2 },
[__NR_lstat]        = { __NR_lstat,        sys_lstat,        "lstat",        2 },
[__NR_mkdir]        = { __NR_mkdir,        sys_mkdir,        "mkdir",        2 },
[__NR_rmdir]        = { __NR_rmdir,        sys_rmdir,        "rmdir",        1 },
[__NR_unlink]       = { __NR_unlink,       sys_unlink,       "unlink",       1 },
[__NR_rename]       = { __NR_rename,       sys_rename,       "rename",       2 },
[__NR_chmod]        = { __NR_chmod,        sys_chmod,        "chmod",        2 },
[__NR_chown]        = { __NR_chown,        sys_chown,        "chown",        3 },
```

### 内存管理

```c
[__NR_brk]          = { __NR_brk,          sys_brk,          "brk",          1 },
[__NR_mmap]         = { __NR_mmap,         sys_mmap,         "mmap",         6 },
[__NR_munmap]       = { __NR_munmap,       sys_munmap,       "munmap",       2 },
[__NR_mprotect]     = { __NR_mprotect,     sys_mprotect,     "mprotect",     3 },
```

### 时间

```c
[__NR_time]         = { __NR_time,         sys_time,         "time",         1 },
[__NR_gettimeofday] = { __NR_gettimeofday, sys_gettimeofday, "gettimeofday", 2 },
[__NR_clock_gettime]= { __NR_clock_gettime,sys_clock_gettime,"clock_gettime",2 },
[__NR_nanosleep]    = { __NR_nanosleep,    sys_nanosleep,    "nanosleep",    2 },
```

### 信号

```c
[__NR_signal]       = { __NR_signal,       sys_signal,       "signal",       2 },
[__NR_sigaction]    = { __NR_sigaction,    sys_sigaction,    "sigaction",    3 },
[__NR_sigprocmask]  = { __NR_sigprocmask,  sys_sigprocmask,  "sigprocmask",  3 },
[__NR_sigreturn]    = { __NR_sigreturn,    sys_sigreturn,    "sigreturn",    0 },
```

### 网络

```c
[__NR_socket]       = { __NR_socket,       sys_socket,       "socket",       3 },
[__NR_bind]         = { __NR_bind,         sys_bind,         "bind",         3 },
[__NR_connect]      = { __NR_connect,      sys_connect,      "connect",      3 },
[__NR_listen]       = { __NR_listen,       sys_listen,       "listen",       2 },
[__NR_accept]       = { __NR_accept,       sys_accept,       "accept",       3 },
[__NR_send]         = { __NR_send,         sys_send,         "send",         4 },
[__NR_recv]         = { __NR_recv,         sys_recv,         "recv",         4 },
```

### I/O多路复用

```c
[__NR_select]       = { __NR_select,       sys_select,       "select",       5 },
[__NR_poll]         = { __NR_poll,         sys_poll,         "poll",         3 },
[__NR_epoll_create] = { __NR_epoll_create, sys_epoll_create, "epoll_create", 1 },
[__NR_epoll_ctl]    = { __NR_epoll_ctl,    sys_epoll_ctl,    "epoll_ctl",    4 },
[__NR_epoll_wait]   = { __NR_epoll_wait,   sys_epoll_wait,   "epoll_wait",   4 },
```

## 宏生成系统调用表

使用宏简化系统调用表定义：

```c
#define SYSCALL(nr, name, nargs) \
    [__NR_##name] = { __NR_##name, sys_##name, #name, nargs }

static const syscall_entry_t syscall_table[] = {
    SYSCALL(1,   exit,         1),
    SYSCALL(2,   fork,         0),
    SYSCALL(3,   read,         3),
    SYSCALL(4,   write,        3),
    SYSCALL(5,   open,         3),
    SYSCALL(6,   close,        1),
    SYSCALL(7,   waitpid,      3),
    SYSCALL(11,  execve,       3),
    SYSCALL(20,  getpid,       0),
    SYSCALL(37,  kill,         2),
    SYSCALL(45,  brk,          1),
    SYSCALL(78,  gettimeofday, 2),
    SYSCALL(90,  mmap,         6),
    SYSCALL(91,  munmap,       2),
    SYSCALL(120, clone,        5),
    SYSCALL(224, gettid,       0),
    SYSCALL(263, clock_gettime,2),
    // ...
};
```

## 未实现的系统调用

对于未实现的系统调用，返回ENOSYS：

```c
static int32_t sys_not_implemented(uint32_t *args)
{
    return -ENOSYS;
}

/* 填充未实现的系统调用 */
static void init_syscall_table(void)
{
    for (int i = 0; i < SYSCALL_TABLE_SIZE; i++) {
        if (!syscall_table[i].handler) {
            syscall_table[i].handler = sys_not_implemented;
            syscall_table[i].name = "not_implemented";
        }
    }
}
```

## 系统调用统计

```c
#ifdef CONFIG_SYSCALL_STATS

typedef struct {
    uint64_t count;      // 调用次数
    uint64_t total_time; // 总耗时（微秒）
    uint64_t max_time;   // 最大耗时
} syscall_stats_t;

static syscall_stats_t g_syscall_stats[SYSCALL_TABLE_SIZE];

void syscall_stats_update(uint32_t nr, uint64_t elapsed_us)
{
    if (nr < SYSCALL_TABLE_SIZE) {
        g_syscall_stats[nr].count++;
        g_syscall_stats[nr].total_time += elapsed_us;
        if (elapsed_us > g_syscall_stats[nr].max_time) {
            g_syscall_stats[nr].max_time = elapsed_us;
        }
    }
}

void syscall_stats_print(void)
{
    printk("=== Syscall Statistics ===\n");
    printk("%-20s %10s %15s %10s\n", "Name", "Count", "Total(us)", "Max(us)");

    for (int i = 0; i < SYSCALL_TABLE_SIZE; i++) {
        if (g_syscall_stats[i].count > 0) {
            printk("%-20s %10llu %15llu %10llu\n",
                   syscall_table[i].name,
                   g_syscall_stats[i].count,
                   g_syscall_stats[i].total_time,
                   g_syscall_stats[i].max_time);
        }
    }
}

#endif
```
