# zepLinux Interface Definition Document (Official Version)


- Interface function descriptions
- Input parameter definitions
- Output parameter definitions and return value semantics



## 1. Document Purpose and Scope

This project does not port the entire Linux kernel to MCUs, but rather implements a Linux/POSIX-style compatibility interface layer on top of Zephyr RTOS, enabling Linux application code to be migrated to resource-constrained platforms such as ARM Cortex-M3 with minimal modifications.

The "interface definitions" in this document have the following characteristics:

1. **Interface form is as compatible with POSIX/Linux as possible**: Function names, parameter forms, and return value styles are kept consistent wherever possible.
2. **Interface semantics closely follow Linux**: For common capabilities such as threads, scheduling, devices, and signals, the behavior model familiar to application developers is preserved as much as possible.
3. **Implementation scope is tailored to MCU scenarios**: For semantics requiring MMU, complete user-space isolation, complex VFS, or full process models, only the implementable subset needed for project validation is retained.
4. **Underlying implementation is based on Zephyr native mechanisms**: Threads are based on `k_thread`, synchronization is based on Zephyr kernel objects, and device access is based on the Zephyr driver model and file descriptor compatibility layer.

## 2. Interface Category Overview

This document divides interfaces into the following twelve categories:

1. Thread Management Interfaces
2. Process Management Interfaces
3. Scheduling and Priority Interfaces
4. Signal and Timer Interfaces
5. Memory Management Interfaces
6. Device Management Interfaces
7. File System and Directory Interfaces
8. File Descriptor Operation Interfaces
9. Network Socket Interfaces
10. IPC and Environment Interfaces
11. Time and Process Control Interfaces
12. I/O Multiplexing Interfaces

---

## 3. Thread Management Interfaces

### 3.1 `pthread_create`

**Function Prototype**
```c
#include <pthread.h>

int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
```

**Function Description**
Creates a new thread and starts its execution from the entry function specified by `start_routine`. This interface is the primary entry point for applications to create concurrent execution entities, presenting POSIX thread semantics to the application layer while mapping to Zephyr thread objects and scheduling entities underneath.

**Input Parameter Definitions**
- `thread`: Output location for the thread handle provided by the caller, must point to writable memory.
- `attr`: Pointer to thread attribute object; if `NULL`, default thread attributes are used.
- `start_routine`: Entry function pointer for the new thread, with signature `void *(*)(void *)`.
- `arg`: Single parameter passed to the thread entry function, as a void pointer.

**Output Parameter Definitions**
- `thread`: Upon successful creation, the thread identifier of the new thread is written here.

**Return Value Definitions**
- `0`: Thread created successfully.
- `EAGAIN`: Insufficient system resources to create a new thread.
- `EINVAL`: Thread attributes are invalid or unsupported.
- `EPERM`: Caller lacks permission to set the requested scheduling attributes.

**Implementation Notes**
- Stack space for the new thread is typically dynamically allocated from the heap.
- Thread priority needs to be mapped from the POSIX semantic space to the Zephyr kernel priority space.
- After thread creation, it should be included in a unified resource tracking table for cleanup and debugging.

---

### 3.2 `pthread_join`

**Function Prototype**
```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

**Function Description**
Waits for the target thread to finish execution and optionally retrieves its exit return value. This interface is used for lifecycle synchronization between threads, ensuring that one thread can wait in an orderly manner for another thread to complete its work.

**Input Parameter Definitions**
- `thread`: ID of the target thread to wait for.
- `retval`: Pointer to receive the target thread's return value; can be `NULL` if the return value is not needed.

**Output Parameter Definitions**
- `retval`: When this parameter is non-null and the wait succeeds, the target thread's exit return value is written here.

**Return Value Definitions**
- `0`: Wait successful, target thread has exited.
- `ESRCH`: Target thread does not exist.
- `EINVAL`: Target thread is not joinable, e.g., already detached.
- `EDEADLK`: Wait relationship would cause deadlock, e.g., thread waiting for itself.

**Implementation Notes**
- After a successful join, resources associated with the target thread should be released.
- The target thread must be in a joinable state.

---

### 3.3 `pthread_cancel`

**Function Prototype**
```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

**Function Description**
Sends a cancellation request to the target thread. Whether the thread exits immediately depends on its cancellation state and cancellation point settings. This interface provides "cooperative termination" rather than forcibly destroying the thread.

