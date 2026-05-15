# Zephyr Linux Build and Development Guide

This document is oriented toward daily development and primarily answers the following practical questions:

- Where code is located
- Which files typically need to be modified when changing the Linux compatibility layer
- How to build, run, and debug existing tests
- How to coordinate between this repository and external Zephyr workspaces
- Where to look first when encountering build issues, semantic issues, or integration problems

This is not a replacement for the official Zephyr environment installation manual, but rather an engineering guide tailored to this project's existing structure.

## 1. Repository Structure and Development Entry Points

### 1.1 Role of This Repository

From the current directory structure, this repository contains documentation, maintains a copy of Zephyr source code, and includes header files and implementation drafts for the Linux compatibility layer itself.

Top-level key directories:

```text
OneWo-zepLinux/
├── code/
├── docs/
├── tests/
├── posix-zephyr-spec/
└── zephyr/
```

It can be understood as a combined repository of "design materials + compatibility layer code + kernel modification carrier."

### 1.2 `code/` Directory

The `code/` directory primarily contains interface definitions and implementations for the compatibility layer itself, typically the first landing point for upper-layer APIs.

Examples:

- `code/include/lmz_sched.h`
- `code/include/lmz_thread.h`
- `code/include/lmz_process.h`
- `code/include/lmz_memory.h`
- `code/include/lmz_signal.h`
- `code/include/lmz_device.h`

If adding interfaces, changing function signatures, adding parameter structures, or supplementing header file descriptions, typically start here.

### 1.3 `zephyr/` Directory

The `zephyr/` directory is the repository's built-in copy of Zephyr kernel source code. Modifications truly related to the Linux compatibility layer are generally completed here.

The current README already provides a fairly clear subdirectory:

```text
zephyr/kernel/linux_sched/
```

If you need to modify scheduling class implementations, ready queue behavior, timeslice logic, or scheduling comparison functions, this is typically the main battleground.

### 1.4 `docs/` Directory

The `docs/` directory is now organized by purpose:

- `docs/zepLinux-interface-and-validation.md`: Overview and validation system
- `docs/zephyr-linux-interface-definition.md`: Interface definitions
- `docs/zephyr-linux-build-and-dev.md`: Development and build methods
- `docs/test-demos/`: Test and demo documentation

### 1.5 `tests/demo/` Directory

To align test documentation with repository structure, a new `tests/demo/` directory has been added, organized by theme:

- `tests/demo/scheduler/`
- `tests/demo/device/`
- `tests/demo/board/`
- `tests/demo/perf/`

This directory currently contains READMEs and explanatory materials, with room to add actual compilable demo code later.

During development, it's recommended to maintain documentation synchronously rather than waiting until interfaces are complete to add it all at once.

## 2. Key Source Code Locations to Focus On

Based on the README and existing documentation, here are several key modification points.

### 2.1 Thread Structure and Metadata

- `zephyr/include/zephyr/kernel/thread.h`

This typically involves thread base metadata. If adding scheduling policy fields or compatibility layer state fields to threads, this file is often modified.

### 2.2 Ready Queue and Kernel Structures

- `zephyr/include/zephyr/kernel_structs.h`
- `zephyr/kernel/include/priority_q.h`

The former is more data structure-oriented, the latter more focused on scheduling comparison logic and priority queue implementation. When Linux-style schedulers coexist with Zephyr's native scheduler, these two locations typically need coordinated changes.

### 2.3 Scheduler Implementation and Tick Logic

- `zephyr/kernel/thread.c`
- `zephyr/kernel/timeslicing.c`
- `zephyr/kernel/linux_sched/`

These locations collectively determine:

- How new threads initialize default scheduling classes
- When RR timeslices decrement and rotate
- How Linux-style scheduling classes participate in thread selection logic

### 2.4 Build Entry Points

- `zephyr/kernel/CMakeLists.txt`
- `zephyr/kernel/Kconfig`

Many "code is written but build fails" issues ultimately come down to these files, such as:

- New source files not included in the build
- New Kconfig options not properly declared or incomplete dependency relationships
- `prj.conf` enables an option but the build system has no corresponding handling

## 3. Relationship Between External Workspace and This Repository

The test commands in the README depend on an external workspace:

```text
/home/zzy/Program/UCAS/zephyrproject/
├── linux_sched/
└── linux_sched_kernel/
```

This indicates that current testing is not necessarily completed directly in this repository's root directory, but rather through building independent test projects in an external Zephyr workspace.

This type of structure is very common in actual development, but also most prone to "source code has been changed, but tests didn't pick up the changes" issues.

It's recommended to focus on confirming the following:

- Which copy of Zephyr source code the external workspace uses
- Whether the built-in `zephyr/` in this repository is synchronized with Zephyr in the external workspace
- Whether the test project references this repository's compatibility layer code or a copy elsewhere

If these three things are unclear, troubleshooting build and behavior issues later will be very time-consuming.

