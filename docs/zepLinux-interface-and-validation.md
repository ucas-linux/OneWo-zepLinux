# zepLinux Interface and Validation System

OneWo zepLinux implements a Linux-compatible interface layer on top of the Zephyr kernel. This document explains:

- How Linux/POSIX scheduling interfaces map to Zephyr threads and scheduler
- How Linux-style device control paths are implemented through fd, `ioctl`, and non-blocking semantics at the board level
- How the entire compatibility layer is validated through scheduling tests, GPIO/IRQ demos, and performance benchmarks

---

## 1. Overall Description

The goal is to provide a Linux-compatible interface layer on Zephyr RTOS, enabling Linux applications to run in MCU environments with minimal modifications.

Interface design involves three constraints:

- **Semantically alignable**: Application-visible behavior should be as close to Linux/POSIX as possible
- **Implementable in kernel**: Must ultimately map to Zephyr's threads, priorities, timeslices, devices, or synchronization primitives
- **Verifiable in engineering**: Must have tests, demos, or performance data to prove it not only "exists" but is "usable"

Currently includes two types of capabilities:

- **Scheduling and priority interfaces**
- **Device control and board-level validation paths**

These two capabilities correspond to the following in `README.md`:

- Linux-style scheduler integration with `k_thread`
- Scheduling/device management sections in Linux-compatible interface classification
- Build and test system

---

## 2. Scheduling Interfaces

The scheduling compatibility layer is responsible for mapping the Linux/POSIX "policy + priority + timeslice" model to Zephyr thread objects and kernel scheduling paths.

### 2.1 Scheduling Model Layering

This layer is divided into three tiers:

1. **Application Interface Layer**
   - `sched_*`
   - `pthread_*sched*`
   - LMZ's `lmz_sched_*` / `lmz_nice()` / `lmz_setpriority()` interfaces
2. **Compatibility Mapping Layer**
   - Bidirectional conversion between POSIX policy/prio and Zephyr internal policy/prio
   - Thread attributes and runtime policy updates
3. **Zephyr Kernel Execution Layer**
   - `struct k_thread`
   - Kernel ready queue / timeslice / priority comparison
   - Manifestation of RR / FIFO / OTHER semantics in actual scheduling behavior

The corresponding chain is: user-space semantics → kernel entities → test validation.

### 2.2 Thread Scheduling Metadata

To make Zephyr threads carry Linux/POSIX scheduling semantics, a scheduling policy field is introduced in the thread base class:

- `_thread_base.sched_policy`

Internal policy enums are:

- `Z_THREAD_SCHED_OTHER`
- `Z_THREAD_SCHED_FIFO`
- `Z_THREAD_SCHED_RR`

Each thread, in addition to Zephyr priority, carries a policy tag used by the compatibility layer for:

- Supporting `sched_getscheduler()` / `sched_setscheduler()`
- Supporting `pthread_getschedparam()` / `pthread_setschedparam()`
- Allowing RR timeslice and FIFO non-rotating behavior to be distinguished in the kernel

### 2.3 Scheduling Policy Mapping

In the compatibility layer, policy mapping relationships are as follows:

| Linux/POSIX Semantics | Zephyr Internal Policy | Description |
|----------------------|------------------------|-------------|
| `SCHED_OTHER` | `Z_THREAD_SCHED_OTHER` | Normal thread policy |
| `SCHED_FIFO` | `Z_THREAD_SCHED_FIFO` | Real-time FIFO |
| `SCHED_RR` | `Z_THREAD_SCHED_RR` | Real-time RR |

In addition to policy name mapping, **priority space mapping** must also be handled.

POSIX priority values are not directly passed to Zephyr, but undergo the following conversion:

- When setting: POSIX priority → Zephyr internal priority
- When querying: Zephyr internal priority → POSIX priority

Corresponding effects:

- Maintain familiar priority interfaces for Linux/POSIX applications
- Maintain Zephyr kernel's own priority representation and scheduling invariants

### 2.4 Kernel Bridge Points

The scheduling compatibility layer depends on the following kernel bridge points.

#### `z_thread_priority_and_policy_set()`

```c
void z_thread_priority_and_policy_set(struct k_thread *thread,
                                      int prio,
                                      uint8_t policy);
```

This is the entry point for coordinated policy and priority updates, responsible for:

- Updating thread priority and policy simultaneously
- Avoiding state inconsistency caused by separate writes to policy and priority
- Providing a unified update path for `sched_setparam()`, `sched_setscheduler()`, `pthread_setschedparam()`

#### `z_get_thread_timeslice_ticks()`

```c
int32_t z_get_thread_timeslice_ticks(struct k_thread *thread);
```

