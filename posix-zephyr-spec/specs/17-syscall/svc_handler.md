# SVC异常处理器

## ARM SVC异常

在ARM Cortex-M3上，SVC（Supervisor Call）指令触发异常，用于从用户代码切换到特权模式执行系统调用。

## 异常向量

```c
// 在Zephyr的异常向量表中注册SVC处理器
void z_arm_svc_handler(void);

// 向量表（startup代码）
__attribute__((section(".vectors")))
const void *vector_table[] = {
    // ...
    z_arm_svc_handler,  // SVC异常
    // ...
};
```

## SVC处理器实现（汇编）

```asm
/* arch/arm/core/cortex_m/svc_handler.S */

.syntax unified
.thumb

.global z_arm_svc_handler
.type z_arm_svc_handler, %function

z_arm_svc_handler:
    /* 保存上下文 */
    /* SVC异常自动保存r0-r3, r12, lr, pc, xpsr到栈 */
    /* 我们需要保存r4-r11 */

    /* 检查是从哪个栈触发的异常 */
    tst lr, #4
    ite eq
    mrseq r0, msp       /* 主栈指针 */
    mrsne r0, psp       /* 进程栈指针 */

    /* r0现在指向异常栈帧 */
    /* 栈帧布局：
     * [r0, r1, r2, r3, r12, lr, pc, xpsr]
     */

    /* 提取系统调用号（从r7） */
    ldr r1, [r0, #24]   /* 获取保存的PC */
    ldrh r2, [r1, #-2]  /* 读取SVC指令（16位Thumb） */
    /* 对于SVC #0，我们从r7获取系统调用号 */

    /* 保存r4-r11到栈 */
    push {r4-r11, lr}

    /* 调用C处理函数 */
    /* r0 = 异常栈帧指针 */
    bl syscall_dispatch

    /* 恢复r4-r11 */
    pop {r4-r11, lr}

    /* 返回 */
    bx lr

.size z_arm_svc_handler, . - z_arm_svc_handler
```

## C语言处理函数

```c
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
} exception_frame_t;

void syscall_dispatch(exception_frame_t *frame)
{
    uint32_t syscall_nr;
    uint32_t args[7];
    int32_t result;

    /* 获取系统调用号（从r7，需要从完整上下文获取） */
    /* 注意：r7不在异常栈帧中，需要从线程上下文获取 */
    struct k_thread *thread = k_current_get();
    syscall_nr = thread->arch.basepri;  /* 临时存储位置 */

    /* 提取参数 */
    args[0] = frame->r0;
    args[1] = frame->r1;
    args[2] = frame->r2;
    args[3] = frame->r3;
    /* r4-r6需要从线程上下文获取 */
    args[4] = thread->callee_saved.r4;
    args[5] = thread->callee_saved.r5;
    args[6] = thread->callee_saved.r6;

    /* 调用系统调用 */
    result = syscall_invoke(syscall_nr, args);

    /* 设置返回值 */
    frame->r0 = result;
}
```

## 改进的汇编实现（完整上下文）

```asm
z_arm_svc_handler:
    /* 确定使用的栈 */
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp

    /* 保存完整上下文到栈 */
    /* 异常已保存：r0-r3, r12, lr, pc, xpsr */
    /* 手动保存：r4-r11 */
    stmdb r0!, {r4-r11}

    /* 保存r7（系统调用号） */
    mov r1, r7

    /* 调用C函数 */
    /* r0 = 栈指针, r1 = 系统调用号 */
    bl syscall_dispatch_full

    /* 恢复r4-r11 */
    ldmia r0!, {r4-r11}

    /* 更新栈指针 */
    tst lr, #4
    ite eq
    msreq msp, r0
    msrne psp, r0

    bx lr
```

## 完整C处理函数

```c
typedef struct {
    /* 手动保存的寄存器 */
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    /* 异常自动保存的寄存器 */
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
} full_context_t;

void syscall_dispatch_full(full_context_t *ctx, uint32_t syscall_nr)
{
    uint32_t args[7];
    int32_t result;

    /* 提取参数（r0-r6） */
    args[0] = ctx->r0;
    args[1] = ctx->r1;
    args[2] = ctx->r2;
    args[3] = ctx->r3;
    args[4] = ctx->r4;
    args[5] = ctx->r5;
    args[6] = ctx->r6;

#ifdef CONFIG_SYSCALL_TRACE
    syscall_trace_enter(syscall_nr, args);
#endif

    /* 调用系统调用 */
    result = syscall_invoke(syscall_nr, args);

#ifdef CONFIG_SYSCALL_TRACE
    syscall_trace_exit(syscall_nr, result);
#endif

    /* 设置返回值 */
    ctx->r0 = result;
}
```

## 系统调用跟踪

```c
#ifdef CONFIG_SYSCALL_TRACE

static const char *syscall_names[] = {
    [1] = "exit",
    [2] = "fork",
    [3] = "read",
    [4] = "write",
    [5] = "open",
    [6] = "close",
    // ...
};

void syscall_trace_enter(uint32_t nr, uint32_t *args)
{
    const char *name = (nr < ARRAY_SIZE(syscall_names)) ?
                       syscall_names[nr] : "unknown";

    printk("[SYSCALL] %s(%u, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x)\n",
           name, nr, args[0], args[1], args[2], args[3], args[4], args[5]);
}

void syscall_trace_exit(uint32_t nr, int32_t result)
{
    const char *name = (nr < ARRAY_SIZE(syscall_names)) ?
                       syscall_names[nr] : "unknown";

    if (result < 0) {
        printk("[SYSCALL] %s = %d (errno=%d)\n", name, result, -result);
    } else {
        printk("[SYSCALL] %s = %d\n", name, result);
    }
}

#endif
```

## 性能优化

### 1. 快速路径

```c
/* 对于常用系统调用，使用快速路径 */
void syscall_dispatch_fast(full_context_t *ctx, uint32_t syscall_nr)
{
    int32_t result;

    /* 快速路径：常用系统调用 */
    switch (syscall_nr) {
    case __NR_read:
        result = sys_read(ctx->r0, (void *)ctx->r1, ctx->r2);
        break;
    case __NR_write:
        result = sys_write(ctx->r0, (const void *)ctx->r1, ctx->r2);
        break;
    case __NR_getpid:
        result = sys_getpid();
        break;
    default:
        /* 慢速路径：查表 */
        result = syscall_invoke(syscall_nr, (uint32_t *)&ctx->r0);
        break;
    }

    ctx->r0 = result;
}
```

### 2. 内联汇编优化

```c
/* 使用内联汇编减少函数调用开销 */
static inline int32_t syscall_invoke_inline(uint32_t nr, uint32_t *args)
{
    register uint32_t r7 asm("r7") = nr;
    register uint32_t r0 asm("r0") = args[0];
    register uint32_t r1 asm("r1") = args[1];
    register uint32_t r2 asm("r2") = args[2];

    asm volatile("svc #0"
                 : "+r"(r0)
                 : "r"(r7), "r"(r1), "r"(r2)
                 : "memory");

    return r0;
}
```

## 错误处理

```c
/* 处理无效系统调用 */
int32_t syscall_invoke(uint32_t nr, uint32_t *args)
{
    if (nr >= SYSCALL_TABLE_SIZE || !syscall_table[nr]) {
        printk("[SYSCALL ERROR] Invalid syscall: %u\n", nr);
        return -ENOSYS;
    }

    return syscall_table[nr](args);
}
```
