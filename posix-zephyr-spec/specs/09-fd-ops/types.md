# 数据类型 - 文件描述符操作

```c
typedef long off_t;

// lseek whence
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

// fcntl命令
#define F_DUPFD   0
#define F_GETFD   1
#define F_SETFD   2
#define F_GETFL   3
#define F_SETFL   4

// 文件描述符标志
#define FD_CLOEXEC  1
```
