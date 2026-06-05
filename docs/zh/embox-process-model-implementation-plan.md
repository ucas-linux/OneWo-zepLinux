# OneWo-zepLinux 进程模型实现计划

## 项目概述

**目标**：在 OneWo-zepLinux 上实现类似 Embox 的完整进程模型，支持 fork/exec/exit/waitpid 等核心进程操作。

**参考实现**：Embox (`/opt/Program/UCAS/embox/src/kernel/task/`)

**目标平台**：
- ARM Cortex-M3 (STM32F401XE @ 84MHz, 96KB RAM)
- ANSILIC RISC-V 32 (AS32X601)

---

## 阶段一：基础框架设计与实现（2周）

### 1.1 进程控制块设计

**文件**：`zephyr/include/zephyr/kernel/process.h`

```c
struct z_process {
    /* 进程标识 */
    pid_t pid;                          // 进程 ID (1-based)
    pid_t ppid;                         // 父进程 ID
    uint32_t status;                    // 进程状态标志

    /* 关联线程 */
    struct k_thread *main_thread;       // 主线程指针

    /* 进程资源 */
    struct z_process_resources *res;    // 资源指针（模块化）

    /* 进程关系 */
    struct z_process *parent;           // 父进程
    sys_dlist_t children;               // 子进程链表
    sys_dnode_t sibling_node;           // 兄弟节点

    /* 同步原语 */
    struct k_sem exit_sem;              // 退出信号量
    int exit_code;                      // 退出码

    /* 调度信息 */
    int priority;                       // 进程优先级
    uint64_t start_time;                // 启动时间
};
```

**任务**：
- [ ] 定义 `struct z_process` 数据结构
- [ ] 定义进程状态宏（RUNNING, ZOMBIE, STOPPED 等）
- [ ] 设计进程资源结构 `struct z_process_resources`

---

### 1.2 进程表管理

**文件**：`zephyr/kernel/process_table.c` / `process_table.h`

**功能**：
- PID 分配与回收（位图或链表）
- 进程表查找（通过 PID）
- 进程表遍历

**接口**：
```c
/* 初始化进程表 */
void z_process_table_init(void);

/* 分配新 PID */
pid_t z_process_alloc_pid(void);

/* 释放 PID */
void z_process_free_pid(pid_t pid);

/* 通过 PID 查找进程 */
struct z_process *z_process_find(pid_t pid);

/* 注册进程到进程表 */
int z_process_register(struct z_process *proc);

/* 从进程表移除进程 */
void z_process_unregister(struct z_process *proc);
```

**任务**：
- [ ] 实现 PID 位图分配器（支持 1-256）
- [ ] 实现进程表哈希表或数组
- [ ] 实现进程查找和遍历接口
- [ ] 添加自旋锁保护并发访问

---

### 1.3 进程-线程映射

**文件**：`zephyr/kernel/include/kernel_structs.h`

**修改**：扩展 `struct _thread_base` 添加进程指针

```c
struct _thread_base {
    // ... 现有字段 ...

    struct z_process *process;  // 所属进程
};
```

**接口**：
```c
/* 获取当前进程 */
struct z_process *z_process_self(void);

/* 设置线程所属进程 */
void z_thread_set_process(struct k_thread *thread, struct z_process *proc);
```

**任务**：
- [ ] 修改 `_thread_base` 结构
- [ ] 实现 `z_process_self()` 获取当前进程
- [ ] 在线程创建时关联进程

---

### 1.4 内核初始化集成

**文件**：`zephyr/kernel/init.c`

**任务**：
- [ ] 在 `z_cstart()` 中调用 `z_process_table_init()`
- [ ] 创建 PID=1 的内核进程（kernel_task）
- [ ] 将 idle 线程关联到内核进程

---

## 阶段二：资源管理模块实现（2周）

### 2.1 文件描述符表（idesc_table）

**文件**：`zephyr/kernel/process/resource/idesc_table.c`

