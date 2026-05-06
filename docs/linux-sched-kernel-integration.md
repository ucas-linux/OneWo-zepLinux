# Linux 风格调度器深度集成 Zephyr 内核 — 技术总结

## 1. 目标

将独立的 linux_sched 测试用例（基于 `task_struct` 模拟调度）升级为 Zephyr 内核的原生调度后端，真正控制 `k_thread` 的调度决策。最终实现 DL(EDF/CBS)、RT(FIFO/RR)、CFS(vruntime)、Idle 四类调度算法作为 Zephyr 第 4 种可插拔就绪队列。

## 2. 方案选型

Zephyr 已有 3 种就绪队列实现（SIMPLE / SCALABLE / MULTIQ），通过 `_priq_run_*` 宏抽象。我们新增第 4 种 `CONFIG_SCHED_LINUX`，在这层抽象内实现 class-based dispatch，无需修改 Zephyr 调度器核心逻辑（`sched.c`）。

关键决策：
- 复用 `k_thread->base.qnode_dlist` 作为子队列节点（线程同一时刻只在一个队列中）
- 在 `_thread_base` 中内嵌调度类元数据，避免额外的 task_struct 映射
- 通过 `z_sched_prio_cmp()` 覆盖实现跨类优先级比较

## 3. 实施步骤

### Phase 1：内核数据结构扩展

**文件：`include/zephyr/kernel_structs.h`**

新增 `struct _priq_linux`，包含：
- DL 子队列：`sys_dlist_t dl_tasks` + `dl_total_bw`（20.12 定点带宽）
- RT 子队列：`sys_dlist_t rt_queue[100]` + `uint32_t rt_bitmap[4]`
- CFS 子队列：`sys_dlist_t cfs_tasks` + `cfs_min_vruntime`
- 单调时钟：`uint64_t clock_ns`

在 `struct _ready_q` 的 `#elif` 链中添加 `CONFIG_SCHED_LINUX` 分支。

**文件：`include/zephyr/kernel/thread.h`**

在 `_thread_base` 中添加：
```c
uint8_t  linux_sched_class;   // 0=DL, 1=RT, 2=CFS, 3=IDLE
uint8_t  linux_policy;        // FIFO/RR/NORMAL/DEADLINE/IDLE
int8_t   linux_nice;          // -20..19
uint64_t vruntime;            // CFS
uint64_t cfs_sum_exec;
uint8_t  rt_prio;             // 1-99
int16_t  rr_time_slice;
uint64_t dl_deadline/period/runtime/runtime_remaining/period_start;
bool     dl_throttled;
```

**文件：`kernel/thread.c`**

在 `z_setup_new_thread()` 中添加默认初始化：所有新线程默认为 CFS 类（class=2, policy=NORMAL, nice=0）。这一步至关重要——因为 `_thread_base` 零初始化时 `linux_sched_class=0` 对应 DL 类，会导致系统线程被错误分类。

### Phase 2：Kconfig + 构建系统

**文件：`kernel/Kconfig`**

在 `choice SCHED_ALGORITHM` 中添加：
```kconfig
config SCHED_LINUX
    bool "Linux-style class-based scheduler (DL/RT/CFS/Idle)"
```

**文件：`kernel/include/priority_q.h`**

添加前向声明 + 宏定义：
```c
#elif defined(CONFIG_SCHED_LINUX)
void z_priq_linux_init(struct _priq_linux *pq);
void z_priq_linux_add(struct _priq_linux *pq, struct k_thread *thread);
// ... 其余声明
#define _priq_run_init    z_priq_linux_init
#define _priq_run_add     z_priq_linux_add
// ... 其余宏
```

覆盖 `z_sched_prio_cmp()`：`CONFIG_SCHED_LINUX` 下按调度类优先级比较（DL > RT > CFS > IDLE），同类内按各自规则（deadline / rt_prio / vruntime）。

注意：必须在宏定义前提供前向声明，否则 `sched.c` 编译时会产生隐式声明警告（函数定义在 .c 文件中，但 priority_q.h 是 inline header）。

**文件：`kernel/CMakeLists.txt`**

```cmake
kernel_sources_ifdef(CONFIG_SCHED_LINUX
  linux_sched/linux_sched_core.c
  linux_sched/linux_sched_dl.c
  linux_sched/linux_sched_rt.c
  linux_sched/linux_sched_fair.c
)
```

### Phase 3：调度类实现

新建 `kernel/linux_sched/` 目录，从原 `linux_sched/src/` 移植算法逻辑。

**核心改动：`task_struct` → `k_thread->base.linux_*`**

| 原始代码 | 内核集成后 |
|----------|-----------|
| `p->dl.deadline` | `thread->base.dl_deadline` |
| `p->rt.prio` | `thread->base.rt_prio` |
| `p->se.vruntime` | `thread->base.vruntime` |
| `p->dl.run_node` | `thread->base.qnode_dlist`（复用） |
| `p->se.run_node` | `thread->base.qnode_dlist`（复用） |
| `p->rt.run_node` | `thread->base.qnode_dlist`（复用） |

