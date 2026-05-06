# 在Zephyr RTOS上实现Linux进程调度子系统

## Goal Description

基于《Linux调度子系统深度剖析 v1.2》文档，在单核嵌入式Zephyr RTOS平台上，从零独立实现一套Linux风格的模块化进程调度子系统。该子系统需复现Linux调度器的核心架构思想——模块化调度类、分层优先级、运行队列管理，并在Zephyr线程模型约束下尽可能完整地实现RT（SCHED_FIFO/SCHED_RR）、CFS（基于vruntime的公平调度）、Deadline（EDF/CBS）和Idle四大调度类，以及相关的带宽控制、抢占机制、调度统计和POSIX兼容调度API。

所有代码完全独立编写，不依赖仓库中任何现有代码、规范或存根实现。

## Acceptance Criteria

Following TDD philosophy, each criterion includes positive and negative tests for deterministic verification.

- AC-1: 调度类架构与运行队列管理
  - Positive Tests (expected to PASS):
    - 系统启动后，调度类按 stop > deadline > rt > fair > idle 优先级链顺序注册，遍历调度类链表返回正确顺序
    - 运行队列初始化后，nr_running 为 0，各子队列（cfs_rq, rt_rq, dl_rq）状态正确
    - pick_next_task 从最高优先级调度类开始查找，当RT队列有任务时优先返回RT任务而非CFS任务
  - Negative Tests (expected to FAIL):
    - 尝试注册一个优先级超出范围的调度类，应被拒绝
    - 在运行队列为空时调用 pick_next_task，应返回 idle 任务而非 NULL
  - AC-1.1: 运行队列时钟
    - Positive: update_rq_clock 调用后 rq->clock 单调递增；clock_task 排除中断时间
    - Negative: clock delta 为负值时不更新时钟

- AC-2: RT调度器（SCHED_FIFO / SCHED_RR）
  - Positive Tests:
    - SCHED_FIFO 任务入队后，优先级位图对应位被设置；出队后位图清除
    - 同优先级的多个 SCHED_FIFO 任务按 FIFO 顺序调度（先入队的先运行）
    - SCHED_RR 任务时间片耗尽后自动移到队列末尾，同优先级下一个任务获得CPU
    - 高优先级RT任务唤醒时立即抢占低优先级RT任务
    - sched_rr_get_interval 返回正确的RR时间片值
  - Negative Tests:
    - 设置优先级为 0 或 100 的RT任务，应返回 EINVAL
    - SCHED_FIFO 任务不应因时间片耗尽而被切换（无时间片概念）
  - AC-2.1: RT带宽控制
    - Positive: RT任务累计运行时间达到配额后被限流；下一周期自动恢复
    - Negative: 配额设为 -1（禁用）时，RT任务不应被限流

- AC-3: CFS调度器（基于vruntime的公平调度）
  - Positive Tests:
    - 两个相同nice值的任务长期运行后，各自获得约50%的CPU时间（误差<5%）
    - nice=-5 的任务获得的CPU时间约为 nice=0 任务的3倍（权重比 3121:1024）
    - 新唤醒的任务 vruntime 被设置为接近 min_vruntime，不会垄断CPU
    - CFS红黑树按 vruntime 排序，pick_next_task 返回 vruntime 最小的任务
    - 调度周期随任务数量动态调整：任务少时使用 sched_latency，任务多时按 min_granularity 扩展
  - Negative Tests:
    - nice 值超出 [-20, 19] 范围时，设置应失败
    - 单个CFS任务运行时不应产生不必要的上下文切换
  - AC-3.1: CFS带宽控制
    - Positive: 设置 quota/period 后，任务组用完配额被限流；配额重置后恢复
    - Negative: quota 设为 -1 时不限流；period 设为 0 应返回错误

- AC-4: Deadline调度器（EDF/CBS）
  - Positive Tests:
    - 多个Deadline任务同时就绪时，选择绝对截止时间最早的任务运行
    - 任务在周期内用完 runtime 后被CBS限流，下一周期预算补充后恢复
    - 可调度性检查：∑(Ci/Pi) ≤ 1 时允许任务加入；超过时拒绝
    - Deadline任务优先级高于所有RT和CFS任务
  - Negative Tests:
    - runtime > deadline 的参数设置应被拒绝
    - deadline > period 的参数设置应被拒绝
    - 总利用率超过100%时，新Deadline任务的加入应失败

