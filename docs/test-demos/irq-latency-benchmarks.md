# IRQ Latency Benchmarks

This document describes the interrupt latency measurement programs used to characterize real-time performance of the zepLinux platform.

---

## 1. Overview

IRQ latency is the time from when a hardware event occurs to when the ISR begins executing. This is a critical metric for real-time systems and directly affects the feasibility of `SCHED_FIFO` / `SCHED_RR` guarantees.

The project provides multiple measurement approaches with increasing complexity:

| Sample | Method | Samples |
|--------|--------|---------|
| `irq_latency_isr/` | Single TIM2 ISR via counter driver | 1 |
| `irq_latency_isr_100/` | TIM2 ISR via counter driver | 100 |
| `irq_latency_cyclictest/` | k_timer wakeup (cyclictest-style) | 10000 |
| `irq_latency_cyclictest_once/` | k_timer wakeup | 1 |
| `irq_latency_tim2_direct/` | Direct TIM2 register access | N |
| `irq_latency_pb1_once/` | PB1 loopback + TIM2 ISR | 1 |
| `irq_latency_pb1_suite/` | PB1 loopback + ISR + k_timer | 20+20 |

---

## 2. Cyclictest-Style Latency Test

**Path**: `zephyr/samples/basic/irq_latency_cyclictest/`

Mimics the Linux `cyclictest` tool: measures the delay between expected timer expiry and actual thread wakeup.

### Methodology

1. Record current cycle count
2. Start a `k_timer` for a known period (1000 us)
3. Block on semaphore until timer fires
4. Measure actual wakeup cycle vs expected cycle
5. Compute delta as wakeup latency

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/irq_latency_cyclictest
west flash
```

### Expected Output

```
cyclictest-style IRQ wakeup latency test
cpu cycles per sec: 84000000
test period: 1000 us, period ticks: 10
iterations: 10000
[100] cur=89 cyc (1059 ns) min=42 max=168 avg=84 tick_jitter=0
[200] cur=84 cyc (1000 ns) min=42 max=168 avg=82 tick_jitter=0
...
[10000] cur=86 cyc (1023 ns) min=38 max=210 avg=84 tick_jitter=0
```

### Key Metrics

- **min**: Best-case wakeup latency (lower bound of system overhead)
- **max**: Worst-case wakeup latency (indicates jitter from other ISRs or scheduler)
- **avg**: Typical wakeup latency
- **tick_jitter**: Drift in tick counter (should be 0 or near-0)

---

## 3. Direct TIM2 ISR Latency

**Path**: `zephyr/samples/basic/irq_latency_tim2_direct/`

Bypasses the Zephyr counter driver and directly programs TIM2 registers for minimal-overhead measurement.

### Methodology

1. Configure TIM2: PSC=83 (1 us tick at 84 MHz), ARR=period
2. Connect ISR directly via `IRQ_CONNECT(TIM2_IRQn, ...)`
3. In ISR: read `k_cycle_get_32()` immediately
4. In main: compare ISR timestamp to expected fire time

### Why Direct Access?

The Zephyr counter driver adds abstraction overhead. Direct register access gives the true hardware IRQ latency without driver stack contribution.

---

## 4. PB1 Loopback + IRQ Suite

**Path**: `zephyr/samples/basic/irq_latency_pb1_suite/`

Combines GPIO validation with latency measurement in a single test run:

1. PB1→PB2 loopback check (sanity gate)
2. 20 TIM2 ISR latency samples
3. 20 k_timer wakeup latency samples

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/irq_latency_pb1_suite
west flash
```

### Expected Output

```
PB1/PB2 suite start
loopback drive=0 sample=0 PASS
loopback drive=1 sample=1 PASS
irq_latency iterations=20 period_us=1000
irq_latency min=40 cyc (476 ns) max=160 cyc (1904 ns) avg=82 cyc (976 ns)
sched_latency iterations=20 sleep_us=1000
sched_latency min=80 cyc (952 ns) max=320 cyc (3809 ns) avg=160 cyc (1904 ns)
suite complete
```

---

## 5. Interpreting Results

### Typical Values on Rocket Pi (STM32F401, 84 MHz)

| Metric | Typical Range | Notes |
|--------|--------------|-------|
| TIM2 ISR latency | 400–2000 ns | Hardware interrupt entry |
| k_timer wakeup latency | 1000–4000 ns | Includes scheduler overhead |
| Tick jitter | 0–1 tick | Should be minimal |

### Factors Affecting Latency

- **Interrupt priority**: Higher priority ISRs have lower latency
- **Scheduler lock**: Held locks delay thread wakeup (not ISR entry)
- **Other ISRs**: Nested or pending ISRs add tail-chaining delay
- **Flash wait states**: STM32F401 at 84 MHz uses 2 wait states

### Relationship to Linux Scheduling

- `SCHED_FIFO` threads depend on predictable wakeup latency
- `SCHED_RR` timeslice accuracy depends on tick jitter
- IRQ latency sets the floor for all real-time guarantees
