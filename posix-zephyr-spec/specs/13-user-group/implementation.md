# 实现指导 - 用户和组

## 核心实现

```c
uid_t getuid(void) { return 0; }  // 简化：总是返回root
gid_t getgid(void) { return 0; }
uid_t geteuid(void) { return 0; }
gid_t getegid(void) { return 0; }

int setuid(uid_t uid) { return 0; }  // 简化：总是成功
int setgid(gid_t gid) { return 0; }
```

## 配置

```kconfig
config POSIX_USER_GROUP
    bool "User/group management"
    default n
    help
      Simplified user/group support (always root)
```
