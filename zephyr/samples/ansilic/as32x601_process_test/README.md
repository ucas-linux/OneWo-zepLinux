# AS32X601 Process Management Test

## 简介

这是一个针对 AS32X601 RISC-V MCU 的进程管理测试程序，用于验证 Embox 风格的进程模型在 Zephyr RTOS 上的实现。

## 测试内容

1. **进程创建和PID分配** - 验证进程创建和唯一PID分配
2. **父子进程关系** - 验证进程层级关系
3. **文件描述符表** - 验证FD分配、检索和释放
4. **环境变量** - 验证环境变量的设置、获取和更新
5. **进程Fork** - 验证资源复制和独立性
6. **getpid()集成** - 验证POSIX getpid()接口

## 编译和烧录

```bash
# 编译
west build -p always -b as32x601_evb/as32x601 zephyr/samples/ansilic/as32x601_process_test

# 烧录
west flash

# 查看串口输出
minicom -D /dev/ttyUSB0 -b 115200
```

## 预期输出

```
=== AS32X601 Process Management Test Suite ===
Starting tests...

Testing process creation...
Process 1 created with PID: 1
Process 2 created with PID: 2
PIDs are unique: 1 != 2
Process creation test PASSED

Testing parent-child relationship...
Parent process created with PID: 3
Child process created with PID: 4
Parent-child relationship verified
Parent-child test PASSED

Testing file descriptor table...
FD1 allocated: 0
FD2 allocated: 1
FD1 removed successfully
FD table test PASSED

Testing environment variables...
Set TEST_VAR=test_value
Got TEST_VAR=test_value
Updated TEST_VAR=new_value
Environment variables test PASSED

Testing process fork...
Parent: Set PARENT_VAR=parent_value
Child forked with PID: 5
Child: Inherited PARENT_VAR=parent_value
Parent env unchanged: PARENT_VAR=parent_value
Fork test PASSED

Testing getpid()...
Process created with PID: 6
Current getpid() returns: 1
getpid test PASSED

=== All Tests Completed ===
```

## 配置说明

- `CONFIG_PROCESS_MODEL=y` - 启用进程模型
- `CONFIG_MAX_PROCESS_COUNT=8` - 最多支持8个进程（MCU内存限制）
- `CONFIG_MAX_FD_PER_PROCESS=16` - 每个进程最多16个文件描述符
- `CONFIG_HEAP_MEM_POOL_SIZE=32768` - 32KB堆内存用于环境变量等动态分配

## 故障排除

如果测试失败，检查：
1. 堆内存是否足够（增加 `CONFIG_HEAP_MEM_POOL_SIZE`）
2. 进程数量限制（调整 `CONFIG_MAX_PROCESS_COUNT`）
3. 串口输出波特率是否正确
4. 板子是否正确连接

## 相关文档

- 进程模型实现：`/PROCESS_MODEL_IMPLEMENTATION.md`
- 实现计划：`/opt/Program/UCAS/embox/plan.md`