**数据结构**：
```c
#define IDESC_TABLE_SIZE 16

struct idesc {
    void *file;           // 文件对象指针
    int flags;            // 打开标志
    int refcount;         // 引用计数
};

struct idesc_table {
    struct idesc descriptors[IDESC_TABLE_SIZE];
    uint32_t bitmap;      // 使用位图
    struct k_mutex lock;  // 互斥锁
};
```

**接口**：
```c
/* 创建文件描述符表 */
struct idesc_table *idesc_table_create(void);

/* 销毁文件描述符表 */
void idesc_table_destroy(struct idesc_table *table);

/* 分配文件描述符 */
int idesc_alloc(struct idesc_table *table, void *file, int flags);

/* 释放文件描述符 */
void idesc_free(struct idesc_table *table, int fd);

/* 获取文件对象 */
void *idesc_get_file(struct idesc_table *table, int fd);

/* 复制文件描述符表（fork 使用） */
struct idesc_table *idesc_table_dup(struct idesc_table *src);
```

**任务**：
- [ ] 实现文件描述符分配/释放
- [ ] 实现描述符表复制（fork 时使用）
- [ ] 添加标准输入/输出/错误（0/1/2）支持

---

### 2.2 环境变量管理（environ）

**文件**：`zephyr/kernel/process/resource/environ.c`

**数据结构**：
```c
struct environ {
    char **envp;          // 环境变量数组
    int count;            // 环境变量数量
    int capacity;         // 容量
};
```

**接口**：
```c
/* 创建环境变量表 */
struct environ *environ_create(void);

/* 销毁环境变量表 */
void environ_destroy(struct environ *env);

/* 设置环境变量 */
int environ_set(struct environ *env, const char *name, const char *value);

/* 获取环境变量 */
const char *environ_get(struct environ *env, const char *name);

/* 复制环境变量表 */
struct environ *environ_dup(struct environ *src);
```

**任务**：
- [ ] 实现环境变量存储和查找
- [ ] 实现 `getenv()` / `setenv()` / `unsetenv()`
- [ ] 实现环境变量表复制

---

### 2.3 命令行参数管理（argv）

**文件**：`zephyr/kernel/process/resource/argv.c`

**数据结构**：
```c
struct argv {
    int argc;             // 参数数量
    char **argv;          // 参数数组
};
```

**接口**：
```c
/* 创建参数表 */
struct argv *argv_create(int argc, char **argv);

/* 销毁参数表 */
void argv_destroy(struct argv *args);

/* 复制参数表 */
struct argv *argv_dup(struct argv *src);
```

**任务**：
- [ ] 实现参数存储和访问
- [ ] 实现参数表复制

---

### 2.4 信号表（sig_table）

**文件**：`zephyr/kernel/process/resource/sig_table.c`

**数据结构**：
```c
#define NSIG 32

struct sig_table {
    void (*handlers[NSIG])(int);  // 信号处理函数
    uint32_t pending;             // 待处理信号位图
    uint32_t blocked;             // 阻塞信号位图
};
```

**接口**：
```c
/* 创建信号表 */
struct sig_table *sig_table_create(void);

/* 销毁信号表 */
void sig_table_destroy(struct sig_table *table);

/* 设置信号处理函数 */
void sig_table_set_handler(struct sig_table *table, int signum, void (*handler)(int));

/* 发送信号 */
int sig_table_send(struct sig_table *table, int signum);

/* 复制信号表 */
struct sig_table *sig_table_dup(struct sig_table *src);
```

**任务**：
- [ ] 实现信号表管理
- [ ] 实现信号发送和处理
- [ ] 与现有信号系统集成

---

### 2.5 进程资源整合

**文件**：`zephyr/kernel/process/resource/resource.c`

**数据结构**：
```c
struct z_process_resources {
    struct idesc_table *idesc_table;
    struct environ *environ;
    struct argv *argv;
    struct sig_table *sig_table;
    void *heap_base;
    size_t heap_size;
};
```

**接口**：
```c
/* 创建进程资源 */
struct z_process_resources *z_process_resources_create(void);

/* 销毁进程资源 */
void z_process_resources_destroy(struct z_process_resources *res);

/* 复制进程资源（fork 使用） */
struct z_process_resources *z_process_resources_dup(struct z_process_resources *src);
```

