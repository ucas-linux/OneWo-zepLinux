# Quick Start Guide - Shell Process Execution

## Build and Flash

```bash
cd /opt/Program/UCAS/OneWo-zepLinux

# Build
west build -b as32x601_evb/as32x601 zephyr/samples/ansilic/as32x601_shell_process -p

# Flash
west flash
```

## Key Files

| File | Purpose |
|------|---------|
| `src/shell_process.h` | API definitions |
| `src/shell_process.c` | Core implementation (new_task, waitpid, etc.) |
| `src/commands.c` | Sample commands |
| `src/main.c` | Main shell loop |

## API Reference

### Create Process
```c
pid_t new_task(const char *name, void *(*run)(void *), void *arg);
```

### Execute Command
```c
int shell_exec_command(const struct shell_cmd *cmd,
                       int argc, char **argv, bool on_fg);
```

### Register Command
```c
SHELL_CMD_REGISTER(name, "description", function);
```

### Example Command
```c
static int cmd_mycommand(int argc, char **argv)
{
    struct z_process *proc = process_current();
    printk("Running in PID %d\n", proc->pid);
    return 0;
}

SHELL_CMD_REGISTER(mycommand, "My command", cmd_mycommand);
```

## Architecture Overview

```
┌─────────────────────────────────────────┐
│         User Input (Shell Loop)          │
└──────────────────┬──────────────────────┘
                   │
                   ├─ parse_command()
                   │
                   ├─ shell_cmd_lookup()
                   │
                   └─ shell_exec_command()
                        │
                        ├─ new_task()
                        │    │
                        │    ├─ process_create()
                        │    ├─ k_thread_create()
                        │    └─ task_trampoline()
                        │         │
                        │         └─ run_cmd()
                        │              │
                        │              └─ cmd->exec()
                        │
                        └─ waitpid() [wait for exit]
```

## Memory Layout

```
Process Structure (~100 bytes)
├─ PID
├─ Parent pointer
├─ Main thread
├─ Children list
├─ Thread list
├─ FD table
└─ Environment variables

Thread Stack (2048 bytes)
├─ Local variables
├─ Function call frames
└─ Trampoline args

Total: ~2.2KB per process
```

## Embox vs Zephyr Mapping

| Embox | Zephyr | Function |
|-------|--------|----------|
| `new_task()` | `new_task()` | Create process |
| `task_trampoline()` | `task_trampoline()` | Thread entry |
| `process_external()` | `shell_exec_command()` | Execute command |
| `run_cmd()` | `run_cmd()` | Command wrapper |
| `struct task` | `struct z_process` | Process structure |
| `struct cmd` | `struct shell_cmd` | Command descriptor |

## Configuration

Edit `prj.conf`:

```ini
CONFIG_MAX_PROCESS_COUNT=32      # Max processes
CONFIG_MAX_FD_PER_PROCESS=16     # FDs per process
CONFIG_HEAP_MEM_POOL_SIZE=16384  # Heap size
```

Edit `shell_process.c`:

```c
#define MAX_COMMANDS 32          // Max registered commands
#define TASK_STACK_SIZE 2048     // Stack per process
```

## Troubleshooting

### Build Errors

**Problem**: `struct process_exit_msg` incomplete type
**Solution**: Move struct definition before `K_MSGQ_DEFINE`

**Problem**: Conflicting shell registration
**Solution**: Use simple shell loop, avoid mixing with Zephyr shell

### Runtime Issues

**Problem**: Process creation fails
**Solution**: Increase heap size (`CONFIG_HEAP_MEM_POOL_SIZE`)

**Problem**: Stack overflow
**Solution**: Increase `TASK_STACK_SIZE` in `shell_process.c`

**Problem**: Commands not found
**Solution**: Check command registration with `SHELL_CMD_REGISTER`

## Testing

### Basic Test
```
shell> hello
Hello from process PID 2!
```

### Process List
```
shell> ps
PID    PPID   Command
------------------------
2      1      ps
1      0      init
```

### Stress Test
```
shell> test
Test command running in PID 5
Creating 3 child processes...
  Created child process PID 6
  Created child process PID 7
  Created child process PID 8
```

## Performance

- Process creation: 5-10 ms
- Memory per process: ~2.2 KB
- Max concurrent: 32 (configurable)
- ROM usage: ~82 KB
- RAM usage: ~32 KB

## Next Steps

1. Add more commands to `commands.c`
2. Implement background execution (`&`)
3. Add signal support (SIGINT, SIGTERM)
4. Implement I/O redirection
5. Add process groups and sessions
