# Embox Process Model Implementation for Zephyr

## Overview

This implementation brings Linux-style process abstraction to Zephyr RTOS, adapted from Embox kernel architecture. It provides process management with PID allocation, file descriptor tables, environment variables, and parent-child relationships on top of Zephyr's existing thread model.

## Implementation Status

### ✅ Phase 1: Process Structure Mapping (COMPLETED)
- **File**: `zephyr/include/zephyr/kernel/process.h`
- **File**: `zephyr/kernel/process.c`
- Created `struct z_process` with:
  - PID management (pid_t)
  - Parent pointer for process hierarchy
  - Main thread reference (k_tid_t)
  - Children list (sys_dlist)
  - Static allocation for MCU constraints

### ✅ Phase 2: File Descriptor Table (COMPLETED)
- **Structure**: `struct idesc_table` in process.h
- Implemented fixed-size array (CONFIG_MAX_FD_PER_PROCESS = 16)
- Bitmap-based allocation (allocated_mask)
- CLOEXEC flags support in low bit
- APIs:
  - `process_idesc_table_get(proc, fd)` - Get descriptor
  - `process_idesc_table_add(proc, idesc)` - Allocate FD
  - `process_idesc_table_remove(proc, fd)` - Free FD

### ✅ Phase 3: Environment Variables (COMPLETED)
- **Structure**: `struct env_entry` linked list
- Key-value pair storage with dynamic allocation
- Fork copy semantics (deep copy)
- APIs:
  - `process_getenv(proc, name)` - Get variable
  - `process_setenv(proc, name, value)` - Set/update variable
  - Automatic cleanup on process exit

### ✅ Phase 4: Process-Thread Association (COMPLETED)
- **Modified**: `zephyr/include/zephyr/kernel/thread.h`
- Extended `struct k_thread` with:
  - `struct z_process *process` - Back pointer to owning process
  - `sys_dnode_t process_thread_node` - Node in process thread list
- Extended `struct z_process` with:
  - `sys_dlist_t threads` - List of threads in process
- APIs:
  - `process_register_thread(proc, thread)` - Associate thread
  - `process_unregister_thread(proc, thread)` - Dissociate thread
  - `process_current()` - Get current thread's process

### ✅ Phase 5: Lifecycle Management (COMPLETED)
- **APIs Implemented**:
  - `process_create(parent)` - Create new process with PID allocation
  - `process_get(pid)` - Lookup process by PID
  - `process_exit(proc, exit_code)` - Terminate and cleanup
  - `process_fork(parent)` - Copy-on-write fork (fd table + env copied)
- **Init Process**: PID 1 created automatically at kernel initialization
- **Resource Cleanup**: Automatic cleanup of FD table and environment on exit

### ✅ POSIX Integration (COMPLETED)
- **Modified**: `zephyr/lib/posix/options/multi_process.c`
- Updated `getpid()` to return actual process PID instead of hardcoded 42
- Now properly returns `process_current()->pid`

## Configuration

### Kconfig Options
Added to `zephyr/kernel/Kconfig`:

```kconfig
menuconfig PROCESS_MODEL
    bool "Linux-style process model support"
    depends on MULTITHREADING
    default n

config MAX_PROCESS_COUNT
    int "Maximum number of processes"
    default 16
    range 1 64

config MAX_FD_PER_PROCESS
    int "Maximum file descriptors per process"
    default 16
    range 1 64
```

### Build Integration
- Added `process.c` to `zephyr/kernel/CMakeLists.txt`
- Conditional compilation based on `CONFIG_PROCESS_MODEL`

## Architecture

### Memory Constraints (MCU-focused)
- Static process table: `struct z_process process_table[CONFIG_MAX_PROCESS_COUNT]`
- Fixed-size FD table per process (no dynamic growth)
- Bitmap-based allocation for O(1) lookups
- Optional environment variables (can be disabled for memory savings)

### Data Structures

