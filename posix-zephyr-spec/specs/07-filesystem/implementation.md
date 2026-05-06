# 实现指导 - 文件系统

## 核心实现

```c
// 直接映射到Zephyr文件系统API
int stat(const char *pathname, struct stat *statbuf)
{
    struct fs_dirent entry;
    int ret = fs_stat(pathname, &entry);

    statbuf->st_size = entry.size;
    statbuf->st_mode = (entry.type == FS_DIR_ENTRY_DIR) ? S_IFDIR : S_IFREG;

    return ret;
}

int mkdir(const char *pathname, mode_t mode)
{
    return fs_mkdir(pathname);
}

int chmod(const char *pathname, mode_t mode)
{
    // 简化：Zephyr文件系统可能不支持权限
    return 0;
}
```

## 配置

```kconfig
config POSIX_FS
    bool "POSIX filesystem support"
    depends on FILE_SYSTEM
    default y
```