- AC-5: Idle调度器
  - Positive Tests:
    - 系统无其他可运行任务时，idle任务被选中执行
    - 任何其他调度类的任务就绪时，立即抢占idle任务
  - Negative Tests:
    - idle任务不应出现在普通运行队列的任务计数中

- AC-6: 优先级系统与映射
  - Positive Tests:
    - Linux RT优先级 1-99 正确映射到 Zephyr 优先级空间（FIFO→协作优先级，RR→抢占优先级）
    - nice 值 [-20, 19] 正确映射到 CFS 权重表（40级权重，每级约1.25倍）
    - 双向映射一致：linux_to_zephyr(zephyr_to_linux(p)) == p
  - Negative Tests:
    - 映射超出范围的优先级值应返回错误

- AC-7: POSIX调度API
  - Positive Tests:
    - sched_yield() 让出CPU后，同优先级的下一个任务获得运行机会，返回0
    - sched_setscheduler() 成功切换任务的调度策略（如从CFS切换到RT）
    - sched_getscheduler() 返回任务当前的调度策略
    - sched_setparam() / sched_getparam() 正确设置和获取调度参数
    - sched_get_priority_max/min() 返回各策略的优先级范围
  - Negative Tests:
    - 对无效PID调用调度API应返回 ESRCH
    - 设置不兼容的策略/优先级组合应返回 EINVAL

- AC-8: 抢占机制
  - Positive Tests:
    - 高优先级任务唤醒时，设置 need_resched 标志并在安全点触发调度
    - tick中断中检测到当前任务时间片耗尽时触发重调度
    - Deadline任务唤醒时抢占正在运行的RT任务
  - Negative Tests:
    - 低优先级任务唤醒时不应抢占高优先级任务

- AC-9: 调度统计
  - Positive Tests:
    - 正确记录每个任务的累计运行时间（sum_exec_runtime）
    - 正确统计自愿上下文切换（nvcsw）和非自愿上下文切换（nivcsw）次数
    - 正确记录任务等待时间（wait_sum）和最大等待时间（wait_max）
  - Negative Tests:
    - 任务处于睡眠状态时，运行时间统计不应增长

- AC-10: 编译与集成
  - Positive Tests:
    - 所有调度子系统代码在Zephyr构建系统下编译无错误无警告
    - Ztest单元测试全部通过
    - 调度子系统可通过Kconfig选项独立启用/禁用各调度类
  - Negative Tests:
    - 禁用某调度类后，使用该策略的API调用应返回 ENOSYS

## Path Boundaries

Path boundaries define the acceptable range of implementation quality and choices.

### Upper Bound (Maximum Acceptable Scope)

实现包含完整的五级调度类架构（Stop、Deadline、RT、CFS、Idle），其中RT调度器支持SCHED_FIFO和SCHED_RR（含优先级位图O(1)调度），CFS调度器支持基于vruntime的公平调度（含红黑树、nice权重、动态时间片），Deadline调度器支持EDF/CBS（含可调度性检查和带宽隔离），Idle调度器支持空闲任务管理。包含RT带宽控制、CFS带宽控制、完整的POSIX调度API集合、抢占机制、调度统计，以及覆盖所有调度类的Ztest单元测试。所有代码遵循Linux调度器的模块化设计思想，通过Kconfig可配置。

### Lower Bound (Minimum Acceptable Scope)

实现包含核心调度类架构（调度类链表、运行队列、pick_next_task流程），RT调度器（SCHED_FIFO/SCHED_RR含优先级数组），简化版CFS调度器（vruntime公平调度，可用有序链表替代红黑树），简化版Deadline调度器（基本EDF选择和CBS限流），Idle调度器，Linux-Zephyr优先级双向映射，基本POSIX调度API（sched_yield、sched_setscheduler、sched_getscheduler、sched_setparam、sched_getparam），基本抢占支持，以及核心功能的单元测试。

### Allowed Choices

