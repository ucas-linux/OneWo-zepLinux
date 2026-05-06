# 实现指导 - 环境变量

## 核心实现

```c
// 全局环境变量表
static struct {
    char *name;
    char *value;
} g_env_table[MAX_ENV_VARS];

static struct k_mutex g_env_lock;

char *getenv(const char *name)
{
    if (!name) return NULL;

    k_mutex_lock(&g_env_lock, K_FOREVER);
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (g_env_table[i].name &&
            strcmp(g_env_table[i].name, name) == 0) {
            char *value = g_env_table[i].value;
            k_mutex_unlock(&g_env_lock);
            return value;
        }
    }
    k_mutex_unlock(&g_env_lock);
    return NULL;
}

int setenv(const char *name, const char *value, int overwrite)
{
    if (!name || !value || strchr(name, '=')) {
        errno = EINVAL;
        return -1;
    }

    k_mutex_lock(&g_env_lock, K_FOREVER);

    // 查找现有变量
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (g_env_table[i].name &&
            strcmp(g_env_table[i].name, name) == 0) {
            if (!overwrite) {
                k_mutex_unlock(&g_env_lock);
                return 0;
            }
            // 更新值
            k_free(g_env_table[i].value);
            g_env_table[i].value = strdup(value);
            k_mutex_unlock(&g_env_lock);
            return 0;
        }
    }

    // 添加新变量
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (!g_env_table[i].name) {
            g_env_table[i].name = strdup(name);
            g_env_table[i].value = strdup(value);
            k_mutex_unlock(&g_env_lock);
            return 0;
        }
    }

    k_mutex_unlock(&g_env_lock);
    errno = ENOMEM;
    return -1;
}
```

## 配置

```kconfig
config POSIX_ENV_VARS
    bool "Environment variables"
    default y

config MAX_ENV_VARS
    int "Maximum environment variables"
    default 32
```
