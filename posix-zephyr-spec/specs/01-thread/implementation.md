# 实现指导

## 总体架构

### 模块组织

```
posix_thread/
├── pthread_core.c          # 核心线程管理
├── pthread_attr.c          # 线程属性操作
├── pthread_mutex.c         # 互斥锁实现
├── pthread_cond.c          # 条件变量实现
├── pthread_cancel.c        # 线程取消机制
├── pthread_internal.h      # 内部数据结构和函数
└── pthread_config.h        # 配置选项
```

### 全局数据结构

```c
// 全局线程表
static posix_thread_t *g_thread_table[CONFIG_PTHREAD_MAX_THREADS];
static struct k_mutex g_thread_table_lock;
static atomic_t g_next_thread_id = ATOMIC_INIT(1);

// 当前线程TLS键
static pthread_key_t g_current_thread_key;
```

## 核心实现

### 1. 线程ID生成

```c
static pthread_t generate_thread_id(void)
{
    return (pthread_t)atomic_inc(&g_next_thread_id);
}
```

### 2. 线程查找

```c
static posix_thread_t *find_thread_by_id(pthread_t id)
{
    k_mutex_lock(&g_thread_table_lock, K_FOREVER);

    for (int i = 0; i < CONFIG_PTHREAD_MAX_THREADS; i++) {
        if (g_thread_table[i] && g_thread_table[i]->pthread_id == id) {
            posix_thread_t *pt = g_thread_table[i];
            k_mutex_unlock(&g_thread_table_lock);
            return pt;
        }
    }

    k_mutex_unlock(&g_thread_table_lock);
    return NULL;
}
```

### 3. 获取当前线程

```c
static posix_thread_t *get_current_posix_thread(void)
{
    // 使用TLS获取当前线程
    return (posix_thread_t *)pthread_getspecific(g_current_thread_key);
}
```

### 4. 线程注册

```c
static int register_thread(posix_thread_t *pt)
{
    k_mutex_lock(&g_thread_table_lock, K_FOREVER);

    for (int i = 0; i < CONFIG_PTHREAD_MAX_THREADS; i++) {
        if (g_thread_table[i] == NULL) {
            g_thread_table[i] = pt;
            k_mutex_unlock(&g_thread_table_lock);
            return 0;
        }
    }

    k_mutex_unlock(&g_thread_table_lock);
    return -EAGAIN;  // 线程表满
}
```

### 5. 线程注销

```c
static void unregister_thread(posix_thread_t *pt)
{
    k_mutex_lock(&g_thread_table_lock, K_FOREVER);

    for (int i = 0; i < CONFIG_PTHREAD_MAX_THREADS; i++) {
        if (g_thread_table[i] == pt) {
            g_thread_table[i] = NULL;
            break;
        }
    }

    k_mutex_unlock(&g_thread_table_lock);
}
```

## 内存管理

### 线程控制块池

```c
// 静态线程池（可选）
#ifdef CONFIG_PTHREAD_STATIC_POOL
static posix_thread_t g_thread_pool[CONFIG_PTHREAD_MAX_THREADS];
static bool g_thread_pool_used[CONFIG_PTHREAD_MAX_THREADS];
static struct k_mutex g_pool_lock;

static posix_thread_t *alloc_posix_thread(void)
{
    k_mutex_lock(&g_pool_lock, K_FOREVER);

    for (int i = 0; i < CONFIG_PTHREAD_MAX_THREADS; i++) {
        if (!g_thread_pool_used[i]) {
            g_thread_pool_used[i] = true;
            k_mutex_unlock(&g_pool_lock);
            return &g_thread_pool[i];
        }
    }

    k_mutex_unlock(&g_pool_lock);
    return NULL;
}

static void free_posix_thread(posix_thread_t *pt)
{
    k_mutex_lock(&g_pool_lock, K_FOREVER);

    int index = pt - g_thread_pool;
    if (index >= 0 && index < CONFIG_PTHREAD_MAX_THREADS) {
        g_thread_pool_used[index] = false;
    }

    k_mutex_unlock(&g_pool_lock);
}
#else
// 动态分配
static posix_thread_t *alloc_posix_thread(void)
{
    return k_malloc(sizeof(posix_thread_t));
}

static void free_posix_thread(posix_thread_t *pt)
{
    k_free(pt);
}
#endif
```

