# 性能基准

本文件说明性能测试资料如何映射到仓库目录，并给出后续建议的整理方式。

## 1. 对应目录

建议把性能相关内容拆成两部分：

```text
tests/stress/
├── test_stress.c
└── prj.conf

tests/demo/perf/
├── README.md
├── sched-latency-benchmark.md
├── rr-jitter-benchmark.md
└── device-latency-benchmark.md
```

其中：

- `tests/stress/` 适合承载可自动运行的压力测试
- `tests/demo/perf/` 适合记录性能测试方法、观察点和结果整理方式

## 2. 已有基础

当前仓库已存在：

- `tests/stress/test_stress.c`

它已经覆盖了一些很有价值的基础压力场景：

- 最大线程创建
- 内存分配压力
- 互斥锁竞争
- 简单碎片化模式

这意味着性能和压力测试并不是从零开始，可以先以这个文件为基础继续扩展。

## 3. 建议增加的性能专题

### 3.1 调度延迟基准

建议文件：`tests/demo/perf/sched-latency-benchmark.md`

建议记录：

- 线程切换观察方法
- 不同策略下的响应时间
- 高负载场景下的延迟变化

### 3.2 RR 时间片抖动基准

建议文件：`tests/demo/perf/rr-jitter-benchmark.md`

建议记录：

- 配置的时间片值
- 实际观察到的轮转节奏
- 抖动是否在可接受范围内

### 3.3 设备控制路径延迟

建议文件：`tests/demo/perf/device-latency-benchmark.md`

建议记录：

- `ioctl` 或设备访问调用开销
- 中断到用户态可见事件的延迟
- 非阻塞重试模式下的平均等待时间

## 4. 现有压力测试的建议定位

文件：`tests/stress/test_stress.c`

它更适合作为“资源与并发压力”验证入口，而不是完整的性能分析报告。因此建议：

- 自动化压力回归继续放在 `tests/stress/`
- 人工分析和结果沉淀放在 `tests/demo/perf/`

这样可以把“测”和“记”分开，结构更清晰。
