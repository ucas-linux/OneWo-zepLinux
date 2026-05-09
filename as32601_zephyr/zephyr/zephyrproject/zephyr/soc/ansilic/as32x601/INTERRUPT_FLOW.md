# AS32X601 当前启动链路与中断链路

## 1. 说明范围

本文档描述 **当前实际生效** 的 AS32X601 Zephyr 启动链路与中断链路。

本文档不描述 HAL 中历史保留的 trap 入口实现，也不描述当前未参与编译的 SoC 私有 `vector.S` / `soc_irq.S`。

## 2. 当前实际参与编译的关键文件

由 `zephyr/soc/ansilic/as32x601/CMakeLists.txt` 可知，当前实际使用的启动/中断相关文件如下：

- `zephyr/soc/common/riscv-privileged/vector.S`
- `zephyr/soc/common/riscv-privileged/soc_irq.S`
- `zephyr/soc/ansilic/as32x601/soc.c`
- `zephyr/drivers/interrupt_controller/intc_as32x601_plic.c`
- `zephyr/drivers/timer/riscv_machine_timer.c`

以下文件当前 **未参与编译**，仅保留作历史参考：

- `zephyr/soc/ansilic/as32x601/vector.S`
- `zephyr/soc/ansilic/as32x601/soc_irq.S`

## 3. 启动链路

### 3.1 复位入口

CPU 上电后进入启动入口 `__start`：

- 文件：`zephyr/soc/common/riscv-privileged/vector.S`

主要动作：

1. 初始化 `gp`
2. 调用 `soc_reset_hook()`（如果开启）
3. 设置 `mtvec`
4. 跳转到 `__reset`

### 3.2 mtvec 当前指向

当前 AS32X601 采用的是 **RISC-V direct mode**，`mtvec` 指向：

- `_isr_wrapper`

对应文件：

- `zephyr/soc/common/riscv-privileged/vector.S`
- `zephyr/arch/riscv/core/isr.S`

这意味着当前 trap 总入口是 **Zephyr 架构层 `_isr_wrapper`**，而不是 HAL 的 `TrapEntry`。

### 3.3 进入内核初始化

`__reset` 之后进入 Zephyr 标准初始化流程，期间会执行：

- `soc_early_init_hook()`
- `SYS_INIT(...)`

当前 AS32X601 中：

- `soc_early_init_hook()` 在 `zephyr/soc/ansilic/as32x601/soc.c`
- 作用：先打开基础 bus clock

## 4. 时钟链路

### 4.1 SoC 时钟初始化

当前时钟初始化入口：

- `as32x601_clock_post_kernel_init()`
- 文件：`zephyr/soc/ansilic/as32x601/soc.c`

该函数通过 `SYS_INIT(..., PRE_KERNEL_1, 0)` 注册。

主要工作：

1. 初始化 PLL
2. 切换系统时钟到 PLL
3. 配置 AXI/APB 分频
4. 更新 Flash 时钟相关设置

### 4.2 系统定时器

当前系统时钟驱动：

- `zephyr/drivers/timer/riscv_machine_timer.c`

当前工程中已启用：

- `CONFIG_SYS_CLOCK_EXISTS=y`
- `CONFIG_RISCV_MACHINE_TIMER=y`

因此 `k_msleep()`、内核 tick、timeout 等能力已经接入。

## 5. 中断链路总览

当前实际链路如下：

1. 外设或 machine interrupt 触发
2. CPU 进入 `mtvec -> _isr_wrapper`
3. `_isr_wrapper` 读取 `mcause`
4. `_isr_wrapper` 调用 `__soc_handle_irq(mcause)`
5. 对于 `MEXT`，AS32X601 SoC glue 从 PLIC claim 中断号
6. SoC glue 结合 Zephyr `_sw_isr_table` 调用对应 ISR
7. ISR 返回后执行 PLIC complete
8. `_isr_wrapper` 完成上下文恢复并返回

## 6. _isr_wrapper 的职责

文件：

- `zephyr/arch/riscv/core/isr.S`

当前由 Zephyr 统一负责：

- 保存/恢复异常现场
- 判断 interrupt / exception
- 维护 nested IRQ 计数
- 调用 SoC 钩子 `__soc_handle_irq`
- 与 Zephyr 调度器协同
- 异常退出恢复 `mepc/mstatus`

