# vfs_ramfs on QEMU (ARM Cortex-M3) — 编译 / 运行 / 调试

本说明针对 `qemu_cortex_m3` 板子(TI LM3S6965，machine `lm3s6965evb`）运行
`tests/subsys/fs/vfs_ramfs`。注意：目标芯片 AS32X601 是 RISC-V，本测试是纯文件
系统逻辑、与硬件无关，因此可以在 ARM Cortex-M3 的 QEMU 上跑来做调试。

## 0. 前置环境

- west v1.5.0
- Zephyr SDK 0.17.4（含 ARM 工具链与 SDK 自带的 qemu-system-arm）
- 必须用 **SDK 自带的** qemu：
  `/home/zzy/zephyr-sdk-0.17.4/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-arm`
  （系统 `/usr/bin/qemu-system-arm` 的串口/chardev 接法和 Zephyr 不一致，会没有输出。
  用 `west build -t run` 会自动选 SDK 的 qemu，推荐用它。）

## 1. 编译

在仓库根目录 `/opt/Program/UCAS/OneWo-zepLinux` 下执行：

```bash
west build -p always -b qemu_cortex_m3 -d build-qemu \
    zephyr/tests/subsys/fs/vfs_ramfs
```

- `-p always`：pristine 全新构建
- `-b qemu_cortex_m3`：ARM Cortex-M3 QEMU 板子
- `-d build-qemu`：独立构建目录，不影响已有的 as32x601 `build/`

### 为什么需要 boards/qemu_cortex_m3.conf

`drivers/serial/Kconfig.as32x601` 里 `CONFIG_UART_AS32X601` 是 `default y` 且仅
`depends on SERIAL`，于是任何开了串口的板子都会编进这个 RISC-V UART 驱动，它
`select PINCTRL`，进而 include lm3s6965 没有的 `pinctrl_soc.h`，导致编译失败。
为不改动共享驱动，本测试目录用板级覆盖文件
`boards/qemu_cortex_m3.conf` 设置 `CONFIG_UART_AS32X601=n` 关掉它。

## 2. 运行

```bash
west build -d build-qemu -t run
```

退出 QEMU：`Ctrl-a x`。

等价的手动命令（需用 SDK qemu）：

```bash
cd build-qemu
/home/zzy/zephyr-sdk-0.17.4/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-arm \
    -cpu cortex-m3 -machine lm3s6965evb -nographic -monitor none \
    -vga none -net none -serial stdio \
    -icount shift=6,align=off,sleep=off -rtc clock=vm \
    -kernel zephyr/zephyr.elf
```

## 3. 调试（GDB）

终端 A：以暂停状态启动 QEMU 并开 gdbserver（端口 1234）。

```bash
west build -d build-qemu -t debugserver
```

终端 B：连接 GDB。

```bash
/home/zzy/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/arm-zephyr-eabi-gdb \
    build-qemu/zephyr/zephyr.elf \
    -ex "target remote :1234"
```

常用：

```gdb
break test_file_in_directory
break ramfs_unmount
continue
backtrace
info registers
```

把崩溃地址转成源码行：

```bash
/home/zzy/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/arm-zephyr-eabi-addr2line \
    -f -e build-qemu/zephyr/zephyr.elf 0x000071ac
```

## 4. 当前已知问题（待调试）

跑测试时会在 `test_file_in_directory` 用例中内核 panic：

```
ASSERTION FAIL [z_spin_unlock_valid(l)] @ .../spinlock.h:308
	Not my spinlock 0x200000cc
ZEPHYR FATAL ERROR 4: Kernel panic
```

`Not my spinlock` 来自 `k_mutex` 的调度自旋锁校验：**k_mutex 必须由当初加锁的
同一个线程解锁**。VFS 用到 mutex 的地方见
`subsys/fs/vfs_core/vfs_superblock.c`（`sb->lock` 的 lock/unlock）和
`subsys/fs/vfs_core/vfs_driver.c`（`driver_lock`）。怀疑存在跨线程 lock/unlock，
或在持锁状态下回调进了又去取/放同一把锁的路径。建议在上面两个文件的
`k_mutex_lock/unlock` 处下断点，结合 `backtrace` 找出加锁与解锁所在线程不一致的调用。