```c
struct z_process {
    pid_t pid;                        // Process ID (unique)
    struct z_process *parent;         // Parent process pointer
    struct k_thread *main_thread;     // Main thread
    sys_dlist_t children;             // Child processes
    sys_dlist_t threads;              // All threads in process
    struct idesc_table fd_table;      // File descriptors
    sys_dlist_t env_list;             // Environment variables
    atomic_t ref_count;               // Reference counting
    int exit_code;                    // Exit status
};
```

### Thread Safety
- `struct k_spinlock process_lock` protects all process table operations
- Atomic reference counting for process lifecycle
- Safe concurrent access from multiple threads

## Test Suite

### Location
`zephyr/tests/kernel/process/`

### Test Coverage
1. **test_process_create** - Process creation and PID allocation
2. **test_process_parent_child** - Parent-child relationships
3. **test_process_get** - PID lookup
4. **test_process_fd_table** - File descriptor allocation/deallocation
5. **test_process_env** - Environment variable get/set/update
6. **test_process_fork** - Fork with resource copying
7. **test_thread_registration** - Thread-process association
8. **test_getpid** - POSIX getpid() integration

### Running Tests
```bash
# Enable process model
CONFIG_PROCESS_MODEL=y
CONFIG_MAX_PROCESS_COUNT=16
CONFIG_MAX_FD_PER_PROCESS=16

# Build and run
west build -b <board> zephyr/tests/kernel/process
west build -t run
```

## Usage Example

```c
#include <zephyr/kernel/process.h>

void example_usage(void)
{
    // Create a process
    struct z_process *proc = process_create(NULL);

    // Register current thread
    process_register_thread(proc, k_current_get());

    // Add file descriptor
    void *file_obj = open_some_file();
    int fd = process_idesc_table_add(proc, file_obj);

    // Set environment variable
    process_setenv(proc, "MY_VAR", "my_value");

    // Get PID
    pid_t pid = getpid();  // Returns proc->pid

    // Fork process (creates child with copied resources)
    struct z_process *child = process_fork(proc);

    // Clean up
    process_exit(proc, 0);
}
```

## Integration with Existing Zephyr Code

### Modified Files
1. `zephyr/include/zephyr/kernel/thread.h` - Added process pointer to k_thread
2. `zephyr/kernel/CMakeLists.txt` - Added process.c compilation
3. `zephyr/kernel/Kconfig` - Added PROCESS_MODEL configuration
4. `zephyr/lib/posix/options/multi_process.c` - Updated getpid() implementation

### New Files
1. `zephyr/include/zephyr/kernel/process.h` - Process API header
2. `zephyr/kernel/process.c` - Process implementation
3. `zephyr/tests/kernel/process/` - Test suite

## Performance Characteristics

- **PID Allocation**: O(N) where N = MAX_PROCESS_COUNT
- **Process Lookup**: O(N) linear search (acceptable for small N on MCU)
- **FD Allocation**: O(N) where N = MAX_FD_PER_PROCESS
- **FD Lookup**: O(1) array access
- **Environment Lookup**: O(N) linked list traversal

## Future Enhancements

### Not Yet Implemented (from plan.md)
- Full fork/exec semantics (limited value on MCU)
- Copy-on-write (COW) for fork optimization
- Process groups and sessions
- Signal delivery to processes
- Wait/waitpid for process synchronization

### Suggested Improvements
1. Hash table for faster process lookup by PID
2. More efficient environment variable storage (e.g., hash table)
3. Integration with VFS layer for file descriptor objects
4. Process priority inheritance
5. Resource limits per process (RLIMIT_*)

## Comparison with Embox

### Similarities
- Process structure with PID, parent, main thread
- File descriptor table with CLOEXEC support
- Environment variable storage
- Parent-child relationship tracking

### Differences (MCU Adaptations)
- Static allocation instead of dynamic
- Fixed maximum process count
- Simplified resource management
- No MMU/memory protection
- Tighter integration with Zephyr scheduler

## Documentation References

- Original plan: `/opt/Program/UCAS/embox/plan.md`
- Zephyr thread documentation: `zephyr/include/zephyr/kernel/thread.h`
- POSIX compatibility: `zephyr/lib/posix/options/multi_process.c`

## Authors

OneWo-rtLinux Team, 2024

## License

Apache-2.0
