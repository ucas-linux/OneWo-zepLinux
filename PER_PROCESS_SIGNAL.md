# Per-Process Signal 机制实现

## 概述

本实现将原来的**全局事件广播机制**改造为类似 Linux 的**per-process signal 机制**。

## 原来的实现（全局事件）

### 问题
```c
// 全局共享的中断标志和事件
static volatile bool interrupt_requested = false;
K_EVENT_DEFINE(interrupt_event);

// UART ISR 中广播给所有进程
k_event_post(&interrupt_event, 0x01);  // 所有进程都收到

// 所有进程等待同一个事件
uint32_t events = k_event_wait(&interrupt_event, 0x01, false, K_MSEC(500));
```

### 缺陷
1. ❌ **无选择性**：Ctrl+D 会同时中断所有等待的进程
2. ❌ **无进程隔离**：进程间无法独立管理信号
3. ❌ **无前台/后台区分**：无法只中断前台进程
4. ❌ **不符合 POSIX 语义**：与 Linux 信号模型差异大

---

## 新的实现（Per-Process Signal）

### 核心设计

#### 1. 进程信号状态结构
```c
struct process_signal {
    uint32_t pending_signals;        // 待处理信号位图
    uint32_t blocked_signals;        // 被阻塞信号位图
    struct k_event signal_event;     // 进程专属事件对象
    signal_handler_t handlers[32];   // 信号处理函数
    struct k_mutex lock;             // 保护信号状态
};
```

每个进程在 `struct z_process` 中包含：
```c
struct z_process {
    ...
    struct process_signal *signal_state;  // 进程专属信号状态
};
```

#### 2. 信号定义（POSIX-like）
```c
#define SIGINT    2   // Interrupt (Ctrl+C/Ctrl+D)
#define SIGTERM   15  // Termination
#define SIGKILL   9   // Kill (cannot be caught)
#define SIGUSR1   10  // User-defined signal 1
#define SIGUSR2   12  // User-defined signal 2
```

#### 3. 前台进程组管理
```c
static pid_t foreground_pgid = 0;  // 当前前台进程组ID

void signal_set_foreground_pgid(pid_t pgid);  // 设置前台进程
pid_t signal_get_foreground_pgid(void);       // 获取前台进程
```

### 信号传递流程

```
┌─────────────────────────────────────────────────────────────┐
│ Step 1: UART ISR 检测到 Ctrl+D (ASCII 4)                   │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ Step 2: 查询前台进程组 ID                                   │
│         pid_t fg_pgid = signal_get_foreground_pgid();       │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ Step 3: 只向前台进程发送 SIGINT                             │
│         kill(fg_pgid, SIGINT);                              │
│         • 设置 proc->signal_state->pending_signals |= (1<<2)│
│         • 发送事件到进程专属的 signal_event                 │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ Step 4: 前台进程在 signal_wait() 中被唤醒                   │
│         int sig = signal_wait(K_MSEC(500));                 │
│         • 只有这个进程的 signal_event 被激活                │
│         • 其他进程的 signal_event 不受影响                  │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ Step 5: 调用信号处理函数                                     │
│         if (handler != SIG_IGN && handler != SIG_DFL)       │
│             handler(sig);                                   │
└─────────────────────────────────────────────────────────────┘
```

### 核心 API

#### 发送信号
```c
// 向指定进程发送信号
int kill(pid_t pid, int sig);

// 向进程组发送信号
int killpg(pid_t pgid, int sig);
```

#### 信号处理
```c
// 设置信号处理函数
signal_handler_t signal(int sig, signal_handler_t handler);

// 自定义处理函数
void my_handler(int sig) {
    printk("Received signal %d\n", sig);
}
signal(SIGINT, my_handler);
```

#### 信号阻塞
```c
// 阻塞信号
sigblock(1 << SIGINT);  // 临时阻塞 SIGINT

// 解除阻塞
sigunblock(1 << SIGINT);
```

#### 等待信号
```c
// 在进程主循环中等待信号（带超时）
int sig = signal_wait(K_MSEC(500));
if (sig == SIGINT) {
    printk("Received SIGINT\n");
    break;
}

// 主动检查待处理的信号
signal_check_pending();
```

### 使用示例

#### 示例 1: loop 命令（前台进程）

```c
static void cmd_loop(void)
{
    struct z_process *proc = process_current();

    // 设置为前台进程
    signal_set_foreground_pgid(proc->pid);

    // 安装信号处理函数
    signal(SIGINT, sigint_handler);

    while (1) {
        // 等待信号（per-process）
        int sig = signal_wait(K_MSEC(500));

        if (sig == SIGINT) {
            printk("Loop interrupted by SIGINT\n");
            break;
        }

        printk("Iteration...\n");
    }

    // 清除前台
    signal_set_foreground_pgid(0);
}
```

