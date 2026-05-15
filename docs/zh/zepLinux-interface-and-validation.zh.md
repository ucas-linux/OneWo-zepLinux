# zepLinux 接口与验证体系

OneWo zepLinux 在 Zephyr 内核之上实现 Linux 兼容接口层，本文说明以下内容：

- Linux/POSIX 调度接口如何映射到 Zephyr 线程与调度器
- Linux 风格设备控制路径如何通过 fd、`ioctl` 与非阻塞语义落到板级样例
- 整个兼容层如何通过调度测试、GPIO/IRQ demo 与性能基准完成验证

---

## 1. 总体说明

目标是在 Zephyr RTOS 上提供 Linux 兼容接口层，使 Linux 应用能够以较小改动运行在 MCU 环境中。

接口设计涉及三项约束：

- **语义上可对齐**：应用看到的行为尽可能接近 Linux / POSIX
- **内核中可落地**：最终映射到 Zephyr 的线程、优先级、时间片、设备或同步原语
- **工程上可验证**：必须有测试、demo 或性能数据来证明它不仅“存在”，而且“可用”

当前包括两类能力：

- **调度与优先级接口**
- **设备控制与板级验证路径**

这两类能力分别对应 `README.md` 中的：

- Linux 风格调度器与 `k_thread` 集成
- Linux 兼容接口分类中的调度/设备管理部分
- 构建与测试体系

---

## 2. 调度接口

调度兼容层负责把 Linux/POSIX 的“策略 + 优先级 + 时间片”模型映射到 Zephyr 线程对象与内核调度路径中。

### 2.1 调度模型分层

这一层分为三层：

1. **应用接口层**
   - `sched_*`
   - `pthread_*sched*`
   - LMZ 的 `lmz_sched_*` / `lmz_nice()` / `lmz_setpriority()` 等接口
2. **兼容映射层**
   - POSIX policy/prio 与 Zephyr 内部 policy/prio 的双向转换
   - 线程属性与运行期策略更新
3. **Zephyr 内核执行层**
   - `struct k_thread`
   - 内核 ready queue / timeslice / priority comparison
   - RR / FIFO / OTHER 等语义在具体调度行为上的体现

对应链路为：用户态语义 → 内核实体 → 测试验证。

### 2.2 线程调度元数据

为了让 Zephyr 线程承载 Linux/POSIX 调度语义，线程基类中引入了调度策略字段：

- `_thread_base.sched_policy`

内部策略枚举为：

- `Z_THREAD_SCHED_OTHER`
- `Z_THREAD_SCHED_FIFO`
- `Z_THREAD_SCHED_RR`

每个线程除 Zephyr 优先级外，还带有兼容层使用的策略标签，用于：

- 支撑 `sched_getscheduler()` / `sched_setscheduler()`
- 支撑 `pthread_getschedparam()` / `pthread_setschedparam()`
- 让 RR 时间片与 FIFO 非轮转行为能够在内核中区分开来

### 2.3 调度策略映射

在兼容层中，策略映射关系如下：

| Linux/POSIX 语义 | Zephyr 内部策略 | 说明 |
|---|---|---|
| `SCHED_OTHER` | `Z_THREAD_SCHED_OTHER` | 普通线程策略 |
| `SCHED_FIFO` | `Z_THREAD_SCHED_FIFO` | 实时 FIFO |
| `SCHED_RR` | `Z_THREAD_SCHED_RR` | 实时 RR |

除策略名映射外，还需要处理 **优先级空间映射**。

POSIX 优先级值不会直接交给 Zephyr，而是经过如下转换：

- 设置时：POSIX 优先级 → Zephyr 内部优先级
- 查询时：Zephyr 内部优先级 → POSIX 优先级

对应效果：

- 对 Linux/POSIX 应用保持熟悉的优先级接口
- 对 Zephyr 内核保持自身的优先级表示与调度不变量

### 2.4 内核桥接点

调度兼容层依赖以下内核桥接点。

#### `z_thread_priority_and_policy_set()`

```c
void z_thread_priority_and_policy_set(struct k_thread *thread,
                                      int prio,
                                      uint8_t policy);
```

这是策略与优先级联动更新的入口，负责：

- 同时更新线程优先级与策略
- 避免策略和优先级分开写入造成状态不一致
- 为 `sched_setparam()`、`sched_setscheduler()`、`pthread_setschedparam()` 提供统一更新路径

