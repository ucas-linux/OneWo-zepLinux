# ANL Loader QEMU 使用帮助

本示例在 Zephyr 的 `qemu_cortex_m3` 板级上运行 ANL（ANsiLic）动态加载器。
加载器从串口接收十六进制编码的 `.anl` 文件，完成格式校验、内存装载、重定位、
外部符号解析，并跳转到模块入口执行。

ANL Loader 与 Zephyr LLEXT 的设计取舍、轻量化优势和适用边界见
[`docs/zh/anl-loader-vs-zephyr-llext.md`](../../../../docs/zh/anl-loader-vs-zephyr-llext.md)。

## 1. 环境要求

- 已配置 Zephyr `west` 工作区。
- 已安装 Zephyr SDK 0.17.4 或可用的 ARM Zephyr 工具链。
- 已安装 `qemu-system-arm`。
- Python 已安装 `pyelftools`：

```bash
python3 -m pip install pyelftools
```

下文命令均在项目根目录执行。

## 快速开始

一条命令构建加载器、生成 Hello ANL 模块、启动 QEMU 并自动加载运行：

```bash
make -C zephyr/samples/ansilic/anl_loader_qemu run
```

其他常用目标：

```bash
# 构建加载器和 ANL 模块，但不启动 QEMU
make -C zephyr/samples/ansilic/anl_loader_qemu

# 只生成 ANL 模块
make -C zephyr/samples/ansilic/anl_loader_qemu module

# 删除加载器和模块构建产物
make -C zephyr/samples/ansilic/anl_loader_qemu clean
```

默认生成文件位于：

```text
zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.o
zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl
```

## 2. 构建加载器

```bash
west build -p always \
  -b qemu_cortex_m3 \
  -d build-anl-qemu \
  zephyr/samples/ansilic/anl_loader_qemu
```

启动 QEMU：

```bash
west build -d build-anl-qemu -t run
```

启动成功后会看到：

```text
ANL loader ready. Commands: load <name> <hexdata>
anl>
```

退出 QEMU：先按 `Ctrl+A`，再按 `X`。

## 3. Hello 测试模块

测试模块已经落盘到：

```text
zephyr/samples/ansilic/anl_loader_qemu/examples/hello.c
```

其内容为：

```c
extern void printk(const char *message);
extern void k_msleep(int milliseconds);

void main(void)
{
    printk("Hello from ANL module!\n");
    k_msleep(100);
}
```

当前加载器向 ANL 模块导出以下符号：

- `printk`：仅支持传入一个字符串，不应使用格式化参数。
- `k_msleep`：按毫秒休眠。

## 4. 编译并生成 ANL 文件

推荐直接使用 Makefile：

```bash
make -C zephyr/samples/ansilic/anl_loader_qemu module
```

生成文件为：

```text
zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl
```

以下为 Makefile 内部执行的手动步骤。

设置工具链路径；如果 SDK 安装位置不同，请相应修改：

```bash
ARM_GCC=/home/zzy/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc
```

将 C 文件编译为 ARM Thumb-2 ELF 目标文件：

```bash
"$ARM_GCC" \
  -c \
  -mthumb \
  -mcpu=cortex-m3 \
  -O1 \
  -ffreestanding \
  -mlong-calls \
  zephyr/samples/ansilic/anl_loader_qemu/examples/hello.c \
  -o zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.o
```

将 `.o` 文件转换为 `.anl` 文件：

```bash
python3 tools/anl_link.py \
  zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.o \
  zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl \
  --entry main
```

正常情况下会输出类似信息：

```text
Written ... bytes to .../build/modules/hello.anl (entry_off=0x...)
```

## 5. 加载并运行模块

推荐使用一键命令：

```bash
make -C zephyr/samples/ansilic/anl_loader_qemu run
```

以下为手动加载方法。

将 ANL 文件转换为大写十六进制字符串：

```bash
ANL=zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl
HEX=$(python3 -c "print(open('$ANL','rb').read().hex().upper())")
```

可以先确认文件大小和命令长度：

```bash
wc -c "$ANL"
echo "${#HEX} hex characters"
```

### 自动发送到 QEMU

以下命令会启动 QEMU、发送模块、等待执行，然后退出：

