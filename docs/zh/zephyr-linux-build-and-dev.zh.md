# Zephyr Linux 编译与开发方法

本文档面向日常开发，主要回答下面这些实际问题：

- 代码分别放在什么位置
- 修改 Linux 兼容层时通常要动哪些文件
- 如何构建、运行、调试现有测试
- 在本仓库和外部 Zephyr 工作区之间如何协同
- 遇到构建问题、语义问题、联调问题时应先看哪里

它不是 Zephyr 官方环境安装手册的替代品，而是结合这个项目现有结构整理的一份工程说明。

## 1. 仓库结构与开发入口

### 1.1 本仓库承担的角色

从当前目录结构看，这个仓库既包含文档，也保留了一份 Zephyr 源码副本，并且放了 Linux 兼容层本身的头文件与实现草稿。

顶层关键目录如下：

```text
OneWo-Linux-Micro/
├── code/
├── docs/
├── ai/
├── human/
├── tests/
└── zephyr/
```

可以把它理解成“设计资料 + 兼容层代码 + 内核改动承载体”的组合仓库。

### 1.2 `code/` 目录

`code/` 目录主要放兼容层自身的接口定义和实现，通常是上层 API 的第一落点。

例如：

- `code/include/lmz_sched.h`
- `code/include/lmz_thread.h`
- `code/include/lmz_process.h`
- `code/include/lmz_memory.h`
- `code/include/lmz_signal.h`
- `code/include/lmz_device.h`

如果是补接口、改函数签名、补参数结构、补头文件说明，通常先从这里入手。

### 1.3 `zephyr/` 目录

`zephyr/` 目录是仓库内置的 Zephyr 内核源码副本。和 Linux 兼容层真正强相关的改动，一般都在这里完成。

当前 README 已经给出一个比较明确的子目录：

```text
zephyr/kernel/linux_sched/
```

如果你要改调度类实现、ready queue 行为、时间片逻辑或调度比较函数，这里通常是主战场。

### 1.4 `docs/` 目录

`docs/` 目录现在已经按用途拆分：

- `docs/zepLinux-interface-and-validation.md`：总览与验证体系
- `docs/zephyr-linux-interface-definition.md`：接口定义
- `docs/zephyr-linux-build-and-dev.md`：开发与构建方法
- `docs/test-demos/`：测试与 demo 文档

### 1.5 `tests/demo/` 目录

为了让测试文档和仓库结构对应起来，新增了 `tests/demo/` 目录，按主题拆分为：

- `tests/demo/scheduler/`
- `tests/demo/device/`
- `tests/demo/board/`
- `tests/demo/perf/`

这个目录当前先放 README 和说明材料，后续可以继续加入真正可编译的 demo 代码。

开发过程中建议同步维护文档，而不是等接口写完再一次性补。

## 2. 需要重点关注的源码位置

根据 README 和现有文档，当前几个关键改动点如下。

### 2.1 线程结构与元数据

- `zephyr/include/zephyr/kernel/thread.h`

这里通常涉及线程基础元数据。如果要给线程补调度策略字段、兼容层状态字段，这个文件往往会被修改。

### 2.2 ready queue 与内核结构

- `zephyr/include/zephyr/kernel_structs.h`
- `zephyr/kernel/include/priority_q.h`

前者更偏数据结构，后者更偏调度比较逻辑和优先级队列实现。涉及 Linux 风格调度器与 Zephyr 原生调度器并存时，这两个位置通常要配合改。

### 2.3 调度器实现与 tick 逻辑

- `zephyr/kernel/thread.c`
- `zephyr/kernel/timeslicing.c`
- `zephyr/kernel/linux_sched/`

这些地方共同决定：

- 新线程如何初始化默认调度类
- RR 时间片何时递减、何时轮转
- Linux 风格调度类怎样参与选线程逻辑

### 2.4 构建入口

- `zephyr/kernel/CMakeLists.txt`
- `zephyr/kernel/Kconfig`

很多“代码写好了但构建不过”的问题，最后都落在这里，例如：

- 新增源文件没有被纳入构建
- 新 Kconfig 选项没有正确声明或依赖关系不完整
- `prj.conf` 打开了选项，但构建系统里没有对应处理

## 3. 外部工作区与本仓库的关系

README 中给出的测试命令依赖外部工作区：

```text
/home/zzy/Program/UCAS/zephyrproject/
├── linux_sched/
└── linux_sched_kernel/
```

这说明当前测试并不一定直接在本仓库根目录下完成，而是通过外部 Zephyr 工作区构建独立的测试工程。

这类结构在实际开发中很常见，但也最容易引入“源码已经改了，测试却没吃到改动”的问题。

建议重点确认以下几件事：

- 外部工作区使用的是哪一份 Zephyr 源码
- 本仓库内置的 `zephyr/` 与外部工作区中的 Zephyr 是否同步
- 测试工程是否引用了本仓库的兼容层代码，还是引用了别处副本

如果这三件事不明确，后续排查构建和行为问题会很费时间。

## 4. 环境准备

### 4.1 基础工具

当前开发至少需要下面这些工具链组件：

- `west`
- Zephyr SDK
- `cmake`
- `ninja`
- `python3`

如果只是阅读文档和改少量源码，本仓库本身就够了；但只要需要跑测试或在 QEMU 上验证，就必须把外部工作区环境也准备好。

### 4.2 建议先做的检查

建议在开始联调前先确认下面几个命令都可用：

```bash
west --version
cmake --version
ninja --version
python3 --version
```

