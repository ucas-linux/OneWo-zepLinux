# 内存映射（模拟）

## 概述

在无MMU系统上模拟mmap/munmap功能，使用堆分配和MPU提供有限的内存保护。

## 目标

- 实现mmap/munmap基本功能
- 使用MPU提供内存保护
- 支持匿名映射（MAP_ANONYMOUS）
- 有限支持文件映射（MAP_FILE）
- 提供mprotect修改保护属性

## mmap实现

```c
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset);
int munmap(void *addr, size_t length);
int mprotect(void *addr, size_t length, int prot);

// 保护标志
#define PROT_NONE  0x0
#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

// 映射标志
#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_FIXED     0x10
#define MAP_ANONYMOUS 0x20
#define MAP_ANON      MAP_ANONYMOUS
```

### 实现

```c
typedef struct mmap_region {
    void *addr;
    size_t length;
    int prot;
    int flags;
    int fd;
    off_t offset;
    struct mmap_region *next;
} mmap_region_t;

static mmap_region_t *g_mmap_regions = NULL;
static struct k_mutex g_mmap_lock;

void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset)
{
    void *mapped_addr;

    if (length == 0) {
        errno = EINVAL;
        return MAP_FAILED;
    }

    // 对齐到页边界
    length = ALIGN_UP(length, 4096);

    k_mutex_lock(&g_mmap_lock, K_FOREVER);

    // 匿名映射
    if (flags & MAP_ANONYMOUS) {
        mapped_addr = k_malloc(length);
        if (!mapped_addr) {
            k_mutex_unlock(&g_mmap_lock);
            errno = ENOMEM;
            return MAP_FAILED;
        }

        // 清零
        if (flags & MAP_PRIVATE) {
            memset(mapped_addr, 0, length);
        }
    } else {
        // 文件映射
        if (fd < 0) {
            k_mutex_unlock(&g_mmap_lock);
            errno = EBADF;
            return MAP_FAILED;
        }

        mapped_addr = k_malloc(length);
        if (!mapped_addr) {
            k_mutex_unlock(&g_mmap_lock);
            errno = ENOMEM;
            return MAP_FAILED;
        }

        // 读取文件内容
        lseek(fd, offset, SEEK_SET);
        ssize_t n = read(fd, mapped_addr, length);
        if (n < 0) {
            k_free(mapped_addr);
            k_mutex_unlock(&g_mmap_lock);
            errno = EIO;
            return MAP_FAILED;
        }
    }

    // 配置MPU保护
    if (configure_mmap_mpu(mapped_addr, length, prot) < 0) {
        k_free(mapped_addr);
        k_mutex_unlock(&g_mmap_lock);
        errno = EACCES;
        return MAP_FAILED;
    }

    // 记录映射
    mmap_region_t *region = k_malloc(sizeof(mmap_region_t));
    region->addr = mapped_addr;
    region->length = length;
    region->prot = prot;
    region->flags = flags;
    region->fd = fd;
    region->offset = offset;
    region->next = g_mmap_regions;
    g_mmap_regions = region;

    k_mutex_unlock(&g_mmap_lock);
    return mapped_addr;
}

int munmap(void *addr, size_t length)
{
    k_mutex_lock(&g_mmap_lock, K_FOREVER);

    mmap_region_t **prev = &g_mmap_regions;
    for (mmap_region_t *r = g_mmap_regions; r; r = r->next) {
        if (r->addr == addr) {
            // 移除MPU配置
            remove_mmap_mpu(addr, length);

            // 释放内存
            k_free(addr);

            // 移除记录
            *prev = r->next;
            k_free(r);

            k_mutex_unlock(&g_mmap_lock);
            return 0;
        }
        prev = &r->next;
    }

    k_mutex_unlock(&g_mmap_lock);
    errno = EINVAL;
    return -1;
}
```

## 使用示例

```c
// 示例1：匿名映射
void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
if (ptr == MAP_FAILED) {
    perror("mmap");
}
// 使用内存
munmap(ptr, 4096);

// 示例2：文件映射
int fd = open("file.txt", O_RDONLY);
void *ptr = mmap(NULL, 4096, PROT_READ,
                 MAP_PRIVATE, fd, 0);
// 读取文件内容
munmap(ptr, 4096);
close(fd);
```

## 限制

1. **无真正的共享内存**：MAP_SHARED实际上是私有的
2. **无写时复制**：MAP_PRIVATE立即分配内存
3. **有限的保护**：MPU区域数量有限
4. **性能**：比真正的mmap慢

## 配置选项

```kconfig
config POSIX_MMAP
    bool "mmap support"
    depends on ARM_MPU
    default n
```
