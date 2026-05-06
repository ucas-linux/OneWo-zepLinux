# ELF加载器实现指导

## 总体架构

```
posix_elf/
├── elf_loader.c          # 主加载器
├── elf_parser.c          # ELF解析
├── elf_reloc.c           # 重定位处理
├── elf_xip.c             # XIP支持
├── elf_internal.h        # 内部头文件
└── elf_debug.c           # 调试支持
```

## 初始化

```c
int elf_loader_init(void)
{
    // 初始化全局状态
    k_mutex_init(&g_elf_loader_lock);

    // 启用Flash优化
#ifdef CONFIG_ELF_LOADER_XIP
    elf_enable_flash_prefetch();
    elf_enable_icache();
#endif

    ELF_DEBUG("ELF loader initialized\n");

    return 0;
}

SYS_INIT(elf_loader_init, POST_KERNEL, CONFIG_ELF_LOADER_INIT_PRIORITY);
```

## 完整加载流程

```c
int elf_load_and_run(const char *path, char *const argv[], char *const envp[])
{
    elf_handle_t handle;
    int ret;

    memset(&handle, 0, sizeof(handle));

    // 1. 解析ELF
    ret = elf_parse(path, &handle);
    if (ret < 0) {
        ELF_ERROR("Failed to parse ELF: %d\n", ret);
        return ret;
    }

    // 2. 加载到内存
    ret = elf_load(&handle, NULL);
    if (ret < 0) {
        ELF_ERROR("Failed to load ELF: %d\n", ret);
        goto cleanup;
    }

    // 3. 创建进程上下文
    pid_t pid = process_spawn_from_elf(&handle, argv, envp);
    if (pid < 0) {
        ELF_ERROR("Failed to spawn process: %d\n", pid);
        ret = pid;
        goto cleanup;
    }

    ELF_DEBUG("Process spawned: pid=%d entry=0x%08x\n", pid, handle.entry);

    return pid;

cleanup:
    elf_close(&handle);
    return ret;
}
```

## 与进程模块集成

```c
// 从ELF创建进程
pid_t process_spawn_from_elf(elf_handle_t *handle,
                              char *const argv[],
                              char *const envp[])
{
    pcb_t *proc;
    int ret;

    // 分配进程控制块
    proc = alloc_process();
    if (!proc) {
        return -EAGAIN;
    }

    // 设置进程信息
    proc->pid = generate_pid();
    proc->ppid = getpid();
    proc->state = PROC_RUNNING;

    // 设置内存布局
    proc->code_start = handle->memory.code_base;
    proc->code_size = handle->memory.code_size;
    proc->data_start = handle->memory.data_base;
    proc->data_size = handle->memory.data_size;
    proc->heap_start = handle->memory.heap_base;
    proc->heap_size = handle->memory.heap_size;
    proc->stack_start = handle->memory.stack_base;
    proc->stack_size = handle->memory.stack_size;

    // 配置MPU
    ret = configure_process_mpu(proc);
    if (ret < 0) {
        free_process(proc);
        return ret;
    }

    // 准备入口环境
    ret = elf_prepare_entry(handle, proc, argv, envp);
    if (ret < 0) {
        free_process(proc);
        return ret;
    }

    // 创建主线程
    ret = process_create_main_thread(proc, handle->entry);
    if (ret < 0) {
        free_process(proc);
        return ret;
    }

    return proc->pid;
}
```

## 入口环境准备

```c
static int elf_prepare_entry(elf_handle_t *handle, pcb_t *proc,
                              char *const argv[], char *const envp[])
{
    // 计算argc
    int argc = 0;
    if (argv) {
        while (argv[argc]) argc++;
    }

    // 计算envp数量
    int envc = 0;
    if (envp) {
        while (envp[envc]) envc++;
    }

    // 在栈上构建参数
    // 栈布局（从高地址到低地址）：
    // - envp字符串
    // - argv字符串
    // - NULL
    // - envp指针数组
    // - NULL
    // - argv指针数组
    // - argc

    uint8_t *stack_top = (uint8_t *)proc->stack_start + proc->stack_size;
    uint8_t *sp = stack_top;

    // 复制envp字符串
    char **envp_ptrs = k_malloc((envc + 1) * sizeof(char *));
    for (int i = envc - 1; i >= 0; i--) {
        size_t len = strlen(envp[i]) + 1;
        sp -= len;
        memcpy(sp, envp[i], len);
        envp_ptrs[i] = (char *)sp;
    }
    envp_ptrs[envc] = NULL;

    // 复制argv字符串
    char **argv_ptrs = k_malloc((argc + 1) * sizeof(char *));
    for (int i = argc - 1; i >= 0; i--) {
        size_t len = strlen(argv[i]) + 1;
        sp -= len;
        memcpy(sp, argv[i], len);
        argv_ptrs[i] = (char *)sp;
    }
    argv_ptrs[argc] = NULL;

    // 对齐到8字节
    sp = (uint8_t *)((uintptr_t)sp & ~7);

    // 写入envp指针数组
    sp -= (envc + 1) * sizeof(char *);
    memcpy(sp, envp_ptrs, (envc + 1) * sizeof(char *));
    char **envp_on_stack = (char **)sp;

    // 写入argv指针数组
    sp -= (argc + 1) * sizeof(char *);
    memcpy(sp, argv_ptrs, (argc + 1) * sizeof(char *));
    char **argv_on_stack = (char **)sp;

    // 写入argc
    sp -= sizeof(int);
    *(int *)sp = argc;

    // 保存栈指针
    proc->entry_sp = (uint32_t)sp;
    proc->entry_argc = argc;
    proc->entry_argv = argv_on_stack;
    proc->entry_envp = envp_on_stack;

    k_free(argv_ptrs);
    k_free(envp_ptrs);

    return 0;
}
```

