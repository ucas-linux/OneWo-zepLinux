# open - 打开文件/设备

## POSIX标准定义

```c
#include <fcntl.h>

int open(const char *pathname, int flags, ...);
int openat(int dirfd, const char *pathname, int flags, ...);
int creat(const char *pathname, mode_t mode);
```

## 功能描述

- **open**: 打开文件或设备，返回文件描述符
- **openat**: 相对于目录fd打开文件
- **creat**: 创建文件（等同于open with O_CREAT|O_WRONLY|O_TRUNC）

## 参数说明

- **pathname**: 文件或设备路径
- **flags**: 打开标志（O_RDONLY, O_WRONLY, O_RDWR等）
- **mode**: 文件权限（仅在创建文件时使用）

## 打开标志

```c
// 访问模式（互斥）
#define O_RDONLY    0x0000  // 只读
#define O_WRONLY    0x0001  // 只写
#define O_RDWR      0x0002  // 读写

// 创建标志
#define O_CREAT     0x0040  // 不存在则创建
#define O_EXCL      0x0080  // 与O_CREAT一起使用，文件存在则失败
#define O_TRUNC     0x0200  // 截断为0

// 操作标志
#define O_APPEND    0x0400  // 追加模式
#define O_NONBLOCK  0x0800  // 非阻塞模式
#define O_SYNC      0x1000  // 同步写入
```

## 返回值

- **成功**: 返回文件描述符（非负整数）
- **失败**: 返回-1，设置errno

## Zephyr适配方案

### 核心挑战

1. **文件描述符抽象**: Zephyr使用设备指针，需要映射到整数fd
2. **路径解析**: 区分文件系统路径和设备路径（/dev/）
3. **统一接口**: 不同类型（文件、设备、socket）需要统一的fd接口

### 数据结构

```c
// 文件描述符类型
enum fd_type {
    FD_TYPE_NONE,
    FD_TYPE_FILE,       // 文件系统文件
    FD_TYPE_DEVICE,     // 字符/块设备
    FD_TYPE_SOCKET,     // 网络socket
    FD_TYPE_PIPE,       // 管道
};

// 文件描述符表项
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

// 全局fd表
#define MAX_FDS 64
static fd_entry_t g_fd_table[MAX_FDS];
static struct k_mutex g_fd_table_lock;
```

### 实现步骤