## 初始化和清理

### 模块初始化

```c
int pthread_subsystem_init(void)
{
    // 初始化全局锁
    k_mutex_init(&g_thread_table_lock);

    // 清空线程表
    memset(g_thread_table, 0, sizeof(g_thread_table));

    // 创建TLS键
    pthread_key_create(&g_current_thread_key, NULL);

#ifdef CONFIG_PTHREAD_STATIC_POOL
    k_mutex_init(&g_pool_lock);
    memset(g_thread_pool_used, 0, sizeof(g_thread_pool_used));
#endif

    return 0;
}

// 使用Zephyr初始化系统
SYS_INIT(pthread_subsystem_init, POST_KERNEL, CONFIG_PTHREAD_INIT_PRIORITY);
```

## 资源跟踪

### 资源跟踪表

```c
typedef struct {
    enum {
        RES_THREAD,
        RES_MUTEX,
        RES_COND,
        RES_SEM
    } type;
    void *resource;
    struct resource_entry *next;
} resource_entry_t;

static resource_entry_t *g_resource_table[CONFIG_PTHREAD_MAX_THREADS];
static struct k_mutex g_resource_lock;

void resource_register(pthread_t pid, int type, void *resource)
{
    resource_entry_t *entry = k_malloc(sizeof(resource_entry_t));
    if (!entry) return;

    entry->type = type;
    entry->resource = resource;

    k_mutex_lock(&g_resource_lock, K_FOREVER);
    entry->next = g_resource_table[pid % CONFIG_PTHREAD_MAX_THREADS];
    g_resource_table[pid % CONFIG_PTHREAD_MAX_THREADS] = entry;
    k_mutex_unlock(&g_resource_lock);
}

void resource_cleanup_all(pthread_t pid)
{
    k_mutex_lock(&g_resource_lock, K_FOREVER);

    resource_entry_t *entry = g_resource_table[pid % CONFIG_PTHREAD_MAX_THREADS];
    while (entry) {
        resource_entry_t *next = entry->next;

        switch (entry->type) {
        case RES_THREAD:
            // 线程资源由pthread_join清理
            break;
        case RES_MUTEX:
            pthread_mutex_destroy((pthread_mutex_t *)entry->resource);
            break;
        case RES_COND:
            pthread_cond_destroy((pthread_cond_t *)entry->resource);
            break;
        }

        k_free(entry);
        entry = next;
    }

    g_resource_table[pid % CONFIG_PTHREAD_MAX_THREADS] = NULL;
    k_mutex_unlock(&g_resource_lock);
}
```

## 配置选项

### Kconfig

```kconfig
menuconfig PTHREAD
    bool "POSIX Thread Support"
    default n
    help
      Enable POSIX thread (pthread) API support.

if PTHREAD

config PTHREAD_DYNAMIC_STACK
    bool "Dynamic thread stack allocation"
    default y
    help
      Use k_malloc to dynamically allocate thread stacks.

config PTHREAD_DEFAULT_STACK_SIZE
    int "Default pthread stack size"
    default 2048
    range 1024 65536
    help
      Default stack size for threads created without attributes.

config PTHREAD_MAX_THREADS
    int "Maximum number of concurrent pthreads"
    default 32
    range 1 256
    help
      Maximum number of POSIX threads that can exist simultaneously.

config PTHREAD_DEFAULT_PRIORITY
    int "Default pthread priority"
    default 5
    range 0 99
    help
      Default Zephyr priority for SCHED_OTHER threads.

config PTHREAD_STATIC_POOL
    bool "Use static thread pool"
    default n
    help
      Pre-allocate thread control blocks statically instead of using k_malloc.

config PTHREAD_INIT_PRIORITY
    int "Pthread subsystem initialization priority"
    default 50
    help
      Initialization priority for pthread subsystem.

endif # PTHREAD
```

## 调试支持

### 调试宏