**`linux_sched_core.c`** — priq 接口实现：
- `z_priq_linux_init()`：初始化所有子队列
- `z_priq_linux_add()`：根据 `linux_sched_class` 分发到对应 enqueue
- `z_priq_linux_remove()`：根据 `linux_sched_class` 分发到对应 dequeue
- `z_priq_linux_best()`：DL → RT → CFS 顺序选取，返回 NULL 则 Zephyr 选 idle
- `z_priq_linux_yield()`：CFS 重新插入排序，RT 移到队尾
- `z_linux_sched_tick()`：分发到各类的 tick 处理

**`linux_sched_dl.c`** — DL 类：
- EDF 排序插入（deadline 升序）
- CBS 预算消耗（每 tick 减 `tick_ns`）
- 节流（budget=0 时 dequeue）+ 周期补充（elapsed >= period 时 replenish）
- 准入控制（`dl_total_bw + new_bw <= 1.0`，20.12 定点）

**`linux_sched_rt.c`** — RT 类：
- Bitmap O(1) 最高优先级查找（从高 word 向低扫描，`__builtin_clz`）
- FIFO：头部选取，无时间片
- RR：`rr_time_slice` 递减，到期 dequeue + enqueue 到队尾

**`linux_sched_fair.c`** — CFS 类：
- vruntime 排序插入（升序 dlist）
- `vruntime += delta * 1024 / weight`（nice → weight 查表）
- `min_vruntime` 维护（新任务从 min_vruntime 起步，防饥饿）
- tick 时重新排序，检查是否需要抢占

### Phase 4：定时器 Tick 集成

**文件：`kernel/timeslicing.c`**

在 `z_time_slice()` 的 `slice_expired` 分支中插入：
```c
#ifdef CONFIG_SCHED_LINUX
    z_linux_sched_tick(curr);
#endif
```

位于 `CONFIG_TIMESLICE_PER_THREAD` handler 之前，确保每次 timeslice 到期时执行 class-specific 逻辑。

### Phase 5：LMZ 兼容 API

**文件：`code/include/lmz_sched.h` + `code/src/lmz_sched.c`**

新增 API：
- `lmz_sched_setscheduler()` / `lmz_sched_getscheduler()`
- `lmz_sched_setattr_dl()`
- `lmz_nice()` / `lmz_setpriority()` / `lmz_getpriority()`

内部直接操作 `k_thread->base.linux_*` 字段，然后 `k_yield()` 触发重新调度。`CONFIG_SCHED_LINUX` 未启用时返回 `-ENOSYS`。

### Phase 6：测试应用

新建 `linux_sched_kernel/`，使用真实 Zephyr 线程验证：

| 测试用例 | 验证内容 |
|----------|----------|
| `test_class_priority_order` | DL 线程先于 RT，RT 先于 CFS |
| `test_rt_fifo_priority` | RT 高优先级先运行 |
| `test_cfs_vruntime_order` | 低 vruntime 先运行 |
| `test_thread_metadata` | 字段设置正确性 |

## 4. 踩坑记录

1. **零初始化陷阱**：`_thread_base` 零初始化时 `linux_sched_class=0` 对应 DL 类，导致所有系统线程（包括 idle、main）被当作 DL 任务。解决：在 `z_setup_new_thread()` 中显式初始化为 CFS。

2. **隐式声明警告**：`priority_q.h` 是 inline header，被 `sched.c` 通过 `ksched.h` 包含。如果只定义宏而不提供前向声明，编译器会报隐式声明警告（函数实际定义在 .c 文件中）。解决：在宏定义前添加 `void z_priq_linux_add(...)` 等前向声明。

3. **qnode_dlist 复用**：线程同一时刻只在一个就绪队列中，因此 DL/RT/CFS 三个子队列可以共用 `base.qnode_dlist` 节点，无需额外的 `sys_dnode_t` 字段。

4. **QEMU PID 文件锁**：`west build -t run` 启动 QEMU 后如果未正常退出，PID 文件会残留导致下次启动失败。需要 `rm -f build/qemu.pid && pkill qemu` 清理。

## 5. 资源占用

| 指标 | linux_sched（算法测试） | linux_sched_kernel（内核集成） |
|------|------------------------|-------------------------------|
| FLASH | 27.6 KB | 26.2 KB |
| RAM | 7.7 KB | 14.0 KB |
| 测试数 | 11 | 4 |

RAM 增加主要来自 `_priq_linux` 结构体（RT 100 个 dlist 头 + bitmap）和线程元数据字段。

## 6. 修改文件清单

### Zephyr 内核（已有文件修改）
- `include/zephyr/kernel_structs.h`
- `include/zephyr/kernel/thread.h`
- `kernel/include/priority_q.h`
- `kernel/Kconfig`
- `kernel/thread.c`
- `kernel/timeslicing.c`
- `kernel/CMakeLists.txt`

### Zephyr 内核（新建文件）
- `kernel/linux_sched/linux_sched.h`
- `kernel/linux_sched/linux_sched_core.c`
- `kernel/linux_sched/linux_sched_dl.c`
- `kernel/linux_sched/linux_sched_rt.c`
- `kernel/linux_sched/linux_sched_fair.c`

### LMZ 兼容层
- `code/include/lmz_sched.h`（重写）
- `code/src/lmz_sched.c`（重写）

### 测试应用（新建）
- `linux_sched_kernel/CMakeLists.txt`
- `linux_sched_kernel/prj.conf`
- `linux_sched_kernel/src/main.c`
