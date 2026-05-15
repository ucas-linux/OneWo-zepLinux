# Zephyr Linux Compatible Interface Definition

This document specifically explains the Linux/POSIX compatible interface definitions provided by zepLinux on Zephyr, and how these interfaces are implemented in the kernel.

Unlike overview documents, this focuses on the concrete implementation details of the interface layer:

- What interfaces are exposed to applications
- What semantics are promised for these interfaces
- How these semantics map to Zephyr's thread, scheduler, and device models
- Which parts are strictly compatible, and which are tailored or constrained for MCU scenarios

## 1. Interface Layer Responsibility Boundaries

In this project, the "Linux compatible interface layer" does not mean transplanting the Linux kernel intact onto Zephyr, but rather creating an application-oriented semantic compatibility layer.

Its responsibility boundaries can be summarized in three points:

- Maintain the calling conventions familiar to Linux/POSIX programmers at the application level
- Reuse Zephyr's existing kernel objects, driver framework, and scheduling mechanisms at the lower level
- In resource-constrained MCU scenarios, only implement the semantic subset that the project truly needs and can stably verify

Therefore, "compatibility" here is not a literal 1:1 copy, but rather "keep interface form unchanged as much as possible, semantics as close as possible, and allow underlying implementation to be refactored."

## 2. Interface Classification

Combined with the current repository organization, compatible interfaces are roughly divided into six categories:

| Category | Focus | Typical Interfaces |
|----------|-------|-------------------|
| Thread Management | Thread creation, synchronization, lifecycle | `pthread_create`, `pthread_join`, mutexes |
| Process Management | Lightweight process semantics or placeholder interfaces | `fork`, `exec`, `exit` |
| Scheduling & Priority | Scheduling policies, priorities, timeslices | `sched_*`, `nice`, `setpriority` |
| Signals & Timers | Delays, notifications, simple async control | `signal`, `kill`, `alarm`, `nanosleep` |
| Memory Management | Dynamic allocation and compatibility wrappers | `malloc`, `realloc`, `free` |
| Device Management | File descriptors and control paths | `open`, `read`, `write`, `ioctl` |

In this breakdown document, two parts need detailed expansion:

- Scheduling and priority interfaces
- Device management interfaces

The reason is simple: these two parts have strong Linux semantic characteristics and directly touch the Zephyr kernel or driver layer, making them the most likely places in the compatibility layer where things "look like they compile but semantics aren't actually implemented."

## 3. Scheduling Interface Definition

### 3.1 Design Goals

The goal of scheduling interfaces is not simply to provide a few `sched_*` function shells, but to let applications see real scheduling behavior changes through these interfaces.

In other words, at least the following must be achieved:

- When policy changes, the thread's scheduling class actually changes
- When priority changes, the thread's competitive relationship in Zephyr's ready queue actually changes
- Information read by query interfaces remains consistent with the thread's actual running state
- `SCHED_FIFO` and `SCHED_RR` exhibit different scheduling semantics at the same priority

If only "interface returns success" is done, but there are no corresponding state and behavior changes on the kernel side, then this compatibility layer has no practical meaning for upper-layer applications.

### 3.2 Three-Layer Structure

Scheduling interfaces can be understood in three layers:

#### Layer 1: Application-Visible Interfaces

This layer is the entry point directly used by Linux/POSIX programmers, mainly including:

- `sched_getparam()`
- `sched_setparam()`
- `sched_getscheduler()`
- `sched_setscheduler()`
- `sched_rr_get_interval()`
- `pthread_attr_*sched*()`
- `pthread_*schedparam()`
- `nice()` / `setpriority()` style extension interfaces

#### Layer 2: Compatibility Mapping Layer

This layer is responsible for semantic conversion, common tasks include:

- Converting POSIX policy values to Zephyr internal policy enums
- Converting application-view priorities to Zephyr kernel priority space
- Performing parameter validity checks
- Ensuring consistent handling of scheduling attributes during creation and runtime phases

#### Layer 3: Kernel Execution Layer

This layer ultimately falls to Zephyr kernel objects and scheduling paths, core objects include:

- `struct k_thread`
- Priority and policy metadata in thread base class
- Ready queue
- Timeslice logic
- Scheduling comparison functions and thread switching conditions

These three layers must be connected for interface semantics to form a closed loop.

## 4. Thread Scheduling Metadata

To make Zephyr threads carry Linux/POSIX-style scheduling semantics, thread objects need to retain at least two dimensions of information: "policy" and "priority."

In the current documentation system, the policy field is represented by `_thread_base.sched_policy`. Its significance is not to replace Zephyr's existing priority, but to supplement the policy dimension that the Linux compatibility layer truly cares about.

Internal policy enums include:

