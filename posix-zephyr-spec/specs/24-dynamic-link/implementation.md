# 实现指导

## 总体架构

```
┌─────────────────────────────────────┐
│     dlopen/dlsym/dlclose API        │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│    动态链接器核心                    │
│  - 库加载管理                        │
│  - 符号表管理                        │
│  - 重定位处理                        │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│    ELF加载器 (Spec 16)              │
│  - ELF解析                          │
│  - 段加载                            │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│    内存管理                          │
│  - 地址池分配                        │
│  - MPU配置                          │
└─────────────────────────────────────┘
```

## 实现步骤

### 1. 初始化动态链接器

```c
static dl_handle_t *g_dl_handles = NULL;
static symbol_table_t g_symbol_table;
static addr_pool_t g_addr_pool;
static struct k_mutex g_dl_lock;

void dl_init(void)
{
    k_mutex_init(&g_dl_lock);

    // 初始化地址池
    g_addr_pool.base = (void *)CONFIG_DYNAMIC_LIB_BASE;
    g_addr_pool.slot_size = CONFIG_DYNAMIC_LIB_SIZE;
    g_addr_pool.slot_count = CONFIG_DYNAMIC_LIB_MAX;
    g_addr_pool.used = k_calloc(g_addr_pool.slot_count, sizeof(bool));
    k_mutex_init(&g_addr_pool.lock);

    // 初始化符号表
    g_symbol_table.capacity = 256;
    g_symbol_table.entries = k_calloc(g_symbol_table.capacity,
                                      sizeof(symbol_entry_t));
    g_symbol_table.count = 0;
    k_mutex_init(&g_symbol_table.lock);
}
```

### 2. 实现dlopen

```c
void *dlopen(const char *filename, int flags)
{
    if (!filename) {
        errno = EINVAL;
        return NULL;
    }

    k_mutex_lock(&g_dl_lock, K_FOREVER);

    // 检查是否已加载
    dl_handle_t *handle = find_loaded_library(filename);
    if (handle) {
        if (flags & RTLD_NOLOAD) {
            handle->refcount++;
            k_mutex_unlock(&g_dl_lock);
            return handle;
        }
    }

    // 分配新句柄
    handle = k_malloc(sizeof(dl_handle_t));
    memset(handle, 0, sizeof(dl_handle_t));
    strncpy(handle->path, filename, sizeof(handle->path) - 1);
    handle->flags = flags;
    handle->refcount = 1;

    // 分配加载地址
    handle->base_addr = alloc_load_address(&g_addr_pool);
    if (!handle->base_addr) {
        k_free(handle);
        k_mutex_unlock(&g_dl_lock);
        errno = ENOMEM;
        return NULL;
    }

    // 加载ELF文件
    if (load_elf_library(filename, handle) < 0) {
        free_load_address(&g_addr_pool, handle->base_addr);
        k_free(handle);
        k_mutex_unlock(&g_dl_lock);
        return NULL;
    }

    // 处理重定位
    if (flags & RTLD_NOW) {
        if (relocate_library(handle) < 0) {
            unload_library(handle);
            k_mutex_unlock(&g_dl_lock);
            return NULL;
        }
        handle->relocated = true;
    }

    // 注册符号
    if (flags & RTLD_GLOBAL) {
        register_symbols(handle, &g_symbol_table);
    }

    // 添加到链表
    handle->next = g_dl_handles;
    g_dl_handles = handle;

    k_mutex_unlock(&g_dl_lock);
    return handle;
}
```

### 3. 实现dlsym

```c
void *dlsym(void *handle, const char *symbol)
{
    if (!symbol) {
        errno = EINVAL;
        return NULL;
    }

    k_mutex_lock(&g_dl_lock, K_FOREVER);

    void *addr = NULL;

    if (handle == RTLD_DEFAULT) {
        // 搜索全局符号表
        addr = lookup_symbol(&g_symbol_table, symbol);
    } else if (handle == RTLD_NEXT) {
        // 搜索下一个库（不支持）
        errno = ENOSYS;
    } else {
        // 搜索指定库
        dl_handle_t *lib = (dl_handle_t *)handle;
        addr = lookup_symbol_in_library(lib, symbol);

        // 如果未找到且是RTLD_GLOBAL，搜索全局表
        if (!addr && (lib->flags & RTLD_GLOBAL)) {
            addr = lookup_symbol(&g_symbol_table, symbol);
        }
    }

    k_mutex_unlock(&g_dl_lock);

    if (!addr) {
        errno = ENOENT;
    }

    return addr;
}
```

### 4. 实现dlclose

