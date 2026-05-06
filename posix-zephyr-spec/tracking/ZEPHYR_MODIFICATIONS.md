# Zephyr改造清单 - 按模块分解

本文档列出26个模块分别需要对Zephyr RTOS进行的具体改造。

## 基础模块（1-15）

### 01-thread（线程管理）

**需要添加**：
- `posix_thread_t` 结构体，扩展Zephyr的 `k_thread`
- 动态栈分配机制（替代 `K_THREAD_STACK_DEFINE`）
- 线程退出状态保存机制（信号量+返回值缓冲区）
- 递归互斥锁实现（记录持有者线程ID）

**需要修改**：
- 在 `k_thread` 结构中添加POSIX扩展字段
- 线程创建流程，支持动态参数传递
- 线程退出流程，保存返回值而非立即销毁

**改造文件**：
- `kernel/include/zephyr/kernel.h` - 添加POSIX线程类型
- `kernel/thread.c` - 修改线程创建/退出逻辑
- 新增 `lib/posix/pthread.c` - pthread实现

---

### 02-process（进程管理）

**需要添加**：
- 进程控制块（PCB）数据结构
- 进程ID分配器
- 父子进程关系表
- 僵尸进程列表

**需要修改**：
- 无（新增独立模块）

**改造文件**：
- 新增 `lib/posix/process/pcb.c` - PCB管理
- 新增 `lib/posix/process/fork.c` - fork实现（无MMU返回ENOSYS）
- 新增 `lib/posix/process/exit.c` - exit实现

---

### 03-scheduling（调度）

**需要添加**：
- POSIX优先级到Zephyr优先级的映射表
- 调度策略记录（SCHED_FIFO/RR/OTHER）

**需要修改**：
- 无（封装现有API）

**改造文件**：
- 新增 `lib/posix/sched.c` - 调度接口封装

---

### 04-signal-timer（信号与定时器）

**需要添加**：
- 信号掩码位图（64位）
- 信号处理函数表
- 待处理信号队列
- 信号分发机制（在系统调用返回时检查）
- POSIX定时器结构（封装 `k_timer`）

**需要修改**：
- 在PCB中添加信号相关字段
- 系统调用返回路径，插入信号检查点

**改造文件**：
- 新增 `lib/posix/signal/signal.c` - 信号处理
- 新增 `lib/posix/signal/timer.c` - POSIX定时器
- 修改 `lib/posix/syscall/dispatcher.c` - 添加信号检查

---

### 05-memory（内存管理）

**需要添加**：
- 内存块头部结构（记录大小、魔数）
- 内存块链表（用于泄漏检测）
- 内存统计结构

**需要修改**：
- 无（封装 `k_malloc`）

**改造文件**：
- 新增 `lib/posix/malloc.c` - malloc/free/realloc实现

---

### 06-device（设备管理）

**需要添加**：
- 全局文件描述符表（fd_table）
- FD类型枚举（文件/设备/socket/管道）
- FD分配/释放函数
- 标准输入输出映射（0/1/2）

**需要修改**：
- 无（新增独立模块）

**改造文件**：
- 新增 `lib/posix/fd/fd_table.c` - FD表管理
- 新增 `lib/posix/fd/open.c` - open实现
- 新增 `lib/posix/fd/rw.c` - read/write分发器

---

### 07-filesystem（文件系统）

**需要添加**：
- 挂载点注册表
- 文件系统类型映射（Linux → Zephyr）
- stat结构填充逻辑

**需要修改**：
- 无（封装Zephyr fs API）

**改造文件**：
- 新增 `lib/posix/fs/stat.c` - stat实现
- 新增 `lib/posix/fs/mount.c` - mount封装

---

### 08-directory（目录操作）

**需要添加**：
- DIR结构（封装 `fs_dir_t`）
- 目录句柄表
- 线程本地工作目录（TLS）

**需要修改**：
- 无（封装Zephyr fs API）

**改造文件**：
- 新增 `lib/posix/fs/opendir.c` - opendir实现
- 新增 `lib/posix/fs/chdir.c` - 工作目录管理

---

### 09-fd-ops（文件描述符操作）

