# RocketPi Shell Process - Ctrl+C Signal Support Implementation

## Overview

Successfully ported commit 1534e5fb (Ctrl+C signal support) from AS32x601 to RocketPi (STM32F401).

## Build Command

```bash
west build -p always -b rocket_pi -d build-rocket-pi-anl zephyr/samples/ansilic/rocket_pi_shell_process
```

## Flash Command

```bash
west flash -d build-rocket-pi-anl --runner openocd
# or use the convenience script:
./flash_rocket_pi_shell.sh
```

## Architecture Compatibility

The implementation now supports both ARM and RISC-V architectures through conditional compilation:

### 1. Debug Stack Trace (debug.c)
- **RISC-V**: Uses `s0` (frame pointer), `ra` (return address)
- **ARM**: Uses `r7` (frame pointer), `lr` (link register)

### 2. Memory Barriers (anl_loader.c)
- **RISC-V**: `fence` + `fence.i` instructions
- **ARM**: `dsb` + `isb` instructions

## Key Features

1. **Ctrl+C Signal Delivery**: Intercepts Ctrl+C (0x03) in UART polling handler
2. **Foreground Process Management**: Sends SIGINT only to foreground process
3. **Process-Based Shell**: Each command runs in separate process
4. **Signal Handling**: Full POSIX-like signal support (SIGINT, SIGTERM, etc.)

## Modified Files

### New Files Created
- `zephyr/samples/ansilic/rocket_pi_shell_process/`
  - `CMakeLists.txt` - Build configuration
  - `prj.conf` - Project configuration (polling mode UART)
  - `sample.yaml` - Test configuration
  - `README.rst` - Documentation
  - `src/main.c` - Main application with shell commands
  - `src/signal.c`, `src/signal.h` - Signal subsystem
  - `src/shell_process.c`, `src/shell_process.h` - Process management
  - `src/commands.c` - Command implementations
  - `src/debug.c`, `src/debug.h` - Debug utilities (ARM/RISC-V compatible)
  - `src/bytecode_vm.c`, `src/bytecode_vm.h` - Bytecode VM
  - `src/anl_loader.c`, `src/anl_loader.h` - ANL loader (ARM/RISC-V compatible)

### Modified Files (Already from commit 1534e5fb)
- `zephyr/subsys/shell/backends/shell_uart.c` - Ctrl+C interception in polling handler

## Usage

### Connect to Serial Console
```bash
picocom -b 115200 /dev/ttyUSB0
```

### Test Ctrl+C Signal
```
shell> loop 10
Loop started (PID 2). Running for 10 seconds...
......^CKilled
shell>
```

### Run Signal Self-Test
```
shell> test_signal
[Testing signal delivery...]
```

### Available Commands
- `loop` - Long-running process for testing interrupts
- `sigint` - Send SIGINT to foreground process
- `test_signal` - Self-test signal delivery
- `ps` - List processes
- `kill` - Terminate a process
- `fork` - Fork child processes
- And many more...

## Firmware Size

```
Memory region         Used Size  Region Size  %age Used
           FLASH:       68928 B       512 KB     13.15%
             RAM:       65560 B        96 KB     66.69%
```

## Implementation Notes

1. **Polling Mode Required**: Uses `CONFIG_SHELL_BACKEND_SERIAL_API_POLLING=y` to intercept Ctrl+C in the polling timer callback

2. **Architecture Abstraction**: Uses `#ifdef CONFIG_ARM` / `#ifdef CONFIG_RISCV` for platform-specific code

3. **Shared Codebase**: Same source files work on both AS32x601 (RISC-V) and RocketPi (ARM)

## Testing Checklist

- [ ] Build successful on RocketPi board
- [ ] Flash successful via OpenOCD
- [ ] Shell boots and shows prompt
- [ ] `loop` command starts and runs
- [ ] Ctrl+C interrupts the loop command
- [ ] `test_signal` passes self-test
- [ ] Process creation/termination works

## Date
2026-08-29
