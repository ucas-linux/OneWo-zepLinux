# 实现指导 - 文件描述符操作

## 核心实现

```c
off_t lseek(int fd, off_t offset, int whence)
{
    fd_entry_t *entry = &g_fd_table[fd];

    int fs_whence = (whence == SEEK_SET) ? FS_SEEK_SET :
                    (whence == SEEK_CUR) ? FS_SEEK_CUR : FS_SEEK_END;

    off_t pos = fs_seek(entry->handle.file, offset, fs_whence);
    entry->offset = pos;
    return pos;
}

int dup(int oldfd)
{
    int newfd = alloc_fd();
    g_fd_table[newfd] = g_fd_table[oldfd];
    return newfd;
}
```

## 配置

```kconfig
config POSIX_FD_OPS
    bool "File descriptor operations"
    default y
```
