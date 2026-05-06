# 数据类型 - 环境变量

```c
#define MAX_ENV_VARS 32

typedef struct {
    char *name;
    char *value;
} env_entry_t;

static env_entry_t g_env_table[MAX_ENV_VARS];
static struct k_mutex g_env_lock;
```
