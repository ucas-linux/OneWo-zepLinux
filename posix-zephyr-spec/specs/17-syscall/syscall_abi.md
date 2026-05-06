# 系统调用ABI和参数传递

## ARM EABI调用约定

### 寄存器使用

```
r0-r3:   参数传递和返回值
r4-r11:  被调用者保存（callee-saved）
r12:     临时寄存器（IP）
r13:     栈指针（SP）
r14:     链接寄存器（LR）
r15:     程序计数器（PC）
```

### 系统调用约定

```
r7:      系统调用号
r0-r6:   系统调用参数（最多7个）
r0:      返回值（成功时>=0，失败时为负errno）
```

## 参数传递规则

### 1-4个参数

```c
// 示例：write(fd, buf, count)
// r0 = fd
// r1 = buf
// r2 = count
// r7 = __NR_write (4)

int write(int fd, const void *buf, size_t count)
{
    register int r0 asm("r0") = fd;
    register const void *r1 asm("r1") = buf;
    register size_t r2 asm("r2") = count;
    register int r7 asm("r7") = __NR_write;

    asm volatile("svc #0"
                 : "+r"(r0)
                 : "r"(r7), "r"(r1), "r"(r2)
                 : "memory");

    return r0;
}
```

### 5-6个参数

```c
// 示例：mmap(addr, length, prot, flags, fd, offset)
// r0 = addr
// r1 = length
// r2 = prot
// r3 = flags
// r4 = fd
// r5 = offset
// r7 = __NR_mmap (90)

void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset)
{
    register void *r0 asm("r0") = addr;
    register size_t r1 asm("r1") = length;
    register int r2 asm("r2") = prot;
    register int r3 asm("r3") = flags;
    register int r4 asm("r4") = fd;
    register off_t r5 asm("r5") = offset;
    register int r7 asm("r7") = __NR_mmap;

    asm volatile("svc #0"
                 : "+r"(r0)
                 : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5)
                 : "memory");

    return (void *)r0;
}
```

### 超过6个参数

对于超过6个参数的系统调用，使用结构体指针传递：

```c
// 示例：clone系统调用（5个参数，但可能需要更多）
struct clone_args {
    uint64_t flags;
    uint64_t pidfd;
    uint64_t child_tid;
    uint64_t parent_tid;
    uint64_t exit_signal;
    uint64_t stack;
    uint64_t stack_size;
    uint64_t tls;
};

// 通过r0传递结构体指针
long clone3(struct clone_args *args, size_t size)
{
    register struct clone_args *r0 asm("r0") = args;
    register size_t r1 asm("r1") = size;
    register int r7 asm("r7") = __NR_clone3;

    asm volatile("svc #0"
                 : "+r"(r0)
                 : "r"(r7), "r"(r1)
                 : "memory");

    return (long)r0;
}
```

## 返回值处理

### 成功返回

```c
// 返回值 >= 0 表示成功
ssize_t ret = read(fd, buf, count);
if (ret >= 0) {
    // 成功，ret是读取的字节数
}
```

### 错误返回

```c
// 返回值 < 0 表示失败，值为负errno
int ret = open("/nonexistent", O_RDONLY);
if (ret < 0) {
    errno = -ret;  // 设置errno
    return -1;     // 返回-1给应用
}
```

## 系统调用包装器生成

### 宏定义

```c
#define SYSCALL0(name) \
static inline long sys_##name(void) \
{ \
    register long r7 asm("r7") = __NR_##name; \
    register long r0 asm("r0"); \
    asm volatile("svc #0" : "=r"(r0) : "r"(r7) : "memory"); \
    return r0; \
}

#define SYSCALL1(name, type1) \
static inline long sys_##name(type1 arg1) \
{ \
    register long r7 asm("r7") = __NR_##name; \
    register long r0 asm("r0") = (long)arg1; \
    asm volatile("svc #0" : "+r"(r0) : "r"(r7) : "memory"); \
    return r0; \
}

#define SYSCALL2(name, type1, type2) \
static inline long sys_##name(type1 arg1, type2 arg2) \
{ \
    register long r7 asm("r7") = __NR_##name; \
    register long r0 asm("r0") = (long)arg1; \
    register long r1 asm("r1") = (long)arg2; \
    asm volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1) : "memory"); \
    return r0; \
}

#define SYSCALL3(name, type1, type2, type3) \
static inline long sys_##name(type1 arg1, type2 arg2, type3 arg3) \
{ \
    register long r7 asm("r7") = __NR_##name; \
    register long r0 asm("r0") = (long)arg1; \
    register long r1 asm("r1") = (long)arg2; \
    register long r2 asm("r2") = (long)arg3; \
    asm volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory"); \
    return r0; \
}
```

