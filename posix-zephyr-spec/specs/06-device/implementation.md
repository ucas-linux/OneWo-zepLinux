# 实现指导

## 总体架构

### 模块组织

```
posix_device/
├── fd_table.c            # 文件描述符表管理
├── open.c                # open实现
├── close.c               # close实现
├── read_write.c          # read/write实现
├── ioctl.c               # ioctl实现
└── device_internal.h     # 内部头文件
```

## 核心实现

### fd表初始化

```c
void fd_table_init(void)
{
    k_mutex_init(&g_fd_table_lock);

    for (int i = 0; i < MAX_FDS; i++) {
        g_fd_table[i].in_use = false;
        g_fd_table[i].type = FD_TYPE_NONE;
        k_mutex_init(&g_fd_table[i].lock);
    }

    // 初始化标准fd
    init_stdio_fds();
}

SYS_INIT(fd_table_init, POST_KERNEL, CONFIG_FD_INIT_PRIORITY);
```

## 配置选项

```kconfig
config POSIX_MAX_FDS
    int "Maximum file descriptors"
    default 64

config POSIX_DEVICE_OPEN
    bool "Support /dev/ devices"
    default y

config FD_INIT_PRIORITY
    int "FD table init priority"
    default 45
```

## 调试支持

```c
void fd_table_dump(void)
{
    printf("=== FD Table ===\n");
    for (int i = 0; i < MAX_FDS; i++) {
        if (g_fd_table[i].in_use) {
            printf("fd %d: type=%d flags=0x%x\n",
                   i, g_fd_table[i].type, g_fd_table[i].flags);
        }
    }
}
```

## 测试框架

```c
void test_open_close(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
    zassert_true(fd >= 0, "open failed");

    int ret = close(fd);
    zassert_equal(ret, 0, "close failed");
}
```

## 常见问题

### Q: 如何增加最大fd数量？
A: 修改CONFIG_POSIX_MAX_FDS配置

### Q: 如何支持新设备类型？
A: 在device_ioctl中添加新的设备类型分支

### Q: 标准fd可以关闭吗？
A: 不建议关闭stdin/stdout/stderr
