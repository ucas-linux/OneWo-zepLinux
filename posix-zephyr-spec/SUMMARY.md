# 规范总结文档

## 项目概述

本项目旨在为ARM Cortex-M3（无MMU）上的Zephyr RTOS提供Linux应用兼容层，使静态链接的Linux应用能够在嵌入式环境中运行。

## 已完成的规范

### Priority 1 - 核心功能（关键路径）

#### 1. ELF加载器 (Spec 16)
**目的**: 加载和执行静态链接的Linux ELF二进制文件

**关键文件**:
- `specs/16-elf-loader/README.md` - 总体架构
- `specs/16-elf-loader/elf_format.md` - ELF格式详解
- `specs/16-elf-loader/parser.md` - 解析器实现
- `specs/16-elf-loader/loader.md` - 加载器实现
- `specs/16-elf-loader/relocation.md` - 重定位处理
- `specs/16-elf-loader/xip.md` - XIP支持
- `specs/16-elf-loader/types.md` - 类型定义
- `specs/16-elf-loader/implementation.md` - 实现指导

**核心特性**:
- 解析ARM 32位ELF文件
- 处理Program Headers和Section Headers
- ARM重定位支持（R_ARM_ABS32, R_ARM_CALL等）
- XIP（Execute-in-Place）从Flash执行
- MPU配置和内存保护

#### 2. 系统调用分发器 (Spec 17)
**目的**: 拦截Linux系统调用并转发到POSIX实现

**关键文件**:
- `specs/17-syscall/README.md` - 总体架构
- `specs/17-syscall/svc_handler.md` - SVC异常处理
- `specs/17-syscall/syscall_table.md` - 系统调用表
- `specs/17-syscall/syscall_abi.md` - ABI和参数传递
- `specs/17-syscall/errno_mapping.md` - 错误码映射
- `specs/17-syscall/types.md` - 类型定义
- `specs/17-syscall/implementation.md` - 实现指导

**核心特性**:
- ARM SVC指令拦截
- 系统调用号映射（400+系统调用）
- ARM EABI参数传递（r0-r6）
- errno转换
- 系统调用跟踪和统计

#### 3. I/O多路复用 (Spec 19)
**目的**: 提供select/poll/epoll支持

**关键文件**:
- `specs/19-io-multiplex/README.md` - 总体架构

**核心特性**:
- select()实现
- poll()实现
- epoll系列（高性能）
- 映射到Zephyr k_poll

#### 4. Thread-as-Process模型 (Spec 23)
**目的**: 在无MMU环境下模拟进程

**关键文件**:
- `specs/23-thread-process/README.md` - 总体架构
- `specs/23-thread-process/spawn.md` - posix_spawn实现

**核心特性**:
- 进程控制块（PCB）
- posix_spawn()作为fork/exec替代
- MPU内存隔离
- 进程生命周期管理
- 资源跟踪

### 已有规范（用户提供）

#### 基础POSIX API (Specs 01-15)
- **01-thread**: pthread线程支持
- **02-process**: 进程管理（fork/exec/exit）
- **03-scheduling**: 调度策略
- **04-signal-timer**: 信号和定时器
- **05-memory**: 内存管理（malloc/free）
- **06-device**: 设备I/O（open/close/read/write）
- **07-filesystem**: 文件系统操作
- **08-directory**: 目录操作
- **09-fd-ops**: 文件描述符操作
- **10-network**: 网络socket
- **11-ipc**: 进程间通信（pipe）
- **12-environment**: 环境变量
- **13-user-group**: 用户和组ID
- **14-time**: 时间相关函数
- **15-process-control**: 进程控制（wait/waitpid）

## 建议的后续规范

### Priority 2 - 常用功能

#### 21. 伪文件系统
- /proc文件系统模拟
- /sys文件系统模拟
- /dev特殊设备（/dev/null, /dev/zero等）

#### 22. 高级文件操作
- fcntl()文件控制
- dup/dup2文件描述符复制
- flock()文件锁
- truncate/ftruncate

#### 26. 完整信号支持
- sigaction()高级信号处理
- sigprocmask()信号屏蔽
- sigsuspend()信号等待
- 实时信号

