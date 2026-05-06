# lseek/dup - 文件描述符操作

## POSIX标准定义

```c
#include <unistd.h>
#include <fcntl.h>

off_t lseek(int fd, off_t offset, int whence);
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int fcntl(int fd, int cmd, ...);
```

## Zephyr适配

```c
off_t lseek(int fd, off_t offset, int whence)
{
    if (fd < 0 || fd >= MAX_FDS) {
        errno = EBADF;
        return -1;
    }

    fd_entry_t *entry = &g_fd_table[fd];
    if (!entry->in_use || entry->type != FD_TYPE_FILE) {
        errno = ESPIPE;  // 不可seek
        return -1;
    }

    int fs_whence;
    switch (whence) {
    case SEEK_SET: fs_whence = FS_SEEK_SET; break;
    case SEEK_CUR: fs_whence = FS_SEEK_CUR; break;
    case SEEK_END: fs_whence = FS_SEEK_END; break;
    default:
        errno = EINVAL;
        return -1;
    }

    off_t new_offset = fs_seek(entry->handle.file, offset, fs_whence);
    if (new_offset < 0) {
        errno = -new_offset;
        return -1;
    }

    entry->offset = new_offset;
    return new_offset;
}

int dup(int oldfd)
{
    if (oldfd < 0 || oldfd >= MAX_FDS || !g_fd_table[oldfd].in_use) {
        errno = EBADF;
        return -1;
    }

    int newfd = alloc_fd();
    if (newfd < 0) {
        return -1;
    }

    // 复制fd表项
    g_fd_table[newfd] = g_fd_table[oldfd];
    return newfd;
}

int dup2(int oldfd, int newfd)
{
    if (oldfd < 0 || oldfd >= MAX_FDS || !g_fd_table[oldfd].in_use) {
        errno = EBADF;
        return -1;
    }

    if (oldfd == newfd) {
        return newfd;
    }

    // 关闭newfd（如果打开）
    if (g_fd_table[newfd].in_use) {
        close(newfd);
    }

    // 复制
    g_fd_table[newfd] = g_fd_table[oldfd];
    g_fd_table[newfd].in_use = true;

    return newfd;
}
```

## 测试

```c
void test_lseek(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
    write(fd, "Hello", 5);

    off_t pos = lseek(fd, 0, SEEK_SET);
    assert(pos == 0);

    char buf[10];
    read(fd, buf, 5);
    assert(memcmp(buf, "Hello", 5) == 0);

    close(fd);
}
```
