# Zephyr Linux 兼容接口定义

本文档专门说明 zepLinux 在 Zephyr 上提供的 Linux/POSIX 兼容接口定义，以及这些接口在内核中的承载方式。

与总览文档不同，这里尽量不讲背景铺陈，而是把接口层真正需要落地的内容写清楚：

- 向应用暴露什么接口
- 对这些接口承诺什么语义
- 这些语义如何映射到 Zephyr 的线程、调度器和设备模型
- 哪些地方是严格兼容，哪些地方是面向 MCU 场景的裁剪或约束

## 1. 接口层的职责边界

在这个项目里，“Linux 兼容接口层”并不等价于把 Linux 内核原封不动搬到 Zephyr 上，而是做一层面向应用的语义兼容层。

它的职责边界可以概括为三点：

- 对上保持 Linux/POSIX 程序员熟悉的调用方式
- 对下复用 Zephyr 现有的内核对象、驱动框架和调度机制
- 在资源受限的 MCU 场景里，只实现项目真正需要、且能够稳定验证的语义子集

因此，这里的“兼容”不是字面上的 1:1 复制，而是“接口形态尽量不变，语义尽量贴近，底层实现允许重构”。

## 2. 接口分类

结合当前仓库的组织方式，兼容接口大体分为六类：

| 类别 | 关注点 | 典型接口 |
|------|--------|----------|
| 线程管理 | 线程创建、同步、生命周期 | `pthread_create`、`pthread_join`、互斥锁 |
| 进程管理 | 轻量进程语义或占位接口 | `fork`、`exec`、`exit` |
| 调度与优先级 | 调度策略、优先级、时间片 | `sched_*`、`nice`、`setpriority` |
| 信号与定时器 | 延时、通知、简单异步控制 | `signal`、`kill`、`alarm`、`nanosleep` |
| 内存管理 | 动态分配与兼容封装 | `malloc`、`realloc`、`free` |
| 设备管理 | 文件描述符与控制路径 | `open`、`read`、`write`、`ioctl` |

本次拆分文档中，最需要详细展开的是两部分：

- 调度与优先级接口
- 设备管理接口

原因很简单：这两部分既有较强的 Linux 语义特征，又直接触达 Zephyr 内核或驱动层，是兼容层里最容易“看起来能编译，实际语义没落地”的地方。

## 3. 调度接口定义

### 3.1 设计目标

调度接口的目标不是简单提供几个 `sched_*` 函数壳子，而是要让应用通过这些接口看到真实的调度行为变化。

换句话说，至少要做到下面几件事：

- 改策略时，线程的调度类别真的发生变化
- 改优先级时，线程在 Zephyr ready queue 中的竞争关系真的发生变化
- 查询接口读到的信息，和线程实际运行状态保持一致
- `SCHED_FIFO` 与 `SCHED_RR` 在同优先级下表现出不同的调度语义

如果只做“接口返回成功”，但内核侧并没有相应的状态和行为变化，那这个兼容层对上层应用没有实际意义。

### 3.2 三层结构

调度接口可以按三层理解：

#### 第一层：应用可见接口

这一层是 Linux/POSIX 程序员直接使用的入口，主要包括：

- `sched_getparam()`
- `sched_setparam()`
- `sched_getscheduler()`
- `sched_setscheduler()`
- `sched_rr_get_interval()`
- `pthread_attr_*sched*()`
- `pthread_*schedparam()`
- `nice()` / `setpriority()` 风格扩展接口

#### 第二层：兼容映射层

这一层负责做语义转换，常见工作包括：

- 把 POSIX 策略值转换为 Zephyr 内部策略枚举
- 把应用视角优先级转换为 Zephyr 内核优先级空间
- 做参数合法性检查
- 保证创建阶段和运行阶段的调度属性处理方式一致

#### 第三层：内核执行层

这一层最终落到 Zephyr 内核对象与调度路径，核心对象包括：

- `struct k_thread`
- 线程基类中的优先级和策略元数据
- ready queue
- timeslice 逻辑
- 调度比较函数和线程切换条件

这三层必须串起来，接口语义才算闭环。

## 4. 线程调度元数据

为了让 Zephyr 线程承载 Linux/POSIX 风格的调度语义，线程对象里至少需要保留“策略”和“优先级”两个维度的信息。