#### `z_get_thread_timeslice_ticks()`

```c
int32_t z_get_thread_timeslice_ticks(struct k_thread *thread);
```

它用于读出线程时间片，并通过 `sched_rr_get_interval()` 向上暴露给应用。

#### `thread_is_sliceable()`

```c
bool thread_is_sliceable(struct k_thread *thread);
```

该判断决定线程是否参与时间片轮转。引入 Linux/POSIX 策略之后，还必须考虑策略本身：

- `SCHED_RR`：需要轮转
- `SCHED_FIFO`：同优先级下不应被 RR 机制误轮转
- `SCHED_OTHER`：按普通线程策略参与系统默认行为

### 2.5 应用可见的调度接口

当前接口如下。

#### `sched_get_priority_min(int policy)` / `sched_get_priority_max(int policy)`

用于查询某一策略下的优先级范围，是所有实时调度接口的基础约束。

#### `sched_getparam(pid_t pid, struct sched_param *param)`

用于读取线程当前优先级。该接口会把 Zephyr 内部优先级重新映射成 POSIX 语义优先级。

#### `sched_setparam(pid_t pid, const struct sched_param *param)`

用于在保持策略不变的前提下调整优先级。

#### `sched_getscheduler(pid_t pid)`

用于读取当前策略，返回 `SCHED_OTHER`、`SCHED_FIFO` 或 `SCHED_RR`。

#### `sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)`

用于同时设置策略与优先级，是 Linux 风格实时调度控制的核心接口。

#### `sched_rr_get_interval(pid_t pid, struct timespec *interval)`

用于读取 RR 时间片，是验证 RR 语义是否真正落地的重要标准接口。

### 2.6 `pthread` 调度接口

调度语义同时体现在进程/线程级 `sched_*` 接口与 `pthread` 的创建属性、运行期控制中。

#### 线程属性接口

```c
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
```

用于在线程创建前声明调度意图。

#### 运行期接口

```c
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);
int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
```

用于在线程创建后观察和修改调度状态。

两组接口合起来覆盖 Linux/POSIX 应用常见的两种使用模式：

- 在 `pthread_create()` 前设置线程属性
- 在线程运行过程中动态切换策略与优先级

### 2.7 与 LMZ 调度接口的关系

`README.md` 中给出的 LMZ 接口，例如：

- `lmz_sched_setscheduler()`
- `lmz_sched_getscheduler()`
- `lmz_nice()`
- `lmz_setpriority()`
- `lmz_getpriority()`

这些接口的稳定落地依赖“POSIX 接口 → Zephyr 线程策略/优先级 → 内核调度行为”这套基础设施。

LMZ 层建立在这套底层调度兼容机制之上，继续提供更贴近 Linux 程序员习惯的封装入口。

---

## 3. 设备控制路径

另一部分内容是 Linux 风格设备控制路径，涉及：

- fd 驱动的设备使用方式可走通
- 非阻塞语义可表达
- 板级样例能把接口、驱动与硬件验证串起来

### 3.1 `ioctl` 的位置

`ioctl` 用于设备控制命令分发，主要体现在：

- 很多 Linux 应用并不只做 `read` / `write`
- 更复杂的设备交互通常依赖 `ioctl` 进行模式切换、状态获取或控制命令下发

因此，设备兼容层不仅要支持数据通路，也要支持控制通路。

### 3.2 非阻塞控制语义

Linux 风格 I/O 的关键能力之一为：

- `fcntl(fd, F_SETFL, O_NONBLOCK)`

它允许应用把 fd 切换为非阻塞模式。其原因包括：

- 事件驱动程序广泛依赖非阻塞 fd
- 用户态轮询/多路复用逻辑常以内核是否返回“立即可用/稍后再试”为前提

非阻塞语义属于设备兼容路径的一部分：

- fd 可被打开
- fd 可被配置
- fd 在阻塞/非阻塞模式下表现出不同的等待语义

### 3.3 设备路径的工程化表达

设备控制路径通过轻量 fd 封装与板级 demo 体现。典型路径包括：

- `open`
- `close`
- `read`
- `write`
- `ioctl`
- `fcntl(..., F_SETFL, O_NONBLOCK)`

这些能力与 `README.md` 中“设备管理接口”一节是直接对齐的。

---

## 4. Rocket Pi 验证平台

为了让兼容层可测，需要一个能够承载 GPIO、IRQ、fd 与时延测试的具体平台。Rocket Pi 在这里承担验证平台角色。

