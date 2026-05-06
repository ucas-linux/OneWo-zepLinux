# 数据类型 - 目录操作

```c
typedef struct fs_dir_t DIR;

struct dirent {
    char d_name[256];
    unsigned char d_type;
};

// 文件类型
#define DT_UNKNOWN  0
#define DT_REG      8   // 普通文件
#define DT_DIR      4   // 目录
#define DT_LNK      10  // 符号链接
```
