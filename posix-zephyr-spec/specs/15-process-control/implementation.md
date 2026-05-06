# 实现指导 - 进程控制

## 核心实现

```c
pid_t getpid(void)
{
    pcb_t *proc = get_current_process();
    return proc ? proc->pid : 1;
}

pid_t getppid(void)
{
    pcb_t *proc = get_current_process();
    return proc ? proc->ppid : 0;
}

pid_t wait(int *status)
{
    return waitpid(-1, status, 0);
}
```

## 配置

```kconfig
config POSIX_PROCESS_CONTROL
    bool "Process control APIs"
    depends on POSIX_PROCESS
    default y
```
