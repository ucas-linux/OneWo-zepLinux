# 模块映射表

本文档建立**规范文档**与**实现代码**的精确映射关系。

## 映射格式

```
模块ID | 规范文档路径 | 实现代码路径 | 测试代码路径 | 依赖模块
```

## 基础设施模块

| 模块 | 规范文档 | 实现代码 | 测试代码 | 依赖 |
|------|----------|----------|----------|------|
| FD表 | specs/06-device/README.md (部分) | implementation/src/common/fd_table.{c,h} | implementation/tests/common/test_fd_table.c | - |
| PCB系统 | specs/02-process/README.md<br>specs/23-thread-process/ | implementation/src/common/pcb.{c,h} | implementation/tests/common/test_pcb.c | - |
| errno | - | implementation/src/common/errno.c | - | - |

## POSIX接口模块

### 01-thread (线程管理)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| pthread_create | specs/01-thread/pthread_create.md | implementation/src/01-thread/pthread_create.c | implementation/tests/01-thread/test_pthread_create.c |
| pthread_join | specs/01-thread/pthread_join.md | implementation/src/01-thread/pthread_join.c | implementation/tests/01-thread/test_pthread_join.c |
| pthread_cancel | specs/01-thread/pthread_cancel.md | implementation/src/01-thread/pthread_cancel.c | implementation/tests/01-thread/test_pthread_cancel.c |
| pthread_mutex | specs/01-thread/pthread_mutex.md | implementation/src/01-thread/pthread_mutex.c | implementation/tests/01-thread/test_pthread_mutex.c |
| pthread_cond | specs/01-thread/pthread_cond.md | implementation/src/01-thread/pthread_cond.c | implementation/tests/01-thread/test_pthread_cond.c |
| 数据类型 | specs/01-thread/types.md | implementation/include/posix/pthread.h | - |

**头文件**: `implementation/include/posix/pthread.h`

### 02-process (进程管理)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| fork | specs/02-process/fork.md | implementation/src/02-process/fork.c | implementation/tests/02-process/test_fork.c |
| exec | specs/02-process/exec.md | implementation/src/02-process/exec.c | implementation/tests/02-process/test_exec.c |
| exit | specs/02-process/exit.md | implementation/src/02-process/exit.c | implementation/tests/02-process/test_exit.c |
| 数据类型 | specs/02-process/types.md | implementation/include/posix/unistd.h | - |

**头文件**: `implementation/include/posix/unistd.h`

### 03-scheduling (调度)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| sched_yield | specs/03-scheduling/sched_yield.md | implementation/src/03-scheduling/sched_yield.c | implementation/tests/03-scheduling/test_sched_yield.c |
| sched_param | specs/03-scheduling/sched_param.md | implementation/src/03-scheduling/sched_param.c | implementation/tests/03-scheduling/test_sched_param.c |

**头文件**: `implementation/include/posix/sched.h`

### 04-signal-timer (信号与定时器)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| signal | specs/04-signal-timer/signal.md | implementation/src/04-signal-timer/signal.c | implementation/tests/04-signal-timer/test_signal.c |
| kill | specs/04-signal-timer/kill.md | implementation/src/04-signal-timer/kill.c | implementation/tests/04-signal-timer/test_kill.c |
| sleep | specs/04-signal-timer/sleep.md | implementation/src/04-signal-timer/sleep.c | implementation/tests/04-signal-timer/test_sleep.c |
| timer_create | specs/04-signal-timer/timer_create.md | implementation/src/04-signal-timer/timer_create.c | implementation/tests/04-signal-timer/test_timer.c |

**头文件**: `implementation/include/posix/signal.h`, `implementation/include/posix/time.h`

### 05-memory (内存管理)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| malloc | specs/05-memory/malloc.md | implementation/src/05-memory/malloc.c | implementation/tests/05-memory/test_malloc.c |
| free | specs/05-memory/free.md | implementation/src/05-memory/malloc.c | implementation/tests/05-memory/test_malloc.c |
| realloc | specs/05-memory/realloc.md | implementation/src/05-memory/malloc.c | implementation/tests/05-memory/test_malloc.c |

**头文件**: `implementation/include/posix/stdlib.h`

### 06-device (设备管理)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| open | specs/06-device/open.md | implementation/src/06-device/open.c | implementation/tests/06-device/test_open.c |
| close | specs/06-device/close.md | implementation/src/06-device/close.c | implementation/tests/06-device/test_close.c |
| read/write | specs/06-device/read_write.md | implementation/src/06-device/rw.c | implementation/tests/06-device/test_rw.c |
| ioctl | specs/06-device/ioctl.md | implementation/src/06-device/ioctl.c | implementation/tests/06-device/test_ioctl.c |

