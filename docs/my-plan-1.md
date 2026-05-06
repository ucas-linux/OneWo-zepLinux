# 最小可运行调度框架 — RT, CFS, Deadline

基于 my-plan.md 的简化版本。目标：在 /opt/Program/UCAS/zephyrproject 中实现一个最小可编译运行的调度框架，包含 RT、CFS、Deadline 三个调度类的核心功能。不追求完整性，只求能跑起来验证架构。

代码在 /opt/Program/UCAS/zephyrproject 中独立实现，不依赖本仓库现有代码。

## Acceptance Criteria

- AC-1: 核心框架可编译运行
  - struct sched_class 接口定义完整（enqueue, dequeue, pick_next, put_prev, task_tick）
  - 单全局 struct rq 含 rt_rq, cfs_rq, dl_rq 子队列
  - pick_next_task 按 dl > rt > cfs > idle 优先级遍历调度类
  - idle 任务作为兜底，系统始终有任务可运行

- AC-2: RT 调度类最小实现
  - SCHED_FIFO: 优先级数组+位图，O(1) 选择最高优先级任务
  - SCHED_RR: 时间片耗尽后移到队列末尾
  - 高优先级 RT 任务抢占低优先级

- AC-3: CFS 调度类最小实现
  - 基于 vruntime 的公平调度（可用有序链表替代红黑树）
  - nice 值通过权重表影响 vruntime 增长速率
  - pick_next 返回 vruntime 最小的任务

- AC-4: Deadline 调度类最小实现
  - EDF: 选择绝对 deadline 最早的任务
  - CBS: runtime 预算耗尽后限流，周期补充
  - 基本可调度性检查 ∑(Ci/Pi) ≤ 1

- AC-5: 基本集成验证
  - Zephyr 下编译通过（west build 无错误）
  - 至少一个简单的 Ztest 验证 pick_next_task 的调度类优先级顺序
  - 能在 QEMU (qemu_cortex_m3) 上运行并输出调度日志

## Path Boundaries

### Upper Bound
核心框架 + RT(FIFO/RR) + CFS(vruntime) + DL(EDF/CBS) + idle + 基本 Ztest。不含带宽控制、POSIX API、调度统计。

### Lower Bound
核心框架 + RT(仅FIFO) + CFS(简化vruntime，链表) + DL(仅EDF选择) + idle + 编译通过。

### Allowed Choices
- 可用: Zephyr API (k_thread, k_timer, sys_dlist, rb_tree), Ztest, C99, CMake/Kconfig
- 可用: 有序链表替代红黑树简化实现
- 不可用: 本仓库现有代码, Linux 内核源码直接复制, malloc/free, SMP 特性

## Milestones

1. Milestone 1: 核心数据结构与框架
   - 定义 task_struct(调度相关字段), sched_class, sched_entity, rq
   - 实现 rq 初始化, pick_next_task 遍历, idle 调度类
   - CMake/Kconfig 项目骨架，确保 west build 通过

2. Milestone 2: RT 调度类
   - 优先级位图 + 链表数组
   - enqueue/dequeue/pick_next for SCHED_FIFO
   - SCHED_RR 时间片管理
   - task_tick_rt 中检查时间片

3. Milestone 3: CFS 调度类
   - vruntime 计算（权重表, calc_delta_fair）
   - 有序链表或 rb_tree 按 vruntime 排序
   - enqueue/dequeue/pick_next for CFS
   - min_vruntime 维护

4. Milestone 4: Deadline 调度类
   - 按 deadline 排序的数据结构
   - EDF pick_next 选择
   - CBS runtime 跟踪与限流
   - 简单可调度性检查

5. Milestone 5: 集成与验证
   - schedule() 主函数，调度 tick (k_timer)
   - need_resched 标志与抢占触发
   - Ztest 验证调度类优先级顺序
   - QEMU 运行验证

### 依赖关系
- M1 → M2, M3, M4 (M2/M3/M4 可并行)
- M1 + 至少一个调度类 → M5

## Implementation Notes

### Code Style
- 代码和注释中不使用 AC-、Milestone、Phase 等计划术语
- 使用描述性的领域命名

--- Original Design Draft Start ---
my-plan.md实现的目标太大了，先尽可能简单的实现一个可以试运行的框架。minimal rt, cfs, and deadline。代码在/opt/Program/UCAS/zephyrproject
--- Original Design Draft End ---
