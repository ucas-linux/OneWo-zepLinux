# GPIO and Board-Level Demos

This document describes the GPIO loopback tests and board-level validation demos running on the Rocket Pi (STM32F401) platform.

---

## 1. GPIO Loopback Sanity Test

**Path**: `zephyr/samples/basic/rocket_pi_pb1_loopback/`

The simplest board validation: drives PB1 (output) and reads PB2 (input) through a physical loopback wire.

### Purpose

- Confirm GPIO driver is functional
- Confirm devicetree aliases (`loopback_out`, `loopback_in`) are correctly mapped
- Provide a basic PASS/FAIL gate before running more complex tests

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/rocket_pi_pb1_loopback
west flash
minicom -D /dev/ttyUSB0 -b 115200
```

### Expected Output

```
PB1→PB2 loopback test
drive=0 sample=0 PASS
drive=1 sample=1 PASS
loopback test: ALL PASS
```

### Hardware Setup

Connect PB1 to PB2 with a jumper wire on the Rocket Pi board.

---

## 2. Extended GPIO Interconnect Test

**Path**: `zephyr/samples/basic/pb1_pb2_extended/`

Extended validation of the PB1→PB2 loopback path with multiple test patterns:

| Test | Description |
|------|-------------|
| Single low | Drive low, verify low |
| Single high | Drive high, verify high |
| 20x toggle | Rapid toggle, verify each state |
| Hold high | Sustained high, verify stability |
| Hold low | Sustained low, verify stability |

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_extended
west flash
```

### Expected Output

```
PB1/PB2 extended interconnect test
single-low drive=0 sample=0 PASS
single-high drive=1 sample=1 PASS
toggle[1] drive=1 sample=1 PASS
toggle[2] drive=0 sample=0 PASS
...
toggle[20] drive=0 sample=0 PASS
hold-high drive=1 sample=1 PASS
hold-low drive=0 sample=0 PASS
RESULT: 24/24 passed, 0 failed
```

---

## 3. Comprehensive Board Benchmark

**Path**: `zephyr/samples/basic/pb1_pb2_benchmark/`

A combined test suite that runs all board-level validations in sequence:

1. **Boot time measurement** — Cycles from `PRE_KERNEL_1` init to `main()` entry
2. **GPIO loopback** — PB1→PB2 sanity check
3. **IRQ latency** — TIM2 hardware timer ISR latency (100 samples)
4. **Scheduling latency** — `k_timer` wakeup latency (100 samples)
5. **Context switch** — Ping-pong thread context switch overhead (100 samples)

### Build and Run

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_benchmark
west flash
```

### Expected Output

```
PB1/PB2 benchmark start
boot_to_main=12345 cyc (147 ns)
boot_to_measure=23456 cyc (279 ns)
cpu cycles per sec: 84000000
single-low drive=0 sample=0 PASS
single-high drive=1 sample=1 PASS
irq_latency_direct iterations=100 period_us=1000
irq_latency_direct min=42 cyc (500 ns) max=168 cyc (2000 ns) avg=84 cyc (1000 ns)
sched_latency iterations=100 sleep_us=1000
sched_latency min=84 cyc (1000 ns) max=336 cyc (4000 ns) avg=168 cyc (2000 ns)
context_switch iterations=100
context_switch avg=252 cyc (3000 ns)
benchmark complete: 2 gpio pass, 0 gpio fail
```

### Implementation Details

- Uses `SYS_INIT(boot_time_mark, PRE_KERNEL_1, 0)` to capture boot timestamp
- TIM2 configured with PSC=83 (1 MHz tick at 84 MHz core) for microsecond-resolution IRQ timing
- Context switch measured via semaphore ping-pong between two threads at equal priority

---

## 4. Planned Device Control Demos

### 4.1 GPIO via Linux-Style Interface

**Planned path**: `tests/demo/device/gpio_control/`

Demonstrates the Linux-compatible device control path:

```c
int fd = open("/dev/gpio0", O_RDWR);
ioctl(fd, GPIO_CONFIG_OUTPUT, 5);  // Configure pin 5 as output
ioctl(fd, GPIO_SET_PIN, 5);        // Set pin 5 high
ioctl(fd, GPIO_CLEAR_PIN, 5);      // Set pin 5 low
close(fd);
```

### 4.2 UART via Linux-Style Interface

**Planned path**: `tests/demo/device/uart_control/`

Demonstrates UART configuration through fd/ioctl:

```c
int fd = open("/dev/uart0", O_RDWR);
struct uart_config cfg = {
    .baudrate = 115200,
    .parity = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
};
ioctl(fd, UART_SET_CONFIG, &cfg);
write(fd, "Hello from zepLinux\n", 20);
close(fd);
```

### 4.3 Non-Blocking I/O

**Planned path**: `tests/demo/device/nonblock_io/`

Demonstrates `O_NONBLOCK` semantics:

```c
int fd = open("/dev/uart0", O_RDWR | O_NONBLOCK);
char buf[64];
ssize_t n = read(fd, buf, sizeof(buf));
// Returns -1 with errno=EAGAIN when no data available
```
