# 实现指导

## 总体架构

```
┌─────────────────────────────────────┐
│     POSIX IPC API Layer             │
│  mq_*, sem_*, shm_*, socket()       │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│    IPC命名空间管理器                 │
│  - 对象注册表                        │
│  - 引用计数                          │
│  - 权限检查                          │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│    Zephyr内核原语                    │
│  k_msgq, k_sem, k_pipe, k_mem       │
└─────────────────────────────────────┘
```

## 实现步骤

### 1. 初始化IPC子系统

```c
void posix_ipc_init(void)
{
    k_mutex_init(&g_ipc_lock);
    g_ipc_registry = NULL;
}
```

### 2. 实现命名空间管理

```c
ipc_object_t *find_ipc_object(const char *name, ipc_type_t type)
{
    k_mutex_lock(&g_ipc_lock, K_FOREVER);

    for (ipc_object_t *obj = g_ipc_registry; obj; obj = obj->next) {
        if (obj->type == type && strcmp(obj->name, name) == 0) {
            k_mutex_unlock(&g_ipc_lock);
            return obj;
        }
    }

    k_mutex_unlock(&g_ipc_lock);
    return NULL;
}

ipc_object_t *register_ipc_object(const char *name, ipc_type_t type,
                                   void *handle, mode_t mode)
{
    ipc_object_t *obj = k_malloc(sizeof(ipc_object_t));
    strncpy(obj->name, name, sizeof(obj->name) - 1);
    obj->type = type;
    obj->handle = handle;
    obj->refcount = 0;
    obj->mode = mode;

    k_mutex_lock(&g_ipc_lock, K_FOREVER);
    obj->next = g_ipc_registry;
    g_ipc_registry = obj;
    k_mutex_unlock(&g_ipc_lock);

    return obj;
}
```

### 3. 实现消息队列

参见 mq.md

### 4. 实现命名信号量

参见 named_sem.md

### 5. 实现共享内存

参见 shm.md

### 6. 实现UNIX域套接字

参见 unix_socket.md

## 测试策略

### 单元测试

```c
void test_mq_basic(void)
{
    struct mq_attr attr = {
        .mq_maxmsg = 10,
        .mq_msgsize = 64
    };

    mqd_t mq = mq_open("/test_queue", O_CREAT | O_RDWR, 0644, &attr);
    assert(mq != (mqd_t)-1);

    const char *msg = "hello";
    assert(mq_send(mq, msg, strlen(msg), 0) == 0);

    char buf[64];
    assert(mq_receive(mq, buf, sizeof(buf), NULL) == strlen(msg));
    assert(strcmp(buf, msg) == 0);

    mq_close(mq);
    mq_unlink("/test_queue");
}
```

### 集成测试

```c
void test_producer_consumer(void)
{
    // 生产者线程
    // 消费者线程
    // 验证消息传递
}

void test_shared_memory(void)
{
    // 进程A写入
    // 进程B读取
    // 验证数据一致性
}
```

## 性能优化

1. **哈希表加速命名查找**
2. **消息队列零拷贝**
3. **共享内存直接访问**
4. **引用计数避免重复创建**

## 调试支持

```c
#ifdef CONFIG_POSIX_IPC_DEBUG
#define IPC_DEBUG(fmt, ...) printk("[IPC] " fmt "\n", ##__VA_ARGS__)
#else
#define IPC_DEBUG(fmt, ...)
#endif
```
