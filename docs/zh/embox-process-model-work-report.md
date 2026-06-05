# OneWo-zepLinux 进程模型实现工作汇报

## 一、项目背景

### 1.1 当前状态
OneWo-zepLinux 是基于 Zephyr RTOS 的 Linux 兼容层项目，目前已实现：
- **调度器层面**：4 种 Linux 风格调度器（DL/RT/CFS/Idle）深度集成到 Zephyr 内核
- **接口层面**：39 个 Linux/POSIX 兼容接口，覆盖线程、进程、调度、信号、内存、设备和 I/O 多路复用
- **目标平台**：ARM Cortex-M3 (STM32F401) 和 ANSILIC RISC-V 32 (AS32X601)

### 1.2 存在的问题
当前实现主要基于 **线程模型**（pthread），缺乏完整的 **进程模型**支持：
- `fork()`, `execve()`, `exit()` 等进程管理接口仅为占位实现
- 缺少进程资源隔离机制（独立地址空间、文件描述符表、环境变量等）
- 无法支持真正的多进程应用场景

### 1.3 参考目标
Embox 是一个模块化的嵌入式操作系统，实现了完整的进程模型：
- **进程抽象**：`struct task` 作为进程控制块，包含 PID、状态、优先级、资源等
- **资源管理**：模块化的资源系统（idesc_table、env、argv、vfork、mmap 等）
- **进程操作**：完整的 fork/exec/exit/waitpid 实现
- **POSIX 兼容**：提供标准的 POSIX 进程接口

## 二、工作目标

在 OneWo-zepLinux 上实现类似 Embox 的进程模型，使其能够：

1. **支持真正的进程抽象**
   - 每个进程拥有独立的 PID 和进程控制块
   - 进程间资源隔离（文件描述符、环境变量、内存空间）
   - 进程生命周期管理（创建、运行、退出、回收）

2. **实现核心进程接口**
   - `fork()` - 创建子进程
   - `execve()` - 加载并执行新程序
   - `exit()` / `_exit()` - 进程退出
   - `waitpid()` - 等待子进程结束
   - `getpid()` / `getppid()` - 获取进程 ID

3. **建立进程资源管理框架**
   - 文件描述符表（idesc_table）
   - 环境变量（env）
   - 命令行参数（argv）
   - 信号表（sig_table）
   - 内存映射（mmap）

4. **保持与现有系统的兼容性**
   - 与 Zephyr 线程模型共存
   - 与现有 Linux 调度器集成
   - 不破坏已有的 pthread 接口

## 三、技术方案概述

### 3.1 架构设计

```
┌─────────────────────────────────────────────────────────┐
│              POSIX Process API Layer                    │
│  fork() execve() exit() waitpid() getpid() getppid()   │
└────────────────────┬────────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────────┐
│           Process Management Layer                      │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐ │
│  │ Process Table│  │ Resource Mgr │  │ Process Ops  │ │
│  │  (PID Pool)  │  │  (Modular)   │  │ (fork/exec)  │ │
│  └──────────────┘  └──────────────┘  └──────────────┘ │
└────────────────────┬────────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────────┐
│         Zephyr Kernel Integration Layer                 │
│  ┌──────────────────────────────────────────────────┐  │
│  │  struct z_process (extends k_thread)             │  │
│  │  - pid, ppid, status                             │  │
│  │  - resource pointers (idesc, env, argv, etc.)    │  │
│  │  - main_thread (k_thread *)                      │  │
│  └──────────────────────────────────────────────────┘  │
└────────────────────┬────────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────────┐
│         Zephyr RTOS (k_thread + Linux Scheduler)        │
└─────────────────────────────────────────────────────────┘
```

### 3.2 核心数据结构

```c
struct z_process {
    /* 进程标识 */
    pid_t pid;                    // 进程 ID
    pid_t ppid;                   // 父进程 ID
    int status;                   // 进程状态

    /* 关联的主线程 */
    struct k_thread *main_thread; // Zephyr 线程对象

    /* 进程资源（模块化设计） */
    struct idesc_table *idesc_table;  // 文件描述符表
    char **environ;                   // 环境变量
    char **argv;                      // 命令行参数
    struct sig_table *sig_table;      // 信号表
    void *heap_base;                  // 堆内存基址

    /* 进程关系 */
    struct z_process *parent;         // 父进程指针
    sys_dlist_t children;             // 子进程链表
    sys_dnode_t sibling_node;         // 兄弟进程节点

    /* 同步与等待 */
    struct k_sem exit_sem;            // 退出信号量
    int exit_code;                    // 退出码
};
```

### 3.3 实现策略

**阶段一：基础框架**
- 定义 `struct z_process` 数据结构
- 实现进程表和 PID 分配机制
- 建立进程-线程映射关系

**阶段二：资源管理**
- 实现文件描述符表（idesc_table）
- 实现环境变量管理（environ）
- 实现命令行参数管理（argv）

**阶段三：进程操作**
- 实现 `fork()` - 复制当前进程
- 实现 `execve()` - 加载新程序（简化版）
- 实现 `exit()` / `waitpid()` - 进程退出与回收

**阶段四：集成与测试**
- 与 Zephyr 调度器集成
- 编写测试用例验证功能
- 性能优化和内存占用优化

## 四、预期成果

1. **功能完整性**
   - 支持基本的进程创建、执行、退出流程
   - 实现进程间资源隔离
   - 通过 POSIX 进程接口测试套件

2. **性能指标**
   - 进程创建时间 < 10ms（在 STM32F401 @ 84MHz）
   - 内存开销：每进程 < 2KB（不含栈和堆）
   - 支持至少 8 个并发进程

3. **文档交付**
   - 进程模型设计文档
   - API 参考手册
   - 测试报告和示例代码

## 五、技术挑战与风险

### 5.1 主要挑战
1. **内存限制**：MCU 内存有限，需要精简进程控制块和资源结构
2. **无 MMU**：无法实现真正的地址空间隔离，需要软件层面的保护
3. **调度器集成**：需要确保进程模型与现有 Linux 调度器无缝配合
4. **execve 实现**：嵌入式环境下动态加载程序较为复杂

### 5.2 风险缓解
- 采用模块化设计，资源按需配置
- 参考 Embox 的成熟实现，降低设计风险
- 分阶段实现，每阶段独立验证
- 保持与现有代码的兼容性，避免破坏性修改

## 六、时间规划

| 阶段 | 工作内容 | 预计时间 |
|------|---------|---------|
| 阶段一 | 基础框架设计与实现 | 2 周 |
| 阶段二 | 资源管理模块实现 | 2 周 |
| 阶段三 | 进程操作实现 | 3 周 |
| 阶段四 | 集成测试与优化 | 2 周 |
| **总计** | | **9 周** |

## 七、参考资料

1. Embox 源码：`/opt/Program/UCAS/embox/src/kernel/task/`
2. Zephyr 内核文档：https://docs.zephyrproject.org/latest/kernel/
3. POSIX 标准：IEEE Std 1003.1-2017
4. OneWo-zepLinux 现有文档：`docs/zh/zephyr-linux-interface-definition.zh.md`
