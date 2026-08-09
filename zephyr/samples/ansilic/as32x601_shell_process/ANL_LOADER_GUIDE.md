# ANL Loader — AS32X601 开发板使用指南

## 概述

ANL（ANsiLic）是一种轻量级可重定位二进制格式，支持在 Zephyr 上动态加载 RISC-V RV32I 模块。

---

## 构建流程

### 1. 编写 ANL 模块源码

模块只能调用已导出的内核符号（见下文），不能链接标准库。

```c
/* 声明外部符号 */
extern void printk(const char *fmt);
extern void k_msleep(int ms);
extern int  new_task(const char *name, void *(*fn)(void *), void *arg);

void main(void)
{
    printk("hello from ANL\n");
}
```

### 2. 编译为 RV32IMC 目标文件

```bash
~/zephyr-sdk-0.17.4/riscv64-zephyr-elf/bin/riscv64-zephyr-elf-gcc \
    -march=rv32imc -mabi=ilp32 -c -O2 \
    -nostdlib -fno-builtin -ffreestanding \
    tools/examples/hello_anl_mini.c -o /tmp/hello_mini.o
```

关键编译选项：
- `-march=rv32imc` — 目标 ISA（含压缩指令 C 扩展）
- `-nostdlib -fno-builtin -ffreestanding` — 不链接 C 运行时

### 3. 转换为 ANL 格式

```bash
python3 tools/anl_link.py /tmp/hello_mini.o /tmp/hello_mini.anl --entry main
```

`anl_link.py` 读取 ELF `.o`，提取 `.text`/`.rodata`/`.data`/`.bss` 及重定位表，生成 ANL 二进制。

`R_RISCV_RELAX`（type=51）和 `R_RISCV_BRANCH`（type=16）会被静默忽略，这是正常的。

### 4. 上传到开发板

```bash
sudo python3 tools/upload/upload_hex.py /dev/ttyUSB0 115200 <name> /tmp/<file>.anl
```

`upload_hex.py` 将二进制转为十六进制字符串，通过串口发送 `upload_hex <name> <hexdata>` 命令。

---

## 已导出的内核符号

| 符号 | 签名 | 说明 |
|------|------|------|
| `printk` | `void printk(const char *fmt)` | 仅支持 `%s` 格式（wrapper） |
| `k_msleep` | `void k_msleep(int ms)` | 睡眠毫秒 |
| `new_task` | `pid_t new_task(const char *name, void *(*fn)(void *), void *arg)` | 创建子进程 |
| `waitpid` | `pid_t waitpid(pid_t pid, int *status, int options)` | 等待子进程退出 |
| `process_current` | `struct z_process *process_current(void)` | 获取当前进程 |

---

## 示例：fork_demo

`tools/examples/fork_demo.c` 演示了通过 `new_task` 创建两个子进程：

```bash
# 编译
~/zephyr-sdk-0.17.4/riscv64-zephyr-elf/bin/riscv64-zephyr-elf-gcc \
    -march=rv32imc -mabi=ilp32 -c -O2 \
    -nostdlib -fno-builtin -ffreestanding \
    tools/examples/fork_demo.c -o /tmp/fork_demo.o

python3 tools/anl_link.py /tmp/fork_demo.o /tmp/fork_demo.anl --entry main

# 上传运行
sudo python3 tools/upload/upload_hex.py /dev/ttyUSB0 115200 fork_demo /tmp/fork_demo.anl
```

预期输出：
```
[anl] main start
[anl] spawned
[child] id=1
[child] id=2
[child1] done
[child2] done
```

---

## 技术说明

### 非对齐访问修复

RV32IMC 含 16 位压缩指令，导致 32 位指令可能出现在 2 字节对齐地址。loader 的 relocation patching 全部使用 `memcpy` 而非直接指针解引用，避免 `mcause=4`（Load address misaligned）。

### I-cache 刷新

代码写入后执行 `fence.i` 刷新指令缓存，避免 CPU 执行旧缓存内容导致 `mcause=2`（Illegal instruction）。

### shell 命令

| 命令 | 用法 |
|------|------|
| `upload_hex` | `upload_hex <name> <hexdata>` |
| `load` | 同 `upload_hex` |
| `fork` | `fork [nchildren 1-4] [iterations]` |