It reads the thread's timeslice and exposes it to applications through `sched_rr_get_interval()`.

#### `thread_is_sliceable()`

```c
bool thread_is_sliceable(struct k_thread *thread);
```

This judgment determines whether a thread participates in timeslice rotation. After introducing Linux/POSIX policies, the policy itself must also be considered:

- `SCHED_RR`: Needs rotation
- `SCHED_FIFO`: Should not be mistakenly rotated by RR mechanism at the same priority
- `SCHED_OTHER`: Participates in system default behavior according to normal thread policy

### 2.5 Application-Visible Scheduling Interfaces

Current interfaces are as follows.

#### `sched_get_priority_min(int policy)` / `sched_get_priority_max(int policy)`

Used to query the priority range for a given policy, serving as the basic constraint for all real-time scheduling interfaces.

#### `sched_getparam(pid_t pid, struct sched_param *param)`

Used to read the thread's current priority. This interface remaps Zephyr internal priority to POSIX semantic priority.

#### `sched_setparam(pid_t pid, const struct sched_param *param)`

Used to adjust priority while keeping the policy unchanged.

#### `sched_getscheduler(pid_t pid)`

Returns the thread's current scheduling policy (SCHED_OTHER / SCHED_FIFO / SCHED_RR).

#### `sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)`

Simultaneously sets policy and priority. This is the most comprehensive scheduling control interface.

#### `sched_rr_get_interval(pid_t pid, struct timespec *tp)`

Returns the timeslice quantum for RR policy threads.

#### `sched_yield()`

Voluntarily yields CPU, allowing other threads at the same priority to run.

### 2.6 pthread Scheduling Interfaces

In addition to `sched_*` interfaces, the compatibility layer also provides pthread-style scheduling interfaces:

- `pthread_attr_setschedpolicy()` / `pthread_attr_getschedpolicy()`
- `pthread_attr_setschedparam()` / `pthread_attr_getschedparam()`
- `pthread_setschedparam()` / `pthread_getschedparam()`

These interfaces should share the same underlying implementation with `sched_*` interfaces to ensure state consistency.

### 2.7 Scheduling Test Validation

Scheduling interface validation includes:

1. **Unit tests**: Verify individual interface behavior
   - Policy setting and querying
   - Priority setting and querying
   - Timeslice querying

2. **Integration tests**: Verify multi-thread scheduling behavior
   - FIFO threads run to completion without preemption at same priority
   - RR threads rotate at same priority
   - Higher priority threads preempt lower priority threads

3. **Stress tests**: Verify stability under high load
   - Many threads competing
   - Frequent priority changes
   - Frequent policy switches

---

## 3. Device Management Interfaces

Device management interfaces provide Linux-style file descriptor operations on top of Zephyr's device model.

### 3.1 File Descriptor Abstraction

Core interfaces include:

- `open(const char *path, int flags, ...)`
- `close(int fd)`
- `read(int fd, void *buf, size_t count)`
- `write(int fd, const void *buf, size_t count)`
- `ioctl(int fd, unsigned long request, ...)`

### 3.2 Implementation Strategy

#### FD Table

Implement a file descriptor table mapping integer FDs to device handles:

```c
struct fd_entry {
    const struct device *dev;
    int flags;
    void *private_data;
};

static struct fd_entry fd_table[MAX_FDS];
```

#### Device Path Mapping

Map device paths to Zephyr device instances:

- `/dev/gpio0` → `DEVICE_DT_GET(DT_NODELABEL(gpio0))`
- `/dev/uart0` → `DEVICE_DT_GET(DT_NODELABEL(uart0))`

#### ioctl() Dispatch

`ioctl()` is the most flexible interface, requiring device-specific handlers:

```c
int ioctl(int fd, unsigned long request, ...) {
    struct fd_entry *entry = &fd_table[fd];

    switch (entry->dev->type) {
    case DEV_TYPE_GPIO:
        return gpio_ioctl(entry->dev, request, ...);
    case DEV_TYPE_UART:
        return uart_ioctl(entry->dev, request, ...);
    // ...
    }
}
```

### 3.3 Non-Blocking Semantics

Device interfaces must support non-blocking operations:

- `O_NONBLOCK` flag in `open()`
- Non-blocking `read()` / `write()` return `EAGAIN` when data unavailable
- Coordination with interrupt-driven I/O

### 3.4 Device Control Examples

#### GPIO Control

```c
int fd = open("/dev/gpio0", O_RDWR);
ioctl(fd, GPIO_SET_PIN, 5);      // Set pin 5 high
ioctl(fd, GPIO_CLEAR_PIN, 5);    // Set pin 5 low
ioctl(fd, GPIO_CONFIG_OUTPUT, 5); // Configure pin 5 as output
close(fd);
```