#### 示例 2: fork 多进程（每个进程独立）

```c
static void *fork_child_main(void *arg)
{
    // 每个子进程有自己的 signal_state
    signal(SIGINT, my_handler);

    for (int i = 0; i < iters; i++) {
        // 等待本进程的信号
        int sig = signal_wait(K_MSEC(500));

        if (sig == SIGINT) {
            printk("[CHILD] Received SIGINT, exiting\n");
            return (void *)-1;
        }

        printk("[CHILD] Iteration %d\n", i);
    }

    return (void *)0;
}
```

## 关键改进

### 1. 进程隔离 ✅
- 每个进程有独立的 `struct process_signal`
- 每个进程有自己的 `k_event signal_event`
- 信号发送到特定进程，不影响其他进程

### 2. 前台/后台区分 ✅
- 只有前台进程接收 Ctrl+D 信号
- 后台进程继续运行不受影响
- 类似 shell 的作业控制

### 3. 信号处理函数 ✅
- 每个进程可以注册自己的信号处理函数
- 支持 SIG_DFL（默认）、SIG_IGN（忽略）、自定义函数
- SIGKILL 和 SIGSTOP 不可捕获

### 4. 信号阻塞 ✅
- 进程可以临时阻塞某些信号
- 支持信号掩码管理
- 解除阻塞时自动处理待处理信号

### 5. POSIX 兼容 ✅
- API 命名符合 POSIX 标准（kill, signal, sigblock）
- 信号编号与 Linux 一致
- 行为语义接近 Linux 信号模型

## 测试场景

### 场景 1: 测试 loop 命令（前台进程中断）
```bash
west build -d build-anl-qemu -t run
anl> loop
[LOOP] Iteration 0 (waiting for per-process signal...)
[LOOP] Iteration 1 (waiting for per-process signal...)
^D                    # 按 Ctrl+D
[ISR] Sending SIGINT to foreground process (PID=2)...
[LOOP] SIGINT received, breaking loop
```

### 场景 2: 测试 fork 多进程（选择性中断）
```bash
anl> fork 3 100       # 启动 3 个子进程，每个运行 100 次迭代
[PARENT] Forking 3 child process(es)
[CHILD 1] Started, PID=3
[CHILD 2] Started, PID=4
[CHILD 3] Started, PID=5

# 设置 CHILD 2 为前台进程，然后按 Ctrl+D
# 只有 CHILD 2 会收到 SIGINT 并退出
# CHILD 1 和 CHILD 3 继续运行
```

### 场景 3: 后台进程不受影响
```bash
anl> fork 2 1000      # 启动 2 个长时间运行的子进程
# 子进程在后台运行
anl> loop             # 启动 loop 作为前台进程
^D                    # 按 Ctrl+D
# 只有 loop 被中断，后台的 fork 子进程继续运行
```

## 与 Linux 信号的对比

| 特性 | Linux | 本实现 | 说明 |
|------|-------|--------|------|
| Per-process signal | ✅ | ✅ | 每进程独立信号状态 |
| 信号处理函数 | ✅ | ✅ | signal(), sigaction() |
| 信号阻塞 | ✅ | ✅ | sigprocmask(), sigblock() |
| 前台/后台进程组 | ✅ | ✅ | 只前台进程接收终端信号 |
| kill() 系统调用 | ✅ | ✅ | 向指定进程发信号 |
| 信号队列 | ✅ | ⚠️ | 简化版（位图，不排队） |
| 实时信号 | ✅ | ❌ | 未实现 |
| sigaction() | ✅ | ❌ | 未实现（有 signal()） |

## 文件结构

```
zephyr/samples/ansilic/anl_loader_qemu/src/
├── signal.h            # 信号 API 定义
├── signal.c            # 信号机制实现
├── main.c              # UART ISR 改为发送 per-process signal
├── shell_process.c     # 创建进程时初始化 signal_state
└── ...

zephyr/include/zephyr/kernel/
└── process.h           # 添加 signal_state 字段
```

## 编译与运行

```bash
# 编译
west build -p always -b qemu_cortex_m3 -d build-anl-qemu \
    zephyr/samples/ansilic/anl_loader_qemu

# 运行
west build -d build-anl-qemu -t run

# 测试
anl> loop
[按 Ctrl+D 中断]

anl> fork 2 50
[观察两个子进程行为]
```

## 总结

本实现成功将全局事件机制改造为 **Linux-like 的 per-process signal 机制**，实现了：

✅ **进程隔离** - 信号只发送到目标进程
✅ **前台控制** - Ctrl+D 只中断前台进程
✅ **信号处理** - 自定义信号处理函数
✅ **信号阻塞** - 灵活的信号掩码管理
✅ **POSIX 兼容** - API 和语义接近 Linux

这为在 Zephyr RTOS 上实现类 POSIX 进程模型提供了重要的信号支持基础设施。