## 4. Environment Preparation

### 4.1 Basic Tools

Current development requires at least these toolchain components:

- `west`
- Zephyr SDK
- `cmake`
- `ninja`
- `python3` (with required packages)

### 4.2 Zephyr SDK Installation

Follow the official Zephyr Getting Started Guide to install the SDK. Typical installation path:

```bash
~/zephyr-sdk-<version>/
```

### 4.3 West Workspace Initialization

If working in an external workspace:

```bash
west init -m https://github.com/zephyrproject-rtos/zephyr --mr main zephyrproject
cd zephyrproject
west update
```

If working directly in this repository:

```bash
cd OneWo-zepLinux
west init -l .
west update
```

## 5. Build Process

### 5.1 Basic Build Command

```bash
west build -b <board> <application_path>
```

Example:

```bash
west build -b rocket_pi/stm32f401xe zephyr/tests/kernel/sched/schedule_api
```

### 5.2 Clean Build

```bash
west build -t clean
# or
rm -rf build/
```

### 5.3 Configuration

View and modify configuration:

```bash
west build -t menuconfig
```

Or directly edit `prj.conf`:

```
CONFIG_SCHED_LINUX=y
CONFIG_TIMESLICING=y
CONFIG_TIMESLICE_SIZE=10
```

### 5.4 Flash to Board

```bash
west flash
```

### 5.5 View Serial Output

```bash
minicom -D /dev/ttyUSB0 -b 115200
# or
screen /dev/ttyUSB0 115200
```

## 6. Test Execution

### 6.1 Unit Tests

Located in `tests/unit/`:

```bash
# Thread tests
west build -b rocket_pi/stm32f401xe tests/unit/test_thread
west flash

# Scheduling tests
west build -b rocket_pi/stm32f401xe tests/unit/test_sched
west flash

# Memory tests
west build -b rocket_pi/stm32f401xe tests/unit/test_memory
west flash
```

### 6.2 Integration Tests

Located in `tests/integration/`:

```bash
west build -b rocket_pi/stm32f401xe tests/integration/test_process
west flash
```

### 6.3 Demo Programs

Located in `tests/demo/`:

```bash
# Scheduler demos
west build -b rocket_pi/stm32f401xe tests/demo/scheduler/rr_vs_fifo
west flash

# Device demos
west build -b rocket_pi/stm32f401xe tests/demo/device/gpio_control
west flash
```

## 7. Common Issues and Troubleshooting

### 7.1 Build Failures

**Symptom**: Compilation errors or linker errors

**Common causes**:
1. New source files not added to `CMakeLists.txt`
2. Kconfig options not properly configured
3. Header file paths incorrect
4. Missing dependencies

**Troubleshooting steps**:
1. Check `zephyr/kernel/CMakeLists.txt` includes new source files
2. Verify `CONFIG_SCHED_LINUX=y` in `prj.conf`
3. Check header include paths
4. Run `west build -t clean` and rebuild

### 7.2 Scheduling Interface Issues

When scheduling interfaces don't behave as expected, check in this order:

1. Are parameters correctly passed from the interface layer?
2. Does the mapping layer correctly translate policy and priority?
3. Does the thread object save the new state?
4. Does the scheduling path actually consume these states?
5. Can query interfaces correctly read current state?

Following this sequence typically helps locate the breakpoint quickly.

### 7.3 Device Control Path Issues

For device issues, troubleshoot in this order:

- First confirm if `fd` is successfully established
- Then confirm if `ioctl` command and parameter structure match
- Then confirm if blocking/non-blocking semantics return as expected
- Finally check Zephyr driver or board adaptation layer

Many device issues appear to be driver bugs on the surface but are actually inconsistencies in compatibility layer command dispatch or parameter interpretation.

## 8. Documentation Maintenance Recommendations

To avoid materials becoming mixed again, it's recommended to strictly follow boundaries when adding content:

- Interface names, parameters, semantics, return values: Write to `docs/zephyr-linux-interface-definition.md`
- Environment, commands, procedures, debugging experience: Write to `docs/zephyr-linux-build-and-dev.md`
- Test steps, serial output phenomena, performance records: Write to `docs/test-demos/`
- Overall design and validation system description: Keep in `docs/zepLinux-interface-and-validation.md`

This way, whether new people are taking over or tracing historical decisions, the search cost will be much lower.

## 9. Current Test and Demo Landing Points

The current repository has formed two layers of validation entry points:

- Automated unit tests: `tests/unit/test_thread.c`, `tests/unit/test_memory.c`, `tests/unit/test_sched.c`
- Manual observation demos: `tests/demo/scheduler/rr_vs_fifo/`, `tests/demo/scheduler/priority_mapping/`, `tests/demo/scheduler/rr_interval/`

It's recommended that after modifying scheduling interfaces, first check `tests/unit/test_sched.c`, then run examples under `tests/demo/scheduler/` as needed to observe behavior.
