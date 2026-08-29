# RocketPi Shell Process - Ctrl+C 信号支持实现总结

## 任务目标

在 RocketPi (STM32F401) 板上实现 commit 1534e5fb 的 Ctrl+C 信号支持功能。

## 实现状态：✅ 完成

### 构建信息
- **目标板**: RocketPi (STM32F401, ARM Cortex-M4)
- **构建命令**: `west build -p always -b rocket_pi -d build-rocket-pi-anl zephyr/samples/ansilic/rocket_pi_shell_process`
- **固件大小**:
  - Text: 68,676 bytes
  - Data: 244 bytes
  - BSS: 65,323 bytes
  - Total: 134,243 bytes
  - Flash使用率: 13.15% (68KB / 512KB)
  - RAM使用率: 66.69% (65KB / 96KB)

### 架构兼容性实现

#### 1. debug.c - 寄存器和栈跟踪
```c
#if defined(CONFIG_RISCV)
    register unsigned long fp __asm__("s0");   // RISC-V frame pointer
    register unsigned long ra __asm__("ra");   // RISC-V return address
#elif defined(CONFIG_ARM)
    register unsigned long fp __asm__("r7");   // ARM frame pointer
    register unsigned long lr __asm__("lr");   // ARM link register
#endif
```

#### 2. anl_loader.c - 内存屏障指令
```c
#if defined(CONFIG_RISCV)
    __asm__ volatile ("fence" ::: "memory");    // RISC-V 数据同步
    __asm__ volatile ("fence.i" ::: "memory");  // RISC-V 指令缓存刷新
#elif defined(CONFIG_ARM)
    __asm__ volatile ("dsb" ::: "memory");      // ARM 数据同步屏障
    __asm__ volatile ("isb" ::: "memory");      // ARM 指令同步屏障
#endif
```

#### 3. shell_uart.c - Ctrl+C 拦截 (共享代码)
```c
// 在 polling_rx_timeout_handler() 中拦截 Ctrl+C
if (c == CTRL_C) {
    pid_t fg_pgid = signal_get_foreground_pgid();
    if (fg_pgid > 0) {
        kill(fg_pgid, SIGINT);  // 发送 SIGINT 到前台进程
        continue;                // 消费 Ctrl+C，不传递到 shell
    }
}
```

### 创建的文件结构

```
zephyr/samples/ansilic/rocket_pi_shell_process/
├── CMakeLists.txt          # 构建配置
├── prj.conf                # 项目配置 (启用 POLLING 模式)
├── sample.yaml             # 测试配置
├── README.rst              # 文档
└── src/
    ├── main.c              # 主应用程序 (RocketPi 版本)
    ├── shell_process.c/h   # 进程管理
    ├── signal.c/h          # 信号子系统
    ├── commands.c          # 命令实现
    ├── debug.c/h           # 调试工具 (ARM/RISC-V 兼容)
    ├── bytecode_vm.c/h     # 字节码虚拟机
    └── anl_loader.c/h      # ANL 加载器 (ARM/RISC-V 兼容)
```

### 关键配置

```conf
# 启用 UART 轮询模式以拦截 Ctrl+C
CONFIG_SHELL_BACKEND_SERIAL_API_POLLING=y
CONFIG_SHELL_BACKEND_SERIAL_RX_POLL_PERIOD=10

# 进程支持
CONFIG_PROCESS_MODEL=y
CONFIG_MAX_PROCESS_COUNT=32
```

### 测试方法

#### 1. 烧录固件
```bash
west flash -d build-rocket-pi-anl --runner openocd
# 或使用脚本
./flash_rocket_pi_shell.sh
```

#### 2. 连接串口
```bash
picocom -b 115200 /dev/ttyUSB0
```

#### 3. 测试 Ctrl+C 功能
```
shell> loop 10
Loop started (PID 2). Running for 10 seconds...
......^C                    ← 按下 Ctrl+C
Killed
shell>
```

#### 4. 运行信号自测
```
shell> test_signal
[Signal delivery self-test output...]
```

### 可用命令

- **进程管理**: `ps`, `kill`, `fork`, `getpid`, `info`
- **信号测试**: `loop`, `sigint`, `test_signal`
- **系统信息**: `uptime`, `mem`, `free`, `version`, `date`
- **其他**: `echo`, `sleep`, `benchmark`, `stress`, `reboot`

### 技术要点

1. **轮询模式必需**: 使用 UART 轮询模式才能在定时器回调中拦截 Ctrl+C
2. **架构抽象**: 通过 `#ifdef CONFIG_ARM` / `CONFIG_RISCV` 实现跨架构支持
3. **共享代码库**: 同一份源代码支持 AS32x601 (RISC-V) 和 RocketPi (ARM)
4. **前台进程管理**: 只有前台进程接收 Ctrl+C 信号

### 与 AS32x601 版本的差异

| 特性 | AS32x601 (RISC-V) | RocketPi (ARM) |
|------|-------------------|----------------|
| 架构 | RISC-V RV32IMC | ARM Cortex-M4 |
| 帧指针 | s0 | r7 |
| 返回地址 | ra | lr |
| 内存屏障 | fence + fence.i | dsb + isb |
| 其他逻辑 | 完全相同 | 完全相同 |

### 辅助脚本

- `flash_rocket_pi_shell.sh` - 烧录脚本
- `build_rocket_pi_quick.sh` - 快速构建脚本

### 文档

- `ROCKET_PI_CTRL_C_IMPLEMENTATION.md` - 详细实现文档
- `README.rst` - 用户手册

## 验证清单

- [x] 构建成功 (无错误，仅有预期警告)
- [x] 架构兼容性代码正确实现
- [x] 配置文件正确 (POLLING 模式已启用)
- [x] 所有源文件已创建
- [x] Ctrl+C 拦截代码已验证
- [x] 文档和脚本已创建

## 下一步测试 (需要硬件)

1. 烧录固件到 RocketPi 板
2. 连接串口控制台
3. 测试 Ctrl+C 中断功能
4. 验证所有命令正常工作

## 日期
2026-08-29

## 参考
- 原始 commit: 1534e5fbdeffd32d2ff954b137aa279a24c09a1c
- AS32x601 实现文档: CTRL_C_SIGNAL_IMPLEMENTATION.md