**Input Parameter Definitions**
- `thread`: ID of the target thread to receive the cancellation request.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Cancellation request sent successfully.
- `ESRCH`: Target thread does not exist.

**Implementation Notes**
- Cancellation requests typically take effect at the next cancellation point.
- The runtime should ensure that cleanup handling logic and lock release rules are executed correctly.

---

### 3.4 `pthread_mutex_lock`

**Function Prototype**
```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
```

**Function Description**
Locks a mutex. If the lock is already held by another thread, the current thread blocks and waits.

**Input Parameter Definitions**
- `mutex`: Pointer to the mutex object to be locked.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Lock acquired successfully.
- `EINVAL`: Mutex is invalid.
- `EDEADLK`: Deadlock detected (for error-checking mutexes).

**Implementation Notes**
- Should map to Zephyr's `k_mutex` or similar synchronization primitives.
- Must support priority inheritance if configured.

---

### 3.5 `pthread_mutex_unlock`

**Function Prototype**
```c
#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

**Function Description**
Releases a mutex currently held by the calling thread, allowing other waiting threads to acquire the lock.

**Input Parameter Definitions**
- `mutex`: Pointer to the mutex object to be unlocked.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Unlock successful.
- `EINVAL`: Mutex object is invalid.
- `EPERM`: Current thread does not own the lock.

**Implementation Notes**
- If waiting threads exist, an appropriate waiter should be awakened according to the scheduling policy.
- For recursive locks, the recursion count must be decremented first before deciding whether to actually release the lock.

---

### 3.6 `pthread_cond_wait`

**Function Prototype**
```c
#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *cond,
                      pthread_mutex_t *mutex);
```

**Function Description**
Atomically releases the mutex and places the current thread on the condition variable's wait queue; when awakened, reacquires the mutex before returning. This interface is used for event condition synchronization.

**Input Parameter Definitions**
- `cond`: Pointer to the condition variable object.
- `mutex`: Mutex used in conjunction with the condition variable, must be held by the current thread when called.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Wait successful and mutex reacquired.
- `EINVAL`: Invalid parameters or incorrect object state.
- `EPERM`: Mutex not held when called.

**Implementation Notes**
- Must ensure that "releasing the lock and entering wait" is an atomic operation to avoid lost wakeups.

---

### 3.7 `pthread_cond_signal`

**Function Prototype**
```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
```

**Function Description**
Wakes up at least one thread waiting on the condition variable, used to notify that a shared condition has changed.

**Input Parameter Definitions**
- `cond`: Condition variable object to signal.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Operation successful.
- `EINVAL`: Condition variable is invalid.

**Implementation Notes**
- If no threads are waiting, the call succeeds but produces no visible wakeup effect.

---

## 4. Process Management Interfaces

### 4.1 `fork`

**Function Prototype**
```c
#include <unistd.h>

pid_t fork(void);
```

**Function Description**
Duplicates the current process, creating a new child process. Parent and child processes continue execution from the same point but see different return values. In this project, this interface typically provides lightweight "process-like duplication semantics."

**Input Parameter Definitions**
- None.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Greater than `0`: Returns child process PID in the parent process.
- `0`: Returns in the child process.
- `-1`: Creation failed, and `errno` is set.

**Error Code Definitions**
- `ENOMEM`: Insufficient memory.
- `EAGAIN`: Process count or system resources reached limit.

**Implementation Notes**
- In no-MMU scenarios, a simplified duplication model is typically used rather than full copy-on-write.

---

### 4.2 `execve`

**Function Prototype**
```c
#include <unistd.h>

int execve(const char *filename,
           char *const argv[],
           char *const envp[]);
```

**Function Description**
Replaces the current process image with the specified executable file. Upon success, the original program no longer continues execution; instead, execution begins at the new program's entry point.

**Input Parameter Definitions**
- `filename`: Path to the target executable file.
- `argv`: Argument vector for the new program, terminated with `NULL`.
- `envp`: Environment variable vector for the new program, terminated with `NULL`.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Does not return on success.
- `-1`: Execution failed, and `errno` is set.

**Error Code Definitions**
- `ENOENT`: Target file does not exist.
- `EACCES`: No execute permission.
- `ENOMEM`: Insufficient memory during loading.
- `EINVAL`: Invalid executable format.

**Implementation Notes**
- In this project, can be implemented using ELF loading and Zephyr LLEXT mechanisms.

---

### 4.3 `exit`

**Function Prototype**
```c
#include <stdlib.h>