- Can use: Zephyr内核API（k_thread、k_yield、k_timer等）、Zephyr内置数据结构（sys_dlist、sys_slist、rb_tree）、Zephyr Ztest框架、C99/C11标准、Kconfig配置系统、CMake构建系统
- Can use: 简化的红黑树实现或Zephyr自带的rb_tree替代Linux内核的rb_root_cached
- Can use: Zephyr的k_timer替代Linux的hrtimer实现周期性tick和带宽控制定时器
- Cannot use: 仓库中任何现有的lmz_sched、lmz_process、lmz_thread代码或posix-zephyr-spec规范
- Cannot use: Linux内核源码直接复制（需理解后独立实现）
- Cannot use: 动态内存分配（malloc/free），应使用静态分配或Zephyr内存池
- Cannot use: SMP相关特性（per-CPU运行队列、负载均衡、任务迁移）——目标为单核平台

## Feasibility Hints and Suggestions

> **Note**: This section is for reference and understanding only. These are conceptual suggestions, not prescriptive requirements.

### Conceptual Approach

整体架构采用Linux调度器的模块化调度类设计，适配Zephyr单核环境：

```
┌─────────────────────────────────────────────┐
│              POSIX Scheduling API            │
│  sched_yield / sched_setscheduler / ...      │
├─────────────────────────────────────────────┤
│           Core Scheduler (核心调度器)         │
│  pick_next_task / context_switch / preempt   │
├──────┬──────┬──────┬──────┬────────────────┤
│ Stop │  DL  │  RT  │ CFS  │     Idle       │
│Class │Class │Class │Class │    Class       │
├──────┴──────┴──────┴──────┴────────────────┤
│         Run Queue (单CPU运行队列)            │
│  dl_rq(rb_tree) | rt_rq(bitmap+list) |      │
│  cfs_rq(rb_tree) | idle               │
├─────────────────────────────────────────────┤
│         Zephyr Kernel (k_thread, k_timer)    │
└─────────────────────────────────────────────┘
```

关键实现思路：

1. 调度类抽象：定义 `struct sched_class` 接口（enqueue、dequeue、pick_next、put_prev、check_preempt、task_tick等），各调度类实现该接口，通过优先级链表串联。

2. 运行队列：单个全局 `struct rq`，内嵌 `cfs_rq`、`rt_rq`、`dl_rq` 子队列。使用Zephyr的自旋锁保护。

3. RT调度器：使用位图（bitmap）+ 链表数组实现O(1)调度。100个优先级级别，每级一个双向链表。SCHED_RR额外维护时间片计数器。

4. CFS调度器：使用Zephyr的 `struct rb_tree` 按vruntime排序。维护 min_vruntime 防止新任务垄断。nice值通过权重表（40级，每级1.25倍）转换为权重。动态计算时间片。

5. Deadline调度器：使用红黑树按绝对deadline排序。CBS机制跟踪每个任务的runtime预算，耗尽后限流。可调度性检查确保总利用率≤100%。

6. 调度tick：使用Zephyr k_timer实现周期性tick（如1ms），在tick回调中调用 task_tick 更新vruntime、检查时间片、处理带宽控制。

7. 抢占：通过 need_resched 标志和Zephyr的 k_thread_priority_set 实现。高优先级任务唤醒时设置标志，在tick或系统调用返回时检查并触发调度。

8. 优先级映射：Linux RT 1-99 映射到 Zephyr 协作/抢占优先级空间；CFS任务使用固定的低优先级运行，由CFS调度器内部管理公平性。

### Relevant References

- `docs/Linux调度子系统深度剖析 v1.2.docx` - 完整的Linux调度器参考文档
- Zephyr内核API: `k_thread_create`, `k_yield`, `k_timer_start`, `k_thread_priority_set`
- Zephyr数据结构: `sys_dlist`（双向链表）, `struct rb_tree`（红黑树）, `sys_bitarray`（位数组）
- Zephyr Ztest框架: `ztest_test_suite`, `zassert_*` 宏

## Dependencies and Sequence

### Milestones

1. Milestone 1: 核心调度框架
   - Phase A: 定义核心数据结构 — `struct rq`、`struct sched_class`、`struct sched_entity`、`struct task_struct`（调度相关字段）、优先级系统和权重表
   - Phase B: 实现运行队列管理 — 初始化、时钟更新、任务入队/出队的公共路径（activate_task / deactivate_task）
   - Phase C: 实现调度类注册和遍历机制 — 调度类优先级链表、pick_next_task 核心循环
   - Phase D: 实现 Idle 调度类 — 作为兜底调度类，确保系统始终有任务可运行

