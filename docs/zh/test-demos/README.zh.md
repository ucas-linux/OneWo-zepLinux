# 测试与演示文档

本目录记录用于验证 zepLinux 兼容层的测试程序和演示样例。

## 目录结构

| 文档 | 内容 |
|------|------|
| [scheduling-tests.zh.md](scheduling-tests.zh.md) | Linux 风格调度接口验证 |
| [gpio-board-demos.zh.md](gpio-board-demos.zh.md) | GPIO 回环与板级验证演示 |
| [irq-latency-benchmarks.zh.md](irq-latency-benchmarks.zh.md) | IRQ 与调度延迟测量 |
| [performance-benchmarks.zh.md](performance-benchmarks.zh.md) | 上下文切换、启动时间与吞吐量基准 |

## 测试分类

### 1. 内核调度测试

位于 `zephyr/tests/kernel/sched/`：

- `schedule_api/` — 优先级调度、时间片、抢占
- `deadline/` — 截止期限调度
- `preempt/` — 抢占行为
- `metairq/` — Meta-IRQ 调度

### 2. 板级样例应用

位于 `zephyr/samples/basic/`：

- `rocket_pi_pb1_loopback/` — GPIO 回环健全性检查
- `pb1_pb2_benchmark/` — 综合板级基准测试套件
- `pb1_pb2_extended/` — 扩展 GPIO 互连测试
- `irq_latency_*` — 多种 IRQ 延迟测量方法

### 3. 计划中的兼容层测试

位于 `tests/`（计划中）：

- `tests/unit/` — 单接口单元测试
- `tests/integration/` — 多接口集成测试
- `tests/demo/scheduler/` — 调度行为演示
- `tests/demo/device/` — 设备控制演示
- `tests/demo/perf/` — 性能基准

## 目标板

主要验证平台：**rocket_pi/stm32f401xe**

- MCU：STM32F401（ARM Cortex-M4，84 MHz）
- RAM：96 KB
- Flash：512 KB
- 关键外设：GPIO（PB1/PB2 回环）、UART、TIM2

## 构建与运行

```bash
# 构建测试
west build -b rocket_pi/stm32f401xe <test_path>

# 烧录到板子
west flash

# 查看串口输出
minicom -D /dev/ttyUSB0 -b 115200
```
