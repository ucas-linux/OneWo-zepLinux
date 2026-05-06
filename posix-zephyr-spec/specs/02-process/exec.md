# exec - 执行新程序

## POSIX标准定义

```c
#include <unistd.h>

int execl(const char *path, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execve(const char *path, char *const argv[], char *const envp[]);
int execlp(const char *file, const char *arg, ...);
int execvp(const char *file, char *const argv[]);
```

## 功能描述

exec系列函数用新程序替换当前进程的内存映像。成功时不返回，失败时返回-1。

## 参数说明

- **path**: 可执行文件的完整路径
- **file**: 可执行文件名（在PATH中搜索）
- **arg/argv**: 参数列表
- **envp**: 环境变量数组

## 返回值

- **成功**：不返回（进程被替换）
- **失败**：返回-1，设置errno

## Linux标准行为

1. 替换当前进程的代码段、数据段、堆、栈
2. 保留进程ID、父进程ID、文件描述符（除非设置了FD_CLOEXEC）
3. 重置信号处理为默认
4. 关闭设置了close-on-exec的文件描述符

## Zephyr兼容性与退化行为

### 结论

Linux ABI要求加载**Linux ELF**并替换进程映像；这需要**进程隔离**与**兼容ELF装载器**。LLEXT仅适用于Zephyr扩展模块，**不等价于Linux ELF**。

### 行为规定

- **无MMU目标**：`execve()`返回`-1`并设置`errno=ENOEXEC`或`ENOSYS`。
- **具备MMU且有Linux ELF装载器**：实现`execve()`，替换当前进程映像并遵循Linux语义。

### 其他exec变体

`execl/execv/execle/execvp`等仅是参数构造与`execve`封装；若`execve`不支持，保持相同错误语义。

### 测试要求

- 无MMU目标：验证`execve()`返回`-1`且`errno`为`ENOEXEC/ENOSYS`。
- MMU目标：验证ELF装载成功、FD_CLOEXEC语义、生效信号重置。