**任务**：
- [ ] 实现资源统一管理接口
- [ ] 实现资源复制（fork 时使用）
- [ ] 实现资源清理（exit 时使用）

---

## 阶段三：进程操作实现（3周）

### 3.1 进程创建（new_process）

**文件**：`zephyr/kernel/process/process.c`

**接口**：
```c
/* 创建新进程 */
struct z_process *z_process_create(const char *name,
                                    void *(*entry)(void *),
                                    void *arg,
                                    size_t stack_size,
                                    int priority);

/* 启动进程 */
int z_process_start(struct z_process *proc);
```

**实现步骤**：
1. 分配 PID
2. 创建进程控制块
3. 创建主线程（使用 `k_thread_create`）
4. 初始化进程资源
5. 建立父子关系
6. 注册到进程表
7. 启动主线程

**任务**：
- [ ] 实现进程创建流程
- [ ] 实现进程启动接口
- [ ] 添加错误处理和资源清理

---

### 3.2 fork() 实现

**文件**：`zephyr/lib/posix/process/fork.c`

**接口**：
```c
pid_t fork(void);
```

**实现策略**：
- **简化版 fork**：不复制完整地址空间（无 MMU）
- 复制进程控制块
- 复制进程资源（文件描述符、环境变量等）
- 创建新线程，从 fork 点继续执行
- 父进程返回子进程 PID，子进程返回 0

**实现步骤**：
1. 获取当前进程
2. 分配新 PID
3. 创建子进程控制块
4. 复制进程资源（`z_process_resources_dup`）
5. 创建子进程主线程
6. 设置线程上下文（返回值为 0）
7. 建立父子关系
8. 启动子进程

**任务**：
- [ ] 实现 fork 核心逻辑
- [ ] 实现线程上下文复制
- [ ] 处理返回值差异（父进程 vs 子进程）
- [ ] 添加 COW（Copy-On-Write）优化（可选）

---

### 3.3 execve() 实现

**文件**：`zephyr/lib/posix/process/exec.c`

**接口**：
```c
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

**实现策略**：
- **简化版 execve**：不从文件系统加载 ELF
- 通过函数指针表查找预注册的程序入口
- 替换当前进程的执行上下文

**数据结构**：
```c
struct exec_program {
    const char *name;
    void *(*entry)(void *);
};

