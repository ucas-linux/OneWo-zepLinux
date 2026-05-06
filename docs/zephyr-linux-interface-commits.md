# Zephyr Linux 接口资料说明

这组文档用于整理 zepLinux 在 Zephyr 上实现 Linux 兼容接口层时形成的接口定义、开发方法和验证材料。

前一版资料把接口说明、编译方法和验证内容放在同一份文档里，查阅时不太方便：

- 做接口实现时，需要反复翻找接口语义和内核桥接点
- 做环境搭建和联调时，更关心源码位置、构建命令和调试方法
- 做回归测试时，又需要把测试场景、demo 步骤和预期现象单独拿出来看

因此这里按用途拆成三类资料，便于后续持续维护。

## 文档划分

### 1. 接口定义

文件：`docs/zephyr-linux-interface-definition.md`

这份文档主要回答三个问题：

- zepLinux 到底向上暴露了哪些 Linux/POSIX 风格接口
- 这些接口在 Zephyr 内核里分别落到哪里
- 哪些语义是完整兼容，哪些语义是结合 MCU 场景做了约束

适合在以下场景阅读：

- 新增或修改 `sched_*`、`pthread_*sched*`、`lmz_sched_*` 接口时
- 需要梳理优先级、策略、时间片与 `k_thread` 的关系时
- 需要给设备控制路径补充 `fd`、`ioctl`、非阻塞语义时

### 2. 编译与开发方法

文件：`docs/zephyr-linux-build-and-dev.md`

这份文档更偏工程落地，主要包含：

- 仓库内源码的分布方式
- 需要改动的 Zephyr 内核文件位置
- 常用的构建、运行和调试方法
- 开发过程中容易踩到的问题

适合在以下场景阅读：

- 第一次接手这个仓库，需要把环境跑起来
- 修改了内核或兼容层后，需要验证构建链路是否正常
- 需要在本仓库和外部 `west` 工作区之间同步改动时

### 3. 测试与 demo 资料

文档目录：`docs/test-demos/`

对应的仓库内 demo 根目录为：`tests/demo/`

这里单独存放验证材料，避免和接口文档混在一起。目录下的内容可以继续扩展，目前按验证主题拆为：

- `docs/test-demos/scheduler-tests.md`
- `docs/test-demos/device-demos.md`
- `docs/test-demos/board-validation.md`
- `docs/test-demos/performance-benchmarks.md`

这样做的好处是：

- 便于后续把“说明文档”逐步沉淀成“可执行验证清单”
- 便于把不同板卡、不同接口类别的样例继续扩充进去
- 便于后续把测试代码、串口日志和性能记录和文档一一对应

## 与现有总览文档的关系

`docs/zepLinux-interface-and-validation.md` 仍然保留，作用不变，继续作为整体总览文档使用。它更适合用于说明：

- 这个兼容层为什么这样设计
- 调度与设备路径的大致结构是什么
- 验证体系覆盖了哪些方面

而本次新增的几份文档，更多是把总览中的内容往下展开，变成可以直接用于实现、联调和回归的工程资料。

## 建议阅读顺序

如果是第一次接手这个模块，建议按下面的顺序阅读：

1. 先看 `README.md`，建立对项目范围和代码布局的整体认识
2. 再看 `docs/zepLinux-interface-and-validation.md`，理解整体设计目标和验证思路
3. 然后看 `docs/zephyr-linux-interface-definition.md`，把接口定义和内核桥接点理清楚
4. 接着看 `docs/zephyr-linux-build-and-dev.md`，把环境、构建和调试流程跑通
5. 最后进入 `docs/test-demos/`，结合具体场景做验证

## 后续维护建议

后续继续补资料时，建议按下面的边界维护：

- 接口名、参数、返回值、语义约束，写进 `docs/zephyr-linux-interface-definition.md`
- 环境、命令、源码位置、开发步骤，写进 `docs/zephyr-linux-build-and-dev.md`
- 样例、测试步骤、预期输出、排查方法，写进 `docs/test-demos/`
- 总体设计背景和验证体系全景，保留在 `docs/zepLinux-interface-and-validation.md`

这样可以避免文档越写越杂，也方便以后把每一部分分别交给不同的人维护。
