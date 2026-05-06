# 系统调用分发器实现指导

## 总体架构

```
posix_syscall/
├── syscall_dispatcher.c   # 主分发器
├── syscall_table.c         # 系统调用表
├── svc_handler.S           # SVC异常处理（汇编）
├── syscall_wrappers.c      # 系统调用包装器
├── errno_convert.c         # 错误码转换
├── syscall_trace.c         # 跟踪和调试
├── syscall_internal.h      # 内部头文件
└── syscall_stats.c         # 统计信息
```

## 初始化

```c
static struct k_mutex g_syscall_lock;

#ifdef CONFIG_SYSCALL_TRACE
static syscall_trace_buffer_t g_trace_buffer;
#endif

#ifdef CONFIG_SYSCALL_STATS
static syscall_stats_t g_syscall_stats[SYSCALL_TABLE_SIZE];
#endif

int syscall_dispatcher_init(void)
{
    // 初始化锁
    k_mutex_init(&g_syscall_lock);

    // 初始化系统调用表
    init_syscall_table();

#ifdef CONFIG_SYSCALL_TRACE
    // 初始化跟踪缓冲区
    k_mutex_init(&g_trace_buffer.lock);
    g_trace_buffer.head = 0;
    g_trace_buffer.tail = 0;
#endif

#ifdef CONFIG_SYSCALL_STATS
    // 清空统计信息
    memset(g_syscall_stats, 0, sizeof(g_syscall_stats));
#endif

    printk("Syscall dispatcher initialized\n");

    return 0;
}

SYS_INIT(syscall_dispatcher_init, POST_KERNEL,
         CONFIG_SYSCALL_DISPATCHER_INIT_PRIORITY);
```

## 完整分发流程

```c
void syscall_dispatch_full(syscall_context_t *ctx, uint32_t syscall_nr)
{
    uint32_t args[CONFIG_SYSCALL_MAX_ARGS];
    int32_t result;
    uint64_t start_time = 0;

    // 提取参数
    args[0] = ctx->r0;
    args[1] = ctx->r1;
    args[2] = ctx->r2;
    args[3] = ctx->r3;
    args[4] = ctx->r4;
    args[5] = ctx->r5;
    args[6] = ctx->r6;

#ifdef CONFIG_SYSCALL_STATS
    start_time = k_uptime_get();
#endif

#ifdef CONFIG_SYSCALL_TRACE
    syscall_trace_enter(syscall_nr, args);
#endif

    // 验证系统调用号
    if (syscall_nr >= SYSCALL_TABLE_SIZE ||
        !syscall_table[syscall_nr].handler) {
        result = -ENOSYS;
        goto out;
    }

    // 调用处理函数
    result = syscall_table[syscall_nr].handler(args);

out:
#ifdef CONFIG_SYSCALL_TRACE
    syscall_trace_exit(syscall_nr, result);
#endif

#ifdef CONFIG_SYSCALL_STATS
    uint64_t elapsed = k_uptime_get() - start_time;
    syscall_stats_update(syscall_nr, elapsed);
#endif

    // 设置返回值
    ctx->r0 = result;
}
```

## 系统调用包装器示例

```c
// read系统调用
static int32_t sys_read(uint32_t *args)
{
    int fd = (int)args[0];
    void *buf = (void *)args[1];
    size_t count = (size_t)args[2];

    // 参数验证
    if (!buf) {
        return -EFAULT;
    }

    if (count == 0) {
        return 0;
    }

    // 调用POSIX实现
    ssize_t ret = read(fd, buf, count);

    if (ret < 0) {
        return -errno;
    }

    return ret;
}

// write系统调用
static int32_t sys_write(uint32_t *args)
{
    int fd = (int)args[0];
    const void *buf = (const void *)args[1];
    size_t count = (size_t)args[2];

    if (!buf) {
        return -EFAULT;
    }

    if (count == 0) {
        return 0;
    }

    ssize_t ret = write(fd, buf, count);

    if (ret < 0) {
        return -errno;
    }

    return ret;
}

// open系统调用
static int32_t sys_open(uint32_t *args)
{
    const char *pathname = (const char *)args[0];
    int flags = (int)args[1];
    mode_t mode = (mode_t)args[2];

    if (!pathname) {
        return -EFAULT;
    }

    int ret = open(pathname, flags, mode);

    if (ret < 0) {
        return -errno;
    }

    return ret;
}

// close系统调用
static int32_t sys_close(uint32_t *args)
{
    int fd = (int)args[0];

    int ret = close(fd);

    if (ret < 0) {
        return -errno;
    }

    return ret;
}

// getpid系统调用
static int32_t sys_getpid(uint32_t *args)
{
    return getpid();
}

// getppid系统调用
static int32_t sys_getppid(uint32_t *args)
{
    return getppid();
}

// fork系统调用（不支持）
static int32_t sys_fork(uint32_t *args)
{
    return -ENOSYS;
}

// execve系统调用（不支持）
static int32_t sys_execve(uint32_t *args)
{
    return -ENOSYS;
}
```

## 配置选项

