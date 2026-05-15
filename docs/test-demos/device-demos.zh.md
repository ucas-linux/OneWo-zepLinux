# 设备接口 Demo

本文件用于规划设备控制路径的 demo，并把文档说明落到仓库中的实际目录。

## 1. 对应目录

当前建议的设备 demo 目录为：

```text
tests/demo/device/
├── README.md
├── gpio-device-demo.md
├── irq-notify-demo.md
└── nonblock-io-demo.md
```

当前仓库里还没有现成的 `test_device.c`，所以这部分先以 demo 文档形式固化下来，后续再逐步补代码。

## 2. 为什么单独放 `tests/demo/device/`

设备接口验证和普通单元测试不太一样，它往往需要：

- 明确设备对象或板级外设
- 说明 `fd` 的来源
- 说明 `ioctl` 命令和参数结构
- 说明阻塞/非阻塞时的现象差异

因此单独放一个目录更利于后续积累样例，而不是把所有内容都塞进一个 `test_device.c`。

## 3. 建议的 demo 划分

### 3.1 GPIO 控制 demo

建议文件：`tests/demo/device/gpio-device-demo.md`

建议内容：

- 兼容层如何建立逻辑设备与 `fd` 的映射
- 应用如何通过 `open()`、`write()` 或 `ioctl()` 控制 GPIO
- 如何观察 GPIO 状态变化

建议后续代码落点：

- `tests/demo/device/gpio_device_demo.c`

### 3.2 IRQ 通知 demo

建议文件：`tests/demo/device/irq-notify-demo.md`

建议内容：

- 如何触发设备中断或模拟中断
- 应用层如何读到事件
- 阻塞与非阻塞读取分别有什么现象

建议后续代码落点：

- `tests/demo/device/irq_notify_demo.c`

### 3.3 非阻塞 I/O demo

建议文件：`tests/demo/device/nonblock-io-demo.md`

建议内容：

- 如何把设备设为非阻塞模式
- 设备无数据时的返回值
- 重试读取后的行为

建议后续代码落点：

- `tests/demo/device/nonblock_io_demo.c`

## 4. 当前与设备路径最接近的现有内容

虽然仓库里还没有独立的设备测试，但可以把下面这些位置作为后续实现入口：

- 头文件：`code/include/lmz_device.h`
- 实现：`code/src/lmz_device.c`
- demo 资料：`tests/demo/device/`

也就是说，设备路径后续最适合采取“代码接口 + demo 文档 + 板级验证”三者同步推进的方式。
