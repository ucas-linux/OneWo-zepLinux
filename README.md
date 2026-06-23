# OneWo zepLinux — A New Zephyr OS with Linux API for MCU, Developed by OneWo-rtLinux Team

Implementing a Linux-compatible interface layer on Zephyr RTOS, enabling Linux applications to run on ARM Cortex-M4 and ANSILIC RISC-V 32 microcontrollers with minimal modifications.

## Core Highlights

- 4 Linux-style schedulers (DL/RT/CFS/Idle) deeply integrated into the Zephyr kernel, truly driving `k_thread` scheduling decisions
- Currently documented 39 Linux/POSIX compatible interfaces, covering threads, processes, scheduling, signals, memory, devices, and I/O multiplexing modules
- Supported development boards: [Rocket-Pi](https://www.rocketpi.club/) (`STM32F401RE`, ARM Cortex-M4) and [ANSIC-EVB601](https://ansilic.com/product-center/ansic-evb601) (`AS32X601`, RISC-V 32)

## Supported Development Boards

- ARM: [Rocket-Pi](https://www.rocketpi.club/), based on `STM32F401RE`
- RISC-V: [ANSIC-EVB601](https://ansilic.com/product-center/ansic-evb601), based on `AS32X601`

## Directory Structure

```
OneWo-zepLinux/
├── README.md                       # Project overview (this file)
├── docs/                           # English documentation
│   ├── zephyr-linux-api-reference.md
│   ├── zephyr-linux-interface-definition.md
│   ├── zepLinux-interface-and-validation.md
│   ├── zephyr-linux-build-and-dev.md
│   └── test-demos/                 # Board validation, device demos, benchmarks, scheduler tests
├── docs/zh/                        # Chinese Version
│   ├── README.zh.md                # Project overview (Chinese Version)
│   ├── zephyr-linux-api-reference.zh.md
│   ├── zephyr-linux-build-and-dev.zh.md
│   ├── zephyr-linux-interface-definition.zh.md
│   ├── zepLinux-interface-and-validation.zh.md
│   └── test-demos/                 # 
├── modules/
│   └── hal/
│       ├── ansilic/                # ANSILIC RISC-V 32 HAL
│       └── stm32/                  # STM32 HAL (placeholder)
└── zephyr/                         # Built-in Zephyr source tree and kernel implementation
    ├── kernel/                     # Kernel scheduling and thread core logic
    ├── include/                    # Public header files
    ├── lib/                        # Base libraries
    ├── subsys/                     # Various subsystems
    ├── tests/                      # Zephyr built-in tests
    ├── samples/                    # Zephyr samples
    └── boards/others/              # Custom board support (rocket_pi, stm32f401_mini)
```

## Scheduler Architecture

Enabled through the `CONFIG_SCHED_LINUX` Kconfig option, serving as Zephyr's 4th pluggable ready queue implementation (alongside SIMPLE/SCALABLE/MULTIQ).

```
┌─────────────────────────────────────────┐
│        Zephyr Kernel Scheduler          │
│  _priq_run_add / remove / best / yield  │
└──────────────┬──────────────────────────┘
               │ CONFIG_SCHED_LINUX
               ▼
┌─────────────────────────────────────────┐
│         z_priq_linux_best()             │
│  DL(EDF/CBS) → RT(FIFO/RR) → CFS → Idle│
└──────────────┬──────────────────────────┘
               │
    ┌──────────┼──────────┬───────────┐
    ▼          ▼          ▼           ▼
  DL Queue   RT Queue   CFS Queue   Idle
  Sorted by  bitmap     Sorted by   (Zephyr
  deadline   O(1)       vruntime    built-in)
  ascending  lookup     ascending
```

**Scheduling Class Priority**: DL > RT > CFS > Idle

| Sched Class | Policy | Sort Criteria | Features |
|-------------|--------|---------------|----------|
| DL | SCHED_DEADLINE | Absolute deadline ascending | EDF + CBS throttling/replenishment |
| RT | SCHED_FIFO / SCHED_RR | Priority 1-99 bitmap | FIFO no timeslice, RR 10 tick round-robin |
| CFS | SCHED_NORMAL | vruntime ascending | nice -20..19 weight table, fair allocation |
| Idle | SCHED_IDLE | — | Zephyr idle thread fallback |

## Modified Zephyr Kernel Files

| File | Modification |
|------|--------------|
| `include/zephyr/kernel_structs.h` | Added `struct _priq_linux`, extended `_ready_q` |
| `include/zephyr/kernel/thread.h` | Added scheduling class metadata fields to `_thread_base` |
| `kernel/include/priority_q.h` | Added `CONFIG_SCHED_LINUX` macro + override `z_sched_prio_cmp` |
| `kernel/Kconfig` | Added `CONFIG_SCHED_LINUX` option |
| `kernel/thread.c` | New threads default initialized to CFS class |
| `kernel/timeslicing.c` | Added `z_linux_sched_tick()` hook |
| `kernel/CMakeLists.txt` | Added `linux_sched/` source files |

## Build and Test

### Using Docker (Recommended)

We provide pre-built Docker images with Zephyr SDK 0.17.4 for easy development:

```bash
# Pull the Docker image
docker pull zhouzhouyi/zephyr-sdk:latest
# Or specific version
docker pull zhouzhouyi/zephyr-sdk:0.17.4

# Run the container with your project mounted
docker run -it --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  zhouzhouyi/zephyr-sdk:latest

# Inside the container, build your project
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/schedule_api

# Flash the firmware (requires USB device passthrough)
docker run -it --rm \
  --privileged \
  -v /dev:/dev \
  -v $(pwd):/workspace \
  -w /workspace \
  zhouzhouyi/zephyr-sdk:latest \
  west flash
```

**Docker Image Features:**
- Ubuntu 22.04 base
- Zephyr SDK 0.17.4 pre-installed
- All required build dependencies (CMake, Ninja, device-tree-compiler, etc.)
- West tool and Python dependencies
- Ready to use, no manual SDK installation needed

### Native Build

```bash
# Build kernel integration test (real thread scheduling)
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/schedule_api
```

## Interface Category Overview

| Category | Interface Count | Examples |
|----------|-----------------|----------|
| Thread Management | 7 | pthread_create, pthread_join, pthread_cancel, pthread_mutex_lock, pthread_mutex_unlock, pthread_cond_wait, pthread_cond_signal |
| Process Management | 3 | fork, execve, exit |
| Scheduling & Priority | 3 | sched_yield, sched_setparam, sched_getparam |
| Signals & Timers | 8 | signal, kill, alarm, timer_create, sleep, usleep, nanosleep, timer_settime |
| Memory Management | 3 | malloc, realloc, free |
| Device Management | 5 | open, close, read, write, ioctl |
| IPC & Environment | 3 | pipe, getenv, getuid |
| Time & Process Control | 4 | clock_gettime, wait, waitpid, posix_spawn |
| I/O Multiplexing | 3 | select, poll, epoll |

## Documentation

### English

- Interface API Reference: `docs/zephyr-linux-api-reference.md`
- Interface Design and Kernel Mapping: `docs/zephyr-linux-interface-definition.md`
- Interface and Validation Overview: `docs/zepLinux-interface-and-validation.md`
- Build and Development Guide: `docs/zephyr-linux-build-and-dev.md`
- Test Demo Documentation: `docs/test-demos/`

### Chinese Version

- README: `docs/zh/README.zh.md`
- Interface API Reference:  `docs/zh/zephyr-linux-api-reference.zh.md`
- Interface Design and Kernel Mapping: `docs/zh/zephyr-linux-interface-definition.zh.md`
- Interface and Validation Overview: `docs/zh/zepLinux-interface-and-validation.zh.md`
- Build and Development Guide: `docs/zh/zephyr-linux-build-and-dev.zh.md`
- Test Demo Documentation: `docs/zh/test-demos/`

## License

Apache-2.0
