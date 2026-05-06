# C库集成

## 概述

为了运行Linux应用，需要提供标准C库支持。本规范描述如何集成newlib或musl libc到Zephyr POSIX层。

## 目标

- 移植newlib到Zephyr POSIX层
- 或移植musl libc（更小更适合嵌入式）
- 提供libm数学库
- 提供libpthread线程库包装
- 确保与Linux应用二进制兼容

## newlib移植

### 系统调用接口

newlib通过系统调用接口与操作系统交互：

```c
// newlib需要的系统调用
int _open(const char *path, int flags, int mode);
int _close(int fd);
ssize_t _read(int fd, void *buf, size_t count);
ssize_t _write(int fd, const void *buf, size_t count);
off_t _lseek(int fd, off_t offset, int whence);
int _fstat(int fd, struct stat *st);
int _stat(const char *path, struct stat *st);
int _isatty(int fd);
void *_sbrk(ptrdiff_t incr);
int _kill(pid_t pid, int sig);
pid_t _getpid(void);
```

### 实现示例

```c
// _sbrk - 堆管理
void *_sbrk(ptrdiff_t incr)
{
    static char *heap_end = NULL;
    char *prev_heap_end;

    if (heap_end == NULL) {
        heap_end = (char *)&_heap_start;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > (char *)&_heap_end) {
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end += incr;
    return prev_heap_end;
}

// _write - 标准输出
ssize_t _write(int fd, const void *buf, size_t count)
{
    if (fd == 1 || fd == 2) {
        // stdout/stderr -> console
        for (size_t i = 0; i < count; i++) {
            printk("%c", ((char *)buf)[i]);
        }
        return count;
    }

    return write(fd, buf, count);
}
```

## musl libc移植

musl更小更适合嵌入式系统：

```
musl特点：
- 代码量小（~600KB vs newlib ~2MB）
- 更好的POSIX兼容性
- 更少的内存占用
- 更简单的移植
```

### 配置

```makefile
# 编译musl for ARM Cortex-M3
CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m3 -mthumb -Os
CFLAGS += -DCONFIG_ZEPHYR_POSIX

# 禁用不需要的功能
CFLAGS += -DNO_LOCALE
CFLAGS += -DNO_COMPLEX
CFLAGS += -DNO_LDSO
```

## libm数学库

```c
// 基本数学函数
double sin(double x);
double cos(double x);
double tan(double x);
double sqrt(double x);
double pow(double x, double y);
double exp(double x);
double log(double x);

// 使用ARM CMSIS-DSP加速
#ifdef CONFIG_CMSIS_DSP
#include "arm_math.h"

double sin(double x) {
    return arm_sin_f32((float)x);
}
#endif
```

## libpthread包装

```c
// pthread包装器
#define pthread_create  posix_pthread_create
#define pthread_join    posix_pthread_join
#define pthread_mutex_lock   posix_pthread_mutex_lock
#define pthread_mutex_unlock posix_pthread_mutex_unlock
```

## 配置选项

```kconfig
choice LIBC_IMPLEMENTATION
    prompt "C library implementation"
    default LIBC_NEWLIB

config LIBC_NEWLIB
    bool "newlib"

config LIBC_MUSL
    bool "musl libc"

config LIBC_MINIMAL
    bool "Minimal C library"

endchoice

config LIBM
    bool "Math library support"
    default y

config LIBPTHREAD
    bool "pthread library"
    depends on PTHREAD
    default y
```
