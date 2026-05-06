# LMZ Migration Guide

## Overview

This guide helps migrate Linux applications to LMZ/Zephyr RTOS.

## Common Migration Patterns

### 1. Static vs Dynamic Allocation

**Linux**: Compile-time stack allocation common
**Zephyr/LMZ**: Runtime stack allocation required

**Migration**:
```c
/* Linux style (static) */
K_THREAD_STACK_DEFINE(stack1, 2048);

/* LMZ style (dynamic) */
pthread_t thread;
pthread_create(&thread, NULL, func, NULL);
```

### 2. Memory Model Differences

**Linux**: Virtual memory, separate address spaces
**Zephyr/LMZ**: Flat memory, shared address space

**Migration**:
- Use explicit synchronization instead of process isolation
- Be aware of shared global state
- Use resource tracking for cleanup

### 3. Priority Scale

**Linux**: Higher number = higher priority (1-99)
**Zephyr**: Lower number = higher priority (0-15)

**Migration**:
```c
/* Linux priority */
int linux_prio = 50;  /* Medium-high priority */

/* Zephyr priority (LMZ handles conversion) */
int zephyr_prio = lmz_prio_linux_to_zephyr(linux_prio);
```

### 4. Signal Handling

**Linux**: Full signal infrastructure
**Zephyr/LMZ**: Simulated signals (limited)

**Migration**:
- Prefer condition variables over signals
- Use semaphores for notification
- Use timers for time-based events

### 5. File Operations

**Linux**: VFS, unified file/directory operations
**Zephyr/LMZ**: Device-specific operations only

**Migration**:
- Use device paths ("/dev/uart0", "/dev/i2c0")
- Expect limited ioctl support
- Check device-specific APIs

## API Compatibility

### Fully Supported

These interfaces work identically to Linux:
- `pthread_create`, `pthread_join`, `pthread_cancel`
- `pthread_mutex_lock`, `pthread_mutex_unlock`
- `pthread_cond_wait`, `pthread_cond_signal`
- `malloc`, `realloc`, `free`

### Partially Supported

These interfaces have limitations:
- `fork`: No copy-on-write, full stack copy
- `exec`: Limited ELF support (LLEXT)
- `signal`: Simulated delivery, limited types
- `ioctl`: Device-specific, limited commands

### Not Supported

These interfaces are not available:
- Full POSIX file system (no mkdir, rmdir, etc.)
- Unix domain sockets
- Shared memory (shm_*)
- Full signal set (limited to basic signals)

## Testing Strategies

### 1. Unit Testing

Test each interface independently:
```c
void test_pthread_create(void) {
    pthread_t thread;
    zassert_equal(0, pthread_create(&thread, NULL, dummy, NULL));
    pthread_join(thread, NULL);
}
```

### 2. Integration Testing

Test interaction between interfaces:
```c
void test_thread_mutex_integration(void) {
    pthread_t thread1, thread2;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, worker, &mutex);
    pthread_create(&thread2, NULL, worker, &mutex);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}
```

### 3. Stress Testing

Test resource limits:
```c
void test_max_threads(void) {
    pthread_t threads[LMZ_MAX_THREADS];
    int created = 0;

    for (int i = 0; i < LMZ_MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, dummy, NULL) == 0) {
            created++;
        }
    }

    zassert_true(created > 0, "should create at least one thread");
}
```

## Common Issues and Solutions

### Issue: Out of Memory

**Symptom**: malloc returns NULL frequently

**Solutions**:
```ini
# prj.conf
CONFIG_HEAP_MEM_POOL_SIZE=131072
CONFIG_MALLOC_ARENA_SIZE=65536
```

### Issue: Stack Overflow

**Symptom**: Hard fault, unexpected behavior

**Solutions**:
```ini
# prj.conf
CONFIG_MAIN_STACK_SIZE=4096
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=2048
CONFIG_THREAD_STACK_MEM_MPU=y
```

### Issue: Priority Inversion

**Symptom**: High-priority thread blocked

**Solutions**:
```ini
# prj.conf
CONFIG_PTHREAD_MUTEX_PRIORITY_INHERIT=y
```

### Issue: Signal Loss

**Symptom**: Signals not delivered

**Solutions**:
- Use condition variables instead
- Check signal handler registration
- Verify signal is not blocked

## Performance Considerations

### 1. Dynamic Allocation Overhead

LMZ uses dynamic allocation more than Linux:
- **Impact**: Slightly slower thread creation
- **Mitigation**: Reuse threads where possible
- **Mitigation**: Use thread pools for frequent operations

### 2. MPU Context Switch Overhead

MPU region switches add overhead:
- **Impact**: Slower context switch
- **Mitigation**: Minimize process count
- **Mitigation**: Use threads instead of processes where possible

### 3. Lock Contention

Global locks can become bottlenecks:
- **Impact**: Reduced parallelism
- **Mitigation**: Use fine-grained locking
- **Mitigation**: Lock-free data structures where appropriate

## Best Practices

### 1. Initialize LMZ First

```c
int main() {
    lmz_init();  /* Must be first */

    /* Your code here */

    lmz_cleanup();  /* Clean shutdown */
    return 0;
}
```

### 2. Check Return Values

Always check error returns:
```c
int rc = pthread_create(&thread, NULL, func, NULL);
if (rc != 0) {
    printf("Failed to create thread: %s\n", strerror(rc));
    return -1;
}
```

### 3. Clean Up Resources

Ensure all resources are released:
```c
void cleanup() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    close(fd);
}
```

### 4. Use Appropriate Synchronization

Choose the right primitive:
- **Mutex**: Mutual exclusion
- **Condition variable**: Wait for condition
- **Semaphore**: Resource counting
- **Event flag**: Simple signaling

## Resources

- [Getting Started Guide](../human/getting-started.md)
- [API Reference](../human/api-reference.md)
- [Testing Guide](../human/testing.md)
- [AI Specifications](../ai/interface-specifications.md)