void exit(int status);
```

**Function Description**
Terminates the current process, performs process-level resource cleanup, and provides the exit status to the parent process or wait interfaces.

**Input Parameter Definitions**
- `status`: Process exit code.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- This function does not return.

**Implementation Notes**
- Should close all open file descriptors.
- Should release dynamic resources and notify the parent process that `wait`/`waitpid` can be performed for cleanup.

---

## 5. Scheduling and Priority Interfaces

### 5.1 `sched_yield`

**Function Prototype**
```c
#include <sched.h>

int sched_yield(void);
```

**Function Description**
The current thread voluntarily yields the processor, giving other ready threads an opportunity to run. This interface embodies the semantics of "voluntarily relinquishing this CPU time slice."

**Input Parameter Definitions**
- None.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: CPU yielded successfully.
- `-1`: Failed, and `errno` is set.

**Implementation Notes**
- Should actually affect the scheduling queue, not just return success.

---

### 5.2 `sched_setparam`

**Function Prototype**
```c
#include <sched.h>

int sched_setparam(pid_t pid, const struct sched_param *param);
```

**Function Description**
Sets the scheduling parameters for the specified process or thread context, primarily the scheduling priority.

**Input Parameter Definitions**
- `pid`: Target process or thread identifier, `0` indicates the current execution context.
- `param`: Pointer to scheduling parameter structure, typically containing `sched_priority`.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Setting successful.
- `-1`: Setting failed, and `errno` is set.

**Error Code Definitions**
- `ESRCH`: Target does not exist.
- `EINVAL`: Invalid parameters.
- `EPERM`: No permission to modify.

**Implementation Notes**
- Needs to map Linux/POSIX priority to Zephyr priority space.

---

### 5.3 `sched_getparam`

**Function Prototype**
```c
#include <sched.h>

int sched_getparam(pid_t pid, struct sched_param *param);
```

**Function Description**
Retrieves the current scheduling parameters for the specified process or thread.

**Input Parameter Definitions**
- `pid`: Target process or thread identifier, `0` indicates the current execution context.
- `param`: Pointer to output structure provided by the caller.

**Output Parameter Definitions**
- `param`: Upon success, the currently effective scheduling parameters are written here.

**Return Value Definitions**
- `0`: Retrieval successful.
- `-1`: Failed, and `errno` is set.

**Error Code Definitions**
- `ESRCH`: Target does not exist.
- `EINVAL`: Invalid parameters.

---

## 6. Signal and Timer Interfaces

### 6.1 `signal`

**Function Prototype**
```c
#include <signal.h>

void (*signal(int sig, void (*func)(int)))(int);
```

**Function Description**
Installs a new handler function for the specified signal, or sets it to default handling or ignore handling.

**Input Parameter Definitions**
- `sig`: Signal number.
- `func`: Signal handler function, or `SIG_DFL`, `SIG_IGN`.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- On success, returns the previous signal handling method.
- On failure, returns `SIG_ERR`.

---

### 6.2 `kill`

**Function Prototype**
```c
#include <signal.h>

int kill(pid_t pid, int sig);
```

**Function Description**
Sends a signal to the specified process.

**Input Parameter Definitions**
- `pid`: Target process ID.
- `sig`: Signal number to send.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Signal sent successfully.
- `-1`: Send failed.

**Error Code Definitions**
- `ESRCH`: Target process does not exist.
- `EINVAL`: Invalid signal number.
- `EPERM`: Insufficient permission.

---

### 6.3 `alarm`

**Function Prototype**
```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

**Function Description**
Sets a one-shot timer that sends `SIGALRM` to the current process after the specified number of seconds.

**Input Parameter Definitions**
- `seconds`: Number of seconds until trigger; if `0`, cancels any existing alarm.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Returns the number of seconds remaining from the previous alarm; if there was no previous alarm, returns `0`.

---

### 6.4 `timer_create`

**Function Prototype**
```c
#include <time.h>

int timer_create(clockid_t clockid,
                 struct sigevent *sevp,
                 timer_t *timerid);
```

**Function Description**
Creates a POSIX timer object for subsequent configuration with `timer_settime`.

**Input Parameter Definitions**
- `clockid`: Clock source on which the timer depends.
- `sevp`: Timeout notification configuration; can be `NULL`.
- `timerid`: Output location for the timer ID provided by the caller.

