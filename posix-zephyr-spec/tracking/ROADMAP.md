# POSIX-Zephyr实现路线图

## 如何使用本路线图

本文档描述了如何将**规范文档**（specs/）转化为**实际代码**（implementation/），建立清晰的映射关系。

## 实现流程

### 步骤1：阅读规范文档

每个模块的规范文档位于 `specs/XX-module-name/`，包含：
- `README.md` - 模块概述和核心挑战
- `*.md` - 具体接口的详细规范
- `types.md` - 数据类型定义
- `implementation.md` - 实现指导

**示例**：实现线程管理模块
```bash
# 阅读规范
cat specs/01-thread/README.md
cat specs/01-thread/pthread_create.md
cat specs/01-thread/types.md
cat specs/01-thread/implementation.md
```

### 步骤2：创建实现代码结构

根据规范创建对应的实现文件：

```
implementation/
├── include/
│   └── posix/
│       └── pthread.h          # 公开API头文件
├── src/
│   └── 01-thread/
│       ├── pthread_create.c   # 对应 specs/01-thread/pthread_create.md
│       ├── pthread_mutex.c    # 对应 specs/01-thread/pthread_mutex.md
│       ├── pthread_cond.c     # 对应 specs/01-thread/pthread_cond.md
│       └── internal.h         # 内部实现头文件
└── tests/
    └── 01-thread/
        ├── test_pthread_create.c
        └── test_pthread_mutex.c
```

### 步骤3：实现代码

按照规范文档中的"实现要点"章节编写代码：

**规范文档** (`specs/01-thread/pthread_create.md`) → **实现代码** (`implementation/src/01-thread/pthread_create.c`)

### 步骤4：编写测试

根据规范文档中的"测试用例"章节编写测试代码。

### 步骤5：更新状态

在 `tracking/STATUS.md` 中更新模块状态。

## 模块实现顺序

### 阶段1：基础设施（2-3周）

**目标**：建立核心数据结构和基础API

1. **common/fd_table** - 文件描述符表
   - 规范：无独立规范，参考 specs/06-device/README.md
   - 实现：`implementation/src/common/fd_table.c`
   - 测试：`implementation/tests/common/test_fd_table.c`
   - 依赖：无
   - 输出：统一的FD管理接口

2. **common/pcb** - 进程控制块
   - 规范：specs/02-process/README.md, specs/23-thread-process/
   - 实现：`implementation/src/common/pcb.c`
   - 测试：`implementation/tests/common/test_pcb.c`
   - 依赖：无
   - 输出：PCB数据结构和管理函数

3. **05-memory** - 内存管理
   - 规范：specs/05-memory/
   - 实现：`implementation/src/05-memory/malloc.c`
   - 测试：`implementation/tests/05-memory/test_malloc.c`
   - 依赖：无
   - 输出：malloc/free/realloc

4. **01-thread** - 线程管理（基础）
   - 规范：specs/01-thread/
   - 实现：`implementation/src/01-thread/pthread_*.c`
   - 测试：`implementation/tests/01-thread/test_pthread_*.c`
   - 依赖：05-memory
   - 输出：pthread_create/join/mutex/cond

5. **06-device** - 设备管理（基础）
   - 规范：specs/06-device/
   - 实现：`implementation/src/06-device/open.c`
   - 测试：`implementation/tests/06-device/test_open.c`
   - 依赖：common/fd_table
   - 输出：open/close/read/write

**里程碑M1**：能运行基本的pthread程序

### 阶段2：ELF加载与系统调用（3-4周）

**目标**：能加载并运行静态链接的Linux ELF程序

6. **16-elf-loader** - ELF加载器
   - 规范：specs/16-elf-loader/
   - 实现：`implementation/src/16-elf-loader/parser.c`
   - 测试：`implementation/tests/16-elf-loader/test_parser.c`
   - 依赖：05-memory
   - 输出：elf_load()函数

