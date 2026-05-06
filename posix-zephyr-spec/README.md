# POSIX接口Zephyr适配规范文档

## 项目简介

本仓库包含将Linux POSIX接口移植到Zephyr RTOS（ARM Cortex-M3架构）的完整技术规范文档。目标是实现望获Linux micro版本的接口定义，使Linux应用能够在Zephyr上运行。

## 接口覆盖范围

### 系统级接口（29个）

1. **线程管理**（7个）：pthread_create, pthread_join, pthread_cancel, pthread_mutex_lock, pthread_mutex_unlock, pthread_cond_wait, pthread_cond_signal
2. **进程管理**（3个）：fork, exec, exit
3. **调度与优先级**（3个）：sched_yield, sched_setparam, sched_getparam
4. **信号与定时器**（8个）：signal, kill, alarm, timer_create, sleep, usleep, nanosleep, timer_settime
5. **内存管理**（3个）：malloc, realloc, free
6. **设备管理**（5个）：open, close, read, write, ioctl

### 扩展接口（约79个）

7. **文件系统操作**（15个）：stat, fstat, lstat, link, unlink, rename, truncate, lseek, chmod, chown, access, symlink, readlink, fsync, fdatasync
8. **目录操作**（7个）：opendir, readdir, closedir, mkdir, rmdir, chdir, getcwd
9. **文件描述符操作**（6个）：pipe, dup, dup2, fcntl, select, poll
10. **网络Socket**（12个）：socket, bind, listen, accept, connect, send, recv, sendto, recvfrom, setsockopt, getsockopt, getaddrinfo
11. **POSIX IPC**（12个）：mq_open, mq_close, mq_send, mq_receive, mq_unlink, sem_open, sem_close, sem_wait, sem_post, sem_unlink, shm_open, shm_unlink等
12. **环境变量**（4个）：getenv, setenv, unsetenv, putenv
13. **用户/组/进程ID**（10个）：getuid, setuid, geteuid, seteuid, getgid, setgid, getegid, setegid, getpid, getppid
14. **时间和时钟**（8个）：time, gettimeofday, clock_gettime, clock_settime, localtime, gmtime, mktime, strftime
15. **进程控制**（5个）：wait, waitpid, waitid, getpgrp, setpgid

### 标准库函数（18个）

字符串操作、标准I/O、数学函数等

## 目录结构

```
posix-zephyr-spec/
├── README.md                    # 项目说明（本文件）
├── docs/
│   ├── human-readable/          # 给人看的文档
│   │   ├── architecture.md      # 架构设计文档
│   │   ├── implementation-guide.md  # 实现指南
│   │   └── testing-guide.md     # 测试指南
│   └── ai-prompts/              # 给AI看的提示词文档
│       ├── master-prompt.md     # 主提示词
│       └── interface-prompts/   # 各接口类别的AI提示词
├── specs/                       # 详细接口规范
│   ├── 01-thread/               # 线程管理接口规范
│   ├── 02-process/              # 进程管理接口规范
│   ├── 03-scheduling/           # 调度接口规范
│   ├── 04-signal-timer/         # 信号和定时器接口规范
│   ├── 05-memory/               # 内存管理接口规范
│   ├── 06-device/               # 设备管理接口规范
│   ├── 07-filesystem/           # 文件系统接口规范
│   ├── 08-directory/            # 目录操作接口规范
│   ├── 09-fd-ops/               # 文件描述符操作规范
│   ├── 10-network/              # 网络Socket接口规范
│   ├── 11-ipc/                  # POSIX IPC接口规范
│   ├── 12-environment/          # 环境变量接口规范
│   ├── 13-user-group/           # 用户组接口规范
│   ├── 14-time/                 # 时间接口规范
│   └── 15-process-control/      # 进程控制接口规范
├── scripts/                     # 辅助脚本
│   ├── generate-stubs.sh        # 生成接口桩代码
│   └── validate-spec.py         # 验证规范完整性
└── examples/                    # 示例代码
    ├── pthread-example/         # 线程示例
    ├── ipc-example/             # IPC示例
    └── socket-example/          # Socket示例
```

## 使用方法

### 给开发者

1. 阅读 `docs/human-readable/architecture.md` 了解整体架构
2. 查看 `specs/` 目录下对应的接口规范文档
3. 参考 `examples/` 中的示例代码
4. 使用 `scripts/` 中的工具辅助开发

### 给AI

1. 加载 `docs/ai-prompts/master-prompt.md` 作为主提示词
2. 根据需要加载 `specs/` 中对应的接口规范
3. 使用规范中的详细要求生成实现代码

## 技术背景

### 目标平台

- **硬件架构**：ARM Cortex-M3
- **操作系统**：Zephyr RTOS
- **POSIX配置**：PSE51/PSE52嵌入式配置文件

### 关键挑战

1. **无MMU环境**：Cortex-M3无MMU，需使用MPU模拟进程隔离
2. **静态vs动态内存**：Zephyr传统静态分配 vs Linux动态分配
3. **调度策略映射**：Linux SCHED_FIFO/RR vs Zephyr优先级模型
4. **单地址空间**：需模拟进程空间隔离

## 参考资料

- [Zephyr POSIX支持文档](https://docs.zephyrproject.org/latest/services/portability/posix/aep/index.html)
- [POSIX.1-2017标准](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [PSE51/PSE52配置文件](https://posix.opengroup.org/docs/pse52-2003.html)

## 贡献指南

1. 每个接口规范应包含：
   - 接口原型和参数说明
   - Linux标准行为描述
   - Zephyr现有支持情况
   - 适配实现方案
   - 测试用例要求

2. 文档格式：
   - 使用Markdown格式
   - 代码示例使用C语言
   - 包含详细的注释说明

## 版本历史

- v0.1.0 (2026-03-03): 初始版本，创建基础结构

## 许可证

待定

## 联系方式

待定
