# Ctrl+C Signal Implementation for AS32x601 Shell

## Overview
Successfully implemented Ctrl+C keyboard interrupt support for the AS32x601 shell with process support. This allows users to interrupt foreground processes using Ctrl+C, similar to standard Unix shells.

## Implementation Details

### Modified Files
1. **zephyr/subsys/shell/backends/shell_uart.c**
   - Added signal handling in UART polling backend
   - Intercepts Ctrl+C (0x03) in `polling_rx_timeout_handler()`
   - Sends SIGINT to foreground process group
   - Prevents Ctrl+C from reaching shell input buffer

### Key Changes

#### 1. Signal Support Definitions
```c
/* Signal support for process-based shell */
typedef int pid_t;
extern pid_t signal_get_foreground_pgid(void);
extern int kill(pid_t pid, int signo);
#define SIGINT 2
#define CTRL_C 0x03
```

#### 2. Polling RX Handler
```c
static void polling_rx_timeout_handler(struct k_timer *timer)
{
    uint8_t c;
    struct shell_uart_polling *sh_uart = k_timer_user_data_get(timer);

    while (uart_poll_in(sh_uart->common.dev, &c) == 0) {
        /* Check for Ctrl+C and send signal to foreground process */
        if (c == CTRL_C) {
            pid_t fg_pgid = signal_get_foreground_pgid();
            if (fg_pgid > 0) {
                kill(fg_pgid, SIGINT);
                continue; // Consume Ctrl+C, don't pass to shell
            }
        }
        // Normal character handling...
    }
}
```

## Usage

### Running a Command
```
shell> loop
Loop started (PID 2). Running for 10 seconds...
[SIGNAL] Foreground process group set to PID=2
.........
```

### Interrupting with Ctrl+C
Press `Ctrl+C` to send SIGINT to the foreground process:
```
shell> loop
Loop started (PID 2). Running for 10 seconds...
.......^CKilled
shell>
```

## Technical Architecture

### Signal Flow
1. User presses Ctrl+C (0x03)
2. UART polling timer callback receives character
3. `polling_rx_timeout_handler()` detects Ctrl+C
4. Calls `signal_get_foreground_pgid()` to get foreground process
5. Calls `kill(pid, SIGINT)` to send signal
6. Process signal handler is invoked
7. Process terminates or handles signal

### Important Notes
- **Polling Mode Only**: Implementation works in UART polling mode (CONFIG_SHELL_BACKEND_SERIAL_API_POLLING=y)
- **No CONFIG_PROCESS Dependency**: Removed `#ifdef CONFIG_PROCESS` guards to ensure code is always compiled
- **Single Terminal**: Works with single terminal (no need for dual terminal setup)
- **Clean Integration**: Minimal changes to Zephyr codebase, only shell UART backend modified

## Build Configuration
The shell application uses these key configs:
```
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
CONFIG_MAX_PROCESS_COUNT=32
CONFIG_SHELL_BACKEND_SERIAL_RX_POLL_PERIOD=10
```

## Testing
Verified working commands:
- `loop` - Long-running process that can be interrupted
- `test_signal` - Signal delivery self-test
- `sigint` - Manual signal sending (from second terminal)

## Comparison with anl_loader_qemu
The AS32x601 implementation differs from anl_loader_qemu:
- **anl_loader_qemu**: Raw UART polling in main loop, no shell subsystem
- **AS32x601**: Uses Zephyr shell subsystem, requires intercepting in UART backend

## ROM Size
Final implementation: 91,892 bytes ROM (clean version without debug output)

## Related Commits
- Reference: commit 3807aee1 (anl_loader_qemu Ctrl+D signal support)
- This implementation adapts the approach for shell-based environment

## Date
2026-08-27