#### 25. C库集成
- newlib移植
- musl libc移植（更小）
- libm数学库
- libpthread线程库

### Priority 3 - 高级功能

#### 20. 内存映射（模拟）
- mmap/munmap（无MMU模拟）
- mprotect（使用MPU）
- 匿名映射
- 文件映射

#### 24. 动态链接（有限）
- dlopen/dlsym/dlclose
- 位置无关代码（PIC）
- GOT/PLT处理

#### 18. 高级IPC
- 共享内存（shm_open）
- 消息队列（mq_open）
- 命名信号量（sem_open）

## 技术架构

```
┌─────────────────────────────────────┐
│      Linux Application              │
│    (静态链接ELF二进制)                │
└──────────────┬──────────────────────┘
               │ SVC指令
               v
┌─────────────────────────────────────┐
│    Syscall Dispatcher (Spec 17)     │
│  - SVC异常处理                       │
│  - 系统调用表                        │
│  - 参数转换                          │
└──────────────┬──────────────────────┘
               │
               v
┌─────────────────────────────────────┐
│    POSIX API Layer (Specs 01-15)    │
│  - pthread, file I/O, network       │
│  - signals, timers, memory          │
└──────────────┬──────────────────────┘
               │
               v
┌─────────────────────────────────────┐
│    Zephyr RTOS                      │
│  - k_thread, k_poll, zsock          │
│  - k_mem_domain, MPU                │
└─────────────────────────────────────┘
```

## 内存布局

```
Flash (0x08000000):
├── Zephyr内核
├── POSIX层代码
└── Linux应用代码 (XIP)

RAM (0x20000000):
├── Zephyr内核数据
├── POSIX层数据
├── 进程1:
│   ├── .data段
│   ├── .bss段
│   ├── heap
│   └── stack
└── 进程2:
    ├── .data段
    ├── .bss段
    ├── heap
    └── stack
```

## 关键限制

1. **无MMU**: 不支持真正的fork()，使用posix_spawn()替代
2. **共享地址空间**: 所有进程共享地址空间，依赖MPU隔离
3. **静态链接**: 仅支持静态链接的ELF，不支持动态链接
4. **内存限制**: RAM通常64KB-256KB，需要精心管理
5. **性能**: Flash访问比RAM慢，XIP有性能影响

## 编译工具链

```bash
# 编译Linux应用为Zephyr兼容的ELF
arm-none-eabi-gcc -static -nostdlib \
    -mcpu=cortex-m3 -mthumb \
    -T linker.ld \
    -o myapp.elf myapp.c

# 验证ELF
readelf -h myapp.elf
readelf -l myapp.elf
```

## 配置示例

```kconfig
# prj.conf
CONFIG_ELF_LOADER=y
CONFIG_ELF_LOADER_XIP=y
CONFIG_SYSCALL_DISPATCHER=y
CONFIG_THREAD_PROCESS_MODEL=y
CONFIG_MAX_PROCESSES=8
CONFIG_PROCESS_DEFAULT_HEAP=4096
CONFIG_PROCESS_DEFAULT_STACK=2048
CONFIG_ARM_MPU=y
```

## 测试策略

1. **单元测试**: 每个模块的独立测试
2. **集成测试**: ELF加载 + 系统调用 + 进程管理
3. **应用测试**: 实际Linux应用（busybox, coreutils等）
4. **性能测试**: 系统调用开销、内存使用

## 下一步行动

1. **实现Priority 1规范**: ELF加载器、系统调用分发器
2. **完善I/O多路复用**: select/poll/epoll详细实现
3. **完成thread-process模型**: wait/waitpid、资源跟踪
4. **开始Priority 2**: 伪文件系统、高级文件操作
5. **编写测试用例**: 验证每个功能模块
6. **性能优化**: 快速路径、缓存、XIP优化

## 参考资料

- Linux系统调用: https://man7.org/linux/man-pages/man2/syscalls.2.html
- ARM EABI: https://github.com/ARM-software/abi-aa
- ELF规范: https://refspecs.linuxfoundation.org/elf/elf.pdf
- Zephyr文档: https://docs.zephyrproject.org/
- uClinux: http://www.uclinux.org/