- `Z_THREAD_SCHED_OTHER`
- `Z_THREAD_SCHED_FIFO`
- `Z_THREAD_SCHED_RR`

The role of these values is mainly reflected in the following places:

- Allow `sched_getscheduler()` to return the thread's actual policy
- Allow `sched_setscheduler()` to write policy modifications back to the thread object
- Allow timeslice logic to distinguish between threads that "need rotation" and "don't need rotation"
- Allow `pthread` scheduling interfaces and `sched_*` interfaces to ultimately fall to the same set of thread metadata

Without this layer of metadata, many interfaces can only achieve "parameters came in" but cannot achieve "thread state actually changed."

## 5. Policy Mapping and Priority Mapping

### 5.1 Policy Mapping

Policy mapping relationships are relatively straightforward:

| Linux/POSIX Semantics | Zephyr Internal Policy |
|----------------------|------------------------|
| `SCHED_OTHER` | `Z_THREAD_SCHED_OTHER` |
| `SCHED_FIFO` | `Z_THREAD_SCHED_FIFO` |
| `SCHED_RR` | `Z_THREAD_SCHED_RR` |

### 5.2 Priority Mapping

Priority mapping is more complex because Linux and Zephyr have different priority space definitions:

- Linux real-time priorities: 1-99 (higher number = higher priority)
- Zephyr priorities: typically 0-31 or wider range (lower number = higher priority)

The compatibility layer needs to provide bidirectional conversion functions:

- `posix_to_zephyr_prio()`: Convert POSIX priority to Zephyr priority
- `zephyr_to_posix_prio()`: Convert Zephyr priority to POSIX priority

Conversion principles:

- Ensure monotonicity: higher POSIX priority maps to higher Zephyr priority (lower number)
- Ensure reversibility: conversion back and forth should yield the original value
- Handle boundary cases: out-of-range values should be clamped or return errors

### 5.3 Default Policy and Priority

When a thread is created without explicitly specifying scheduling attributes:

- Default policy: `SCHED_OTHER` (mapped to `Z_THREAD_SCHED_OTHER`)
- Default priority: Zephyr's default cooperative priority or a predefined value

This ensures that threads created through standard `pthread_create()` without attributes have predictable behavior.

## 6. Timeslice and Round-Robin Semantics

### 6.1 SCHED_FIFO vs SCHED_RR

The key difference between these two policies:

- **SCHED_FIFO**: No timeslice. A thread runs until it blocks, yields, or is preempted by a higher-priority thread.
- **SCHED_RR**: Has timeslice (typically 10 ticks). Threads at the same priority take turns running.

### 6.2 Implementation Approach

To implement this distinction in Zephyr:

1. **Timeslice field**: Each thread needs a timeslice value
   - FIFO threads: timeslice = 0 or K_TICKS_FOREVER
   - RR threads: timeslice = configured value (e.g., 10 ticks)

2. **Tick handler**: On each timer tick, check if the current thread:
   - Is RR policy
   - Has exhausted its timeslice
   - If yes, move to end of same-priority queue and reset timeslice

3. **Scheduler integration**: The ready queue must support moving threads within the same priority level

### 6.3 sched_rr_get_interval()

This interface returns the timeslice quantum for RR threads:

```c
int sched_rr_get_interval(pid_t pid, struct timespec *tp);
```

Implementation:
- Look up the thread by pid
- If policy is SCHED_RR, return the configured timeslice
- If policy is not SCHED_RR, return error or 0

## 7. Runtime Policy and Priority Changes

### 7.1 sched_setscheduler()

This interface changes both policy and priority:

```c
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
```

Implementation steps:
1. Validate policy and priority values
2. Convert POSIX policy to Zephyr internal policy
3. Convert POSIX priority to Zephyr priority
4. Update thread metadata
5. If thread is ready, remove from ready queue and re-insert with new priority
6. Trigger reschedule if necessary

### 7.2 sched_setparam()

This interface changes only priority, keeping policy unchanged:

```c
int sched_setparam(pid_t pid, const struct sched_param *param);
```

Implementation is similar to `sched_setscheduler()` but only updates priority.

### 7.3 Thread State Considerations

When changing scheduling attributes:
- **Running thread**: Update metadata, may trigger immediate reschedule
- **Ready thread**: Remove from ready queue, update, re-insert
- **Blocked thread**: Update metadata, will take effect when unblocked
- **Suspended thread**: Update metadata, will take effect when resumed

## 8. pthread Scheduling Interfaces

### 8.1 pthread_attr_setschedpolicy() and pthread_attr_setschedparam()

These set scheduling attributes before thread creation:

```c
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
```

These store values in the `pthread_attr_t` structure, which are then applied during `pthread_create()`.

### 8.2 pthread_setschedparam() and pthread_getschedparam()

