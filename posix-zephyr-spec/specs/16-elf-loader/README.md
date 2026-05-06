# ELF Loader - 静态二进制加载器

## 概述

在无MMU的Cortex-M3上，无法使用fork/exec，但可以通过ELF加载器在启动时或运行时加载预编译的静态链接Linux二进制文件。

## 目标

- 解析标准Linux ELF格式（ARM 32位）
- 加载静态链接的可执行文件
- 处理重定位（R_ARM_*类型）
- 支持从Flash执行（XIP）或加载到RAM
- 提供Linux ABI兼容的入口环境

## 限制

- **仅支持静态链接**：不支持动态链接的.so依赖
- **无MMU**：所有进程共享地址空间，依赖MPU隔离
- **无fork/exec**：使用spawn模型替代
- **内存限制**：二进制大小受RAM/Flash限制

## 使用场景

```c
// 场景1：启动时加载应用
int main(void) {
    elf_loader_init();

    // 从Flash加载静态二进制
    int ret = elf_load("/flash/bin/myapp", NULL, NULL);
    if (ret < 0) {
        printk("Failed to load app\n");
    }

    return 0;
}

// 场景2：运行时spawn新"进程"
pid_t spawn_app(const char *path, char *const argv[]) {
    elf_handle_t handle;

    // 加载ELF到内存
    if (elf_parse(path, &handle) < 0) {
        return -1;
    }

    // 创建隔离的执行环境
    pid_t pid = process_spawn(&handle, argv);

    return pid;
}
```

## 架构组件

### 1. ELF解析器
- 验证ELF魔数和架构（ARM）
- 解析Program Headers和Section Headers
- 提取代码段、数据段、BSS段

### 2. 加载器
- 分配内存区域（代码、数据、堆、栈）
- 复制段到目标地址
- 处理重定位条目

### 3. 重定位处理器
- 支持ARM重定位类型（R_ARM_ABS32, R_ARM_REL32等）
- 修正符号引用

### 4. 入口准备
- 设置argc/argv/envp
- 初始化栈帧（符合ARM EABI）
- 跳转到入口点

## 与其他模块的关系

```
┌─────────────────┐
│  Linux Binary   │
│   (ELF file)    │
└────────┬────────┘
         │
         v
┌─────────────────┐
│  ELF Loader     │ ← 本模块
│  - Parse        │
│  - Load         │
│  - Relocate     │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Thread-Process  │ (Spec 23)
│  - Spawn        │
│  - MPU Setup    │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ Syscall Layer   │ (Spec 17)
│  - Dispatcher   │
└─────────────────┘
```

## 配置选项

```kconfig
config ELF_LOADER
    bool "ELF binary loader"
    depends on !MMU
    default n
    help
      Enable loading of statically-linked ELF binaries.

config ELF_LOADER_XIP
    bool "Execute-in-place from flash"
    depends on ELF_LOADER
    default y
    help
      Allow executing code directly from flash without copying to RAM.

config ELF_MAX_SEGMENTS
    int "Maximum ELF segments"
    depends on ELF_LOADER
    default 8
    range 4 32

config ELF_LOADER_DEBUG
    bool "Enable ELF loader debugging"
    depends on ELF_LOADER
    default n
```

## 文件组织

```
specs/16-elf-loader/
├── README.md              # 本文件
├── elf_format.md          # ELF格式详解
├── parser.md              # 解析器实现
├── loader.md              # 加载器实现
├── relocation.md          # 重定位处理
├── xip.md                 # XIP支持
├── types.md               # 数据类型定义
└── implementation.md      # 实现指导
```

## 参考资料

- ELF Specification: https://refspecs.linuxfoundation.org/elf/elf.pdf
- ARM ELF ABI: https://github.com/ARM-software/abi-aa
- uClinux BFLT format (替代方案)
