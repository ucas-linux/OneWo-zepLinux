# stat - 文件状态

## POSIX标准定义

```c
#include <sys/stat.h>

int stat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
int mkdir(const char *pathname, mode_t mode);
int chmod(const char *pathname, mode_t mode);
```

## Zephyr适配

```c
int stat(const char *pathname, struct stat *statbuf)
{
    struct fs_dirent entry;
    int ret = fs_stat(pathname, &entry);
    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    memset(statbuf, 0, sizeof(*statbuf));
    statbuf->st_size = entry.size;
    statbuf->st_mode = (entry.type == FS_DIR_ENTRY_DIR) ? S_IFDIR : S_IFREG;

    return 0;
}

int fstat(int fd, struct stat *statbuf)
{
    fd_entry_t *entry = &g_fd_table[fd];
    if (entry->type != FD_TYPE_FILE) {
        errno = EBADF;
        return -1;
    }

    return stat("", statbuf);  // 简化实现
}
```

## 测试

```c
void test_stat(void)
{
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
    write(fd, "Hello", 5);

    struct stat st;
    fstat(fd, &st);
    assert(st.st_size == 5);

    close(fd);
}
```
