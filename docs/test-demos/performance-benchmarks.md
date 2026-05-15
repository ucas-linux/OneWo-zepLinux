# Performance Benchmarks

This document describes the performance measurement tests for the zepLinux compatibility layer, covering context switch overhead, boot time, and system resource usage.

---

## 1. Context Switch Overhead

**Path**: `zephyr/samples/basic/pb1_pb2_benchmark/` (context switch section)

### Methodology

Two threads at equal priority (`K_PRIO_PREEMPT(4)`) ping-pong via semaphores:

```
ping_thread:                    pong_thread:
  take(ping_sem)                  take(pong_sem)
  end_cycle = now()               start_cycle = now()
  sum += (end - start)            give(ping_sem)
  give(pong_sem)
```

Each semaphore give/take pair forces a context switch. The cycle count difference measures the switch overhead.

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_benchmark
west flash
```

### Expected Output

```
context_switch iterations=100
context_switch avg=252 cyc (3000 ns)
```

### Typical Values

| Platform | Context Switch | Notes |
|----------|---------------|-------|
| STM32F401 @ 84 MHz | 2–4 us | Without FPU context |
| STM32F401 @ 84 MHz | 4–8 us | With FPU context save |

---

## 2. Boot Time Measurement

**Path**: `zephyr/samples/basic/pb1_pb2_benchmark/` (boot time section)

### Methodology

Uses `SYS_INIT` at `PRE_KERNEL_1` priority 0 to capture the earliest possible cycle count:

```c
static int boot_time_mark(void)
{
    boot_mark_cycle = k_cycle_get_32();
    return 0;
}
SYS_INIT(boot_time_mark, PRE_KERNEL_1, 0);
```

Then in `main()`:
```c
main_entry_cycle = k_cycle_get_32();
uint32_t boot_to_main = main_entry_cycle - boot_mark_cycle;
```

### Expected Output

```
boot_to_main=12345 cyc (147000 ns)
boot_to_measure=23456 cyc (279000 ns)
```

### What It Measures

- **boot_to_main**: Time from earliest kernel init to application entry
- **boot_to_measure**: Time from earliest kernel init to first measurement point

---

## 3. Image Size

The `pb1_pb2_benchmark` sample also reports binary size implicitly through the build output:

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_benchmark
# Check build output for memory usage:
cat build/zephyr/zephyr.map | grep "Total"
# Or use:
arm-zephyr-eabi-size build/zephyr/zephyr.elf
```

### Typical Footprint

| Component | Flash | RAM | Notes |
|-----------|-------|-----|-------|
| Minimal Zephyr | ~20 KB | ~4 KB | Kernel + basic drivers |
| + GPIO driver | +2 KB | +0.5 KB | GPIO subsystem |
| + Linux sched layer | +4 KB | +1 KB | `CONFIG_SCHED_LINUX=y` |
| + FD table | +1 KB | +0.5 KB | File descriptor abstraction |

---

## 4. Scheduling Latency

**Path**: `zephyr/samples/basic/pb1_pb2_benchmark/` (sched latency section)

### Methodology

Measures the delay from `k_timer` expiry callback to thread wakeup:

1. Record expected wakeup cycle
2. Start `k_timer` with known period
3. Timer callback records actual fire cycle and gives semaphore
4. Main thread takes semaphore, computes delta

### Expected Output

```
sched_latency iterations=100 sleep_us=1000
sched_latency min=84 cyc (1000 ns) max=336 cyc (4000 ns) avg=168 cyc (2000 ns)
```

### Difference from IRQ Latency

- **IRQ latency**: Time from hardware event to ISR entry (hardware path)
- **Scheduling latency**: Time from ISR (timer callback) to thread resumption (software path)

Scheduling latency includes: ISR exit → scheduler decision → context restore → thread resume.

---

## 5. Planned Benchmarks

### 5.1 UART Throughput

**Planned path**: `tests/demo/perf/uart_throughput/`

Measure sustained data transfer rate:
- Transmit a known buffer size
- Measure wall-clock time
- Report bytes/second

### 5.2 GPIO Toggle Frequency

**Planned path**: `tests/demo/perf/gpio_toggle/`

Measure maximum GPIO toggle rate:
- Tight loop toggling a GPIO pin
- Measure with oscilloscope or cycle counter
- Report toggle frequency in kHz/MHz

### 5.3 Memory Allocation Overhead

**Planned path**: `tests/demo/perf/malloc_bench/`

Measure `malloc`/`free` cycle time:
- Allocate and free blocks of various sizes
- Report cycles per allocation/free
- Compare with Zephyr's native `k_malloc`

### 5.4 Compatibility Layer Overhead

**Planned path**: `tests/demo/perf/syscall_overhead/`

Measure the cost of the Linux compatibility shim:
- Direct Zephyr API call vs equivalent Linux/POSIX call
- Report overhead in cycles per call
- Target: < 10% overhead for scheduling interfaces
