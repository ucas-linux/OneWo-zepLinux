/* SPDX-License-Identifier: Apache-2.0 */
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/arch/riscv/csr.h>
#include <zephyr/arch/riscv/arch.h>
#include <zephyr/irq.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/arch/riscv/irq.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/printk.h>
#include <soc.h>

#include <as32x601_smu.h>
#include <as32x601_flash.h>
#include <as32x601_clint_regs.h>
#include <as32x601_plic.h>

void as32x601_plic_handle_mext(void);

void soc_reset_hook(void)
{
#ifdef CONFIG_FPU
	csr_set(mstatus, MSTATUS_FS_INIT);
	__asm__ volatile ("fscsr zero");
#endif

	csr_write(mie, 0);
	csr_write(mip, 0);
}

static void as32x601_bus_clock_init(void)
{
	AXIBUS3_CLK_ENABLE();
    AXIBUS3_SET();
    AXI4TOAPB0_CLK_ENABLE();
    AXI4TOAPB0_SET();
    APBBUS0_CLK_ENABLE();
	APBBUS0_SET();
	AXI4TOAPB1_CLK_ENABLE();
    AXI4TOAPB1_SET();
    APBBUS1_CLK_ENABLE();
    APBBUS1_SET();
    AXILITEBUS1_CLK_ENABLE();
    AXILITEBUS1_SET();
    AXILITEBUS2_CLK_ENABLE();
    AXILITEBUS2_SET();
    EFLASH_CLK_ENABLE();   
    EFLASH_SET();
    PLIC_CLK_ENABLE();
    PLIC_SET();
    CLINT_CLK_ENABLE();
    CLINT_SET();
}

static void as32x601_clock_init(void)
{
	SMU_PLLInitTypeDef pll_init;
	SMU_ClockInitTypeDef clock_init;

	as32x601_bus_clock_init();

	SMU_PLLStructInit(&pll_init);
	pll_init.OscillatorType = SMU_OSCILLATORTYPE_OSC;
	pll_init.FIRCOscState = DISABLE;
	pll_init.FIRCCalibrationValue = 0x00;
	pll_init.PLLConfig.PLLState = ENABLE;
	pll_init.PLLConfig.PLLSource = SMU_PLLCLK_OSC;
	/*
	 * PLLR feeds CANCLKx2 only (system clock derives from PLLQ), so PLLDivR
	 * can be tuned for CAN without disturbing the 180MHz system clock.
	 * VCO = (20MHz/N=20)*F=180 = 180MHz; PLLR = VCO/R=3 = 60MHz.
	 * With CANX2CLKDiv1 => CANCLKx2 = 60MHz, real CAN timing clock = 30MHz,
	 * which divides evenly into 500k (x60) and 2M (x15). The previous
	 * R=4/div8 gave CANCLKx2=5.625MHz, unable to produce any standard rate.
	 */
	pll_init.PLLConfig.PLLDivR = 0x03;
	pll_init.PLLConfig.PLLDivQ = 0x01;
	pll_init.PLLConfig.PLLDivN = 0x14;
	pll_init.PLLConfig.PLLDivF = 0xB4;
	SMU_PLLInit(&pll_init);

	FLASH_UnlockCtrl();
	FLASH_SetCLKFreq(0xB4);

	SMU_ClockStructInit(&clock_init);
	clock_init.SYSCLKSelect = SMU_SYSCLK_PLL;
	clock_init.AXI4Bus3CLKDiv = AXI4Bus3CLKDiv2;
	clock_init.APBBus0CLKDiv = APBBus0CLKDiv2;
	clock_init.APBBus1CLKDiv = APBBus1CLKDiv2;
	clock_init.CANX2CLKDiv = CANX2CLKDiv1;
	SMU_ClockInit(&clock_init);
	SMU_GetClocksFreq(&SMU_ClocksStruct);

	EFLASH_CLK_UPDATE_ENABLE();
	EFLASH_CLK_UPDATE_DISABLE();
	FLASH_LockCtrl();
}

void soc_prep_hook(void)
{
}

void arch_cpu_idle(void)
{
	sys_trace_idle();
	sys_trace_idle_exit();
	irq_unlock(MSTATUS_IEN);
}

void soc_early_init_hook(void)
{
	as32x601_bus_clock_init();
}

#ifdef CONFIG_RISCV_SOC_INTERRUPT_INIT
void soc_interrupt_init(void)
{
	csr_write(mie, 0);
	csr_write(mip, 0);
	irq_enable(RISCV_IRQ_MEXT);
	irq_enable(RISCV_IRQ_MSOFT);
}

#endif

static int as32x601_irq_post_kernel_init(void)
{
	return 0;
}

static int as32x601_clock_post_kernel_init(void)
{
	as32x601_clock_init();
	return 0;
}

SYS_INIT(as32x601_clock_post_kernel_init, PRE_KERNEL_1, 0);
SYS_INIT(as32x601_irq_post_kernel_init, PRE_KERNEL_2, 41);

void as32x601_debug_mext_claim(uint32_t claim)
{
	ARG_UNUSED(claim);
}

void as32x601_debug_mext_gpio_hit(uint32_t claim)
{
	ARG_UNUSED(claim);
}

void as32x601_debug_mext_entry(uint32_t mie, uint32_t mip, uint32_t mstatus)
{
	ARG_UNUSED(mie);
	ARG_UNUSED(mip);
	ARG_UNUSED(mstatus);
}

void as32x601_handle_irq_c(uint32_t mcause)
{
	ARG_UNUSED(mcause);
}

void as32x601_mext_posthandle_isr(const void *arg)
{
	ARG_UNUSED(arg);
	as32x601_plic_handle_mext();
}

Z_ISR_DECLARE(RISCV_IRQ_MEXT, 0, as32x601_mext_posthandle_isr, 0);
