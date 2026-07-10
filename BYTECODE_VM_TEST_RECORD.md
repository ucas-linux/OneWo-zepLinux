# Bytecode VM 测试记录

## 测试环境
- 平台: QEMU Cortex-M3
- Zephyr RTOS
- 字节码虚拟机实现

## 测试步骤

### 1. 启动系统
```bash
cd /opt/Program/UCAS/OneWo-zepLinux
west build -t run
```

### 2. 测试命令可用性
```
qemu> ls
```
**预期结果**: 显示内置命令和已加载程序列表（初始为空）

### 3. 上传 Hello World 程序
```
qemu> upload myhello hello
```
**预期结果**:
```
Uploading program 'myhello' (23 bytes)...
Program 'myhello' loaded (23 bytes)
Upload complete. Use './run myhello' to execute.
```

### 4. 验证程序已加载
```
qemu> ls
```
**预期结果**: 列表中应显示 `myhello`

### 5. 执行 Hello World
```
qemu> run myhello
```
**预期结果**:
```
Executing 'myhello'...
Hello World!
42
Program 'myhello' completed successfully
```

### 6. 上传并运行 Counter 程序
```
qemu> upload mycounter counter
qemu> run mycounter
```
**预期结果**: 打印数字 1-10，每个数字间隔约100ms

### 7. 上传并运行 Calculator 程序
```
qemu> upload mycalc calc
qemu> run mycalc
```
**预期结果**:
```
Calc: (5+3)*2 =
16
```

### 8. 查看所有程序
```
qemu> ls
```
**预期结果**: 显示 myhello, mycounter, mycalc

### 9. 删除程序
```
qemu> rm myhello
qemu> ls
```
**预期结果**: myhello 被删除，列表中只剩 mycounter 和 mycalc

### 10. 测试错误处理
```
qemu> run nonexistent
```
**预期结果**: 错误信息 "Program 'nonexistent' not found"

```
qemu> upload duplicate hello
qemu> upload duplicate hello
```
**预期结果**: 第二次上传失败，提示程序已存在

## 性能指标

- 内存使用:
  - FLASH: ~47KB (18%)
  - RAM: ~43KB (67%)

- 每个程序最大: 2KB
- 最多程序数: 8个
- VM栈深度: 64

## 功能验证

✅ 程序上传
✅ 程序列表
✅ 程序执行
✅ 程序删除
✅ 错误处理
✅ 多程序管理
✅ 栈操作指令
✅ 算术运算指令
✅ 控制流指令
✅ 系统调用指令

## 下一步

- [ ] 实现 TCP/UDP 网络上传接口
- [ ] 添加程序签名验证
- [ ] 实现更多系统调用（文件IO等）
- [ ] 优化VM性能（JIT编译）
- [ ] 添加调试接口