## 配置选项

```kconfig
menuconfig ELF_LOADER
    bool "ELF binary loader"
    depends on !MMU
    default n
    help
      Enable loading of statically-linked ELF binaries.

if ELF_LOADER

config ELF_LOADER_XIP
    bool "Execute-in-place from flash"
    default y
    help
      Allow executing code directly from flash.

config ELF_MAX_SEGMENTS
    int "Maximum ELF segments"
    default 8
    range 4 32

config ELF_LOADER_DEBUG
    bool "Enable ELF loader debugging"
    default n

config ELF_LOADER_INIT_PRIORITY
    int "ELF loader initialization priority"
    default 60

config ELF_SUPPORT_RELOCATION
    bool "Support ELF relocations"
    default y
    help
      Enable relocation processing for non-PIC binaries.

endif # ELF_LOADER
```

## 调试宏

```c
#ifdef CONFIG_ELF_LOADER_DEBUG
#define ELF_DEBUG(fmt, ...) printk("[ELF] " fmt, ##__VA_ARGS__)
#else
#define ELF_DEBUG(fmt, ...)
#endif

#define ELF_ERROR(fmt, ...) printk("[ELF ERROR] " fmt, ##__VA_ARGS__)
```

## 测试框架

```c
#ifdef CONFIG_ELF_LOADER_TESTS

void test_elf_parse_basic(void)
{
    elf_handle_t handle;
    int ret = elf_parse("/flash/test.elf", &handle);
    zassert_equal(ret, 0, "Failed to parse ELF");
    zassert_not_equal(handle.entry, 0, "Invalid entry point");
    elf_close(&handle);
}

void test_elf_load_xip(void)
{
    elf_handle_t handle;
    int ret;

    ret = elf_parse("/flash/test.elf", &handle);
    zassert_equal(ret, 0, "Parse failed");

    ret = elf_load(&handle, NULL);
    zassert_equal(ret, 0, "Load failed");

    // 验证代码段在Flash中
    zassert_true(is_flash_address(handle.memory.code_base),
                 "Code not in flash");

    elf_close(&handle);
}

ZTEST_SUITE(elf_loader_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(elf_loader_tests, test_parse) { test_elf_parse_basic(); }
ZTEST(elf_loader_tests, test_xip) { test_elf_load_xip(); }

#endif
```

## 性能优化

### 1. 缓存ELF解析结果

```c
#define ELF_CACHE_SIZE 4

static struct {
    char path[256];
    elf_handle_t handle;
    bool valid;
} g_elf_cache[ELF_CACHE_SIZE];

static elf_handle_t *elf_cache_lookup(const char *path)
{
    for (int i = 0; i < ELF_CACHE_SIZE; i++) {
        if (g_elf_cache[i].valid &&
            strcmp(g_elf_cache[i].path, path) == 0) {
            return &g_elf_cache[i].handle;
        }
    }
    return NULL;
}
```

### 2. 延迟重定位

```c
// 仅在首次访问时进行重定位
static int elf_relocate_on_demand(elf_handle_t *handle, uint32_t addr)
{
    // 查找包含该地址的段
    for (int i = 0; i < handle->num_segments; i++) {
        if (addr >= handle->segments[i].load_addr &&
            addr < handle->segments[i].load_addr + handle->segments[i].memsz) {

            if (!handle->segments[i].relocated) {
                // 执行重定位
                elf_apply_segment_relocations(handle, i);
                handle->segments[i].relocated = true;
            }
            return 0;
        }
    }
    return -EINVAL;
}
```

## 错误处理

```c
const char *elf_strerror(int error)
{
    switch (error) {
    case 0:
        return "Success";
    case -ENOMEM:
        return "Out of memory";
    case -ENOEXEC:
        return "Invalid executable format";
    case -EINVAL:
        return "Invalid argument";
    case -EIO:
        return "I/O error";
    case -ENOENT:
        return "File not found";
    case -ENOSYS:
        return "Function not implemented";
    case -ERANGE:
        return "Value out of range";
    default:
        return "Unknown error";
    }
}
```

## 常见问题

### Q: ELF加载失败，返回ENOEXEC
A: 检查ELF是否为ARM 32位静态链接可执行文件，使用`readelf -h`验证

### Q: 重定位失败
A: 确保CONFIG_ELF_SUPPORT_RELOCATION=y，检查重定位类型是否支持

### Q: XIP不工作
A: 验证ELF文件在Flash中，检查Flash地址配置

### Q: 内存不足
A: 减小CONFIG_PROCESS_DEFAULT_HEAP/STACK，或使用XIP减少RAM使用

## 最佳实践

1. **编译选项**：使用`-static -nostdlib`生成静态链接ELF
2. **链接脚本**：确保段对齐到MPU要求
3. **代码优化**：使用`-Os`减小代码大小
4. **调试**：启用CONFIG_ELF_LOADER_DEBUG查看详细信息
5. **测试**：使用简单的hello world程序先测试加载器
