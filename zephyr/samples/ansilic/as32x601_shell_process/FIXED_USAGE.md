# Shell Process Execution - 使用说明

## 问题修复

**问题**: 之前的实现使用自定义shell循环，但无法正确接收输入
**原因**: 使用`fgets(stdin)`在Zephyr环境下无法正常工作
**解决**: 改用Zephyr的shell子系统，main函数返回后让shell线程处理输入

## 构建和烧录

```bash
cd /opt/Program/UCAS/OneWo-zepLinux

# 清理并构建
rm -rf build
west build -b as32x601_evb/as32x601 zephyr/samples/ansilic/as32x601_shell_process

# 烧录到板子
west flash
```

## 使用说明

### 启动信息

```
========================================
  AS32X601 Shell with Process Support
  Based on Embox process model
========================================

Init process PID: 1

Process-based commands available:
  hello     - Print hello message
  echo      - Echo arguments
  ps        - List processes
  getpid    - Show process ID
  info      - Show process info
  sleep     - Sleep for milliseconds
  test      - Process creation test
  prochelp  - Show this help

Each command runs in a separate process!
Type 'help' to see all available commands.

shell>
```

### 可用命令

每个命令都在**独立进程**中运行：

1. **hello** - 打印问候消息和进程ID
   ```
   shell> hello
   Hello from process PID 2!
   ```

2. **echo [args...]** - 回显参数
   ```
   shell> echo test 123
   test 123
   ```

3. **ps** - 列出进程
   ```
   shell> ps
   PID    PPID   Command
   ------------------------
   3      1      ps
   1      0      init
   ```

4. **getpid** - 显示当前进程ID
   ```
   shell> getpid
   Current PID: 4
   Parent PID: 1
   ```

5. **info** - 显示详细进程信息
   ```
   shell> info
   Process Information:
     PID: 5
     Parent PID: 1
     Main thread: 0x20001234
     Ref count: 1
     Exit code: 0
   ```

6. **sleep <milliseconds>** - 休眠测试
   ```
   shell> sleep 1000
   Sleeping for 1000 ms in PID 6...
   Woke up!
   ```

7. **test** - 进程创建压力测试
   ```
   shell> test
   Test command running in PID 7
   Creating 3 child processes...
     Created child process PID 8
     Created child process PID 9
     Created child process PID 10
   ```

8. **prochelp** - 显示进程命令帮助
   ```
   shell> prochelp
   Available commands:
     help     - Show this help message
     hello    - Print hello message
     ...
   ```

### Zephyr内置命令

除了进程命令，还可以使用Zephyr shell的内置命令：

- **help** - 显示所有可用命令
- **clear** - 清屏
- **history** - 命令历史
- **kernel** - 内核相关命令
- **device** - 设备列表
- 等等...

## 实现原理

### 架构

```
Zephyr Shell Thread
    ↓
shell_hello_handler() (在shell线程中)
    ↓
shell_exec_command()
    ↓
new_task() [创建新进程]
    ↓
    ├─ process_create()
    ├─ k_thread_create()
    └─ task_trampoline()
         ↓
         run_cmd()
         ↓
         cmd_hello() [在新进程中执行]
         ↓
         退出并发送消息
    ↓
waitpid() [等待进程完成]
    ↓
返回到shell
```

### 关键点

1. **Zephyr Shell集成**: 使用`SHELL_CMD_ARG_REGISTER`注册命令
2. **每个命令一个handler**: 每个handler查找对应的进程命令并执行
3. **main()返回**: main函数初始化后返回，让shell线程接管
4. **进程隔离**: 每个命令在独立的进程和线程中运行

## 与Embox的对比

| 特性 | Embox | Zephyr实现 |
|------|-------|-----------|
| Shell输入 | 自定义readline | Zephyr shell子系统 |
| 命令执行 | process_external() | shell_exec_command() |
| 进程创建 | new_task() | new_task() (兼容) |
| 输入处理 | tish主循环 | Shell线程 |
| 命令注册 | CMD_REGISTER | SHELL_CMD_ARG_REGISTER |

## 配置

### prj.conf关键配置

```ini
# Shell (必须)
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
CONFIG_SHELL_PROMPT_UART="shell> "
CONFIG_SHELL_CMD_BUFF_SIZE=256
CONFIG_SHELL_PRINTF_BUFF_SIZE=256

# 进程支持
CONFIG_MAX_PROCESS_COUNT=32
CONFIG_MAX_FD_PER_PROCESS=16

# 内存
CONFIG_HEAP_MEM_POOL_SIZE=16384
CONFIG_MAIN_STACK_SIZE=2048

# 动态线程
CONFIG_DYNAMIC_THREAD=y
CONFIG_THREAD_NAME=y

# 禁用日志以避免冲突
CONFIG_LOG=n
CONFIG_BOOT_BANNER=n
```

## 内存占用

```
ROM: 70,864 bytes (~69 KB)
RAM: 29,856 bytes (~29 KB)

每个进程: ~2.2 KB
最大进程数: 32
```

## 故障排除

### 无法输入字符

**问题**: shell>提示符显示，但无法输入
**原因**:
- 使用了自定义shell循环（fgets）
- Serial backend未正确配置

**解决**:
- 使用Zephyr shell子系统
- 确保`CONFIG_SHELL_BACKEND_SERIAL=y`
- main()函数要返回，不能阻塞

### 进程创建失败

**问题**: new_task返回负数
**解决**: 增加heap大小 `CONFIG_HEAP_MEM_POOL_SIZE`

### 串口无输出

**问题**: 烧录后无任何输出
**检查**:
- Serial配置是否正确
- 波特率设置
- Console配置

## 测试验证

1. **基本测试**
   ```
   shell> hello
   ```

2. **进程隔离测试**
   ```
   shell> getpid
   shell> getpid
   # 应该看到不同的PID
   ```

3. **进程层次测试**
   ```
   shell> test
   # 创建多个子进程
   ```

4. **长时间运行测试**
   ```
   shell> sleep 5000
   # 等待5秒
   ```

## 参考项目

成功运行的参考实现：
```
zephyr/samples/ansilic/as32x601_boot_demo/
```

该项目使用相同的Zephyr shell子系统，可以正常接收输入。

## 总结

✅ **修复完成**
- 改用Zephyr shell子系统
- main()函数正确返回
- 所有命令在独立进程中运行
- 输入输出正常工作

**关键改变**:
1. 删除了自定义shell循环
2. 使用`SHELL_CMD_ARG_REGISTER`注册命令
3. 每个命令有独立的handler
4. main()返回让shell线程工作