此外还建议确认 Zephyr SDK 与环境变量是否已经正确配置，否则很多构建问题会被误判成项目本身的问题。

### 4.3 目标运行环境

当前文档里明确提到的目标平台是 `qemu_cortex_m3`。这意味着：

- 绝大多数接口验证可以先在 QEMU 中完成
- 先把语义跑通，再迁移到真实硬件更稳妥
- 板级问题和内核语义问题要尽量分开排查

## 5. 构建方法

### 5.1 内核集成测试

这组测试更接近真实线程调度行为，适合验证策略切换、优先级比较和时间片逻辑是否真的生效。

```bash
cd /home/zzy/Program/UCAS/zephyrproject
west build -b qemu_cortex_m3 linux_sched_kernel -p always
west build -t run
```

按 README 的描述，预期结果是：

- `4/4 PASS`

这类测试通常是最值得优先跑的，因为它覆盖的是“接口到底有没有进入真实线程调度路径”。

### 5.2 算法级单元测试

这组测试更偏调度算法本身，适合验证 task 组织、排序规则和某些策略逻辑。

```bash
cd /home/zzy/Program/UCAS/zephyrproject
west build -b qemu_cortex_m3 linux_sched -p always
west build -t run
```

按 README 的描述，预期结果是：

- `11/11 PASS`

这类测试适合在改算法实现时快速回归。

### 5.3 关键配置项

当前已知的关键配置示例如下：

```ini
CONFIG_SCHED_LINUX=y
CONFIG_ZTEST=y
CONFIG_TIMESLICING=y
CONFIG_TIMESLICE_SIZE=10
```

需要注意几点：

- `CONFIG_SCHED_LINUX` 决定 Linux 风格调度器是否启用
- `CONFIG_TIMESLICING` 和 `CONFIG_TIMESLICE_SIZE` 直接影响 RR 语义验证
- 如果只开了接口代码，没打开调度器相关配置，很多测试现象会失真

## 6. 日常开发流程建议

从经验上看，这类项目最怕“改了一点接口，结果不知道是构建坏了、映射坏了，还是行为坏了”。因此建议把开发流程拆细一点。

### 第一步：先明确改动类型

通常先判断自己这次改动属于哪一类：

- 只是补头文件、结构体或声明
- 改兼容层参数转换和返回值处理
- 改 Zephyr 内核调度行为
- 改设备路径或 demo

不同类型的改动，后面的验证重点不一样。

### 第二步：只动必要文件

建议优先沿着“最短路径”修改：

- 接口定义问题，先看 `code/include/`
- 映射逻辑问题，先看兼容层实现
- 行为问题，重点看 `zephyr/kernel/`
- 测试资料问题，单独补到 `docs/test-demos/`

这样后面回顾变更也更清楚。

### 第三步：先跑最小验证

不要一上来就全量跑所有东西。更高效的做法是：

- 改调度接口，先跑调度相关最小测试
- 改设备路径，先跑设备 demo 或最小板级验证
- 改构建入口，先验证能否成功编译

### 第四步：再做整体验证

当最小测试通过后，再去跑更完整的集成测试、QEMU 回归或板级验证。

## 7. 调试建议

### 7.1 调度语义不对时先看什么

如果现象是“接口调用成功，但运行效果不对”，通常优先检查下面几项：

- 线程对象里是否真的写入了新策略
- 优先级映射是否正反一致
- ready queue 是否在修改后被正确刷新
- RR 时间片逻辑是否误作用于 `SCHED_FIFO`
- 查询接口是否从真实线程状态读值

这类问题最忌讳只盯着上层 API。很多时候接口层没错，错的是底层状态根本没跟着变。

### 7.2 构建通过但行为异常时怎么查

这类问题一般更隐蔽，建议按链路去查：

1. 参数是否从接口层正确传下来了
2. 映射层是否把策略和优先级翻译对了
3. 线程对象是否保存了新状态
4. 调度路径是否真正消费了这些状态
5. 查询接口是否能把当前状态正确读出来

只要按这个顺序排，通常能比较快定位到断点。

### 7.3 设备控制路径问题怎么查

设备问题建议按下面顺序排：

- 先确认 `fd` 是否建立成功
- 再确认 `ioctl` 命令字和参数结构是否匹配
- 再确认阻塞/非阻塞语义是否按预期返回
- 最后再往 Zephyr 驱动或板级适配层查

很多设备问题表面上像驱动 bug，实际是兼容层命令分发或参数解释不一致。

## 8. 文档维护建议

为了避免资料再次混杂，建议后续严格按边界补充：

- 接口名、参数、语义、返回值：写进 `docs/zephyr-linux-interface-definition.md`
- 环境、命令、流程、调试经验：写进 `docs/zephyr-linux-build-and-dev.md`
- 测试步骤、串口现象、性能记录：写进 `docs/test-demos/`
- 总体设计和验证体系说明：保留在 `docs/zepLinux-interface-and-validation.md`

这样以后不管是新人接手，还是回溯历史决策，查找成本都会低很多。

## 9. 当前测试与 demo 落点

当前仓库已经形成两层验证入口：

- 自动化单元测试：`tests/unit/test_thread.c`、`tests/unit/test_memory.c`、`tests/unit/test_sched.c`
- 手工观察型 demo：`tests/demo/scheduler/rr_vs_fifo/`、`tests/demo/scheduler/priority_mapping/`、`tests/demo/scheduler/rr_interval/`

建议在修改调度接口后，先看 `tests/unit/test_sched.c`，再根据需要运行 `tests/demo/scheduler/` 下的示例观察行为。
