# OneWo zepLinux — A New Zephyr OS with Linux API for MCU, Developed by OneWo-rtLinux Team

Implementing a Linux-compatible interface layer on Zephyr RTOS, enabling Linux applications to run on ARM Cortex-M3 microcontrollers with minimal modifications.

## Core Highlights

- 4 Linux-style schedulers (DL/RT/CFS/Idle) deeply integrated into the Zephyr kernel, truly driving `k_thread` scheduling decisions
- Currently documented 39 Linux/POSIX compatible interfaces, covering threads, processes, scheduling, signals, memory, devices, and I/O multiplexing modules
- Target platforms: rocket_pi/stm32f401xe (Arm Cortex M3), ANSILIC/as32x601 (RISC-V 32)

## Directory Structure

```
OneWo-zepLinux/
├── README.md                # Project overview, architecture description, interface and test index
├── docs/                    # Design documents, migration guides, interface definitions and demo descriptions
│   ├── design-principles.md
│   ├── linux-sched-kernel-integration.md
│   ├── migration-guide.md
│   ├── zepLinux-interface-and-validation.md
│   └── test-demos/          # Board validation, device demos, performance benchmarks, scheduler test docs
├── scripts/                 # Development helper scripts
│   ├── setup.sh
│   ├── build.sh
│   ├── test.sh
│   ├── check.sh
│   └── format.sh
├── tests/                   # Examples, unit, integration, stress and demo tests
│   ├── examples/hello_lmz/  # Minimal example program
│   ├── unit/                # `thread` / `sched` / `memory` unit tests
│   ├── integration/         # Process integration tests
│   ├── stress/              # Stress tests
│   └── demo/                # Scheduler, device, board, performance demos
├── posix-zephyr-spec/       # Linux/POSIX compatibility layer specifications and interface breakdown docs
│   ├── specs/               # Module-specific interface specs (threads, processes, scheduling, signals, etc.)
│   ├── tracking/            # Roadmap, status tracking, Zephyr modification records
│   ├── examples/            # Specification examples
│   └── scripts/             # Specification helper scripts
└── zephyr/                  # Built-in Zephyr source tree and kernel implementation
    ├── kernel/              # Kernel scheduling and thread core logic
    ├── include/             # Public header files
    ├── lib/                 # Base libraries
    ├── subsys/              # Various subsystems
    ├── tests/               # Zephyr built-in tests
    ├── samples/             # Zephyr samples
    └── boards/              # Board support packages

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

## Documentation Index

### Design and Usage Documentation Overview

- Project Overview: `README.md`
- Chinese Formal Interface Definition Document: `docs/zephyr-linux-api-reference-zh.md`
- Interface Design and Kernel Mapping Description: `docs/zephyr-linux-interface-definition.md`
- Interface and Validation Overview: `docs/zepLinux-interface-and-validation.md`
- Build and Development Methods: `docs/zephyr-linux-build-and-dev.md`
- Test Demo Documentation: `docs/test-demos/`



## Interface Documentation and Validation Materials

- Interface and Validation Overview: `docs/zepLinux-interface-and-validation.md`
- Interface Definition Document: `docs/zephyr-linux-interface-definition.md`
- Build and Development Methods: `docs/zephyr-linux-build-and-dev.md`
- Test Demo Documentation: `docs/test-demos/`
- Actual Demo Directory: `tests/demo/`
- Documentation Index Page: `docs/zephyr-linux-interface-commits.md`

## License

Apache-2.0