**Output Parameter Definitions**
- `timerid`: Upon success, the newly created timer identifier is written here.

**Return Value Definitions**
- `0`: Creation successful.
- `-1`: Creation failed.

**Error Code Definitions**
- `EAGAIN`: Resources exhausted.
- `EINVAL`: Invalid clock source.
- `ENOMEM`: Insufficient memory.

---

### 6.5 `sleep`

**Function Prototype**
```c
#include <unistd.h>

unsigned int sleep(unsigned int seconds);
```

**Function Description**
Causes the current thread to sleep for the specified number of seconds, or returns early if interrupted by a signal.

**Input Parameter Definitions**
- `seconds`: Requested sleep duration in seconds.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Sleep completed fully.
- Greater than `0`: Remaining unslept seconds after interruption.

---

### 6.6 `usleep`

**Function Prototype**
```c
#include <unistd.h>

int usleep(useconds_t usec);
```

**Function Description**
Causes the current thread to sleep for the specified number of microseconds.

**Input Parameter Definitions**
- `usec`: Sleep duration in microseconds.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Success.
- `-1`: Failure.

---

### 6.7 `nanosleep`

**Function Prototype**
```c
#include <time.h>

int nanosleep(const struct timespec *req,
              struct timespec *rem);
```

**Function Description**
Requests thread sleep with nanosecond precision, providing a high-precision relative timing sleep interface.

**Input Parameter Definitions**
- `req`: Requested sleep duration.
- `rem`: If interrupted, used to return remaining sleep duration; can be `NULL`.

**Output Parameter Definitions**
- `rem`: Upon interruption, the remaining sleep time is written here.

**Return Value Definitions**
- `0`: Sleep completed successfully.
- `-1`: Interrupted or failed.

**Error Code Definitions**
- `EINTR`: Interrupted by signal.
- `EINVAL`: Invalid time value.

---

## 7. Memory Management Interfaces

### 7.1 `malloc`

**Function Prototype**
```c
#include <stdlib.h>

void *malloc(size_t size);
```

**Function Description**
Allocates a contiguous block of memory of the specified size from the heap.

**Input Parameter Definitions**
- `size`: Number of bytes to allocate.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Non-`NULL`: Pointer to the allocated memory block.
- `NULL`: Allocation failed.

**Implementation Notes**
- Should map to Zephyr's heap management mechanism.
- Allocated memory is not initialized.

---

### 7.2 `free`

**Function Prototype**
```c
#include <stdlib.h>

void free(void *ptr);
```

**Function Description**
Releases a previously allocated memory block back to the heap.

**Input Parameter Definitions**
- `ptr`: Pointer to the memory block to be freed; can be `NULL`.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- No return value.

---

## 8. Device Management Interfaces

### 8.1 `open`

**Function Prototype**
```c
#include <fcntl.h>

int open(const char *pathname, int flags, mode_t mode);
```

**Function Description**
Opens a device or file-style object and returns a file descriptor for subsequent use with `read`, `write`, `ioctl`, and other interfaces.

**Input Parameter Definitions**
- `pathname`: Target path, such as `/dev/uart0`.
- `flags`: Open mode and behavior flags.
- `mode`: Permission bits for creation scenarios; typically ignored for pure device paths.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Non-negative integer: Open successful, returns file descriptor.
- `-1`: Open failed.

**Error Code Definitions**
- `ENOENT`: Path does not exist.
- `EACCES`: Insufficient permission.
- `EMFILE`: File descriptor limit reached.

---

### 8.2 `close`

**Function Prototype**
```c
#include <unistd.h>

int close(int fd);
```

**Function Description**
Closes a file descriptor and releases associated resources.

**Input Parameter Definitions**
- `fd`: File descriptor to be closed.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- `0`: Close successful.
- `-1`: Close failed.

**Error Code Definitions**
- `EBADF`: Invalid file descriptor.
- `EIO`: I/O error occurred during device closure.

---

### 8.3 `read`

