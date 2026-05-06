# 进程管理接口规范

## 概述

本模块定义3个进程管理接口在Zephyr RTOS上的实现规范，并明确Linux syscall/ABI兼容的前置条件与退化行为。

## 接口列表

1. `fork` - 创建子进程
2. `exec` - 执行新程序
3. `exit` - 终止进程

## 核心挑战

### 1. 进程隔离与ABI前提
**问题**：Linux ABI要求进程隔离与地址空间管理；Cortex-M3无MMU无法提供这些能力。

**解决方案**：
- **MMU目标**：支持受限的进程语义与ELF装载；实现Linux ABI所需syscall子集。
- **无MMU目标**：`fork/exec`等返回`ENOSYS`或`ENOEXEC`；仅保留线程级兼容与部分IPC。

### 2. fork语义
**问题**：无MMU无法实现Linux `fork`语义（地址空间复制+COW）。

**解决方案**：
- 无MMU：`fork()`直接返回`-1`并设置`errno=ENOSYS`。
- MMU：实现受限`fork`（可不支持COW，但必须保证父/子地址空间隔离）。

### 3. exec语义
**问题**：Linux ABI要求加载**Linux ELF**并替换进程映像。

**解决方案**：
- 必须使用**Linux ABI兼容的ELF装载器**；LLEXT并不等价于Linux ELF。
- 无MMU：通常不支持`exec`，返回`ENOEXEC`或`ENOSYS`。

### 4. 进程资源管理
**问题**：单地址空间下需要显式管理进程资源。

**解决方案**：
- 维护进程控制块（PCB）表
- 每个进程有独立的资源跟踪表
- exit时遍历并释放所有资源
- 父进程wait时回收子进程PCB

## 数据结构

```c
// 进程ID类型
typedef int pid_t;

// 进程状态
enum process_state {
    PROC_RUNNING,
    PROC_ZOMBIE,
    PROC_TERMINATED
};

// 进程控制块
typedef struct process_control_block {
    pid_t pid;                          // 进程ID
    pid_t ppid;                         // 父进程ID
    enum process_state state;           // 进程状态
    int exit_status;                    // 退出状态

    struct k_thread *main_thread;       // 主线程
    struct k_mem_domain mem_domain;     // MPU内存域

    void *heap_start;                   // 堆起始地址
    size_t heap_size;                   // 堆大小
    void *stack_start;                  // 栈起始地址
    size_t stack_size;                  // 栈大小

    struct k_sem wait_sem;              // wait同步信号量
    int wait_count;                     // 等待的父进程数

    resource_entry_t *resources;        // 资源链表
    struct process_control_block *next; // PCB链表
} pcb_t;
```

## 实现优先级

1. **高优先级**：exit（基础功能，其他依赖它）
2. **中优先级**：fork（仅MMU目标）
3. **低优先级**：exec（需Linux ELF装载器）

## 测试要求

- fork创建子进程，验证返回值（仅MMU目标）
- 父子进程内存隔离（仅MMU目标）
- 子进程修改变量不影响父进程
- exit正确设置退出状态
- wait/waitpid正确获取子进程状态
- exec加载并执行新程序（仅Linux ELF装载器可用时）
- 资源泄漏检测

## 性能考虑

- fork全量拷贝开销大（数百微秒到毫秒级）
- 建议使用线程而非进程进行并发
- exec动态加载需要文件系统支持
- MPU配置切换有一定开销

## 限制

1. 无MMU目标不支持`fork/exec`
2. MMU目标进程数量受限于RAM（建议<8个）
3. exec需要Linux ABI兼容ELF装载器
4. 不支持进程组、会话等高级特性
5. 信号处理简化
