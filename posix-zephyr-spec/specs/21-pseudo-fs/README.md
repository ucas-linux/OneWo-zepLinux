# 伪文件系统 - /proc, /sys, /dev

## 概述

伪文件系统提供了一个文件接口来访问内核信息和设备。许多Linux应用依赖这些文件系统来获取系统信息。

## 目标

- 实现/proc文件系统（进程和系统信息）
- 实现/sys文件系统（设备和驱动信息）
- 实现/dev特殊设备（/dev/null, /dev/zero等）
- 提供只读访问（大部分情况）
- 动态生成内容

## /proc文件系统

### 核心文件

```
/proc/
├── self/              # 当前进程的符号链接
│   ├── cmdline        # 命令行参数
│   ├── environ        # 环境变量
│   ├── exe            # 可执行文件路径
│   ├── maps           # 内存映射
│   ├── stat           # 进程状态
│   └── status         # 详细状态
├── cpuinfo            # CPU信息
├── meminfo            # 内存信息
├── uptime             # 系统运行时间
├── version            # 内核版本
├── loadavg            # 负载平均值
├── filesystems        # 支持的文件系统
└── [pid]/             # 进程目录
    ├── cmdline
    ├── stat
    └── status
```

### 实现示例

```c
// /proc/cpuinfo
static ssize_t proc_cpuinfo_read(struct file *file, char *buf,
                                  size_t count, off_t *offset)
{
    static const char content[] =
        "processor\t: 0\n"
        "model name\t: ARM Cortex-M3\n"
        "BogoMIPS\t: 100.00\n"
        "Features\t: thumb\n"
        "CPU implementer\t: 0x41\n"
        "CPU architecture: 7M\n"
        "CPU variant\t: 0x0\n"
        "CPU part\t: 0xc23\n"
        "CPU revision\t: 0\n";

    return simple_read_from_buffer(buf, count, offset,
                                   content, sizeof(content) - 1);
}

// /proc/meminfo
static ssize_t proc_meminfo_read(struct file *file, char *buf,
                                  size_t count, off_t *offset)
{
    char content[512];
    size_t total_ram = CONFIG_SRAM_SIZE * 1024;
    size_t free_ram = k_mem_free_get();
    size_t used_ram = total_ram - free_ram;

    snprintf(content, sizeof(content),
             "MemTotal:       %zu kB\n"
             "MemFree:        %zu kB\n"
             "MemAvailable:   %zu kB\n"
             "Buffers:        0 kB\n"
             "Cached:         0 kB\n",
             total_ram / 1024,
             free_ram / 1024,
             free_ram / 1024);

    return simple_read_from_buffer(buf, count, offset,
                                   content, strlen(content));
}

// /proc/self/cmdline
static ssize_t proc_self_cmdline_read(struct file *file, char *buf,
                                       size_t count, off_t *offset)
{
    pcb_t *proc = get_current_process();
    char *cmdline = proc->cmdline;
    size_t len = strlen(cmdline);

    return simple_read_from_buffer(buf, count, offset, cmdline, len);
}
```

## /sys文件系统

### 核心目录

```
/sys/
├── class/             # 设备类
│   ├── net/           # 网络设备
│   ├── gpio/          # GPIO
│   └── leds/          # LED
├── devices/           # 设备树
│   └── platform/
└── kernel/            # 内核参数
    └── version
```

### 实现示例

```c
// /sys/class/net/eth0/address
static ssize_t sysfs_net_address_read(struct file *file, char *buf,
                                       size_t count, off_t *offset)
{
    uint8_t mac[6];
    get_mac_address(mac);

    char content[32];
    snprintf(content, sizeof(content),
             "%02x:%02x:%02x:%02x:%02x:%02x\n",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return simple_read_from_buffer(buf, count, offset,
                                   content, strlen(content));
}

// /sys/class/gpio/export (写入)
static ssize_t sysfs_gpio_export_write(struct file *file,
                                        const char *buf,
                                        size_t count, off_t *offset)
{
    int gpio_num = atoi(buf);

    if (gpio_num < 0 || gpio_num >= MAX_GPIO) {
        return -EINVAL;
    }

    // 导出GPIO
    gpio_export(gpio_num);

    return count;
}
```

## /dev特殊设备

### 标准设备

```c
// /dev/null - 丢弃所有写入，读取返回EOF
static ssize_t dev_null_read(struct file *file, char *buf,
                              size_t count, off_t *offset)
{
    return 0;  // EOF
}

static ssize_t dev_null_write(struct file *file, const char *buf,
                               size_t count, off_t *offset)
{
    return count;  // 假装写入成功
}

// /dev/zero - 读取返回零字节
static ssize_t dev_zero_read(struct file *file, char *buf,
                              size_t count, off_t *offset)
{
    memset(buf, 0, count);
    return count;
}

// /dev/random - 返回随机数据
static ssize_t dev_random_read(struct file *file, char *buf,
                                size_t count, off_t *offset)
{
    for (size_t i = 0; i < count; i++) {
        buf[i] = sys_rand32_get() & 0xFF;
    }
    return count;
}

// /dev/urandom - 返回伪随机数据（与random相同）
static ssize_t dev_urandom_read(struct file *file, char *buf,
                                 size_t count, off_t *offset)
{
    return dev_random_read(file, buf, count, offset);
}
```

## 虚拟文件系统框架

```c
typedef struct vfs_node {
    char name[64];
    enum {
        VFS_FILE,
        VFS_DIR
    } type;

    // 文件操作
    ssize_t (*read)(struct file *, char *, size_t, off_t *);
    ssize_t (*write)(struct file *, const char *, size_t, off_t *);

    // 目录操作
    struct vfs_node *children;
    struct vfs_node *next;
} vfs_node_t;

// 注册虚拟文件
void vfs_register_file(const char *path,
                       ssize_t (*read)(struct file *, char *, size_t, off_t *),
                       ssize_t (*write)(struct file *, const char *, size_t, off_t *))
{
    vfs_node_t *node = create_vfs_node(path);
    node->type = VFS_FILE;
    node->read = read;
    node->write = write;
}
```

## 使用场景

```c
// 应用读取CPU信息
int fd = open("/proc/cpuinfo", O_RDONLY);
char buf[1024];
ssize_t n = read(fd, buf, sizeof(buf));
printf("%s", buf);
close(fd);

// 应用读取内存信息
fd = open("/proc/meminfo", O_RDONLY);
n = read(fd, buf, sizeof(buf));
close(fd);

// 应用使用/dev/null
fd = open("/dev/null", O_WRONLY);
write(fd, "ignored", 7);
close(fd);

// 应用使用/dev/random
fd = open("/dev/random", O_RDONLY);
uint8_t random_bytes[16];
read(fd, random_bytes, sizeof(random_bytes));
close(fd);
```

## 配置选项

```kconfig
config PSEUDO_FS
    bool "Pseudo filesystem support"
    default n
    help
      Enable /proc, /sys, /dev pseudo filesystems.

config PROC_FS
    bool "/proc filesystem"
    depends on PSEUDO_FS
    default y

config SYS_FS
    bool "/sys filesystem"
    depends on PSEUDO_FS
    default y

config DEV_FS
    bool "/dev special devices"
    depends on PSEUDO_FS
    default y
```

## 文件组织

```
specs/21-pseudo-fs/
├── README.md              # 本文件
├── proc_fs.md             # /proc实现
├── sys_fs.md              # /sys实现
├── dev_fs.md              # /dev实现
├── vfs_framework.md       # 虚拟文件系统框架
├── types.md               # 数据类型
└── implementation.md      # 实现指导
```
