# 系统调用接口 - Syscall Dispatcher

## 概述

系统调用接口是Linux应用与Zephyr POSIX层之间的桥梁。它拦截Linux系统调用（通过SVC指令），将其转发到相应的POSIX实现。

## 目标

- 拦截ARM SVC/SWI指令
- 将Linux系统调用号映射到POSIX函数
- 处理参数传递（遵循ARM EABI）
- 返回结果和错误码
- 提供系统调用跟踪和调试

## Linux系统调用约定

### ARM EABI系统调用约定

```
指令: SVC #0 (或 SWI #0)
系统调用号: r7
参数: r0-r6 (最多7个参数)
返回值: r0 (成功时>=0，失败时为负errno)
```

### 常用系统调用号

```c
// 进程管理
#define __NR_exit           1
#define __NR_fork           2
#define __NR_read           3
#define __NR_write          4
#define __NR_open           5
#define __NR_close          6
#define __NR_waitpid        7
#define __NR_execve         11
#define __NR_getpid         20
#define __NR_kill           37

// 文件操作
#define __NR_lseek          19
#define __NR_stat           106
#define __NR_fstat          108
#define __NR_mkdir          39
#define __NR_rmdir          40
#define __NR_dup            41
#define __NR_pipe           42

// 内存管理
#define __NR_brk            45
#define __NR_mmap           90
#define __NR_munmap         91

// 线程
#define __NR_clone          120
#define __NR_gettid         224

// 时间
#define __NR_time           13
#define __NR_gettimeofday   78
#define __NR_clock_gettime  263

// 网络
#define __NR_socket         281
#define __NR_bind           282
#define __NR_connect        283
#define __NR_accept         285
```

## 架构组件

```
┌─────────────────┐
│ Linux Binary    │
│   (SVC #0)      │
└────────┬────────┘
         │
         v
┌─────────────────┐
│  SVC Handler    │ ← ARM异常处理
│  (汇编入口)      │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Syscall Table   │ ← 系统调用号→函数映射
│  Dispatcher     │
└────────┬────────┘
         │
         v
┌─────────────────┐
│  POSIX Layer    │ ← 实际实现
│  (pthread, etc) │
└─────────────────┘
```

## 使用场景

```c
// Linux应用代码
int main() {
    // 这些调用会触发SVC指令
    int fd = open("/tmp/test.txt", O_RDWR);
    write(fd, "hello", 5);
    close(fd);
    return 0;
}

// 编译后的汇编（简化）
open:
    mov r7, #5          // __NR_open
    svc #0              // 触发系统调用
    bx lr

write:
    mov r7, #4          // __NR_write
    svc #0
    bx lr
```

## 与其他模块的关系

- **ELF Loader (Spec 16)**：加载包含系统调用的Linux二进制
- **Thread-Process (Spec 23)**：提供进程上下文
- **POSIX APIs (Specs 1-15)**：实际的系统调用实现

## 配置选项

```kconfig
config SYSCALL_DISPATCHER
    bool "Linux syscall dispatcher"
    depends on ELF_LOADER
    default n
    help
      Enable Linux system call interception and dispatching.

config SYSCALL_TRACE
    bool "Enable syscall tracing"
    depends on SYSCALL_DISPATCHER
    default n
    help
      Log all system calls for debugging.

config SYSCALL_MAX_ARGS
    int "Maximum syscall arguments"
    depends on SYSCALL_DISPATCHER
    default 6
    range 4 7
```

## 文件组织

```
specs/17-syscall/
├── README.md              # 本文件
├── svc_handler.md         # SVC异常处理器
├── syscall_table.md       # 系统调用表
├── syscall_abi.md         # ABI和参数传递
├── errno_mapping.md       # 错误码映射
├── types.md               # 数据类型
└── implementation.md      # 实现指导
```

## 参考资料

- Linux ARM syscall ABI: https://www.kernel.org/doc/Documentation/arm/kernel_user_helpers.txt
- ARM EABI: https://github.com/ARM-software/abi-aa
- Linux syscall table: arch/arm/tools/syscall.tbl
