# 调度测试

本文档描述用于验证 Zephyr 上 Linux 风格调度接口的测试程序。

---

## 1. 内核调度 API 测试

**路径**：`zephyr/tests/kernel/sched/schedule_api/`

这些测试验证支撑 Linux 兼容层的 Zephyr 调度原语。

### 测试文件

| 文件 | 用途 |
|------|------|
| `test_priority_scheduling.c` | 基于优先级的线程选择 |
| `test_sched_priority.c` | 优先级设置/获取操作 |
| `test_slice_scheduling.c` | 时间片轮转行为 |
| `test_sched_timeslice_reset.c` | 上下文切换时时间片重置 |
| `test_sched_timeslice_and_lock.c` | 时间片与调度器锁的交互 |
| `test_sched_is_preempt_thread.c` | 可抢占线程与协作线程检测 |

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/schedule_api
west flash
```

### 预期输出

```
Running TESTSUITE schedule_api
===================================================================
START - test_priority_scheduling
 PASS - test_priority_scheduling in 0.020 seconds
START - test_sched_timeslice_reset
 PASS - test_sched_timeslice_reset in 0.050 seconds
...
===================================================================
TESTSUITE schedule_api succeeded
```

---

## 2. 截止期限调度测试

**路径**：`zephyr/tests/kernel/sched/deadline/`

验证 EDF（最早截止期限优先）调度行为，与 `SCHED_DEADLINE` 类映射相关。

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/deadline
west flash
```

---

## 3. 抢占测试

**路径**：`zephyr/tests/kernel/sched/preempt/`

验证高优先级线程正确抢占低优先级线程，这是 `SCHED_FIFO` 语义的基础。

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/preempt
west flash
```

---

## 4. Linux 调度兼容性验证

Linux 兼容层（`CONFIG_SCHED_LINUX=y`）在 Zephyr 原生调度器之上添加策略感知调度。验证覆盖：

### 4.1 SCHED_FIFO 行为

- 相同优先级的 `SCHED_FIFO` 线程运行至完成，不轮转
- 高优先级 FIFO 线程立即抢占低优先级线程
- `sched_yield()` 将调用线程移到其优先级队列末尾

### 4.2 SCHED_RR 行为

- 相同优先级的 `SCHED_RR` 线程在时间片耗尽后轮转
- 时间片量子可通过 `sched_rr_get_interval()` 查询
- RR 线程不会抢占相同优先级的 FIFO 线程

### 4.3 策略与优先级接口测试

| 接口 | 验证内容 |
|------|----------|
| `sched_setscheduler()` | 原子设置策略 + 优先级 |
| `sched_getscheduler()` | 读回当前策略 |
| `sched_setparam()` | 不改变策略的情况下修改优先级 |
| `sched_getparam()` | 读回当前优先级 |
| `sched_get_priority_min/max()` | 返回每种策略的有效范围 |
| `sched_rr_get_interval()` | 返回 RR 线程的时间片 |

### 4.4 计划中的演示：RR vs FIFO

**计划路径**：`tests/demo/scheduler/rr_vs_fifo/`

创建 3 个相同优先级的线程：
- 线程 A：`SCHED_FIFO` — 运行至完成
- 线程 B：`SCHED_RR` — 与线程 C 轮转
- 线程 C：`SCHED_RR` — 与线程 B 轮转

预期串口输出：
```
[RR vs FIFO demo]
Thread A (FIFO, prio=5): start
Thread A (FIFO, prio=5): work 1/3
Thread A (FIFO, prio=5): work 2/3
Thread A (FIFO, prio=5): work 3/3
Thread A (FIFO, prio=5): done
Thread B (RR, prio=5): start
Thread B (RR, prio=5): work 1/3
Thread C (RR, prio=5): start        <-- 时间片轮转
Thread C (RR, prio=5): work 1/3
Thread B (RR, prio=5): work 2/3     <-- 时间片轮转
...
```

### 4.5 计划中的演示：优先级映射

**计划路径**：`tests/demo/scheduler/priority_mapping/`

验证 POSIX ↔ Zephyr 优先级双向转换：
- 通过 `sched_setparam()` 设置 POSIX 优先级
- 通过 `sched_getparam()` 读回
- 验证 Zephyr 内部优先级与预期映射一致

### 4.6 计划中的演示：RR 时间间隔

**计划路径**：`tests/demo/scheduler/rr_interval/`

验证时间片量子报告：
- 创建已知时间片的 RR 线程
- 通过 `sched_rr_get_interval()` 查询
- 验证返回的 `struct timespec` 与配置的 `CONFIG_TIMESLICE_SIZE` 一致

---

## 5. 配置

调度测试的关键 Kconfig 选项：

```
CONFIG_SCHED_LINUX=y
CONFIG_TIMESLICING=y
CONFIG_TIMESLICE_SIZE=10
CONFIG_NUM_PREEMPT_PRIORITIES=32
```
