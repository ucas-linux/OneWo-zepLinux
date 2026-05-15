# 板级验证

本文件用于说明板级联调 demo 应该如何落到仓库结构中，以及后续真实验证资料建议放在哪里。

## 1. 对应目录

建议的板级验证目录如下：

```text
tests/demo/board/
├── README.md
├── qemu-cortex-m3-validation.md
├── rocket-pi-validation.md
└── concurrent-device-validation.md
```

这里仍然先以文档为主，因为板级验证往往依赖实际硬件、日志和接线信息，不一定一开始就适合写成纯自动化测试。

## 2. 已有基础

从当前 README 可以确认两点：

- 当前主验证平台之一是 `qemu_cortex_m3`
- 仓库目标是把 Linux 风格接口落在 Zephyr/MCU 场景上

因此板级验证至少可以先分成两类：

- QEMU 验证
- 真实板卡验证

## 3. 建议的验证材料组织方式

### 3.1 QEMU 验证

建议文件：`tests/demo/board/qemu-cortex-m3-validation.md`

建议记录：

- 运行的测试目录
- 构建命令
- QEMU 输出要点
- 哪些接口已经在 QEMU 中跑通
- 哪些问题无法仅靠 QEMU 观察

### 3.2 Rocket Pi 验证

建议文件：`tests/demo/board/rocket-pi-validation.md`

建议记录：

- 板卡型号
- 烧录方式
- 串口波特率
- 使用的外设
- 运行步骤
- 预期现象与异常现象

### 3.3 并发设备访问验证

建议文件：`tests/demo/board/concurrent-device-validation.md`

建议记录：

- 多线程调度策略组合
- 设备访问组合场景
- 是否出现优先级反转、饥饿或事件丢失

## 4. 与其他测试目录的关系

- `tests/unit/` 更偏接口级正确性
- `tests/integration/` 更偏跨模块联动
- `tests/demo/board/` 更偏真实联调过程与可观察现象

三者不是互相替代，而是不同层次的验证。
