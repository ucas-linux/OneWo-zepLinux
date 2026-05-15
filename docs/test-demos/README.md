# Test and Demo Documentation

This directory documents the test programs and demonstration samples used to validate the zepLinux compatibility layer.

## Directory Structure

| Document | Content |
|----------|---------|
| [scheduling-tests.md](scheduling-tests.md) | Linux-style scheduling interface validation |
| [gpio-board-demos.md](gpio-board-demos.md) | GPIO loopback and board-level validation demos |
| [irq-latency-benchmarks.md](irq-latency-benchmarks.md) | IRQ and scheduling latency measurement |
| [performance-benchmarks.md](performance-benchmarks.md) | Context switch, boot time, and throughput benchmarks |

## Test Categories

### 1. Kernel Scheduling Tests

Located in `zephyr/tests/kernel/sched/`:

- `schedule_api/` — Priority scheduling, timeslice, preemption
- `deadline/` — Deadline scheduling
- `preempt/` — Preemption behavior
- `metairq/` — Meta-IRQ scheduling

### 2. Board-Level Sample Applications

Located in `zephyr/samples/basic/`:

- `rocket_pi_pb1_loopback/` — GPIO loopback sanity check
- `pb1_pb2_benchmark/` — Comprehensive board benchmark suite
- `pb1_pb2_extended/` — Extended GPIO interconnect tests
- `irq_latency_*` — Various IRQ latency measurement approaches

### 3. Planned Compatibility Layer Tests

Located in `tests/` (planned):

- `tests/unit/` — Unit tests for individual interfaces
- `tests/integration/` — Multi-interface integration tests
- `tests/demo/scheduler/` — Scheduling behavior demos
- `tests/demo/device/` — Device control demos
- `tests/demo/perf/` — Performance benchmarks

## Target Board

Primary validation platform: **rocket_pi/stm32f401xe**

- MCU: STM32F401 (ARM Cortex-M4, 84 MHz)
- RAM: 96 KB
- Flash: 512 KB
- Key peripherals: GPIO (PB1/PB2 loopback), UART, TIM2

## Build and Run

```bash
# Build a test
west build -b rocket_pi/stm32f401xe <test_path>

# Flash to board
west flash

# View serial output
minicom -D /dev/ttyUSB0 -b 115200
```
