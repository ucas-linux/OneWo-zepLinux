# getenv/setenv - 环境变量操作

## 定义

```c
char *getenv(const char *name);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
```

## 实现

```c
char *getenv(const char *name);  // 获取环境变量
int setenv(const char *name, const char *value, int overwrite);  // 设置
int unsetenv(const char *name);  // 删除
```

## 测试

```c
void test_env(void)
{
    setenv("TEST", "value", 1);
    assert(strcmp(getenv("TEST"), "value") == 0);
    unsetenv("TEST");
    assert(getenv("TEST") == NULL);
}
```