```kconfig
menuconfig SYSCALL_DISPATCHER
    bool "Linux syscall dispatcher"
    depends on ELF_LOADER
    default n
    help
      Enable Linux system call interception and dispatching.

if SYSCALL_DISPATCHER

config SYSCALL_TRACE
    bool "Enable syscall tracing"
    default n
    help
      Log all system calls for debugging.

config SYSCALL_STATS
    bool "Enable syscall statistics"
    default n
    help
      Track system call performance statistics.

config SYSCALL_MAX_ARGS
    int "Maximum syscall arguments"
    default 6
    range 4 7
    help
      Maximum number of arguments for system calls.

config SYSCALL_DISPATCHER_INIT_PRIORITY
    int "Syscall dispatcher initialization priority"
    default 65
    help
      Initialization priority for syscall dispatcher.

config SYSCALL_FAST_PATH
    bool "Enable fast path for common syscalls"
    default y
    help
      Optimize frequently used system calls.

endif # SYSCALL_DISPATCHER
```

## 调试宏

```c
#ifdef CONFIG_SYSCALL_DEBUG
#define SYSCALL_DEBUG(fmt, ...) printk("[SYSCALL] " fmt, ##__VA_ARGS__)
#else
#define SYSCALL_DEBUG(fmt, ...)
#endif

#define SYSCALL_ERROR(fmt, ...) printk("[SYSCALL ERROR] " fmt, ##__VA_ARGS__)
```

## 测试框架

```c
#ifdef CONFIG_SYSCALL_TESTS

void test_syscall_getpid(void)
{
    uint32_t args[7] = {0};
    int32_t ret = sys_getpid(args);
    zassert_true(ret > 0, "getpid failed");
}

void test_syscall_read_write(void)
{
    char buf[64];
    uint32_t args[7];

    // 测试write
    args[0] = 1;  // stdout
    args[1] = (uint32_t)"test";
    args[2] = 4;
    int32_t ret = sys_write(args);
    zassert_equal(ret, 4, "write failed");
}

void test_syscall_invalid(void)
{
    uint32_t args[7] = {0};
    int32_t ret = syscall_invoke(9999, args);
    zassert_equal(ret, -ENOSYS, "invalid syscall should return ENOSYS");
}

ZTEST_SUITE(syscall_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(syscall_tests, test_getpid) { test_syscall_getpid(); }
ZTEST(syscall_tests, test_rw) { test_syscall_read_write(); }
ZTEST(syscall_tests, test_invalid) { test_syscall_invalid(); }

#endif
```

## 性能优化

### 1. 快速路径

```c
#ifdef CONFIG_SYSCALL_FAST_PATH

// 常用系统调用的快速路径
static inline int32_t syscall_fast_path(uint32_t nr, uint32_t *args)
{
    switch (nr) {
    case __NR_getpid:
        return getpid();

    case __NR_gettid:
        return pthread_self();

    case __NR_read:
        if (args[0] < 3) {  // stdin/stdout/stderr
            return sys_read(args);
        }
        break;

    case __NR_write:
        if (args[0] < 3) {
            return sys_write(args);
        }
        break;
    }

    return INT32_MIN;  // 使用慢速路径
}

void syscall_dispatch_full(syscall_context_t *ctx, uint32_t syscall_nr)
{
    uint32_t args[7] = {
        ctx->r0, ctx->r1, ctx->r2, ctx->r3,
        ctx->r4, ctx->r5, ctx->r6
    };

    // 尝试快速路径
    int32_t result = syscall_fast_path(syscall_nr, args);
    if (result != INT32_MIN) {
        ctx->r0 = result;
        return;
    }

    // 慢速路径
    // ... 完整处理
}

#endif
```

### 2. 系统调用缓存

```c
// 缓存最近的系统调用结果（适用于幂等操作）
#define SYSCALL_CACHE_SIZE 4

static struct {
    uint32_t nr;
    uint32_t args_hash;
    int32_t result;
    bool valid;
} g_syscall_cache[SYSCALL_CACHE_SIZE];

static uint32_t hash_args(uint32_t *args, int num_args)
{
    uint32_t hash = 0;
    for (int i = 0; i < num_args; i++) {
        hash ^= args[i];
        hash = (hash << 5) | (hash >> 27);
    }
    return hash;
}
```

## 错误处理

```c
// 处理系统调用异常
static int32_t handle_syscall_exception(uint32_t nr, int32_t error)
{
    switch (error) {
    case -EFAULT:
        SYSCALL_ERROR("Bad address in syscall %u\n", nr);
        break;

    case -EINVAL:
        SYSCALL_ERROR("Invalid argument in syscall %u\n", nr);
        break;

    case -ENOSYS:
        SYSCALL_ERROR("Syscall %u not implemented\n", nr);
        break;

    default:
        if (error < 0) {
            SYSCALL_DEBUG("Syscall %u failed: %d\n", nr, error);
        }
        break;
    }

    return error;
}
```

## 与ELF加载器集成

```c
// 在加载ELF时设置系统调用处理
int elf_setup_syscall_handler(elf_handle_t *handle)
{
    // 确保SVC处理器已安装
    if (!is_svc_handler_installed()) {
        return -ENOSYS;
    }

    // 配置进程的系统调用上下文
    // ...

    return 0;
}
```

## 常见问题

### Q: 系统调用返回ENOSYS
A: 检查系统调用号是否正确，确保对应的POSIX函数已实现

### Q: 参数传递错误
A: 验证ARM EABI调用约定，确保r0-r6正确传递参数

### Q: 性能问题
A: 启用CONFIG_SYSCALL_FAST_PATH，使用快速路径优化

### Q: 如何调试系统调用
A: 启用CONFIG_SYSCALL_TRACE查看所有系统调用

## 最佳实践

1. **参数验证**：始终验证用户空间指针
2. **错误处理**：正确转换errno
3. **性能**：对频繁调用的系统调用使用快速路径
4. **调试**：使用跟踪功能定位问题
5. **测试**：为每个系统调用编写测试用例