**头文件**: `implementation/include/posix/fcntl.h`, `implementation/include/posix/unistd.h`

### 07-filesystem (文件系统)

| 接口 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| stat | specs/07-filesystem/stat.md | implementation/src/07-filesystem/stat.c | implementation/tests/07-filesystem/test_stat.c |
| mount | specs/07-filesystem/mount.md | implementation/src/07-filesystem/mount.c | implementation/tests/07-filesystem/test_mount.c |

**头文件**: `implementation/include/posix/sys/stat.h`

### 16-elf-loader (ELF加载器)

| 组件 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| ELF解析器 | specs/16-elf-loader/parser.md | implementation/src/16-elf-loader/parser.c | implementation/tests/16-elf-loader/test_parser.c |
| 加载器 | specs/16-elf-loader/loader.md | implementation/src/16-elf-loader/loader.c | implementation/tests/16-elf-loader/test_loader.c |
| 重定位 | specs/16-elf-loader/relocation.md | implementation/src/16-elf-loader/relocation.c | implementation/tests/16-elf-loader/test_relocation.c |
| XIP支持 | specs/16-elf-loader/xip.md | implementation/src/16-elf-loader/xip.c | implementation/tests/16-elf-loader/test_xip.c |

**头文件**: `implementation/include/internal/elf_loader.h`

### 17-syscall (系统调用)

| 组件 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| SVC处理器 | specs/17-syscall/svc_handler.md | implementation/src/17-syscall/svc_handler.S | implementation/tests/17-syscall/test_svc.c |
| 系统调用表 | specs/17-syscall/syscall_table.md | implementation/src/17-syscall/syscall_table.c | implementation/tests/17-syscall/test_syscall_table.c |
| 分发器 | specs/17-syscall/implementation.md | implementation/src/17-syscall/dispatcher.c | implementation/tests/17-syscall/test_dispatcher.c |
| errno映射 | specs/17-syscall/errno_mapping.md | implementation/src/17-syscall/errno_mapping.c | - |

**头文件**: `implementation/include/internal/syscall.h`

### 23-thread-process (Thread-as-Process)

| 组件 | 规范文档 | 实现代码 | 测试代码 |
|------|----------|----------|----------|
| posix_spawn | specs/23-thread-process/spawn.md | implementation/src/23-thread-process/spawn.c | implementation/tests/23-thread-process/test_spawn.c |
| wait/waitpid | specs/23-thread-process/wait.md | implementation/src/23-thread-process/wait.c | implementation/tests/23-thread-process/test_wait.c |
| MPU隔离 | specs/23-thread-process/mpu_isolation.md | implementation/src/23-thread-process/mpu.c | implementation/tests/23-thread-process/test_mpu.c |

**头文件**: `implementation/include/posix/spawn.h`

## 快速查找

### 按功能查找

- **线程相关**: 01-thread, 03-scheduling
- **进程相关**: 02-process, 15-process-control, 23-thread-process
- **内存相关**: 05-memory, 20-mmap
- **文件相关**: 06-device, 07-filesystem, 08-directory, 09-fd-ops, 22-advanced-file
- **网络相关**: 10-network, 19-io-multiplex
- **IPC相关**: 11-ipc
- **信号相关**: 04-signal-timer, 26-signals
- **系统相关**: 12-environment, 13-user-group, 14-time
- **加载相关**: 16-elf-loader, 17-syscall, 25-libc

### 按依赖关系查找

```
基础层:
  - common/fd_table
  - common/pcb
  - 05-memory

核心层:
  - 01-thread (依赖: 05-memory)
  - 06-device (依赖: common/fd_table)

加载层:
  - 16-elf-loader (依赖: 05-memory)
  - 17-syscall (依赖: 所有POSIX接口)
  - 23-thread-process (依赖: common/pcb, 16-elf-loader)

应用层:
  - 02-process (依赖: 23-thread-process)
  - 其他所有模块
```

## 使用示例

### 实现一个新模块

1. 查找规范文档路径
2. 创建对应的实现目录和文件
3. 编写代码
4. 编写测试
5. 更新本映射表

### 查找某个接口的实现

```bash
# 查找pthread_create的实现
grep "pthread_create" tracking/MODULE_MAPPING.md
# 输出: implementation/src/01-thread/pthread_create.c
```

### 运行特定模块的测试

```bash
# 运行线程模块测试
cd implementation
ctest -R "01-thread"
```
