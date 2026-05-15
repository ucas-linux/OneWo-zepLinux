# Scheduling Tests

This document describes the test programs that validate Linux-style scheduling interfaces on Zephyr.

---

## 1. Kernel Scheduling API Tests

**Path**: `zephyr/tests/kernel/sched/schedule_api/`

These tests validate Zephyr's scheduling primitives that underpin the Linux compatibility layer.

### Test Files

| File | Purpose |
|------|---------|
| `test_priority_scheduling.c` | Priority-based thread selection |
| `test_sched_priority.c` | Priority set/get operations |
| `test_slice_scheduling.c` | Timeslice rotation behavior |
| `test_sched_timeslice_reset.c` | Timeslice reset on context switch |
| `test_sched_timeslice_and_lock.c` | Timeslice interaction with scheduler lock |
| `test_sched_is_preempt_thread.c` | Preemptible vs cooperative thread detection |

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/schedule_api
west flash
```

### Expected Output

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

## 2. Deadline Scheduling Tests

**Path**: `zephyr/tests/kernel/sched/deadline/`

Validates EDF (Earliest Deadline First) scheduling behavior, relevant to `SCHED_DEADLINE` class mapping.

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/deadline
west flash
```

---

## 3. Preemption Tests

**Path**: `zephyr/tests/kernel/sched/preempt/`

Validates that higher-priority threads correctly preempt lower-priority threads, which is fundamental to `SCHED_FIFO` semantics.

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/preempt
west flash
```

---

## 4. Linux Scheduling Compatibility Validation

The Linux compatibility layer (`CONFIG_SCHED_LINUX=y`) adds policy-aware scheduling on top of Zephyr's native scheduler. Validation covers:

### 4.1 SCHED_FIFO Behavior

- Threads with `SCHED_FIFO` at the same priority run to completion without rotation
- Higher-priority FIFO threads preempt lower-priority threads immediately
- `sched_yield()` moves the calling thread to the end of its priority queue

### 4.2 SCHED_RR Behavior

- Threads with `SCHED_RR` at the same priority rotate after their timeslice expires
- Timeslice quantum is queryable via `sched_rr_get_interval()`
- RR threads do not preempt FIFO threads at the same priority

### 4.3 Policy and Priority Interface Tests

| Interface | Validation |
|-----------|-----------|
| `sched_setscheduler()` | Set policy + priority atomically |
| `sched_getscheduler()` | Read back current policy |
| `sched_setparam()` | Change priority without changing policy |
| `sched_getparam()` | Read back current priority |
| `sched_get_priority_min/max()` | Return valid range per policy |
| `sched_rr_get_interval()` | Return timeslice for RR threads |

### 4.4 Planned Demo: RR vs FIFO

**Planned path**: `tests/demo/scheduler/rr_vs_fifo/`

Creates 3 threads at the same priority:
- Thread A: `SCHED_FIFO` — runs to completion
- Thread B: `SCHED_RR` — rotates with Thread C
- Thread C: `SCHED_RR` — rotates with Thread B

Expected serial output:
```
[RR vs FIFO demo]
Thread A (FIFO, prio=5): start
Thread A (FIFO, prio=5): work 1/3
Thread A (FIFO, prio=5): work 2/3
Thread A (FIFO, prio=5): work 3/3
Thread A (FIFO, prio=5): done
Thread B (RR, prio=5): start
Thread B (RR, prio=5): work 1/3
Thread C (RR, prio=5): start        <-- timeslice rotation
Thread C (RR, prio=5): work 1/3
Thread B (RR, prio=5): work 2/3     <-- timeslice rotation
...
```

### 4.5 Planned Demo: Priority Mapping

**Planned path**: `tests/demo/scheduler/priority_mapping/`

Validates bidirectional POSIX ↔ Zephyr priority conversion:
- Set POSIX priority via `sched_setparam()`
- Read back via `sched_getparam()`
- Verify Zephyr internal priority matches expected mapping

### 4.6 Planned Demo: RR Interval

**Planned path**: `tests/demo/scheduler/rr_interval/`

Validates timeslice quantum reporting:
- Create RR thread with known timeslice
- Query via `sched_rr_get_interval()`
- Verify returned `struct timespec` matches configured `CONFIG_TIMESLICE_SIZE`

---

## 5. Configuration

Key Kconfig options for scheduling tests:

```
CONFIG_SCHED_LINUX=y
CONFIG_TIMESLICING=y
CONFIG_TIMESLICE_SIZE=10
CONFIG_NUM_PREEMPT_PRIORITIES=32
```