**需要添加**：
- FD复制逻辑（引用计数）
- fcntl命令分发器
- 管道实现（基于 `k_msgq` 或环形缓冲区）

**需要修改**：
- FD表，添加引用计数字段

**改造文件**：
- 新增 `lib/posix/fd/dup.c` - dup/dup2实现
- 新增 `lib/posix/fd/fcntl.c` - fcntl实现
- 新增 `lib/posix/fd/pipe.c` - 管道实现

---

### 10-network（网络Socket）

**需要添加**：
- 无（直接映射）

**需要修改**：
- 无（Zephyr zsock已兼容）

**改造文件**：
- 新增 `lib/posix/net/socket_wrapper.h` - 宏定义映射

---

### 11-ipc（IPC）

**需要添加**：
- IPC对象注册表（名称→对象映射）
- POSIX消息队列结构（封装 `k_msgq`）
- POSIX信号量结构（封装 `k_sem`）
- 共享内存对象（基于MPU分区）

**需要修改**：
- 无（封装Zephyr IPC原语）

**改造文件**：
- 新增 `lib/posix/ipc/mq.c` - 消息队列
- 新增 `lib/posix/ipc/sem.c` - 信号量
- 新增 `lib/posix/ipc/shm.c` - 共享内存

---

### 12-environment（环境变量）

**需要添加**：
- 全局环境变量哈希表
- 环境变量链表
- 互斥锁保护

**需要修改**：
- 无（新增独立模块）

**改造文件**：
- 新增 `lib/posix/env/getenv.c` - 环境变量实现

---

### 13-user-group（用户组）

**需要添加**：
- 用户/组数据库（静态数组）
- 线程本地UID/GID（TLS）

**需要修改**：
- 无（简化实现）

**改造文件**：
- 新增 `lib/posix/user/getuid.c` - UID/GID实现

---

### 14-time（时间管理）

**需要添加**：
- 时钟源映射（CLOCK_REALTIME → RTC）
- 高精度时间计算（基于 `k_cycle_get_32`）

**需要修改**：
- 无（封装Zephyr时间API）

**改造文件**：
- 新增 `lib/posix/time/clock_gettime.c` - 时钟实现

---

### 15-process-control（进程控制）

**需要添加**：
- wait队列（信号量）
- atexit函数链表

**需要修改**：
- exit流程，调用atexit函数

**改造文件**：
- 新增 `lib/posix/process/wait.c` - wait/waitpid实现
- 新增 `lib/posix/process/atexit.c` - atexit实现

---

## 高级模块（16-26）

### 16-elf-loader（ELF加载器）

**需要添加**：
- ELF解析器（读取ELF头、Program Headers、Section Headers）
- 重定位处理器（支持R_ARM_ABS32、R_ARM_REL32等）
- 段加载器（代码段、数据段、BSS段）
- XIP支持（从Flash执行）

**需要修改**：
- 无（新增独立模块）

**改造文件**：
- 新增 `lib/posix/elf/parser.c` - ELF解析
- 新增 `lib/posix/elf/loader.c` - 加载器
- 新增 `lib/posix/elf/relocation.c` - 重定位

---

### 17-syscall（系统调用）

**需要添加**：
- SVC异常处理器（汇编）
- 系统调用表（syscall号→函数指针）
- 参数提取逻辑（从寄存器r0-r6）
- errno映射表（Linux errno → Zephyr errno）

**需要修改**：
- 中断向量表，注册SVC处理器
- 链接脚本，放置syscall表

**改造文件**：
- 新增 `arch/arm/core/svc_handler.S` - SVC处理器
- 新增 `lib/posix/syscall/syscall_table.c` - 系统调用表
- 新增 `lib/posix/syscall/dispatcher.c` - 分发器

---

### 18-advanced-ipc（高级IPC）

**需要添加**：
- （待定，规范未完成）

---

### 19-io-multiplex（I/O多路复用）

**需要添加**：
- fd_set操作函数
- pollfd数组管理
- epoll实例结构（红黑树或哈希表）
- 就绪检测逻辑（映射到 `k_poll`）

**需要修改**：
- FD表，添加就绪状态字段

