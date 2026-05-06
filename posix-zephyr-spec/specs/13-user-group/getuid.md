# getuid/setuid - 用户ID管理

## 定义

```c
uid_t getuid(void);
uid_t geteuid(void);
int setuid(uid_t uid);
```

## 简化实现

```c
// Cortex-M3无多用户，简化实现
uid_t getuid(void) { return 0; }  // 总是root
uid_t geteuid(void) { return 0; }
int setuid(uid_t uid) { return 0; }  // 总是成功
```