**Function Prototype**
```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

**Function Description**
Reads data from the object corresponding to the file descriptor into a user-provided buffer.

**Input Parameter Definitions**
- `fd`: Source file descriptor.
- `buf`: Destination buffer.
- `count`: Maximum number of bytes to read.

**Output Parameter Definitions**
- `buf`: Upon success, the read data is written here.

**Return Value Definitions**
- Positive number: Actual number of bytes read.
- `0`: Reached EOF.
- `-1`: Read failed.

**Error Code Definitions**
- `EBADF`: Invalid descriptor or not readable.
- `EAGAIN`: Non-blocking mode and no data currently available.
- `EIO`: I/O error.

---

### 8.4 `write`

**Function Prototype**
```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
```

**Function Description**
Writes data to the object corresponding to the file descriptor.

**Input Parameter Definitions**
- `fd`: Target file descriptor.
- `buf`: Source data buffer.
- `count`: Maximum number of bytes to write.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Non-negative number: Actual number of bytes written.
- `-1`: Write failed.

**Error Code Definitions**
- `EBADF`: Invalid descriptor or not writable.
- `EAGAIN`: Non-blocking mode and currently not writable.
- `EIO`: I/O error.

---

### 8.5 `ioctl`

**Function Prototype**
```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```

**Function Description**
Executes device-specific control operations, used to accomplish device configuration or status queries that cannot be expressed through simple byte stream read/write.

**Input Parameter Definitions**
- `fd`: Target file descriptor.
- `request`: Control command encoding.
- `...`: Variable parameters depending on the command, typically a structure pointer or scalar value.

**Output Parameter Definitions**
- Variable parameters may also serve as output buffers for some commands.

**Return Value Definitions**
- `0`: Operation successful.
- `-1`: Operation failed.

**Error Code Definitions**
- `EBADF`: Invalid descriptor.
- `ENOTTY`: Device does not support this control command.
- `EINVAL`: Invalid command parameters.

---

## 9. IPC and Environment Interfaces

### 9.1 `pipe`

**Function Prototype**
```c
#include <unistd.h>

int pipe(int pipefd[2]);
```

**Function Description**
Creates a unidirectional pipe for data transfer between two execution contexts.

**Input Parameter Definitions**
- `pipefd`: Integer array of length 2.

**Output Parameter Definitions**
- `pipefd[0]`: Read end file descriptor.
- `pipefd[1]`: Write end file descriptor.

**Return Value Definitions**
- `0`: Success.
- `-1`: Failure.

---

### 9.2 `getenv`

**Function Prototype**
```c
#include <stdlib.h>

char *getenv(const char *name);
```

**Function Description**
Retrieves the value of an environment variable by name.

**Input Parameter Definitions**
- `name`: Environment variable name.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Non-`NULL`: Returns the environment variable value string.
- `NULL`: Variable does not exist.

---

### 9.3 `getuid`

**Function Prototype**
```c
#include <unistd.h>

uid_t getuid(void);
```

**Function Description**
Retrieves the real user ID of the current process. In MCU scenarios, this is typically a compatibility placeholder interface.

**Input Parameter Definitions**
- None.

**Output Parameter Definitions**
- None.

**Return Value Definitions**
- Returns the current user ID.

---

## 10. Time and Process Control Interfaces

### 10.1 `clock_gettime`

**Function Prototype**
```c
#include <time.h>

int clock_gettime(clockid_t clockid, struct timespec *tp);
```

**Function Description**
Reads the current time value from the specified clock source.

**Input Parameter Definitions**
- `clockid`: Clock source identifier.
- `tp`: Time output structure.

**Output Parameter Definitions**
- `tp`: Upon success, the current time is written here.

**Return Value Definitions**
- `0`: Success.
- `-1`: Failure.

---

### 10.2 `wait`

**Function Prototype**
```c
#include <sys/wait.h>

pid_t wait(int *status);
```

**Function Description**
Waits for any child process to enter a reclaimable state, typically an exit state.

**Input Parameter Definitions**
- `status`: Used to receive child process exit status; can be `NULL`.

**Output Parameter Definitions**
- `status`: If non-null, the child process status encoding is written here.

**Return Value Definitions**
- Positive number: PID of the reclaimed child process.
- `-1`: Failure.

---

### 10.3 `waitpid`

**Function Prototype**
```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

**Function Description**
Waits for a specified child process or group of child processes to change state, providing a more general process waiting interface.

**Input Parameter Definitions**
- `pid`: Target selection rule for waiting.
- `status`: Status output buffer.
- `options`: Wait options, such as `WNOHANG`.

**Output Parameter Definitions**
- `status`: Upon successful reclamation, the child process status is written here.

**Return Value Definitions**
- Positive number: Target child process PID.
- `0`: Non-blocking mode and no reclaimable child process currently.
- `-1`: Failure.