2. Milestone 2: RT调度器
   - Phase A: 实现优先级数组 + 位图数据结构（rt_prio_array）
   - Phase B: 实现 RT 调度类接口 — enqueue_task_rt、dequeue_task_rt、pick_next_task_rt、check_preempt_rt
   - Phase C: 实现 SCHED_FIFO 调度逻辑
   - Phase D: 实现 SCHED_RR 时间片管理（task_tick_rt 中递减时间片，耗尽后重新入队）
   - Phase E: 实现 RT 带宽控制（周期/配额模型，限流与恢复）

3. Milestone 3: CFS调度器
   - Phase A: 实现 CFS 红黑树管理（按 vruntime 排序的任务组织）
   - Phase B: 实现 vruntime 计算与更新（update_curr、calc_delta_fair、权重表查询）
   - Phase C: 实现 CFS 调度类接口 — enqueue_task_fair、dequeue_task_fair、pick_next_task_fair
   - Phase D: 实现动态时间片计算（sched_latency、min_granularity）
   - Phase E: 实现 CFS 抢占逻辑（唤醒抢占、tick 抢占）
   - Phase F: 实现 CFS 带宽控制（quota/period 模型）

4. Milestone 4: Deadline调度器
   - Phase A: 实现 Deadline 红黑树管理（按绝对 deadline 排序）
   - Phase B: 实现 EDF 调度逻辑 — pick_next_task_dl 选择 deadline 最早的任务
   - Phase C: 实现 CBS 带宽隔离 — runtime 预算跟踪、限流、周期补充
   - Phase D: 实现可调度性检查 — 准入控制（∑Ci/Pi ≤ 1）

5. Milestone 5: 抢占与上下文切换
   - Phase A: 实现 need_resched 标志机制
   - Phase B: 实现调度 tick 处理（k_timer 回调 → task_tick → 检查抢占）
   - Phase C: 实现 schedule() 主函数 — 选择下一个任务、执行上下文切换（通过 Zephyr k_thread API）
   - Phase D: 实现唤醒路径中的抢占检查（check_preempt_curr）

6. Milestone 6: POSIX调度API与优先级映射
   - Phase A: 实现 Linux ↔ Zephyr 双向优先级映射函数
   - Phase B: 实现 sched_yield、sched_setscheduler、sched_getscheduler
   - Phase C: 实现 sched_setparam、sched_getparam、sched_get_priority_max/min
   - Phase D: 实现 sched_setattr / sched_getattr（用于 Deadline 参数设置）

7. Milestone 7: 调度统计与Kconfig集成
   - Phase A: 实现调度统计收集（运行时间、等待时间、上下文切换计数）
   - Phase B: 实现 Kconfig 配置选项（各调度类的启用/禁用开关）
   - Phase C: CMake 构建集成

8. Milestone 8: 测试
   - Phase A: RT 调度器单元测试（FIFO 顺序、RR 轮转、优先级抢占、带宽控制）
   - Phase B: CFS 调度器单元测试（vruntime 公平性、nice 权重、动态时间片、带宽控制）
   - Phase C: Deadline 调度器单元测试（EDF 选择、CBS 限流、可调度性检查）
   - Phase D: 集成测试（跨调度类抢占、策略切换、API 完整性）

### 依赖关系

- Milestone 1 是所有后续里程碑的基础，必须首先完成
- Milestone 2、3、4 依赖 Milestone 1，但三者之间相互独立，可并行开发
- Milestone 5 依赖 Milestone 1，并需要至少一个调度类（Milestone 2 或 3）来验证
- Milestone 6 依赖 Milestone 1-4（需要所有调度类就绪才能实现完整的策略切换API）
- Milestone 7 依赖 Milestone 1-6
- Milestone 8 依赖对应的功能里程碑完成

## Implementation Notes

### Code Style Requirements
- Implementation code and comments must NOT contain plan-specific terminology such as "AC-", "Milestone", "Step", "Phase", or similar workflow markers
- These terms are for plan documentation only, not for the resulting codebase
- Use descriptive, domain-appropriate naming in code instead

--- Original Design Draft Start ---

按照 'Linux调度子系统深度剖析 v1.2.docx' 尽量在zephyr上实现Linux的进程调度，注意！不要依赖仓库中现有的内容！
--- Original Design Draft End ---
