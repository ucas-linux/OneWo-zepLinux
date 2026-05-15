# IRQ 延迟基准测试

本文档描述用于表征 zepLinux 平台实时性能的中断延迟测量程序。

---

## 1. 概述

IRQ 延迟是从硬件事件发生到 ISR 开始执行的时间。这是实时系统的关键指标，直接影响 `SCHED_FIFO` / `SCHED_RR` 保证的可行性。

本项目提供多种复杂度递增的测量方法：

| 样例 | 方法 | 采样数 |
|------|------|--------|
| `irq_latency_isr/` | 通过 counter 驱动的单次 TIM2 ISR | 1 |
| `irq_latency_isr_100/` | 通过 counter 驱动的 TIM2 ISR | 100 |
| `irq_latency_cyclictest/` | k_timer 唤醒（cyclictest 风格） | 10000 |
| `irq_latency_cyclictest_once/` | k_timer 唤醒 | 1 |
| `irq_latency_tim2_direct/` | 直接 TIM2 寄存器访问 | N |
| `irq_latency_pb1_once/` | PB1 回环 + TIM2 ISR | 1 |
| `irq_latency_pb1_suite/` | PB1 回环 + ISR + k_timer | 20+20 |

---

## 2. Cyclictest 风格延迟测试

**路径**：`zephyr/samples/basic/irq_latency_cyclictest/`

模拟 Linux `cyclictest` 工具：测量预期定时器到期与实际线程唤醒之间的延迟。

### 方法

1. 记录当前周期计数
2. 启动已知周期（1000 us）的 `k_timer`
3. 在信号量上阻塞直到定时器触发
4. 测量实际唤醒周期与预期周期的差值
5. 计算差值作为唤醒延迟

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/irq_latency_cyclictest
west flash
```

### 预期输出

```
cyclictest-style IRQ wakeup latency test
cpu cycles per sec: 84000000
test period: 1000 us, period ticks: 10
iterations: 10000
[100] cur=89 cyc (1059 ns) min=42 max=168 avg=84 tick_jitter=0
[200] cur=84 cyc (1000 ns) min=42 max=168 avg=82 tick_jitter=0
...
[10000] cur=86 cyc (1023 ns) min=38 max=210 avg=84 tick_jitter=0
```

### 关键指标

- **min**：最佳唤醒延迟（系统开销下限）
- **max**：最差唤醒延迟（反映其他 ISR 或调度器的抖动）
- **avg**：典型唤醒延迟
- **tick_jitter**：tick 计数器漂移（应为 0 或接近 0）

---

## 3. 直接 TIM2 ISR 延迟

**路径**：`zephyr/samples/basic/irq_latency_tim2_direct/`

绕过 Zephyr counter 驱动，直接编程 TIM2 寄存器以实现最小开销测量。

### 方法

1. 配置 TIM2：PSC=83（84 MHz 下 1 us 计数），ARR=周期
2. 通过 `IRQ_CONNECT(TIM2_IRQn, ...)` 直接连接 ISR
3. 在 ISR 中：立即读取 `k_cycle_get_32()`
4. 在 main 中：比较 ISR 时间戳与预期触发时间

### 为什么使用直接访问？

Zephyr counter 驱动增加了抽象开销。直接寄存器访问给出不含驱动栈贡献的真实硬件 IRQ 延迟。

---

## 4. PB1 回环 + IRQ 套件

**路径**：`zephyr/samples/basic/irq_latency_pb1_suite/`

在单次测试运行中组合 GPIO 验证与延迟测量：

1. PB1→PB2 回环检查（健全性门控）
2. 20 个 TIM2 ISR 延迟样本
3. 20 个 k_timer 唤醒延迟样本

### 构建与运行

```bash
west build -b rocket_pi/stm32f401xe zephyr/samples/basic/irq_latency_pb1_suite
west flash
```

### 预期输出

```
PB1/PB2 suite start
loopback drive=0 sample=0 PASS
loopback drive=1 sample=1 PASS
irq_latency iterations=20 period_us=1000
irq_latency min=40 cyc (476 ns) max=160 cyc (1904 ns) avg=82 cyc (976 ns)
sched_latency iterations=20 sleep_us=1000
sched_latency min=80 cyc (952 ns) max=320 cyc (3809 ns) avg=160 cyc (1904 ns)
suite complete
```

---

## 5. 结果解读

### Rocket Pi（STM32F401，84 MHz）上的典型值

| 指标 | 典型范围 | 说明 |
|------|----------|------|
| TIM2 ISR 延迟 | 400–2000 ns | 硬件中断入口 |
| k_timer 唤醒延迟 | 1000–4000 ns | 包含调度器开销 |
| Tick 抖动 | 0–1 tick | 应该很小 |

### 影响延迟的因素

- **中断优先级**：更高优先级的 ISR 延迟更低
- **调度器锁**：持有锁会延迟线程唤醒（不影响 ISR 入口）
- **其他 ISR**：嵌套或挂起的 ISR 增加尾链延迟
- **Flash 等待状态**：STM32F401 在 84 MHz 下使用 2 个等待状态

### 与 Linux 调度的关系

- `SCHED_FIFO` 线程依赖可预测的唤醒延迟
- `SCHED_RR` 时间片精度依赖 tick 抖动
- IRQ 延迟设定了所有实时保证的下限