#### UART Configuration

```c
int fd = open("/dev/uart0", O_RDWR);
struct uart_config cfg = {
    .baudrate = 115200,
    .parity = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
};
ioctl(fd, UART_SET_CONFIG, &cfg);
write(fd, "Hello\n", 6);
close(fd);
```

---

## 4. Board-Level Validation

### 4.1 Target Platform: Rocket Pi

Primary validation platform:
- Board: rocket_pi/stm32f401xe
- MCU: ARM Cortex-M3
- Peripherals: GPIO, UART, timers

### 4.2 GPIO Demo

Demonstrates device control path through GPIO operations:

1. Open GPIO device via `open()`
2. Configure pin direction via `ioctl()`
3. Toggle pin state
4. Observe LED or measure with oscilloscope

### 4.3 IRQ Demo

Demonstrates interrupt handling and non-blocking I/O:

1. Configure GPIO pin for interrupt
2. Register interrupt handler
3. Wait for interrupt event
4. Verify interrupt latency

### 4.4 Multi-Thread Scheduling Demo

Demonstrates real-time scheduling behavior:

1. Create multiple threads with different policies (FIFO, RR, OTHER)
2. Set different priorities
3. Observe scheduling order and timeslice behavior
4. Measure context switch overhead

---

## 5. Performance Benchmarks

### 5.1 Scheduling Latency

Measure time from event occurrence to thread activation:

- Interrupt latency
- Semaphore post-to-wake latency
- Priority inheritance latency

### 5.2 Context Switch Overhead

Measure time to switch between threads:

- Same priority context switch
- Different priority context switch
- With and without FPU context

### 5.3 Device I/O Throughput

Measure data transfer rates:

- UART throughput
- SPI throughput
- GPIO toggle frequency

### 5.4 Memory Footprint

Measure code and data size:

- Compatibility layer overhead
- Per-thread metadata overhead
- FD table overhead

---

## 6. Test Organization

### 6.1 Test Directory Structure

```
tests/
├── examples/hello_lmz/    # Minimal example
├── unit/                  # Unit tests
│   ├── test_thread.c
│   ├── test_sched.c
│   └── test_memory.c
├── integration/           # Integration tests
│   └── test_process.c
├── stress/                # Stress tests
│   └── test_stress.c
└── demo/                  # Demos
    ├── scheduler/         # Scheduling demos
    ├── device/            # Device demos
    ├── board/             # Board-specific demos
    └── perf/              # Performance benchmarks
```

### 6.2 Test Execution

Build and run tests:

```bash
# Build unit test
west build -b rocket_pi/stm32f401xe tests/unit/test_sched

# Flash to board
west flash

# View output
minicom -D /dev/ttyUSB0
```

---

## 7. Validation Methodology

The validation system follows a four-layer approach:

1. **Interface definition**
   - POSIX/Linux semantics
   - Zephyr mapping
2. **Kernel implementation**
   - policy/prio mapping
   - timeslice / ready queue / device control paths
3. **Validation methods**
   - POSIX realtime tests
   - Rocket Pi GPIO/IRQ demos
   - Latency benchmarks
4. **Result feedback**
   - Behavioral correctness
   - Board-level runnability
   - Acceptable performance cost

Interfaces, examples, and performance correspond to different aspects of the same system.

---

## 8. Relationship with Existing Repository Content

This document's relationship with existing repository structure:

- `README.md`
  - Provides overall goals, interface classification, Zephyr scheduling direction
- `code/include/lmz_sched.h`
  - Provides LMZ layer wrapper definitions for Linux-style scheduling interfaces
- `human/api-reference.md`
  - Suitable for more fine-grained interface-level descriptions
- `human/testing.md`
  - Suitable for more specific test steps, board connection methods, and execution procedures
- `docs/linux-sched-kernel-integration.md`
  - Suitable for more detailed kernel scheduling integration details

Correspondence:

- Why these interfaces exist
- How these interfaces map to Zephyr
- Why these demos are needed
- What performance results indicate

---

## 9. Summary

The core of OneWo zepLinux is to establish a Linux-compatible system that connects **interface semantics, kernel implementation, board-level validation, and performance evaluation**.

Around this idea:

- The scheduling interface system maps Linux/POSIX thread policy and priority semantics to the Zephyr thread model
- The device control path maps fd, `ioctl`, and non-blocking semantics to real board-level scenarios
- Rocket Pi and various demos/benchmarks advance interfaces from "declared to exist" to "verifiable, measurable, comparable"

Interface documentation, test examples, and performance results correspond to different layers of the same system.
