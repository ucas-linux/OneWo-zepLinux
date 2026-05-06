# opendir/readdir - 目录操作

## POSIX标准定义

```c
#include <dirent.h>

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
void rewinddir(DIR *dirp);
int mkdir(const char *pathname, mode_t mode);
int rmdir(const char *pathname);
```

## Zephyr适配

```c
DIR *opendir(const char *name)
{
    struct fs_dir_t *zdir = k_malloc(sizeof(*zdir));
    if (!zdir) {
        errno = ENOMEM;
        return NULL;
    }

    fs_dir_t_init(zdir);
    int ret = fs_opendir(zdir, name);
    if (ret < 0) {
        k_free(zdir);
        errno = -ret;
        return NULL;
    }

    return (DIR *)zdir;
}

struct dirent *readdir(DIR *dirp)
{
    static struct dirent entry;
    struct fs_dirent zentry;

    int ret = fs_readdir((struct fs_dir_t *)dirp, &zentry);
    if (ret < 0 || zentry.name[0] == 0) {
        return NULL;
    }

    strncpy(entry.d_name, zentry.name, sizeof(entry.d_name));
    entry.d_type = (zentry.type == FS_DIR_ENTRY_DIR) ? DT_DIR : DT_REG;

    return &entry;
}

int closedir(DIR *dirp)
{
    int ret = fs_closedir((struct fs_dir_t *)dirp);
    k_free(dirp);
    return ret;
}

int mkdir(const char *pathname, mode_t mode)
{
    int ret = fs_mkdir(pathname);
    if (ret < 0) {
        errno = -ret;
        return -1;
    }
    return 0;
}
```

## 测试

```c
void test_directory(void)
{
    mkdir("/tmp/testdir", 0755);

    DIR *dir = opendir("/tmp/testdir");
    assert(dir != NULL);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("Entry: %s\n", entry->d_name);
    }

    closedir(dir);
    rmdir("/tmp/testdir");
}
```
