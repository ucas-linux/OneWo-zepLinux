# 数据类型 - 进程控制

```c
typedef int pid_t;

// wait选项
#define WNOHANG    1
#define WUNTRACED  2

// 状态宏
#define WEXITSTATUS(s) (((s) >> 8) & 0xFF)
#define WIFEXITED(s)   (((s) & 0x7F) == 0)
#define WIFSIGNALED(s) (((s) & 0x7F) != 0)
#define WTERMSIG(s)    ((s) & 0x7F)
```