### 4.1 为什么需要板级支撑

没有具体板级支持时，接口兼容层通常只能停留在：

- 编译通过
- 单元测试通过
- 接口原型存在

Rocket Pi 使以下问题可以直接检查：

- GPIO 连接是否真的可测
- 中断路径是否真的可测
- 设备控制与非阻塞路径是否真的可走
- 性能变化是否真的可量化

### 4.2 Rocket Pi 验证范围

围绕 Rocket Pi，当前验证内容包括：

- GPIO loopback
- PB1/PB2 连线测试
- 单次与批量 IRQ latency 测试
- cyclictest 风格周期唤醒延迟测试
- 带 fd/非阻塞语义的设备样例

Rocket Pi 承担接口层落地验证的平台角色。

---

## 5. 测试与 Demo

测试与 demo 分为三类：

- 调度语义验证
- 板级 I/O / IRQ 验证
- 性能基准验证

### 5.1 调度语义验证

`tests/posix/xsi_realtime` 用于验证接口是否具备 POSIX 调度语义，而不只是函数名存在。

它重点覆盖：

- `pthread_attr_setschedpolicy()`
- `pthread_attr_setschedparam()`
- `pthread_setschedparam()`
- `pthread_getschedparam()`
- `sched_getscheduler()`
- `sched_getparam()`
- `sched_setscheduler()`
- `sched_rr_get_interval()`

这组测试主要验证三件事：

- 策略值能否正确写入和读回
- 优先级是否在合法范围内转换与生效
- RR 时间片是否可被观测到

### 5.2 IRQ 延迟与 cyclictest 风格样例

这类样例用于观察兼容层和内核调度扩展后的时间敏感路径表现。

#### `irq_latency_cyclictest`

用于周期性定时唤醒延迟测量，风格上接近 Linux `cyclictest`。关注指标通常包括：

- 最小延迟
- 最大延迟
- 平均延迟
- 迭代次数

#### `irq_latency_cyclictest_once`

用于一次性快速测量，适合做最小化冒烟测试。

#### `irq_latency_isr`

用于观察 ISR 响应链路的基本时延。

#### `irq_latency_isr_100`

用于多次 ISR 采样，形成更稳定的统计结果。

#### `irq_latency_tim2_direct`

更贴近底层定时器路径，适合分析硬件定时器直接触发场景下的响应特征。

### 5.3 GPIO 与板级连线验证

这类样例把设备/中断语义落实到具体引脚与物理连接上。

#### `rocket_pi_pb1_loopback`

最基础的 GPIO 回环验证：

- `PB1` 输出
- `PB2` 输入
- 判断采样结果是否与输出一致

这是板级 bring-up 和最小设备路径验证的起点。

#### `pb1_pb2_extended`

在基础 loopback 之上，扩展为：

- 单次低电平检测
- 单次高电平检测
- 多轮 toggle
- 持续高电平稳定性检测
- 持续低电平稳定性检测

它验证的不仅是“能连通”，而是“在一系列操作模式下仍能稳定连通”。

#### `irq_latency_pb1_once`

这是一个连接多条路径的样例：

- 既涉及 GPIO 输入事件
- 又涉及 fd 语义
- 还涉及单次延迟测量
- 还能体现 `fcntl(F_SETFL, O_NONBLOCK)` 这样的 Linux 风格设备控制路径

它把接口、设备模型与板级行为三者连在了一起。

#### `irq_latency_pb1_suite`

这是更偏完整回归的测试集合，适合做板级 bring-up 之后的持续验证。

### 5.4 benchmark 与综合样例

#### `pb1_pb2_benchmark`

用于对 PB1/PB2 路径做更系统的 benchmark，适合记录 GPIO 翻转、采样、中断链路等性能表现。

#### `pb1_pb2_extended`

除了功能验证，还可以作为稳定性和重复性测试样例，用于发现边界问题和偶发失败。

---

## 6. 性能结果

接口兼容层不仅要功能可用，也要控制系统代价。性能基准因此被纳入主文档。

### 6.1 实测数据来源

当前已有两份实测数据：

- 原生 Zephyr：`/tmp/latency.txt`
- zepLinux：`/tmp/zep.txt`

两份数据覆盖线程切换、ISR 返回、线程生命周期、FIFO/LIFO、events、semaphore、condvar、heap 分配释放等内核基础操作。

### 6.2 原生 Zephyr 与 zepLinux 对比