7. **17-syscall** - 系统调用分发器
   - 规范：specs/17-syscall/
   - 实现：`implementation/src/17-syscall/dispatcher.c`
   - 测试：`implementation/tests/17-syscall/test_syscall.c`
   - 依赖：所有已实现的POSIX接口
   - 输出：SVC处理器和syscall表

8. **23-thread-process** - Thread-as-Process模型
   - 规范：specs/23-thread-process/
   - 实现：`implementation/src/23-thread-process/spawn.c`
   - 测试：`implementation/tests/23-thread-process/test_spawn.c`
   - 依赖：common/pcb, 16-elf-loader
   - 输出：posix_spawn()

9. **02-process** - 进程管理
   - 规范：specs/02-process/
   - 实现：`implementation/src/02-process/exit.c`
   - 测试：`implementation/tests/02-process/test_exit.c`
   - 依赖：23-thread-process
   - 输出：exit/wait/waitpid

**里程碑M2**：能加载并运行简单的静态ELF程序（如hello world）

### 阶段3：完善功能（4-6周）

**目标**：支持更多POSIX接口，提升兼容性

10. **04-signal-timer** - 信号与定时器
11. **26-signals** - 完整信号支持
12. **21-pseudo-fs** - 伪文件系统
13. **11-ipc** - IPC
14. **19-io-multiplex** - I/O多路复用
15. **07-filesystem** - 文件系统
16. **08-directory** - 目录操作
17. **09-fd-ops** - FD操作
18. **22-advanced-file** - 高级文件操作

**里程碑M3**：支持多进程和基本IPC

### 阶段4：集成与优化（2-3周）

**目标**：完整的POSIX兼容层

19. **25-libc** - C库集成
20. **10-network** - 网络Socket
21. **03-scheduling** - 调度
22. **12-environment** - 环境变量
23. **13-user-group** - 用户组
24. **14-time** - 时间管理
25. **15-process-control** - 进程控制
26. **20-mmap** - 内存映射

**里程碑M4**：完整的POSIX兼容层

**里程碑M5**：通过LTP测试套件

## 代码组织规范

### 目录结构

```
implementation/
├── include/
│   ├── posix/              # POSIX标准头文件
│   │   ├── pthread.h
│   │   ├── unistd.h
│   │   ├── fcntl.h
│   │   └── ...
│   └── internal/           # 内部实现头文件
│       ├── fd_table.h
│       ├── pcb.h
│       └── ...
├── src/
│   ├── common/             # 共享基础设施
│   │   ├── fd_table.c
│   │   ├── pcb.c
│   │   └── errno.c
│   ├── 01-thread/
│   ├── 02-process/
│   └── ...
├── tests/
│   ├── unit/               # 单元测试
│   └── integration/        # 集成测试
└── CMakeLists.txt
```

### 文件命名规范

- 头文件：`module_name.h`
- 源文件：`function_name.c` 或 `module_name.c`
- 测试文件：`test_function_name.c`

### 代码注释规范

每个实现文件开头应包含：

```c
/**
 * @file pthread_create.c
 * @brief POSIX pthread_create implementation for Zephyr
 *
 * Specification: specs/01-thread/pthread_create.md
 *
 * Key challenges:
 * - Dynamic stack allocation
 * - Thread exit status preservation
 * - Priority mapping
 */
```

## 追踪与协作

### 认领模块

1. 在 `tracking/STATUS.md` 中填写负责人
2. 创建对应的实现目录
3. 开始编码

### 更新进度

定期更新 `tracking/STATUS.md`：
- 🔴 未开始 → 🟡 进行中
- 🟡 进行中 → 🟢 已完成
- 遇到问题标记 ⚠️

### 代码审查

- 每个模块完成后提交PR
- PR标题：`[ModuleID] Module Name - Brief Description`
- PR描述包含：规范文档链接、实现说明、测试结果

## 参考资源

- 规范文档：`specs/`
- 架构设计：`docs/human-readable/architecture.md`
- 示例代码：`examples/`
- Zephyr文档：https://docs.zephyrproject.org/