These change or query scheduling attributes of an existing thread:

```c
int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);
```

Implementation should call the same underlying functions as `sched_setscheduler()` and `sched_getscheduler()`.

### 8.3 Consistency Between pthread and sched Interfaces

To avoid state inconsistency:
- Both interface families should operate on the same thread metadata
- Use common internal functions for policy/priority conversion and updates
- Ensure query interfaces read from actual thread state, not cached values

## 9. nice() and setpriority() Extensions

### 9.1 nice() Semantics

The `nice()` interface adjusts thread priority by a relative value:

```c
int nice(int inc);
```

- Positive `inc`: Lower priority (be "nicer" to other threads)
- Negative `inc`: Raise priority (requires privileges)
- Typical range: -20 to +19

### 9.2 Implementation Approach

For SCHED_OTHER threads:
1. Get current nice value
2. Add increment (clamping to valid range)
3. Convert nice value to Zephyr priority
4. Update thread priority

For real-time threads (SCHED_FIFO/RR):
- May return error, or
- Treat as no-op, or
- Convert to equivalent priority adjustment

### 9.3 setpriority() and getpriority()

These interfaces set/get priority for processes, process groups, or users:

```c
int setpriority(int which, id_t who, int prio);
int getpriority(int which, id_t who);
```

In MCU environment without full process model:
- `PRIO_PROCESS`: Operate on thread
- `PRIO_PGRP` and `PRIO_USER`: May return error or operate on current thread

## 10. Device Management Interfaces

### 10.1 Design Goals

Device management interfaces aim to provide Linux-style file descriptor operations on top of Zephyr's device model:

- `open()`: Open device, return file descriptor
- `close()`: Close file descriptor
- `read()` / `write()`: Data transfer
- `ioctl()`: Device-specific control operations

### 10.2 File Descriptor Abstraction

Zephyr doesn't have a native file descriptor table. The compatibility layer needs to implement:

1. **FD table**: Map integer FDs to device pointers or handles
2. **FD allocation**: Assign unique FDs when devices are opened
3. **FD lifecycle**: Track open/close state

### 10.3 Device Mapping

Map device paths to Zephyr device instances:

- `/dev/gpio0` → `DEVICE_DT_GET(DT_NODELABEL(gpio0))`
- `/dev/uart0` → `DEVICE_DT_GET(DT_NODELABEL(uart0))`

Implementation approaches:
- Static table mapping paths to device tree nodes
- Dynamic registration mechanism
- Hybrid approach

### 10.4 ioctl() Implementation

`ioctl()` is the most flexible and device-specific interface:

```c
int ioctl(int fd, unsigned long request, ...);
```

Implementation strategy:
1. Look up device from FD
2. Decode request code
3. Dispatch to device-specific handler
4. Common operations:
   - GPIO: Set/clear pins, configure direction
   - UART: Set baud rate, configure flow control
   - SPI/I2C: Configure bus parameters

The goal is not to replicate all Linux ioctl complexity, but to support the operations actually needed by the project.

### 10.5 Blocking and Non-Blocking Semantics

The device compatibility layer must handle not just "can read/write" but also waiting semantics.

Key aspects typically include:

- Non-blocking open
- Non-blocking read/write
- Return values and error codes when device is temporarily unavailable
- Coordination between interrupt events and polling logic

For MCU scenarios, fully replicating all Linux I/O semantics is costly, so a more pragmatic approach is to first cover the patterns actually used in the project and clearly document uncovered semantics.

## 11. Principles for Interface Implementation

To make subsequent implementation and maintenance more stable, always follow these principles:

### 11.1 Query Interfaces Should Not Forge State

Query interfaces like `sched_getscheduler()` and `sched_getparam()` must read from the thread's current effective state, not from initialization values, cached values, or compile-time constants.

### 11.2 Set Interfaces Must Drive Underlying State Changes

All `set*` interfaces should not just modify an upper-layer structure, but must drive actual changes in thread objects, ready queues, or device state.

### 11.3 Standard and Extension Interfaces Share Unified Underlying Paths

POSIX `sched_*`, `pthread_*sched*`, and zepLinux extension interfaces should share the same underlying bridging logic. Otherwise, as interfaces multiply, state can easily become inconsistent.

### 11.4 Clearly Define "Degree of Compatibility"

There's no need to implement semantics the project won't use just to pursue formal completeness, but the boundaries of what's supported, partially supported, and not yet supported must be clearly documented.

## 12. Related Materials

- Overview document: `docs/zepLinux-interface-and-validation.md`
- Development and build: `docs/zephyr-linux-build-and-dev.md`
- Tests and demos: `docs/test-demos/README.md`
- Documentation index: `docs/zephyr-linux-interface-commits.md`
