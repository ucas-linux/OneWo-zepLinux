# 便捷编译和烧录脚本

本目录包含两个板子的一键编译脚本。

## 🚀 RocketPi (STM32F401)

### 编译

```bash
# 完全清理编译
./build_rocket_pi.sh clean

# 增量编译（默认）
./build_rocket_pi.sh

# 编译测试版本
./build_rocket_pi.sh test
```

### 烧录

```bash
# 烧录到板子
./build_rocket_pi.sh flash

# 手动烧录
west flash -d build-rocket-pi-anl --runner openocd
```

### 测试

```bash
# 打开串口
screen /dev/ttyACM0 115200

# 在串口中测试
uart:~$ test_phase1
uart:~$ fork
uart:~$ fork_demo
```

### 内存使用

```
FLASH: 70016 B / 512 KB (13.35%)
RAM:   82208 B / 96 KB  (83.63%)
```

---

## 🚀 AS32x601 (RISC-V)

### 编译

```bash
# 完全清理编译
./build_as32x601.sh clean

# 增量编译（默认）
./build_as32x601.sh

# 编译测试版本
./build_as32x601.sh test
```

### 烧录

```bash
# 烧录到板子
./build_as32x601.sh flash

# 手动烧录
west flash -d build-as32-anl --runner jlink
```

### 测试

```bash
# 打开串口
screen /dev/ttyUSB0 115200

# 在串口中测试
uart:~$ test_phase1
uart:~$ fork
uart:~$ fork_demo
```

### 内存使用

```
ROM: 92212 B / 2 MB   (4.40%)
RAM: 71120 B / 512 KB (13.57%)
```

---

## 📝 手动编译命令

### RocketPi

```bash
west build -p always -b rocket_pi -d build-rocket-pi-anl \
    zephyr/samples/ansilic/rocket_pi_shell_process
```

### AS32x601

```bash
west build -p always -b as32x601_evb/as32x601 -d build-as32-anl \
    zephyr/samples/ansilic/as32x601_shell_process
```

---

## 🧪 测试

两个板子都包含完整的 Phase 1-5 功能：

- ✅ FD 引用计数和深度复制
- ✅ 孤儿进程处理
- ✅ vfork 支持
- ✅ 环境变量优化
- ✅ 完整测试套件

---

## 📊 对比

| 特性 | RocketPi | AS32x601 |
|------|----------|----------|
| **架构** | ARM Cortex-M4 | RISC-V |
| **Flash** | 512 KB | 2 MB |
| **RAM** | 96 KB | 512 KB |
| **使用率** | 13%/84% | 4%/14% |
| **烧录工具** | OpenOCD | J-Link |
| **串口** | /dev/ttyACM0 | /dev/ttyUSB0 |

---

**更新日期**: 2026-08-15
