# Shell Process Execution Implementation

## Overview

This implementation ports Embox's shell command execution mechanism to Zephyr RTOS. Each shell command executes in a separate process created via `new_task()`, providing process isolation similar to Unix shells.

## Architecture

### Key Components

1. **Process Management** (`zephyr/kernel/process.c`)
   - Process creation and lifecycle management
   - Process table with parent-child relationships
   - Resource management (file descriptors, environment variables)

2. **Task Creation** (`shell_process.c::new_task()`)
   - Creates new process with dedicated thread
   - Implements Embox-style task creation API
   - Manages process hierarchy and resource inheritance

3. **Command Execution** (`shell_process.c::shell_exec_command()`)
   - Parses command line into argc/argv
   - Creates new process for command execution
   - Waits for foreground commands, returns immediately for background

4. **Command Registry** (`commands.c`)
   - Command registration framework
   - Sample commands: hello, echo, ps, sleep, test, getpid, info, help

## Implementation Details

### new_task() Function

Based on Embox's `new_task()` in `src/kernel/task/multi/multi.c`:

```c
pid_t new_task(const char *name, void *(*run)(void *), void *arg)
```

**Steps:**
1. Get current process as parent
2. Create child process with `process_create(parent)`
3. Allocate thread stack and structure
4. Create trampoline argument structure
5. Register thread with process
6. Create and start thread with `k_thread_create()`
7. Return child process PID

**Trampoline Mechanism:**
- Thread entry point is `task_trampoline()`
- Trampoline calls user function: `result = arg->run(arg->run_arg)`
- On exit, sends notification via message queue
- Enables `waitpid()` to collect exit status

### Command Execution Flow

Similar to Embox's `tish.c::process_external()`:

```
User Input → parse_command()
    ↓
shell_cmd_lookup() → find command in registry
    ↓
shell_exec_command() → new_task("", run_cmd, &cmd_data)
    ↓
new_task() creates:
    - Child process (process_create)
    - Thread (k_thread_create)
    - Trampoline setup
    ↓
task_trampoline() → run_cmd() → cmd->exec() → exit
    ↓
waitpid() collects exit status (foreground)
```

### Process Structure

From `zephyr/include/zephyr/kernel/process.h`:

```c
struct z_process {
    pid_t pid;                        /* Process ID */
    struct z_process *parent;         /* Parent process */
    struct k_thread *main_thread;     /* Main thread */
    sys_dlist_t children;             /* Child processes */
    sys_dlist_t threads;              /* Thread list */
    struct idesc_table fd_table;      /* File descriptors */
    sys_dlist_t env_list;             /* Environment variables */
    atomic_t ref_count;
    int exit_code;
};
```

### Command Data Structure

Based on Embox's `struct cmd_data`:

```c
struct cmd_data {
    int argc;
    char **argv;
    const struct shell_cmd *cmd;
    bool on_fg;              /* Foreground flag */
    volatile int started;    /* Synchronization */
    int result;              /* Exit code */
};
```

## Differences from Embox

1. **Threading Model**
   - Embox: Custom thread implementation
   - Zephyr: Uses `k_thread` API

2. **Memory Management**
   - Embox: Static task table + stack allocation
   - Zephyr: Dynamic allocation with `k_malloc()`

3. **Synchronization**
   - Embox: `sched_lock()` for atomic operations
   - Zephyr: `k_spinlock` and `k_mutex`

4. **Process Exit**
   - Embox: `waitpid()` checks task status directly
   - Zephyr: Message queue for exit notifications

## Usage Example

```c
/* Define command function */
static int cmd_hello(int argc, char **argv)
{
    struct z_process *proc = process_current();
    printk("Hello from PID %d!\n", proc->pid);
    return 0;
}

/* Register command */
SHELL_CMD_REGISTER(hello, "Print hello", cmd_hello);

/* Execute in new process */
char *argv[] = {"hello"};
shell_exec_command(cmd, 1, argv, true);
```

## Building and Testing

### Build for AS32X601

```bash
cd /opt/Program/UCAS/OneWo-zepLinux
west build -b as32x601_evb/as32x601 \
    zephyr/samples/ansilic/as32x601_shell_process
```

### Flash to Board

```bash
west flash
```

### Expected Output

```
shell> hello
Hello from process PID 2!

shell> ps
PID    PPID   Command
------------------------
2      1      ps
1      0      init

shell> sleep 1000
Sleeping for 1000 ms in PID 3...
Woke up!

shell> test
Test command running in PID 4
Creating 3 child processes...
  Created child process PID 5
  Created child process PID 6
  Created child process PID 7
```

## Configuration

### Kconfig Options

```
CONFIG_MAX_PROCESS_COUNT=32          # Max processes
CONFIG_MAX_FD_PER_PROCESS=16         # FDs per process
CONFIG_HEAP_MEM_POOL_SIZE=16384      # Heap size
CONFIG_SHELL_STACK_SIZE=2048         # Shell stack
CONFIG_DYNAMIC_THREAD=y              # Dynamic threads
```

### Process Stack Size

Defined in `shell_process.c`:

```c
#define TASK_STACK_SIZE 2048
```

## References

### Embox Source Files

- `/opt/Program/UCAS/embox/src/cmds/shell/tish.c` - Shell implementation
- `/opt/Program/UCAS/embox/src/kernel/task/multi/multi.c` - Task creation
- `/opt/Program/UCAS/embox/src/include/kernel/task.h` - Task API

### Zephyr Implementation

- `zephyr/kernel/process.c` - Process management
- `zephyr/include/zephyr/kernel/process.h` - Process API
- `samples/ansilic/as32x601_shell_process/` - This implementation

## Future Enhancements

1. **Background Execution**
   - Support `&` suffix for background commands
   - Job control (jobs, fg, bg commands)

2. **Signal Support**
   - SIGINT (Ctrl+C) handling
   - SIGTERM for process termination

3. **I/O Redirection**
   - File descriptor redirection (>, <, >>)
   - Pipe support (|)

4. **Process Groups**
   - Process group management
   - Session control

5. **exec() Family**
   - `execv()`, `execve()` for program loading
   - Dynamic command loading

## Performance Considerations

- Process creation overhead: ~5-10ms on AS32X601
- Memory per process: ~2KB (stack) + process structure
- Maximum concurrent processes: 32 (configurable)

## Known Limitations

1. No dynamic command loading (all commands linked at compile time)
2. No fork() implementation (only process_create)
3. Simplified waitpid() (message queue based)
4. No signal handling
5. No I/O redirection