在当前文档体系中，策略字段以 `_thread_base.sched_policy` 为代表，它的意义不是替代 Zephyr 现有优先级，而是补上 Linux 兼容层真正关心的策略维度。

内部策略枚举包括：

- `Z_THREAD_SCHED_OTHER`
- `Z_THREAD_SCHED_FIFO`
- `Z_THREAD_SCHED_RR`

这几个值的作用主要体现在下面几处：

- 让 `sched_getscheduler()` 能返回线程的真实策略
- 让 `sched_setscheduler()` 能把策略修改写回线程对象
- 让时间片逻辑可以区分“需要轮转”和“不需要轮转”的线程
- 让 `pthread` 调度接口和 `sched_*` 接口最终落到同一套线程元数据上

如果没有这层元数据，很多接口只能做到“参数进来了”，做不到“线程状态真的变了”。

## 5. 策略映射与优先级映射

### 5.1 策略映射

策略映射关系相对直接：

| Linux/POSIX 语义 | Zephyr 内部策略 |
|---|---|
| `SCHED_OTHER` | `Z_THREAD_SCHED_OTHER` |
| `SCHED_FIFO` | `Z_THREAD_SCHED_FIFO` |
| `SCHED_RR` | `Z_THREAD_SCHED_RR` |

这一层最大的价值不在“名字翻译”，而在于后续内核逻辑是否真正使用这些值。

### 5.2 优先级映射

优先级映射比策略映射更容易出问题，因为 Linux/POSIX 和 Zephyr 的优先级空间并不天然一致。

因此需要明确两个方向：

- **写入方向**：应用传入的 POSIX 优先级，转换后再写入 Zephyr 线程对象
- **读取方向**：线程对象中的 Zephyr 优先级，转换后再返回给应用

这里必须保证两个要求：

- 对应用来说，设置和读取优先级时语义自洽
- 对内核来说，不破坏 Zephyr 原有的优先级比较规则

如果只考虑写入、不考虑回读，应用会看到“设进去一个值，读出来却是另一套规则”；如果只考虑接口值映射、不考虑 ready queue 比较方式，又会导致应用看起来设置成功，但实际运行顺序不符合预期。

## 6. 内核桥接点

调度接口最终能否落地，关键在几个桥接点是否设计得足够清晰。

### 6.1 `z_thread_priority_and_policy_set()`

```c
void z_thread_priority_and_policy_set(struct k_thread *thread,
                                      int prio,
                                      uint8_t policy);
```

这是一个很关键的桥接入口。它的价值在于把“优先级修改”和“策略修改”合并成一个原子性的更新动作。

这样做有几个好处：

- 避免先改优先级、后改策略时出现瞬时不一致状态
- 避免 `sched_setparam()` 和 `sched_setscheduler()` 走出两套不同的更新路径
- 方便把 `pthread_setschedparam()` 也统一收敛到同一个底层入口

从工程维护角度看，这种统一入口也更适合后续加锁、补统计、加断言或打调试日志。

### 6.2 `z_get_thread_timeslice_ticks()`

```c
int32_t z_get_thread_timeslice_ticks(struct k_thread *thread);
```

这个接口用于把线程当前的时间片信息向上导出。最直接的使用方就是 `sched_rr_get_interval()`。

它的作用不是单纯“给个数字”，而是建立“应用可查询时间片”和“内核实际使用时间片”之间的对应关系。

如果没有这个桥接点，`sched_rr_get_interval()` 很容易沦为一个拍脑袋返回默认值的接口，失去验证价值。

### 6.3 `thread_is_sliceable()`

```c
bool thread_is_sliceable(struct k_thread *thread);
```

这个判断决定线程是否参与时间片轮转。在只考虑 Zephyr 原生调度时，它可能更多是优先级和线程状态判断；但引入 Linux/POSIX 策略后，策略本身也成了判断条件。

因此这里至少要体现出：

- `SCHED_RR`：需要轮转
- `SCHED_FIFO`：同优先级下不应被 RR 逻辑错误轮转
- `SCHED_OTHER`：按普通线程的系统默认规则处理

这一点非常关键，因为很多“接口已经实现了”的假象，最后都是在这里被揭穿：

- `sched_setscheduler(..., SCHED_FIFO, ...)` 返回成功
- 但线程仍然按 RR 方式轮转