```c
#ifdef CONFIG_PTHREAD_DEBUG
#define PTHREAD_DEBUG(fmt, ...) printk("[PTHREAD] " fmt "\n", ##__VA_ARGS__)
#else
#define PTHREAD_DEBUG(fmt, ...)
#endif

#define PTHREAD_ERROR(fmt, ...) printk("[PTHREAD ERROR] " fmt "\n", ##__VA_ARGS__)
```

### 线程状态转储

```c
void pthread_dump_threads(void)
{
    printk("=== POSIX Thread Dump ===\n");

    k_mutex_lock(&g_thread_table_lock, K_FOREVER);

    for (int i = 0; i < CONFIG_PTHREAD_MAX_THREADS; i++) {
        posix_thread_t *pt = g_thread_table[i];
        if (pt) {
            printk("Thread %u:\n", pt->pthread_id);
            printk("  Detached: %d\n", pt->detached);
            printk("  Joined: %d\n", pt->joined);
            printk("  Terminated: %d\n", pt->terminated);
            printk("  Stack: %p (%zu bytes)\n", pt->stack, pt->stack_size);
        }
    }

    k_mutex_unlock(&g_thread_table_lock);
}
```

## 性能优化

### 1. 快速路径优化

```c
// 使用likely/unlikely提示编译器
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    if (unlikely(!mutex)) {
        return EINVAL;
    }

    // 快速路径：无竞争情况
    if (likely(k_mutex_lock(&mutex->zmutex, K_NO_WAIT) == 0)) {
        mutex->owner = pthread_self();
        return 0;
    }

    // 慢速路径：有竞争
    return pthread_mutex_lock_slow(mutex);
}
```

### 2. 缓存对齐

```c
// 避免伪共享
typedef struct posix_thread {
    // 热数据（频繁访问）
    struct k_thread zephyr_thread __aligned(64);
    pthread_t pthread_id;
    atomic_t cancel_requested;

    // 冷数据（不常访问）
    void *(*start_routine)(void *);
    void *arg;
    void *retval;
    // ...
} posix_thread_t;
```

## 测试框架

### 单元测试

```c
#ifdef CONFIG_PTHREAD_TESTS

void test_pthread_create_basic(void)
{
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, test_func, NULL);
    zassert_equal(ret, 0, "pthread_create failed");
}

void test_pthread_mutex_lock(void)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int ret = pthread_mutex_lock(&mutex);
    zassert_equal(ret, 0, "pthread_mutex_lock failed");
    pthread_mutex_unlock(&mutex);
}

ZTEST_SUITE(pthread_tests, NULL, NULL, NULL, NULL, NULL);
ZTEST(pthread_tests, test_create) { test_pthread_create_basic(); }
ZTEST(pthread_tests, test_mutex) { test_pthread_mutex_lock(); }

#endif
```

## 移植注意事项

### ARM Cortex-M3特定

1. **栈对齐**：栈地址必须8字节对齐
2. **MPU配置**：如果使用MPU，栈大小必须是2的幂次方
3. **中断优先级**：确保线程优先级不与中断冲突
4. **FPU支持**：如果使用浮点，需要配置FPU上下文保存

### 内存限制

1. **最小栈大小**：建议至少2KB
2. **线程数量**：根据可用RAM调整CONFIG_PTHREAD_MAX_THREADS
3. **动态分配**：确保堆大小足够（CONFIG_HEAP_MEM_POOL_SIZE）

## 集成步骤

1. 将pthread源文件添加到项目
2. 在prj.conf中启用CONFIG_PTHREAD=y
3. 配置栈大小和线程数量
4. 包含<pthread.h>头文件
5. 编译并测试

## 常见问题

### Q: 线程创建失败，返回EAGAIN
A: 检查CONFIG_PTHREAD_MAX_THREADS和堆大小配置

### Q: 互斥锁死锁
A: 使用PTHREAD_MUTEX_ERRORCHECK类型调试

### Q: 线程栈溢出
A: 增加CONFIG_PTHREAD_DEFAULT_STACK_SIZE或使用pthread_attr_setstacksize

### Q: 性能不佳
A: 考虑使用CONFIG_PTHREAD_STATIC_POOL减少动态分配开销
