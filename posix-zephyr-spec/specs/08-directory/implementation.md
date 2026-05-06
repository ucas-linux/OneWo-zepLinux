# 实现指导 - 目录操作

## 核心实现

```c
// 直接映射到Zephyr文件系统API
DIR *opendir(const char *name)
{
    struct fs_dir_t *zdir = k_malloc(sizeof(*zdir));
    fs_dir_t_init(zdir);
    fs_opendir(zdir, name);
    return (DIR *)zdir;
}

struct dirent *readdir(DIR *dirp)
{
    static struct dirent entry;
    struct fs_dirent zentry;

    if (fs_readdir((struct fs_dir_t *)dirp, &zentry) < 0) {
        return NULL;
    }

    strncpy(entry.d_name, zentry.name, sizeof(entry.d_name));
    entry.d_type = (zentry.type == FS_DIR_ENTRY_DIR) ? DT_DIR : DT_REG;

    return &entry;
}
```

## 配置

```kconfig
config POSIX_DIR_OPS
    bool "Directory operations"
    depends on FILE_SYSTEM
    default y
```
