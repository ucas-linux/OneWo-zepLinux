# OneWo zepLinux — a new Zephyr OS with Linux API for MCU，developed by OneWo-rtLinux team.

在 Zephyr RTOS 上实现 Linux 兼容接口层，让 Linux 应用以最小改动运行在 ARM Cortex-M3 微控制器上。

## 核心亮点

- 4 类 Linux 风格调度器（DL/RT/CFS/Idle）深度集成到 Zephyr 内核，真正驱动 `k_thread` 调度决策
- 当前文档已整理 56 个 Linux/POSIX 兼容接口，覆盖线程、进程、调度、信号、内存、设备、文件系统、网络与 I/O 多路复用等模块
- 目标平台： rocket_pi/stm32f401xe

## 目录结构

```
OneWo-zepLinux/
├── README.md                # 项目总览、架构说明、接口与测试索引
├── docs/                    # 设计文档、迁移说明、接口定义与演示说明
│   ├── design-principles.md
│   ├── linux-sched-kernel-integration.md
│   ├── migration-guide.md
│   ├── zepLinux-interface-and-validation.md
│   └── test-demos/          # 板级验证、设备演示、性能基准、调度测试文档
├── scripts/                 # 开发辅助脚本
│   ├── setup.sh
│   ├── build.sh
│   ├── test.sh
│   ├── check.sh
│   └── format.sh
├── tests/                   # 示例、单元、集成、压力与演示测试
│   ├── examples/hello_lmz/  # 最小示例程序
│   ├── unit/                # `thread` / `sched` / `memory` 单元测试
│   ├── integration/         # 进程等集成测试
│   ├── stress/              # 压力测试
│   └── demo/                # 调度、设备、板级、性能演示
├── posix-zephyr-spec/       # Linux/POSIX 兼容层规格与接口拆解文档
│   ├── specs/               # 分模块接口规格（线程、进程、调度、信号等）
│   ├── tracking/            # 路线图、状态跟踪、Zephyr 修改记录
│   ├── examples/            # 规格配套示例
│   └── scripts/             # 规格辅助脚本
└── zephyr/                  # 内置 Zephyr 源码树与内核实现
    ├── kernel/              # 内核调度与线程核心逻辑
    ├── include/             # 公共头文件
    ├── lib/                 # 基础库
    ├── subsys/              # 各类子系统
    ├── tests/               # Zephyr 自带测试
    ├── samples/             # Zephyr 示例
    └── boards/              # 板级支持包

```

## 调度器架构

通过 `CONFIG_SCHED_LINUX` Kconfig 选项启用，作为 Zephyr 第 4 种可插拔就绪队列实现（与 SIMPLE/SCALABLE/MULTIQ 并列）。

```
┌─────────────────────────────────────────┐
│           Zephyr 内核调度器              │
│  _priq_run_add / remove / best / yield  │
└──────────────┬──────────────────────────┘
               │ CONFIG_SCHED_LINUX
               ▼
┌─────────────────────────────────────────┐
│         z_priq_linux_best()             │
│  DL(EDF/CBS) → RT(FIFO/RR) → CFS → Idle│
└──────────────┬──────────────────────────┘
               │
    ┌──────────┼──────────┬───────────┐
    ▼          ▼          ▼           ▼
  DL 队列   RT 队列    CFS 队列   Idle
  按 deadline  bitmap    按 vruntime  (Zephyr
  升序排列    O(1)查找   升序排列    内置)
```

**调度类优先级**：DL > RT > CFS > Idle

| 调度类 | 策略 | 排序依据 | 特性 |
|--------|------|----------|------|
| DL | SCHED_DEADLINE | 绝对 deadline 升序 | EDF + CBS 节流/补充 |
| RT | SCHED_FIFO / SCHED_RR | 优先级 1-99 bitmap | FIFO 无时间片，RR 10 tick 轮转 |
| CFS | SCHED_NORMAL | vruntime 升序 | nice -20..19 权重表，公平分配 |
| Idle | SCHED_IDLE | — | Zephyr idle thread 兜底 |

## 修改的 Zephyr 内核文件

| 文件 | 改动 |
|------|------|
| `include/zephyr/kernel_structs.h` | 新增 `struct _priq_linux`，扩展 `_ready_q` |
| `include/zephyr/kernel/thread.h` | `_thread_base` 中添加调度类元数据字段 |
| `kernel/include/priority_q.h` | 添加 `CONFIG_SCHED_LINUX` 宏 + 覆盖 `z_sched_prio_cmp` |
| `kernel/Kconfig` | 添加 `CONFIG_SCHED_LINUX` 选项 |
| `kernel/thread.c` | 新线程默认初始化为 CFS 类 |
| `kernel/timeslicing.c` | 添加 `z_linux_sched_tick()` 钩子 |
| `kernel/CMakeLists.txt` | 添加 `linux_sched/` 源文件 |

## 构建与测试

```bash


# 构建内核集成测试（真实线程调度）
west build -b -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/schedule_api

```



## 接口分类总览

| 类别 | 接口数 | 示例 |
|------|--------|------|
| 线程管理 | 7 | pthread_create, pthread_join, pthread_cancel, pthread_mutex_lock, pthread_mutex_unlock, pthread_cond_wait, pthread_cond_signal |
| 进程管理 | 3 | fork, execve, exit |
| 调度与优先级 | 3 | sched_yield, sched_setparam, sched_getparam |
| 信号与定时器 | 8 | signal, kill, alarm, timer_create, sleep, usleep, nanosleep, timer_settime |
| 内存管理 | 3 | malloc, realloc, free |
| 设备管理 | 5 | open, close, read, write, ioctl |
| 文件系统与目录 | 6 | stat, fstat, opendir, readdir, closedir, mkdir |
| 文件描述符操作 | 4 | lseek, dup, dup2, fcntl |
| 网络套接字 | 7 | socket, bind, listen, accept, connect, send, recv |
| IPC 与环境 | 3 | pipe, getenv, getuid |
| 时间与进程控制 | 4 | clock_gettime, wait, waitpid, posix_spawn |
| I/O 多路复用 | 3 | select, poll, epoll |

## 文档索引

### 设计及使用文档总览

- 项目总览：`README.md`
- 中文正式接口定义文档：`docs/zephyr-linux-api-reference-zh.md`
- 接口设计与内核映射说明：`docs/zephyr-linux-interface-definition.md`
- 接口与验证总览：`docs/zepLinux-interface-and-validation.md`
- 编译开发方法：`docs/zephyr-linux-build-and-dev.md`
- 测试 Demo 文档：`docs/test-demos/`



## 接口文档与验证资料

- 接口与验证总览：`docs/zepLinux-interface-and-validation.md`
- 接口定义文档：`docs/zephyr-linux-interface-definition.md`
- 编译开发方法：`docs/zephyr-linux-build-and-dev.md`
- 测试 Demo 文档：`docs/test-demos/`
- 真实 Demo 目录：`tests/demo/`
- 文档索引页：`docs/zephyr-linux-interface-commits.md`

## License

Apache-2.0