---

### 10.4 `posix_spawn`

**Function Prototype**
```c
#include <spawn.h>

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);
```

**Function Description**
Creates a new process and loads the target program in a single operation, suitable for no-MMU systems as an alternative to the `fork + exec` combination.

**Input Parameter Definitions**
- `pid`: Output location for the new process PID.
- `path`: Path to the program to execute.
- `file_actions`: List of file actions to execute before child process startup; can be `NULL`.
- `attrp`: Spawn attributes; can be `NULL`.
- `argv`: Argument vector.
- `envp`: Environment variable vector.

**Output Parameter Definitions**
- `pid`: Upon success, the child process PID is written here.

**Return Value Definitions**
- `0`: Success.
- Non-zero error code: Failure.

---

## 11. I/O Multiplexing Interfaces

### 11.1 `select`

**Function Prototype**
```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
```

**Function Description**
Simultaneously monitors multiple file descriptors for read, write, and exception state changes.

**Input Parameter Definitions**
- `nfds`: Maximum file descriptor value to check plus one.
- `readfds`: Set of descriptors to monitor for readability; can be `NULL`.
- `writefds`: Set of descriptors to monitor for writability; can be `NULL`.
- `exceptfds`: Set of descriptors to monitor for exceptions; can be `NULL`.
- `timeout`: Wait timeout duration; can be `NULL`.

**Output Parameter Definitions**
- `readfds`: Upon return, only readable descriptors remain.
- `writefds`: Upon return, only writable descriptors remain.
- `exceptfds`: Upon return, only exception descriptors remain.

**Return Value Definitions**
- Positive number: Number of ready descriptors.
- `0`: Timeout.
- `-1`: Failure.

---

### 11.2 `poll`

**Function Prototype**
```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**Function Description**
Monitors event states of multiple file descriptors, more suitable for sparse descriptor sets compared to `select`.

**Input Parameter Definitions**
- `fds`: Array of descriptors to monitor.
- `nfds`: Number of array entries.
- `timeout`: Timeout duration in milliseconds.

**Output Parameter Definitions**
- `fds[i].revents`: Upon return, the actual events that occurred are written here.

**Return Value Definitions**
- Positive number: Number of descriptors with events.
- `0`: Timeout.
- `-1`: Failure.

---

### 11.3 `epoll`

**Function Prototype**
```c
#include <sys/epoll.h>

int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);
```

**Function Description**
Provides an event-driven I/O multiplexing mechanism that separates "monitoring object registration" from "waiting for events to occur," suitable for frequently and repeatedly monitoring a dynamic set of file descriptors.

**Input Parameter Definitions**
- `flags`: `epoll_create1` creation flags.
- `epfd`: epoll instance descriptor.
- `op`: Control operation type, such as add, modify, delete.
- `fd`: Target descriptor being managed.
- `event`: Event configuration structure.
- `events`: Event result output array.
- `maxevents`: Output array capacity.
- `timeout`: Wait timeout duration.

**Output Parameter Definitions**
- `events`: Upon `epoll_wait` return, the ready event list is written here.

**Return Value Definitions**
- `epoll_create1`: On success, returns epoll descriptor; on failure, returns `-1`.
- `epoll_ctl`: On success, returns `0`; on failure, returns `-1`.
- `epoll_wait`: Returns number of ready events; on timeout, returns `0`; on failure, returns `-1`.

---

## 12. Common Error Code Descriptions

| Error Code | Meaning |
|--------|------|
| `EAGAIN` | Resource temporarily unavailable |
| `EBADF` | Invalid file descriptor |
| `EDEADLK` | Deadlock detected |
| `EINTR` | System call interrupted |
| `EINVAL` | Invalid parameter |
| `EIO` | I/O error |
| `ENOMEM` | Insufficient memory |
| `ENOENT` | Object does not exist |
| `EPERM` | Insufficient permission |
| `ESRCH` | Target process or thread does not exist |

## 13. Document Notes

1. This document emphasizes interface layer definitions, prioritizing "how to call, what to pass, what is returned" for each interface.
2. For more formal preconditions, postconditions, invariants, and concurrency constraints, refer to: `ai/interface-specifications.md`.
3. For underlying implementation mapping and scheduler design details, refer to: `docs/zephyr-linux-interface-definition.md`.
4. For testing and validation materials, refer to: `docs/zepLinux-interface-and-validation.md` and `tests/demo/`.