这也是为什么当前不能再让 HAL `TrapEntry` 作为主入口：

因为 `_isr_wrapper` 已经承担了完整的 Zephyr 架构职责。

## 7. __soc_handle_irq 的职责

当前实际实现位置：

- `zephyr/soc/ansilic/as32x601/soc.c`

当前设计原则：

- `__soc_handle_irq()` 只做 **SoC glue**
- 不再承担完整 trap 分发逻辑
- 不再作为总入口

当前实现逻辑：

1. 判断 `mcause`
2. 如果是 `RISCV_IRQ_MEXT`
   - 调用 `riscv_plic_get_irq()` claim 本地 IRQ
   - 转换成 Zephyr IRQ 编号
   - 查 `_sw_isr_table`
   - 调用对应 ISR
   - 调用 `riscv_plic_irq_complete()` complete

## 8. PLIC 驱动链路

当前驱动文件：

- `zephyr/drivers/interrupt_controller/intc_as32x601_plic.c`

### 8.1 当前设计原则

当前采用 **Zephyr 主链 + HAL 薄封装** 模式：

- Zephyr 负责 IRQ 架构抽象
- HAL 负责 PLIC 寄存器访问/配置细节

### 8.2 当前驱动职责

`intc_as32x601_plic.c` 当前负责：

- `arch_irq_enable()`
- `arch_irq_disable()`
- `arch_irq_is_enabled()`
- `z_riscv_irq_priority_set()`
- `riscv_plic_irq_enable()`
- `riscv_plic_irq_disable()`
- `riscv_plic_get_irq()`
- `riscv_plic_irq_complete()`

### 8.3 HAL 复用边界

当前允许复用 HAL 的部分：

- `PLIC_InitTypeDef`
- `PLIC_StructInit()`
- `PLIC_Init()`
- `PLIC->AXI_PLIC_EN[]`
- `PLIC->AXI_PLIC_PRIORITY[]`
- `PLIC->AXI_PLIC_ID`

当前 **不作为主链使用** 的 HAL 部分：

- `TrapEntry`
- `PLIC_TrapHandler()`
- `PLIC_SwitchMEXTI()`
- `PLIC_IRQHandlers[]`

原因：这些属于 HAL 自己的 trap/dispatch 体系，会与 Zephyr `_isr_wrapper` 冲突。

## 9. 为什么历史文件会造成误解

仓库中仍保留了：

- `zephyr/soc/ansilic/as32x601/vector.S`
- `zephyr/soc/ansilic/as32x601/soc_irq.S`
- HAL 中的 `as32x601_trapentry_gcc.S`
- HAL 中的 `PLIC_TrapHandler()`

这些文件容易让人误以为当前仍是：

- `mtvec -> HAL TrapEntry -> HAL PLIC_TrapHandler`

但根据当前 `CMakeLists.txt` 与最终链接结果，实际并不是这样。

## 10. 当前实际生效结论

### 启动入口

- 生效：`zephyr/soc/common/riscv-privileged/vector.S`
- 未生效：`zephyr/soc/ansilic/as32x601/vector.S`

### SoC IRQ glue

- 生效：`zephyr/soc/common/riscv-privileged/soc_irq.S`
- 生效扩展：`zephyr/soc/ansilic/as32x601/soc.c::__soc_handle_irq`
- 未生效：`zephyr/soc/ansilic/as32x601/soc_irq.S`

### trap 主入口

- 生效：`_isr_wrapper`
- 未作为主入口生效：HAL `TrapEntry`

## 11. 当前 UART 验证状态

在切换到方案 1 后，UART 工程已重新编译通过，说明：

- 启动链路构建正常
- trap / system timer / PLIC glue 构建未破坏串口工程

产物位于：

- `build-as32x601-uart-verify/zephyr/zephyr.elf`
- `build-as32x601-uart-verify/zephyr/zephyr.bin`
- `build-as32x601-uart-verify/zephyr/zephyr.hex`

## 12. 后续建议

后续建议按以下顺序继续：

1. 上板重新确认 UART 输出是否正常
2. 确认 machine timer 延时是否准确
3. 补齐 multilevel IRQ 配置
4. 完成专门的 PLIC/外设中断测试工程