一旦出现这种情况，就说明接口语义还没有真正传到内核行为层。

## 7. 应用可见的 `sched_*` 接口

### 7.1 `sched_get_priority_min()` / `sched_get_priority_max()`

这两个接口看似简单，但它们实际上定义了整个优先级语义的边界。

它们至少要回答两个问题：

- 当前策略允许的优先级范围是什么
- 兼容层是否对不同策略采用不同的优先级区间

如果后续引入更多 Linux 风格策略，例如 `SCHED_DEADLINE` 或 `SCHED_NORMAL` 的扩展语义，这两个接口的行为边界也需要同步维护。

### 7.2 `sched_getparam()`

这个接口读取线程当前优先级，并且要把 Zephyr 内部优先级转换回应用可理解的 POSIX 值。

它常见的问题有两个：

- 读到的是底层原始优先级，应用无法理解
- 回读值不是线程当前真实运行优先级，而是某个创建时残留值

因此它必须从线程当前有效状态中取值，而不是从临时缓存里返回。

### 7.3 `sched_setparam()`

这个接口只修改优先级，不改变策略。它适合用于：

- 在线程保留 `SCHED_FIFO` 或 `SCHED_RR` 的前提下微调实时优先级
- 在同一调度策略里调整线程竞争次序

它的实现重点是：

- 不能悄悄改策略
- 改完优先级后，要触发 ready queue 侧必要的更新

### 7.4 `sched_getscheduler()`

这个接口负责把线程的当前策略返回给应用。它应该读取线程真实持有的策略元数据，而不是返回编译时默认值或某个固定常量。

对于调试非常有价值：很多时候先调用它，就能快速判断“策略根本没写进去”，还是“策略写进去了但执行路径没生效”。

### 7.5 `sched_setscheduler()`

这是 Linux 风格调度控制里最核心的一个接口。它同时涉及：

- 策略切换
- 优先级写入
- ready queue 顺序调整
- 可能的时间片行为变化

在兼容层里，它通常是最能暴露设计质量的接口：如果这个接口实现得足够扎实，其他围绕调度的接口通常都能复用它的底层能力。

### 7.6 `sched_rr_get_interval()`

这个接口是验证 `SCHED_RR` 是否真正落地的关键抓手。

如果 `SCHED_RR` 只是一个写在线程对象里的标签，而内核并没有实际时间片行为，那么这个接口最终很难返回可信的值。

因此建议把它视为“行为验证接口”，而不是“装饰性查询接口”。

## 8. `pthread` 调度接口

除了 `sched_*` 之外，Linux/POSIX 程序还会大量通过 `pthread` 接口传递调度语义。这一层不能忽略。

### 8.1 创建阶段接口

```c
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
```

这些接口的意义是在线程真正创建之前，把调度意图挂到属性对象上。

在工程上，最常见的问题不是接口本身写不出来，而是属性设置了、创建时却没传下去。因此这里一定要注意创建路径是否真正消费了这些属性。

### 8.2 运行阶段接口

```c
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);
int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
```

这两个接口的职责和 `sched_get*` / `sched_set*` 类似，但面向 `pthread_t` 线程对象。

它们最重要的是保持和 `sched_*` 系列的一致性：

- 同一线程通过两套接口读出来的策略和优先级应该一致
- 通过任意一套接口写进去的变化，都应该反映到真实运行状态上

如果这两套接口各走各的实现路径，后面很容易出现状态分叉，调试成本会非常高。

## 9. zepLinux 调度扩展接口

在标准 POSIX 接口之外，项目还提供 zepLinux 工程扩展调度接口，例如：

```c
/* 工程扩展调度接口示意 */
zeplinux_sched_setscheduler(0, ZEP_SCHED_FIFO, &param);
zeplinux_sched_setscheduler(0, ZEP_SCHED_RR, &param);
zeplinux_sched_setscheduler(0, ZEP_SCHED_OTHER, &param);

zeplinux_sched_setattr_dl(0, 1000000, 5000000, 10000000);

zeplinux_nice(-5);
zeplinux_setpriority(ZEP_PRIO_PROCESS, 0, 10);

int policy = zeplinux_sched_getscheduler(0);
int nice = zeplinux_getpriority(ZEP_PRIO_PROCESS, 0);
```