### 使用示例

```c
// 定义系统调用包装器
SYSCALL0(getpid)
SYSCALL1(close, int)
SYSCALL2(kill, pid_t, int)
SYSCALL3(read, int, void*, size_t)

// 使用
pid_t pid = sys_getpid();
int ret = sys_close(fd);
```

## 栈对齐

ARM EABI要求栈在公共接口处8字节对齐：

```c
// 检查栈对齐
static inline bool is_stack_aligned(void)
{
    uint32_t sp;
    asm volatile("mov %0, sp" : "=r"(sp));
    return (sp & 7) == 0;
}

// 系统调用入口检查
void syscall_dispatch_full(full_context_t *ctx, uint32_t syscall_nr)
{
    // 验证栈对齐
    if (!is_stack_aligned()) {
        printk("[SYSCALL ERROR] Stack misaligned\n");
        ctx->r0 = -EFAULT;
        return;
    }

    // 处理系统调用...
}
```

## 浮点参数

如果系统调用需要浮点参数（罕见），使用VFP寄存器：

```c
// 浮点参数通过s0-s15/d0-d7传递
// 但Linux系统调用通常不使用浮点参数
```

## 64位参数

64位参数（如off_t）需要对齐到偶数寄存器：

```c
// lseek(fd, offset, whence)
// fd: r0
// offset (64位): r2-r3 (跳过r1以对齐)
// whence: r4

off_t lseek(int fd, off_t offset, int whence)
{
    register int r0 asm("r0") = fd;
    register long r2 asm("r2") = (long)(offset & 0xFFFFFFFF);
    register long r3 asm("r3") = (long)(offset >> 32);
    register int r4 asm("r4") = whence;
    register int r7 asm("r7") = __NR_lseek;

    asm volatile("svc #0"
                 : "+r"(r0), "+r"(r2), "+r"(r3)
                 : "r"(r7), "r"(r4)
                 : "memory");

    return ((off_t)r3 << 32) | (r2 & 0xFFFFFFFF);
}
```

## 可变参数系统调用

某些系统调用（如ioctl, fcntl）接受可变参数：

```c
int ioctl(int fd, unsigned long request, ...)
{
    va_list ap;
    void *arg;

    va_start(ap, request);
    arg = va_arg(ap, void *);
    va_end(ap);

    register int r0 asm("r0") = fd;
    register unsigned long r1 asm("r1") = request;
    register void *r2 asm("r2") = arg;
    register int r7 asm("r7") = __NR_ioctl;

    asm volatile("svc #0"
                 : "+r"(r0)
                 : "r"(r7), "r"(r1), "r"(r2)
                 : "memory");

    if (r0 < 0) {
        errno = -r0;
        return -1;
    }
    return r0;
}
```

## 性能优化

### 内联系统调用

```c
// 对于频繁调用的系统调用，使用内联
static inline __attribute__((always_inline))
pid_t getpid(void)
{
    register int r7 asm("r7") = __NR_getpid;
    register pid_t r0 asm("r0");

    asm volatile("svc #0" : "=r"(r0) : "r"(r7) : "memory");

    return r0;
}
```

### VDSO支持

对于某些系统调用（如gettimeofday），可以通过VDSO避免陷入内核：

```c
// VDSO页面映射（如果支持）
extern void *__vdso_gettimeofday;

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    // 尝试使用VDSO
    if (__vdso_gettimeofday) {
        typedef int (*vdso_gettimeofday_t)(struct timeval *, struct timezone *);
        vdso_gettimeofday_t func = (vdso_gettimeofday_t)__vdso_gettimeofday;
        return func(tv, tz);
    }

    // 回退到系统调用
    return sys_gettimeofday(tv, tz);
}
```