/* 程序注册表 */
extern struct exec_program exec_programs[];
```

**实现步骤**：
1. 查找程序入口（通过 pathname）
2. 保存当前进程 PID
3. 清理当前进程资源
4. 重新初始化资源（新的 argv/envp）
5. 重置线程栈
6. 跳转到新程序入口

**任务**：
- [ ] 实现程序注册表机制
- [ ] 实现 execve 核心逻辑
- [ ] 实现线程栈重置
- [ ] 添加参数和环境变量传递

---

### 3.4 exit() 实现

**文件**：`zephyr/lib/posix/process/exit.c`

**接口**：
```c
void exit(int status);
void _exit(int status);
```

**实现步骤**：
1. 设置进程状态为 ZOMBIE
2. 保存退出码
3. 清理进程资源（文件描述符、内存等）
4. 将子进程过继给 init 进程（PID=1）
5. 通知父进程（`k_sem_give(&parent->exit_sem)`）
6. 终止主线程（`k_thread_abort`）

**任务**：
- [ ] 实现进程退出流程
- [ ] 实现资源清理
- [ ] 实现子进程过继
- [ ] 实现父进程通知

---

### 3.5 waitpid() 实现

**文件**：`zephyr/lib/posix/process/waitpid.c`

**接口**：
```c
pid_t waitpid(pid_t pid, int *status, int options);
```

**实现步骤**：
1. 查找目标子进程
2. 检查进程状态
3. 如果未退出且非 WNOHANG，等待 `exit_sem`
4. 获取退出码
5. 回收子进程资源（释放 PID、进程控制块）
6. 返回子进程 PID

**任务**：
- [ ] 实现 waitpid 核心逻辑
- [ ] 支持 WNOHANG 选项
- [ ] 实现进程回收（zombie reaping）
- [ ] 处理特殊 PID（-1 表示任意子进程）

---

### 3.6 其他进程接口

**文件**：`zephyr/lib/posix/process/pid.c`

**接口**：
```c
pid_t getpid(void);
pid_t getppid(void);
```

**任务**：
- [ ] 实现 `getpid()` - 返回当前进程 PID
- [ ] 实现 `getppid()` - 返回父进程 PID

---

## 阶段四：集成测试与优化（2周）

### 4.1 单元测试

**文件**：`zephyr/tests/kernel/process/`

**测试用例**：
- [ ] 进程表管理测试
- [ ] PID 分配/释放测试
- [ ] 文件描述符表测试
- [ ] 环境变量管理测试
- [ ] 进程创建/销毁测试

---

### 4.2 集成测试

**测试场景**：
1. **基本 fork 测试**
   - 父进程 fork 子进程
   - 验证 PID 不同
   - 验证资源隔离

2. **fork + exit + waitpid 测试**
   - 父进程 fork 子进程
   - 子进程执行任务后 exit
   - 父进程 waitpid 回收子进程

3. **fork + execve 测试**
   - 父进程 fork 子进程
   - 子进程 execve 执行新程序
   - 验证程序切换成功

4. **多进程并发测试**
   - 创建多个子进程
   - 验证调度正确性
   - 验证资源隔离

5. **信号测试**
   - 父进程向子进程发送信号
   - 验证信号处理

**任务**：
- [ ] 编写集成测试用例
- [ ] 在 STM32F401 上运行测试
- [ ] 在 AS32X601 上运行测试
- [ ] 记录测试结果

---

### 4.3 性能测试

**测试指标**：
- 进程创建时间
- fork 时间
- execve 时间
- 上下文切换时间
- 内存占用

**任务**：
- [ ] 编写性能测试程序
- [ ] 测量各项指标
- [ ] 与 Embox 对比
- [ ] 优化热点路径

---

### 4.4 内存优化

**优化方向**：
- 减小进程控制块大小
- 优化资源结构布局
- 实现资源按需分配
- 添加内存池管理

**任务**：
- [ ] 分析内存占用
- [ ] 优化数据结构
- [ ] 实现内存池
- [ ] 验证优化效果

---

### 4.5 文档完善

**文档清单**：
- [ ] 进程模型设计文档
- [ ] API 参考手册
- [ ] 移植指南
- [ ] 测试报告
- [ ] 示例代码

---

## 关键文件清单

### 新增文件

```
zephyr/
├── include/zephyr/kernel/
│   ├── process.h                    # 进程控制块定义
│   └── process_table.h              # 进程表接口
├── kernel/
│   ├── process_table.c              # 进程表实现
│   ├── process/
│   │   ├── process.c                # 进程创建/销毁
│   │   └── resource/
│   │       ├── idesc_table.c        # 文件描述符表
│   │       ├── environ.c            # 环境变量
│   │       ├── argv.c               # 命令行参数
│   │       ├── sig_table.c          # 信号表
│   │       └── resource.c           # 资源管理
├── lib/posix/process/
│   ├── fork.c                       # fork 实现
│   ├── exec.c                       # execve 实现
│   ├── exit.c                       # exit 实现
│   ├── waitpid.c                    # waitpid 实现
│   └── pid.c                        # getpid/getppid 实现
└── tests/kernel/process/
    ├── test_process_table.c
    ├── test_fork.c
    ├── test_exec.c
    └── test_integration.c