| 指标 | 原生 Zephyr(ns) | zepLinux(ns) | 差值(ns) | 变化 |
|---|---:|---:|---:|---:|
| `thread.yield.preemptive.ctx.k_to_k` | 2439 | 2621 | +182 | +7.46% |
| `thread.yield.cooperative.ctx.k_to_k` | 2439 | 2616 | +177 | +7.26% |
| `isr.resume.interrupted.thread.kernel` | 2451 | 2499 | +48 | +1.96% |
| `isr.resume.different.thread.kernel` | 3269 | 3312 | +43 | +1.32% |
| `thread.create.kernel.from.kernel` | 3509 | 3775 | +266 | +7.58% |
| `thread.start.kernel.from.kernel` | 4230 | 4258 | +28 | +0.66% |
| `thread.resume.kernel.from.kernel` | 3005 | 3044 | +39 | +1.30% |
| `fifo.put.wake+ctx.k_to_k` | 5433 | 5460 | +27 | +0.50% |
| `events.post.wake+ctx.k_to_k` | 7349 | 7355 | +6 | +0.08% |
| `semaphore.take.blocking.k_to_k` | 4231 | 4228 | -3 | -0.07% |
| `condvar.signal.wake+ctx.k_to_k` | 5969 | 5941 | -28 | -0.47% |
| `heap.malloc.immediate` | 3981 | 3934 | -47 | -1.18% |
| `heap.free.immediate` | 3858 | 3859 | +1 | +0.03% |

### 6.3 结果解读

这组数据表明，兼容层开销主要集中在调度语义扩展附近。

#### 主要增加的路径

- `k_yield` 触发的线程切换
- 线程创建
- 部分线程恢复/切换相关操作

这些路径增幅大致在 `1% ~ 8%` 左右，主要来自调度策略元数据和调度桥接逻辑。

#### 基本稳定的路径

- ISR 返回
- FIFO / events
- semaphore / condvar
- heap 分配与释放

这些路径整体与原生 Zephyr 接近，没有出现大面积退化。

### 6.4 对项目的意义

对应目标为：

- 在引入 Linux/POSIX 兼容语义后
- 让成本尽量集中在真正需要额外语义的信息路径上
- 同时保持 Zephyr 大部分底层机制的原始效率

从当前结果看，基本符合预期：

- **调度语义增强有代价，但代价集中且可解释**
- **同步原语和基础内存操作没有被大面积拖慢**

---

## 7. 接口、Demo 与结果

工程关系如下：

1. **接口定义**
   - `sched_*`
   - `pthread_*sched*`
   - `ioctl` / 非阻塞 fd 语义
2. **内核/设备实现路径**
   - `k_thread` 调度策略字段
   - policy/prio 映射
   - timeslice / ready queue / 设备控制路径
3. **验证手段**
   - POSIX realtime 测试
   - Rocket Pi GPIO/IRQ demo
   - latency benchmark
4. **结果反馈**
   - 行为正确性
   - 板级可运行性
   - 性能代价是否可接受

接口、样例和性能在工程上相互对应。

---

## 8. 与仓库现有内容的关系

这份文档与现有仓库结构的对应关系如下：

- `README.md`
  - 提供总目标、接口分类、Zephyr 调度整体方向
- `code/include/lmz_sched.h`
  - 提供 LMZ 层对 Linux 风格调度接口的封装定义
- `human/api-reference.md`
  - 适合承接更细粒度的接口级说明
- `human/testing.md`
  - 适合承接更具体的测试步骤、板级连接方式与运行方法
- `docs/linux-sched-kernel-integration.md`
  - 适合承接更细的内核调度集成细节

对应关系如下：

- 为什么要有这些接口
- 这些接口如何映射到 Zephyr
- 为什么需要这些 demo
- 性能结果说明了什么

---

## 9. 小结

OneWo zepLinux 的核心，是建立一套把**接口语义、内核落地、板级验证和性能评估**连起来的 Linux 兼容体系。

围绕这一思路：

- 调度接口体系负责把 Linux/POSIX 线程策略与优先级语义落到 Zephyr 线程模型中
- 设备控制路径负责把 fd、`ioctl` 与非阻塞语义落到真实板级场景中
- Rocket Pi 与各类 demo/benchmark 负责把接口从“声明存在”推进到“可验证、可测量、可比较”

接口文档、测试样例和性能结果对应同一套系统的不同层面。
