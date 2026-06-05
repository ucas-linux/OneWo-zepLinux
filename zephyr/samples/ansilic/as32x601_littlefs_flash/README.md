# AS32X601 LittleFS on QSPI Flash

这个示例演示了如何在 AS32X601 的 QSPI Flash 上使用 LittleFS 文件系统。

## 功能特性

- ✅ 基于 QSPI Flash 的持久化存储
- ✅ LittleFS 文件系统（支持掉电保护和磨损均衡）
- ✅ 完整的目录支持（创建、删除、嵌套目录）
- ✅ 文件操作（创建、读、写、追加、删除）
- ✅ 文件系统统计信息
- ✅ 目录树递归遍历

## 构建和运行

```bash
west build -p always -b as32x601_evb/as32x601 zephyr/samples/ansilic/as32x601_littlefs_flash
west flash
```

## 测试内容

该示例包含以下测试：

1. **基本文件操作** - 创建、写入、读取文件
2. **目录操作** - 创建多级目录结构
3. **追加操作** - 向现有文件追加内容
4. **文件状态** - 获取文件和目录的元数据
5. **文件系统统计** - 查看存储空间使用情况
6. **文件删除** - 删除文件并验证

## 文件系统结构

示例会创建以下目录结构：

```
/lfs
├── test.txt
├── docs/
│   └── readme.txt
├── config/
│   └── settings.ini
└── data/
    └── logs/
        └── system.log
```

## LittleFS 特性

LittleFS 是专为嵌入式系统设计的文件系统，具有以下特点：

- **掉电安全** - 在任何时刻断电都不会损坏文件系统
- **磨损均衡** - 自动分散写入操作，延长 Flash 寿命
- **小内存占用** - 适合资源受限的嵌入式系统
- **动态磨损均衡** - 无需预留额外空间

## 配置说明

在 `prj.conf` 中的关键配置：

```
CONFIG_FILE_SYSTEM=y              # 启用文件系统支持
CONFIG_FILE_SYSTEM_LITTLEFS=y     # 启用 LittleFS
CONFIG_FLASH=y                    # 启用 Flash 驱动
CONFIG_FLASH_AS32X601_QSPI=y      # 启用 AS32X601 QSPI Flash 驱动
CONFIG_HEAP_MEM_POOL_SIZE=16384   # LittleFS 需要的堆内存
```

## 与 RAMFS 的对比

| 特性 | RAMFS | LittleFS on Flash |
|------|-------|-------------------|
| 存储介质 | RAM | Flash |
| 掉电保持 | ❌ | ✅ |
| 容量 | 受 RAM 限制 | 受 Flash 限制 |
| 速度 | 快 | 中等 |
| 磨损均衡 | 不需要 | ✅ |
| 适用场景 | 临时文件、缓存 | 配置文件、日志、数据存储 |

## 注意事项

1. 首次运行时会自动格式化 Flash 分区
2. LittleFS 分区大小为 1MB（可在代码中调整）
3. Flash 擦除操作较慢，请耐心等待
4. 建议定期检查文件系统统计信息，避免空间耗尽