#### 1. 初始化fd表

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
    // 0=stdin, 1=stdout, 2=stderr
    init_stdio_fds();
}
```

#### 2. 分配fd

```c
static int alloc_fd(void)
{
    k_mutex_lock(&g_fd_table_lock, K_FOREVER);

    // 从3开始（0,1,2是标准fd）
    for (int fd = 3; fd < MAX_FDS; fd++) {
        if (!g_fd_table[fd].in_use) {
            g_fd_table[fd].in_use = true;
            k_mutex_unlock(&g_fd_table_lock);
            return fd;
        }
    }

    k_mutex_unlock(&g_fd_table_lock);
    errno = EMFILE;  // 文件描述符耗尽
    return -1;
}
```

#### 3. open实现

```c
int open(const char *pathname, int flags, ...)
{
    if (!pathname) {
        errno = EINVAL;
        return -1;
    }

    // 分配fd
    int fd = alloc_fd();
    if (fd < 0) {
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];
    entry->flags = flags;
    entry->offset = 0;

    // 判断路径类型
    if (strncmp(pathname, "/dev/", 5) == 0) {
        // 设备文件
        if (open_device(fd, pathname + 5, flags) < 0) {
            entry->in_use = false;
            return -1;
        }
    } else {
        // 普通文件
        if (open_file(fd, pathname, flags) < 0) {
            entry->in_use = false;
            return -1;
        }
    }

    return fd;
}
```

#### 4. 打开设备

```c
static int open_device(int fd, const char *devname, int flags)
{
    // 查找设备
    const struct device *dev = device_get_binding(devname);
    if (!dev) {
        errno = ENOENT;
        return -1;
    }

    // 检查设备是否就绪
    if (!device_is_ready(dev)) {
        errno = ENXIO;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];
    entry->type = FD_TYPE_DEVICE;
    entry->handle.dev = dev;

    return 0;
}
```

#### 5. 打开文件

```c
static int open_file(int fd, const char *pathname, int flags)
{
    // 分配文件结构
    struct fs_file_t *file = k_malloc(sizeof(*file));
    if (!file) {
        errno = ENOMEM;
        return -1;
    }

    fs_file_t_init(file);

    // 转换标志
    int fs_flags = 0;
    if (flags & O_RDONLY) fs_flags |= FS_O_READ;
    if (flags & O_WRONLY) fs_flags |= FS_O_WRITE;
    if (flags & O_RDWR) fs_flags |= FS_O_READ | FS_O_WRITE;
    if (flags & O_CREAT) fs_flags |= FS_O_CREATE;
    if (flags & O_APPEND) fs_flags |= FS_O_APPEND;
    if (flags & O_TRUNC) fs_flags |= FS_O_TRUNC;

    // 打开文件
    int ret = fs_open(file, pathname, fs_flags);
    if (ret < 0) {
        k_free(file);
        errno = -ret;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];
    entry->type = FD_TYPE_FILE;
    entry->handle.file = file;

    return 0;
}
```

#### 6. 标准fd初始化

```c
static void init_stdio_fds(void)
{
    // stdin (fd=0)
    g_fd_table[0].in_use = true;
    g_fd_table[0].type = FD_TYPE_DEVICE;
    g_fd_table[0].flags = O_RDONLY;
    g_fd_table[0].handle.dev = device_get_binding("UART_0");

    // stdout (fd=1)
    g_fd_table[1].in_use = true;
    g_fd_table[1].type = FD_TYPE_DEVICE;
    g_fd_table[1].flags = O_WRONLY;
    g_fd_table[1].handle.dev = device_get_binding("UART_0");

    // stderr (fd=2)
    g_fd_table[2].in_use = true;
    g_fd_table[2].type = FD_TYPE_DEVICE;
    g_fd_table[2].flags = O_WRONLY;
    g_fd_table[2].handle.dev = device_get_binding("UART_0");
}
```

## 测试用例

### 测试1：打开文件

```c
void test_open_file(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
    assert(fd >= 0);

    close(fd);
}
```

### 测试2：打开设备

```c
void test_open_device(void)
{
    int fd = open("/dev/uart0", O_RDWR);
    assert(fd >= 0);

    close(fd);
}
```

### 测试3：文件不存在

```c
void test_open_nonexist(void)
{
    int fd = open("/nonexist.txt", O_RDONLY);
    assert(fd == -1);
    assert(errno == ENOENT);
}
```

### 测试4：O_EXCL标志

```c
void test_open_excl(void)
{
    // 创建文件
    int fd1 = open("/tmp/test.txt", O_CREAT | O_EXCL, 0644);
    assert(fd1 >= 0);
    close(fd1);

    // 再次创建应该失败
    int fd2 = open("/tmp/test.txt", O_CREAT | O_EXCL, 0644);
    assert(fd2 == -1);
    assert(errno == EEXIST);
}
```

### 测试5：fd耗尽

```c
void test_fd_exhaustion(void)
{
    int fds[MAX_FDS];
    int count = 0;

    // 打开直到耗尽
    for (int i = 0; i < MAX_FDS; i++) {
        fds[i] = open("/tmp/test.txt", O_RDONLY);
        if (fds[i] < 0) {
            break;
        }
        count++;
    }

    // 应该耗尽
    assert(count < MAX_FDS);
    assert(errno == EMFILE);

    // 清理
    for (int i = 0; i < count; i++) {
        close(fds[i]);
    }
}
```

## 性能考虑

1. **fd分配**: O(n)，n为MAX_FDS
2. **设备查找**: O(1)（device_get_binding使用哈希表）
3. **文件打开**: 取决于文件系统实现

## 注意事项

1. **标准fd**: 0=stdin, 1=stdout, 2=stderr预留
2. **路径格式**: /dev/开头的是设备，其他是文件
3. **权限检查**: 简化实现，不完全支持Unix权限
4. **O_CLOEXEC**: 不支持（无exec）
5. **符号链接**: 取决于文件系统支持

## 限制

1. 最大fd数量受MAX_FDS限制
2. 不支持O_DIRECTORY, O_NOFOLLOW等高级标志
3. 不支持O_ASYNC（异步I/O）
4. mode参数简化处理
5. 不支持文件锁（flock/fcntl）