```

### 修改文件

```
zephyr/
├── include/zephyr/kernel_structs.h  # 添加 process 指针到 _thread_base
├── kernel/init.c                    # 初始化进程表
├── kernel/CMakeLists.txt            # 添加进程相关源文件
└── lib/posix/CMakeLists.txt         # 添加进程接口源文件
```

---

## 技术难点与解决方案

### 难点 1：无 MMU 环境下的地址空间隔离

**问题**：MCU 无 MMU，无法实现真正的地址空间隔离

**解决方案**：
- 采用软件层面的资源隔离（文件描述符、环境变量等）
- fork 时不复制完整内存，仅复制必要的数据结构
- 依赖 MPU（Memory Protection Unit）提供基本的内存保护（如果硬件支持）

---

### 难点 2：execve 动态加载

**问题**：嵌入式环境下难以实现 ELF 动态加载

**解决方案**：
- 采用静态注册机制，预先注册可执行程序
- 通过函数指针表查找程序入口
- 未来可扩展支持简单的二进制加载器

---

### 难点 3：fork 后的线程上下文复制

**问题**：需要复制父进程的执行上下文到子进程

**解决方案**：
- 使用 Zephyr 的线程创建接口
- 手动设置子进程线程的栈和寄存器状态
- 利用 trampoline 函数统一处理返回值

---

### 难点 4：与现有调度器的集成

**问题**：进程模型需要与现有 Linux 调度器无缝配合

**解决方案**：
- 进程的主线程使用 `k_thread`，自然集成到调度器
- 进程优先级映射到线程优先级
- 进程状态变化时同步更新线程状态

---

## 验收标准

### 功能验收
- [ ] 支持 fork/exec/exit/waitpid 基本流程
- [ ] 支持至少 8 个并发进程
- [ ] 进程间资源正确隔离
- [ ] 父子进程关系正确维护
- [ ] 通过所有单元测试和集成测试

### 性能验收
- [ ] 进程创建时间 < 10ms（STM32F401 @ 84MHz）
- [ ] fork 时间 < 5ms
- [ ] 内存占用：每进程 < 2KB（不含栈和堆）

### 文档验收
- [ ] 完整的设计文档
- [ ] 完整的 API 文档
- [ ] 测试报告
- [ ] 示例代码

---

## 风险管理

| 风险 | 影响 | 概率 | 缓解措施 |
|------|------|------|---------|
| 内存不足 | 高 | 中 | 优化数据结构，按需分配资源 |
| 调度器冲突 | 高 | 低 | 充分测试，确保兼容性 |
| 性能不达标 | 中 | 中 | 性能分析，优化热点路径 |
| 测试覆盖不足 | 中 | 中 | 编写完整测试用例，代码审查 |

---

## 参考资料

1. **Embox 源码**
   - `/opt/Program/UCAS/embox/src/kernel/task/`
   - `/opt/Program/UCAS/embox/src/compat/posix/proc/`

2. **Zephyr 文档**
   - Kernel Services: https://docs.zephyrproject.org/latest/kernel/services/
   - Threads: https://docs.zephyrproject.org/latest/kernel/services/threads/

3. **POSIX 标准**
   - IEEE Std 1003.1-2017
   - fork: https://pubs.opengroup.org/onlinepubs/9699919799/functions/fork.html
   - exec: https://pubs.opengroup.org/onlinepubs/9699919799/functions/exec.html

4. **OneWo-zepLinux 文档**
   - `docs/zh/zephyr-linux-interface-definition.zh.md`
   - `docs/zh/zephyr-linux-api-reference.zh.md`

---

## 附录：代码示例

### 示例 1：基本 fork 测试

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void test_basic_fork(void) {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return;
    }

    if (pid == 0) {
        // 子进程
        printf("Child process: PID=%d, PPID=%d\n", getpid(), getppid());
        exit(0);
    } else {
        // 父进程
        printf("Parent process: PID=%d, Child PID=%d\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", status);
    }
}
```

### 示例 2：fork + execve 测试

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// 预注册的程序
void *child_program(void *arg) {
    printf("Child program running\n");
    return NULL;
}

void test_fork_exec(void) {
    pid_t pid = fork();

    if (pid == 0) {
        // 子进程执行新程序
        char *argv[] = {"child_program", NULL};
        char *envp[] = {NULL};
        execve("child_program", argv, envp);
        // 如果 execve 失败才会到这里
        printf("execve failed\n");
        exit(1);
    } else {
        // 父进程等待
        int status;
        waitpid(pid, &status, 0);
        printf("Child finished\n");
    }
}
```

---

**文档版本**：v1.0
**创建日期**：2026-05-31
**最后更新**：2026-05-31
