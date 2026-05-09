# AS32X601 Zephyr BSP

AS32X601 芯片的 Zephyr RTOS 板级支持包（BSP），基于 Zephyr 4.3.0。

## 仓库结构

```
as32601_zephyr/
├── zephyr/zephyrproject/       # West 工作区（含 AS32X601 补丁的 Zephyr 主树）
│   ├── zephyr/soc/ansilic/     # SoC 适配
│   ├── zephyr/boards/ansilic/  # 板级配置
│   ├── zephyr/dts/riscv/ansilic/  # 设备树
│   ├── zephyr/samples/ansilic/ # AS32X601 示例工程
│   └── modules/hal/ansilic/    # AS32X601 HAL 模块
└── tools/openocd/              # AS32X601 专用 OpenOCD
    ├── bin/openocd             # OpenOCD 可执行文件
    ├── lib/                    # 依赖库
    ├── cfg/                    # 烧录配置文件
    ├── firmware/               # 示例固件
    ├── run_openocd.sh          # 启动脚本
    └── flash_bin.sh            # 烧录脚本
```

## 环境准备

### 1. 系统依赖

请手动安装 Zephyr 所需依赖，参考官方文档：
https://docs.zephyrproject.org/latest/develop/getting_started/index.html

### 2. Zephyr SDK

下载 Zephyr SDK 0.17.4：
https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v0.17.4

选择 `zephyr-sdk-0.17.4_linux-x86_64.tar.xz`，解压到仓库根目录：

```bash
tar xf zephyr-sdk-0.17.4_linux-x86_64.tar.xz -C <仓库路径>/
cd <仓库路径>/zephyr-sdk-0.17.4
./setup.sh
```

### 3. 配置环境变量

将以下内容添加到 `~/.bashrc`：

```bash
export ZEPHYR_SDK_INSTALL_DIR=<仓库路径>/zephyr-sdk-0.17.4
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_BASE=<仓库路径>/zephyr/zephyrproject/zephyr
```

## 编译

```bash
cd zephyr/zephyrproject
west build -b as32x601_evb/as32x601 zephyr/samples/ansilic/as32x601_gpio_blink
```

切换工程时需清除旧的 build 目录，使用 `-p`（`--pristine`）：

```bash
west build -p -b as32x601_evb/as32x601 zephyr/samples/ansilic/as32x601_gpio_blink
```

## 烧录

仓库内置了针对 AS32X601 定制的 OpenOCD（位于 `tools/openocd/`），支持通过 J-Link 调试器进行烧录。

### 使用 west 烧录

```bash
west flash -d <build目录>
```


## 支持的示例

| 示例 | 说明 |
|------|------|
| `as32x601_gpio_blink` | GPIO 闪灯 |
| `as32x601_button_interrupt` | 按键中断 |
| `as32x601_interrupt_test` | 定时器/中断测试 |
| `as32x601_multithread_demo` | 多线程示例 |