**改造文件**：
- 新增 `lib/posix/io/select.c` - select实现
- 新增 `lib/posix/io/poll.c` - poll实现
- 新增 `lib/posix/io/epoll.c` - epoll实现

---

### 20-mmap（内存映射）

**需要添加**：
- mmap区域链表
- MPU配置函数（设置内存保护属性）
- 匿名映射实现（基于 `k_malloc`）

**需要修改**：
- 无（模拟实现）

**改造文件**：
- 新增 `lib/posix/mem/mmap.c` - mmap/munmap实现

---

### 21-pseudo-fs（伪文件系统）

**需要添加**：
- 虚拟文件系统框架（VFS节点树）
- /proc文件生成器（cpuinfo、meminfo、self/等）
- /sys文件生成器（class/net/、devices/等）
- /dev特殊设备（null、zero、random、urandom）

**需要修改**：
- 文件系统注册表，添加伪文件系统类型

**改造文件**：
- 新增 `lib/posix/pseudo_fs/proc_fs.c` - /proc实现
- 新增 `lib/posix/pseudo_fs/sys_fs.c` - /sys实现
- 新增 `lib/posix/pseudo_fs/dev_fs.c` - /dev实现

---

### 22-advanced-file（高级文件操作）

**需要添加**：
- 文件锁链表
- fcntl扩展命令处理
- flock实现（基于互斥锁）

**需要修改**：
- FD表，添加锁信息字段

**改造文件**：
- 新增 `lib/posix/fd/fcntl_advanced.c` - fcntl扩展
- 新增 `lib/posix/fd/flock.c` - 文件锁

---

### 23-thread-process（Thread-as-Process）

**需要添加**：
- posix_spawn实现
- MPU内存域配置（代码段、数据段、堆、栈）
- 进程资源跟踪表
- 进程树管理

**需要修改**：
- PCB结构，添加MPU相关字段
- 线程创建流程，配置MPU

**改造文件**：
- 新增 `lib/posix/process/spawn.c` - posix_spawn实现
- 新增 `lib/posix/process/mpu.c` - MPU配置

---

### 24-dynamic-link（动态链接）

**需要添加**：
- （待定，规范未完成）

---

### 25-libc（C库集成）

**需要添加**：
- newlib系统调用接口（_open、_read、_write、_sbrk等）
- 或musl libc移植
- libm数学库（可选CMSIS-DSP加速）

**需要修改**：
- 链接脚本，链接newlib/musl
- 构建系统，添加libc编译选项

**改造文件**：
- 新增 `lib/posix/libc/syscalls.c` - newlib系统调用
- 修改 `CMakeLists.txt` - 添加libc依赖

---

### 26-signals（完整信号支持）

**需要添加**：
- sigaction结构和处理
- sigprocmask实现（信号屏蔽集操作）
- sigsuspend实现（原子等待）
- 实时信号队列（SIGRTMIN-SIGRTMAX）
- siginfo_t结构

**需要修改**：
- 信号分发逻辑，支持SA_SIGINFO标志
- PCB结构，添加信号屏蔽集

**改造文件**：
- 新增 `lib/posix/signal/sigaction.c` - sigaction实现
- 新增 `lib/posix/signal/sigprocmask.c` - 信号屏蔽
- 新增 `lib/posix/signal/realtime.c` - 实时信号

---

## 改造总结

### 新增文件统计
- 核心基础设施：~10个文件
- POSIX接口实现：~80个文件
- 测试代码：~50个文件
- **总计**：约140个新增文件

### 修改现有文件
- `kernel/include/zephyr/kernel.h` - 添加POSIX扩展
- `kernel/thread.c` - 修改线程生命周期
- `arch/arm/core/` - 添加SVC处理器
- 链接脚本 - 添加syscall表段
- 构建系统 - 添加POSIX库编译

### 关键依赖
```
基础设施（FD表、PCB）
    ↓
POSIX接口（01-15模块）
    ↓
ELF加载器 + 系统调用
    ↓
Thread-as-Process
    ↓
完整应用支持
```

### 配置选项（Kconfig）
需要添加约50个配置选项，控制：
- 各模块的启用/禁用
- 资源限制（最大FD数、最大进程数等）
- 功能特性（信号支持、伪文件系统等）