```bash
(
  sleep 2
  printf 'load hello %s\r' "$HEX"
  sleep 3
  printf '\001x'
) | west build -d build-anl-qemu -t run
```

预期输出包含：

```text
loaded ... bytes, running 'hello'...
anl_load[hello]: jumping to entry ...
Hello from ANL module!
anl_load returned 0
```

### 手动发送

先运行：

```bash
west build -d build-anl-qemu -t run
```

然后在 `anl>` 提示符输入一整行命令：

```text
load hello <ANL 文件的十六进制内容>
```

由于 `$HEX` 是宿主 Shell 变量，不能在 QEMU 的 `anl>` 提示符中直接输入
`load hello $HEX` 并期待变量展开。推荐使用上面的自动发送方式，或者先在宿主终端执行：

```bash
printf 'load hello %s\n' "$HEX"
```

再复制输出的完整命令。

## 6. 串口上传脚本

`tools/anl_upload.py` 可用于连接真实串口的加载器：

```bash
python3 tools/anl_upload.py \
  /dev/ttyUSB0 \
  115200 \
  hello \
  zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl
```

该脚本依赖 `pyserial`：

```bash
python3 -m pip install pyserial
```

QEMU 通常直接使用标准输入和标准输出，不需要该串口上传脚本。

## 7. 当前限制

- 仅支持 ANL 格式版本 1 和 ARM Thumb-2 模块。
- 加载器二进制缓冲区为 8192 字节。
- 命令输入缓冲区约为 8256 字符；由于每个字节编码为两个十六进制字符，
  通过当前 `load` 命令实际可传输的 ANL 文件约为 4 KiB，而不是完整的 8 KiB。
- 当前最多记录 16 个节的运行时地址。
- 当前动态导出符号只有 `printk` 和 `k_msleep`。
- 模块入口无参数、无返回值，并以 Thumb 模式调用。
- 每次加载完成后会释放模块占用的堆内存，因此模块不能在入口返回后继续使用该内存。

## 8. 常见错误

### `ModuleNotFoundError: No module named 'elftools'`

```bash
python3 -m pip install pyelftools
```

### `error: too large`

ANL 文件转换后的十六进制命令超过输入缓冲区。检查文件大小：

```bash
wc -c zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl
```

减小模块体积，例如使用 `-Os`、减少静态数据，或修改加载协议以支持分块传输。

### `error: odd hex length`

十六进制字符串字符数不是偶数，通常表示复制时内容被截断。

### `error: bad hex`

输入包含非十六进制字符。十六进制数据中只能出现 `0-9`、`A-F` 或 `a-f`。

### `validate failed -2`

文件魔数错误，输入内容不是有效的 ANL 文件或开头数据被截断。

### `validate failed -3`

ANL 文件版本与加载器不匹配。

### `validate failed -4`

输入数据长度小于 ANL 文件头中记录的文件大小，通常表示传输被截断。

### `validate failed -5`

CRC-32 校验失败，通常表示文件生成后被修改或串口传输内容损坏。

### `unresolved symbol '...'`

模块引用了加载器未导出的外部符号。删除该依赖，或在
`src/main.c` 的 `_anl_exports` 表中增加对应符号后重新构建加载器。

## 9. 完整命令摘要

推荐方式：

```bash
make -C zephyr/samples/ansilic/anl_loader_qemu run
```

等价的手动流程：

```bash
west build -p always -b qemu_cortex_m3 -d build-anl-qemu \
  zephyr/samples/ansilic/anl_loader_qemu

ARM_GCC=/home/zzy/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc
"$ARM_GCC" -c -mthumb -mcpu=cortex-m3 -O1 -ffreestanding -mlong-calls \
  zephyr/samples/ansilic/anl_loader_qemu/examples/hello.c \
  -o zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.o

python3 tools/anl_link.py \
  zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.o \
  zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl \
  --entry main

ANL=zephyr/samples/ansilic/anl_loader_qemu/build/modules/hello.anl
HEX=$(python3 -c "print(open('$ANL','rb').read().hex().upper())")

(
  sleep 2
  printf 'load hello %s\r' "$HEX"
  sleep 3
  printf '\001x'
) | west build -d build-anl-qemu -t run
```
