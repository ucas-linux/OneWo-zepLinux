# Thread-as-Process实现指导

## 总体架构

```
posix_thread_process/
├── pcb.c                  # 进程控制块管理
├── spawn.c                # posix_spawn实现
├── wait.c                 # wait/waitpid实现
├── mpu_isolation.c        # MPU隔离
├── resource_tracking.c    # 资源跟踪
├── process_internal.h     # 内部头文件
└── process_debug.c        # 调试支持
```

## 初始化

```c
static pcb_t g_process_table[CONFIG_MAX_PROCESSES];
static struct k_mutex g_process_table_lock;
static atomic_t g_next_pid = ATOMIC_INIT(2);
static pcb_t *g_init_process = NULL;

int thread_process_init(void)
{
    k_mutex_init(&g_process_table_lock);

    // 初始化进程表
    for (int i = 0; i < CONFIG_MAX_PROCESSES; i++) {
        g_process_table[i].state = PROC_TERMINATED;
        g_process_table[i].pid = PID_INVALID;
    }

    // 创建init进程
    g_init_process = alloc_process();
    g_init_process->pid = 1;
    g_init_process->ppid = 0;
    g_init_process->state = PROC_RUNNING;
    g_init_process->main_thread = k_current_get();

    printk("Thread-as-process model initialized\n");
    return 0;
}

SYS_INIT(thread_process_init, POST_KERNEL,
         CONFIG_THREAD_PROCESS_INIT_PRIORITY);
```

## 完整spawn流程

```c
int posix_spawn_complete(pid_t *pid, const char *path,
                         const posix_spawn_file_actions_t *file_actions,
                         const posix_spawnattr_t *attrp,
                         char *const argv[], char *const envp[])
{
    elf_handle_t elf;
    pcb_t *proc;
    int ret;

    // 1. 加载ELF
    ret = elf_parse(path, &elf);
    if (ret < 0) return ret;

    ret = elf_load(&elf, NULL);
    if (ret < 0) {
        elf_close(&elf);
        return ret;
    }

    // 2. 分配进程
    proc = alloc_process();
    if (!proc) {
        elf_close(&elf);
        return EAGAIN;
    }

    // 3. 初始化进程
    proc->pid = generate_pid();
    proc->ppid = getpid();
    proc->pgid = proc->pid;
    proc->state = PROC_RUNNING;

    // 4. 设置内存
    proc->code_start = elf.memory.code_base;
    proc->code_size = elf.memory.code_size;
    proc->data_start = elf.memory.data_base;
    proc->data_size = elf.memory.data_size;
    proc->heap_start = elf.memory.heap_base;
    proc->heap_size = elf.memory.heap_size;
    proc->stack_start = elf.memory.stack_base;
    proc->stack_size = elf.memory.stack_size;

    // 5. 配置MPU
    ret = configure_process_mpu(proc);
    if (ret < 0) goto cleanup;

    // 6. 应用spawn属性
    if (attrp) {
        apply_spawn_attributes(proc, attrp);
    }

    // 7. 应用文件操作
    if (file_actions) {
        apply_file_actions(proc, file_actions);
    }

    // 8. 准备入口
    ret = prepare_process_entry(proc, &elf, argv, envp);
    if (ret < 0) goto cleanup;

    // 9. 创建主线程
    ret = create_process_main_thread(proc, elf.entry);
    if (ret < 0) goto cleanup;

    // 10. 添加到父进程
    add_child_process(get_current_process(), proc);

    *pid = proc->pid;
    return 0;

cleanup:
    free_process(proc);
    elf_close(&elf);
    return ret;
}
```

## 配置选项

```kconfig
menuconfig THREAD_PROCESS_MODEL
    bool "Thread-as-process model"
    depends on !MMU
    default n

if THREAD_PROCESS_MODEL

config MAX_PROCESSES
    int "Maximum processes"
    default 16
    range 2 256

config PROCESS_DEFAULT_HEAP
    int "Default heap size"
    default 4096

config PROCESS_DEFAULT_STACK
    int "Default stack size"
    default 2048

config THREAD_PROCESS_INIT_PRIORITY
    int "Initialization priority"
    default 55

endif
```

## 测试框架

```c
#ifdef CONFIG_THREAD_PROCESS_TESTS

void test_spawn_basic(void)
{
    pid_t pid;
    char *argv[] = {"/bin/test", NULL};
    char *envp[] = {NULL};

    int ret = posix_spawn(&pid, "/bin/test", NULL, NULL, argv, envp);
    zassert_equal(ret, 0, "spawn failed");
    zassert_true(pid > 0, "invalid pid");

    int status;
    waitpid(pid, &status, 0);
    zassert_true(WIFEXITED(status), "not exited");
}

ZTEST_SUITE(thread_process_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(thread_process_tests, test_spawn) { test_spawn_basic(); }

#endif
```

## 最佳实践

1. **资源管理**: 确保所有资源在进程退出时清理
2. **MPU配置**: 正确设置内存区域权限
3. **错误处理**: 处理spawn失败情况
4. **僵尸进程**: 父进程必须调用wait回收子进程
5. **测试**: 为每个功能编写测试用例