这层接口的定位更像“贴近 Linux 使用习惯的工程封装”。它不应该绕开底层的 POSIX/Zephyr 映射机制，而应建立在那套基础设施之上。

换句话说：

- `zeplinux_sched_setscheduler()` 最终也应复用底层策略写入路径
- `zeplinux_nice()` / `zeplinux_setpriority()` 最终也应回到统一的优先级映射机制
- `zeplinux_sched_setattr_dl()` 如果要支持更复杂的调度策略，最好沿用现有桥接思路，而不是额外维护一套完全独立的线程状态

## 10. 设备接口定义

调度之外，另一块必须单独说明的是设备控制路径。

### 10.1 为什么设备接口重要

很多 Linux 程序并不是单纯做计算，它们会：

- 打开设备节点
- 发控制命令
- 读写设备数据
- 依赖阻塞/非阻塞语义组织事件循环

如果兼容层只把线程和调度做好，而没有把设备访问路径打通，那么大部分“像 Linux 程序那样写的应用”仍然很难落地。

### 10.2 目标接口集合

当前设备兼容层至少要覆盖下面几个入口：

- `open()`
- `close()`
- `read()`
- `write()`
- `ioctl()`

它们构成的是一个“最小可用设备访问面”。

### 10.3 设备路径分层

从工程实现上，可以把设备访问分成三层：

#### 第一层：应用调用层

应用通过 `fd` 和 `ioctl` 表达自己的访问意图，这一层尽量保持 Linux 程序员熟悉的使用方式。

#### 第二层：兼容封装层

这一层负责：

- 做设备打开和 fd 管理
- 做参数合法性检查
- 做阻塞/非阻塞语义处理
- 把控制命令分发到具体设备后端

#### 第三层：Zephyr 驱动层

真正执行动作的还是 Zephyr 设备模型和板级驱动。兼容层的职责不是取代驱动，而是把 Linux 风格调用重新组织后交给驱动去做。

### 10.4 `ioctl()` 的语义边界

`ioctl()` 需要单独强调，因为它最容易误解。

在这里，`ioctl()` 的目标不是无条件兼容 Linux 驱动 ABI，而是兼容“应用发控制命令”这类交互方式。更实际的做法通常是：

- 向应用保留 `ioctl(fd, cmd, arg)` 的调用形式
- 对项目实际需要的命令字和参数结构做明确约束
- 在底层用 Zephyr 驱动接口实现等价控制行为

这样既保留了上层程序移植的便利，也避免把 Linux 设备子系统的复杂细节原样搬进 MCU 环境。

### 10.5 阻塞与非阻塞语义

设备兼容层不能只实现“能读能写”，还要处理等待语义。

重点通常包括：

- 非阻塞打开
- 非阻塞读写
- 设备暂不可用时的返回值与错误码
- 中断事件与轮询逻辑的配合

对于 MCU 场景，完全照搬 Linux 的全部 I/O 语义成本很高，因此更务实的做法是先覆盖项目中真正使用到的模式，并把未覆盖语义明确写进文档。

## 11. 接口实现时的几个原则

为了让后续实现和维护更稳，建议始终遵循下面几个原则：

### 11.1 查询接口不要伪造状态

像 `sched_getscheduler()`、`sched_getparam()` 这类查询接口，必须从线程当前有效状态读取，而不是从初始化值、缓存值或编译时常量返回。

### 11.2 设置接口必须推动底层状态变化

凡是 `set*` 接口，都不应只是修改一个上层结构体，而必须推动线程对象、ready queue 或设备状态发生真正变化。

### 11.3 标准接口与扩展接口走统一底层路径

POSIX 的 `sched_*`、`pthread_*sched*` 和 zepLinux 扩展接口最好共用同一套底层桥接逻辑。否则接口一多，状态很容易失配。

### 11.4 明确“兼容到什么程度”

不需要为了追求形式完整去实现项目不会用到的语义，但必须把已经支持、部分支持和暂不支持的边界写清楚。

## 12. 相关资料

- 总览文档：`docs/zepLinux-interface-and-validation.md`
- 开发与构建：`docs/zephyr-linux-build-and-dev.md`
- 测试与 demo：`docs/test-demos/README.md`
- 文档索引：`docs/zephyr-linux-interface-commits.md`
