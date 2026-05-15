# GPIO 与板级演示

本文档描述在 Rocket Pi（STM32F401）平台上运行的 GPIO 回环测试和板级验证演示。

---

## 1. GPIO 回环健全性测试

**路径**：`zephyr/samples/basic/rocket_pi_pb1_loopback/`

最简单的板级验证：驱动 PB1（输出）并通过物理回环线读取 PB2（输入）。

### 目的

- 确认 GPIO 驱动功能正常
- 确认设备树别名（`loopback_out`、`loopback_in`）正确映射
- 在运行更复杂测试之前提供基本的 PASS/FAIL 门控

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/rocket_pi_pb1_loopback
west flash
minicom -D /dev/ttyUSB0 -b 115200
```

### 预期输出

```
PB1→PB2 loopback test
drive=0 sample=0 PASS
drive=1 sample=1 PASS
loopback test: ALL PASS
```

### 硬件连接

在 Rocket Pi 板上用跳线将 PB1 连接到 PB2。

---

## 2. 扩展 GPIO 互连测试

**路径**：`zephyr/samples/basic/pb1_pb2_extended/`

PB1→PB2 回环路径的扩展验证，包含多种测试模式：

| 测试 | 描述 |
|------|------|
| 单次低电平 | 驱动低电平，验证低电平 |
| 单次高电平 | 驱动高电平，验证高电平 |
| 20 次翻转 | 快速翻转，验证每个状态 |
| 保持高电平 | 持续高电平，验证稳定性 |
| 保持低电平 | 持续低电平，验证稳定性 |

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_extended
west flash
```

### 预期输出

```
PB1/PB2 extended interconnect test
single-low drive=0 sample=0 PASS
single-high drive=1 sample=1 PASS
toggle[1] drive=1 sample=1 PASS
toggle[2] drive=0 sample=0 PASS
...
toggle[20] drive=0 sample=0 PASS
hold-high drive=1 sample=1 PASS
hold-low drive=0 sample=0 PASS
RESULT: 24/24 passed, 0 failed
```

---

## 3. 综合板级基准测试

**路径**：`zephyr/samples/basic/pb1_pb2_benchmark/`

组合测试套件，按顺序运行所有板级验证：

1. **启动时间测量** — 从 `PRE_KERNEL_1` 初始化到 `main()` 入口的周期数
2. **GPIO 回环** — PB1→PB2 健全性检查
3. **IRQ 延迟** — TIM2 硬件定时器 ISR 延迟（100 个样本）
4. **调度延迟** — `k_timer` 唤醒延迟（100 个样本）
5. **上下文切换** — 乒乓线程上下文切换开销（100 个样本）

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_benchmark
west flash
```

### 预期输出

```
PB1/PB2 benchmark start
boot_to_main=12345 cyc (147 ns)
boot_to_measure=23456 cyc (279 ns)
cpu cycles per sec: 84000000
single-low drive=0 sample=0 PASS
single-high drive=1 sample=1 PASS
irq_latency_direct iterations=100 period_us=1000
irq_latency_direct min=42 cyc (500 ns) max=168 cyc (2000 ns) avg=84 cyc (1000 ns)
sched_latency iterations=100 sleep_us=1000
sched_latency min=84 cyc (1000 ns) max=336 cyc (4000 ns) avg=168 cyc (2000 ns)
context_switch iterations=100
context_switch avg=252 cyc (3000 ns)
benchmark complete: 2 gpio pass, 0 gpio fail
```

### 实现细节

- 使用 `SYS_INIT(boot_time_mark, PRE_KERNEL_1, 0)` 捕获启动时间戳
- TIM2 配置为 PSC=83（84 MHz 核心时钟下 1 MHz 计数），实现微秒级 IRQ 计时
- 上下文切换通过两个相同优先级线程的信号量乒乓测量

---

## 4. 计划中的设备控制演示

### 4.1 通过 Linux 风格接口控制 GPIO

**计划路径**：`tests/demo/device/gpio_control/`

演示 Linux 兼容设备控制路径：

```c
int fd = open("/dev/gpio0", O_RDWR);
ioctl(fd, GPIO_CONFIG_OUTPUT, 5);  // 配置引脚 5 为输出
ioctl(fd, GPIO_SET_PIN, 5);        // 设置引脚 5 为高电平
ioctl(fd, GPIO_CLEAR_PIN, 5);      // 设置引脚 5 为低电平
close(fd);
```

### 4.2 通过 Linux 风格接口控制 UART

**计划路径**：`tests/demo/device/uart_control/`

演示通过 fd/ioctl 配置 UART：

```c
int fd = open("/dev/uart0", O_RDWR);
struct uart_config cfg = {
    .baudrate = 115200,
    .parity = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
};
ioctl(fd, UART_SET_CONFIG, &cfg);
write(fd, "Hello from zepLinux\n", 20);
close(fd);
```

### 4.3 非阻塞 I/O

**计划路径**：`tests/demo/device/nonblock_io/`

演示 `O_NONBLOCK` 语义：

```c
int fd = open("/dev/uart0", O_RDWR | O_NONBLOCK);
char buf[64];
ssize_t n = read(fd, buf, sizeof(buf));
// 无数据可用时返回 -1，errno=EAGAIN
```