```c
int dlclose(void *handle)
{
    if (!handle) {
        errno = EINVAL;
        return -1;
    }

    k_mutex_lock(&g_dl_lock, K_FOREVER);

    dl_handle_t *lib = (dl_handle_t *)handle;
    lib->refcount--;

    if (lib->refcount == 0 && !(lib->flags & RTLD_NODELETE)) {
        // 移除符号
        if (lib->flags & RTLD_GLOBAL) {
            unregister_symbols(lib, &g_symbol_table);
        }

        // 卸载库
        unload_library(lib);

        // 从链表移除
        remove_from_list(&g_dl_handles, lib);

        k_free(lib);
    }

    k_mutex_unlock(&g_dl_lock);
    return 0;
}
```

### 5. 符号查找实现

```c
void *lookup_symbol(symbol_table_t *table, const char *name)
{
    k_mutex_lock(&table->lock, K_FOREVER);

    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].name, name) == 0) {
            void *addr = table->entries[i].addr;
            k_mutex_unlock(&table->lock);
            return addr;
        }
    }

    k_mutex_unlock(&table->lock);
    return NULL;
}

void *lookup_symbol_in_library(dl_handle_t *lib, const char *name)
{
    // 使用ELF符号表查找
    for (int i = 0; i < lib->symtab_count; i++) {
        Elf32_Sym *sym = &lib->symtab[i];
        const char *sym_name = &lib->strtab[sym->st_name];

        if (strcmp(sym_name, name) == 0) {
            if (ELF32_ST_TYPE(sym->st_info) == STT_FUNC ||
                ELF32_ST_TYPE(sym->st_info) == STT_OBJECT) {
                return (void *)(lib->base_addr + sym->st_value);
            }
        }
    }

    return NULL;
}
```

### 6. 重定位处理

```c
int relocate_library(dl_handle_t *lib)
{
    // 处理REL重定位
    for (size_t i = 0; i < lib->rel_count; i++) {
        Elf32_Rel *rel = &lib->rel[i];
        if (apply_relocation(lib, rel, 0) < 0) {
            return -1;
        }
    }

    // 处理RELA重定位
    for (size_t i = 0; i < lib->rela_count; i++) {
        Elf32_Rela *rela = &lib->rela[i];
        if (apply_relocation(lib, (Elf32_Rel *)rela, rela->r_addend) < 0) {
            return -1;
        }
    }

    return 0;
}

int apply_relocation(dl_handle_t *lib, Elf32_Rel *rel, int32_t addend)
{
    uint32_t *loc = (uint32_t *)(lib->base_addr + rel->r_offset);
    uint32_t type = ELF32_R_TYPE(rel->r_info);
    uint32_t sym_idx = ELF32_R_SYM(rel->r_info);

    Elf32_Sym *sym = &lib->symtab[sym_idx];
    void *sym_addr = resolve_symbol(lib, sym);

    switch (type) {
    case R_ARM_RELATIVE:
        *loc = (uint32_t)lib->base_addr + addend;
        break;

    case R_ARM_ABS32:
        *loc = (uint32_t)sym_addr + addend;
        break;

    case R_ARM_GLOB_DAT:
    case R_ARM_JUMP_SLOT:
        *loc = (uint32_t)sym_addr;
        break;

    default:
        printk("Unsupported relocation type: %d\n", type);
        return -1;
    }

    return 0;
}
```

## 测试策略

### 单元测试

```c
void test_dlopen_basic(void)
{
    void *handle = dlopen("/lib/libtest.so", RTLD_NOW);
    assert(handle != NULL);

    dlclose(handle);
}

void test_dlsym(void)
{
    void *handle = dlopen("/lib/libtest.so", RTLD_NOW);
    assert(handle != NULL);

    typedef int (*test_func_t)(void);
    test_func_t func = dlsym(handle, "test_function");
    assert(func != NULL);

    int result = func();
    assert(result == 42);

    dlclose(handle);
}
```

### 集成测试

```c
void test_multiple_libraries(void)
{
    void *lib1 = dlopen("/lib/lib1.so", RTLD_NOW | RTLD_GLOBAL);
    void *lib2 = dlopen("/lib/lib2.so", RTLD_NOW);

    // lib2应该能看到lib1的符号
    void *sym = dlsym(lib2, "lib1_function");
    assert(sym != NULL);

    dlclose(lib2);
    dlclose(lib1);
}
```

## 性能优化

1. **符号哈希表**：使用ELF哈希表加速符号查找
2. **延迟绑定**：RTLD_LAZY模式延迟重定位
3. **符号缓存**：缓存常用符号地址
4. **预加载**：启动时预加载常用库

## 调试支持

```c
#ifdef CONFIG_DYNAMIC_LINK_DEBUG
#define DL_DEBUG(fmt, ...) printk("[DL] " fmt "\n", ##__VA_ARGS__)
#else
#define DL_DEBUG(fmt, ...)
#endif

void dl_dump_info(void *handle)
{
    dl_handle_t *lib = (dl_handle_t *)handle;
    printk("Library: %s\n", lib->path);
    printk("Base: %p\n", lib->base_addr);
    printk("Size: %zu\n", lib->size);
    printk("Refcount: %d\n", lib->refcount);
}
```
