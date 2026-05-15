# 性能基准测试

本文档描述 zepLinux 兼容层的性能测量测试，涵盖上下文切换开销、启动时间和系统资源使用。

---

## 1. 上下文切换开销

**路径**：`zephyr/samples/basic/pb1_pb2_benchmark/`（上下文切换部分）

### 方法

两个相同优先级（`K_PRIO_PREEMPT(4)`）的线程通过信号量乒乓：

```
ping_thread:                    pong_thread:
  take(ping_sem)                  take(pong_sem)
  end_cycle = now()               start_cycle = now()
  sum += (end - start)            give(ping_sem)
  give(pong_sem)
```

每次信号量 give/take 对强制一次上下文切换。周期计数差值测量切换开销。

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_benchmark
west flash
```

### 预期输出

```
context_switch iterations=100
context_switch avg=252 cyc (3000 ns)
```

### 典型值

| 平台 | 上下文切换 | 说明 |
|------|-----------|------|
| STM32F401 @ 84 MHz | 2–4 us | 不含 FPU 上下文 |
| STM32F401 @ 84 MHz | 4–8 us | 含 FPU 上下文保存 |

---

## 2. 启动时间测量

**路径**：`zephyr/samples/basic/pb1_pb2_benchmark/`（启动时间部分）

### 方法

使用 `SYS_INIT` 在 `PRE_KERNEL_1` 优先级 0 捕获最早可能的周期计数：

```c
static int boot_time_mark(void)
{
    boot_mark_cycle = k_cycle_get_32();
    return 0;
}
SYS_INIT(boot_time_mark, PRE_KERNEL_1, 0);
```

然后在 `main()` 中：
```c
main_entry_cycle = k_cycle_get_32();
uint32_t boot_to_main = main_entry_cycle - boot_mark_cycle;
```

### 预期输出

```
boot_to_main=12345 cyc (147000 ns)
boot_to_measure=23456 cyc (279000 ns)
```

### 测量内容

- **boot_to_main**：从最早内核初始化到应用入口的时间
- **boot_to_measure**：从最早内核初始化到第一个测量点的时间

---

## 3. 镜像大小

`pb1_pb2_benchmark` 样例通过构建输出隐式报告二进制大小：

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/pb1_pb2_benchmark
# 检查构建输出的内存使用：
cat build/zephyr/zephyr.map | grep "Total"
# 或使用：
arm-zephyr-eabi-size build/zephyr/zephyr.elf
```

### 典型占用

| 组件 | Flash | RAM | 说明 |
|------|-------|-----|------|
| 最小 Zephyr | ~20 KB | ~4 KB | 内核 + 基本驱动 |
| + GPIO 驱动 | +2 KB | +0.5 KB | GPIO 子系统 |
| + Linux 调度层 | +4 KB | +1 KB | `CONFIG_SCHED_LINUX=y` |
| + FD 表 | +1 KB | +0.5 KB | 文件描述符抽象 |

---

## 4. 调度延迟

**路径**：`zephyr/samples/basic/pb1_pb2_benchmark/`（调度延迟部分）

### 方法

测量从 `k_timer` 到期回调到线程唤醒的延迟：

1. 记录预期唤醒周期
2. 启动已知周期的 `k_timer`
3. 定时器回调记录实际触发周期并释放信号量
4. 主线程获取信号量，计算差值

### 预期输出

```
sched_latency iterations=100 sleep_us=1000
sched_latency min=84 cyc (1000 ns) max=336 cyc (4000 ns) avg=168 cyc (2000 ns)
```

### 与 IRQ 延迟的区别

- **IRQ 延迟**：从硬件事件到 ISR 入口的时间（硬件路径）
- **调度延迟**：从 ISR（定时器回调）到线程恢复的时间（软件路径）

调度延迟包括：ISR 退出 → 调度器决策 → 上下文恢复 → 线程恢复。

---

## 5. 计划中的基准测试

### 5.1 UART 吞吐量

**计划路径**：`tests/demo/perf/uart_throughput/`

测量持续数据传输速率：
- 发送已知大小的缓冲区
- 测量实际耗时
- 报告字节/秒

### 5.2 GPIO 翻转频率

**计划路径**：`tests/demo/perf/gpio_toggle/`

测量最大 GPIO 翻转速率：
- 紧密循环翻转 GPIO 引脚
- 用示波器或周期计数器测量
- 报告翻转频率（kHz/MHz）

### 5.3 内存分配开销

**计划路径**：`tests/demo/perf/malloc_bench/`

测量 `malloc`/`free` 周期时间：
- 分配和释放各种大小的块
- 报告每次分配/释放的周期数
- 与 Zephyr 原生 `k_malloc` 对比

### 5.4 兼容层开销

**计划路径**：`tests/demo/perf/syscall_overhead/`

测量 Linux 兼容层的代价：
- 直接 Zephyr API 调用 vs 等效 Linux/POSIX 调用
- 报告每次调用的开销周期数
- 目标：调度接口开销 < 10%
