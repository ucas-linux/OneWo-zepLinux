# 数据类型 - 文件系统

```c
typedef unsigned int mode_t;
typedef long off_t;

struct stat {
    mode_t st_mode;
    off_t st_size;
    time_t st_mtime;
};

// 文件类型
#define S_IFMT   0170000
#define S_IFREG  0100000
#define S_IFDIR  0040000
#define S_IFLNK  0120000

// 权限位
#define S_IRWXU  0700
#define S_IRUSR  0400
#define S_IWUSR  0200
#define S_IXUSR  0100
```
