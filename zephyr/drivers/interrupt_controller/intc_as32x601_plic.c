/* SPDX-License-Identifier: Apache-2.0 */
#define DT_DRV_COMPAT ansilic_as32x601_plic

#include <errno.h>

#include <zephyr/device.h>
#include <zephyr/drivers/interrupt_controller/riscv_plic.h>
#include <zephyr/init.h>
#include <zephyr/irq.h>
#include <zephyr/arch/riscv/csr.h>
#include <zephyr/arch/riscv/irq.h>
#include <zephyr/irq_multilevel.h>
#include <zephyr/sys/util_macro.h>

#include <as32x601_plic.h>
#include <soc.h>

#define AS32X601_PLIC_MAX_IRQ          63U
#define AS32X601_PLIC_PRIO_MAX         63U

struct as32x601_plic_cfg {
	uint32_t num_irqs;
};

static uint8_t as32x601_plic_prio[AS32X601_PLIC_MAX_IRQ + 1U];
static bool as32x601_plic_enabled[AS32X601_PLIC_MAX_IRQ + 1U];
static PLIC_EXTITypeDef as32x601_plic_exti;

static ALWAYS_INLINE uint32_t plic_local_irq(uint32_t irq)
{
#if defined(CONFIG_MULTI_LEVEL_INTERRUPTS)
	if (irq_get_level(irq) == 2U) {
		return irq_from_level_2(irq);
	}
#endif
	return irq & 0xffU;
}

static ALWAYS_INLINE bool plic_irq_valid(uint32_t irq)
{
	return (irq > 0U) && (irq <= AS32X601_PLIC_MAX_IRQ);
}

static ALWAYS_INLINE bool plic_irq_encoded(uint32_t irq)
{
	return irq > RISCV_IRQ_MEXT;
}

static ALWAYS_INLINE PLICIRQn_TypeDef plic_hal_irq(uint32_t irq)
{
	return (PLICIRQn_TypeDef)plic_local_irq(irq);
}

static ALWAYS_INLINE void plic_apply(uint32_t irq, uint32_t prio, FunctionalState enable)
{
	PLIC_InitTypeDef init;

	PLIC_StructInit(&init);
	init.PLIC_IRQChannel = plic_hal_irq(irq);
	init.PLIC_IRQPriority = MIN(prio, AS32X601_PLIC_PRIO_MAX);
	init.PLIC_IRQChannelCmd = enable;
	PLIC_Init(&init);
}

void riscv_plic_irq_enable(uint32_t irq)
{
	uint32_t local_irq = plic_local_irq(irq);

	if (!plic_irq_valid(local_irq)) {
		return;
	}

	plic_apply(local_irq, as32x601_plic_prio[local_irq], ENABLE);
	as32x601_plic_enabled[local_irq] = true;
}

void arch_irq_enable(unsigned int irq)
{
	if (plic_irq_encoded(irq)) {
		riscv_plic_irq_enable(irq);
		return;
	}

	csr_set(mie, BIT(irq));
}

void arch_irq_disable(unsigned int irq)
{
	if (plic_irq_encoded(irq)) {
		riscv_plic_irq_disable(irq);
		return;
	}

	csr_clear(mie, BIT(irq));
}

int arch_irq_is_enabled(unsigned int irq)
{
	if (plic_irq_encoded(irq)) {
		return riscv_plic_irq_is_enabled(irq);
	}

	return (csr_read(mie) & BIT(irq)) != 0U;
}

void z_riscv_irq_priority_set(unsigned int irq, unsigned int prio, uint32_t flags)
{
	ARG_UNUSED(flags);

	if (plic_irq_encoded(irq)) {
		riscv_plic_set_priority(irq, prio);
	}
}

void riscv_plic_irq_disable(uint32_t irq)
{
	uint32_t local_irq = plic_local_irq(irq);

	if (!plic_irq_valid(local_irq)) {
		return;
	}

	plic_apply(local_irq, as32x601_plic_prio[local_irq], DISABLE);
	as32x601_plic_enabled[local_irq] = false;
}

int riscv_plic_irq_is_enabled(uint32_t irq)
{
	uint32_t local_irq = plic_local_irq(irq);

	if (!plic_irq_valid(local_irq)) {
		return 0;
	}

	return as32x601_plic_enabled[local_irq] ? 1 : 0;
}

void riscv_plic_set_priority(uint32_t irq, uint32_t prio)
{
	uint32_t local_irq = plic_local_irq(irq);

	if (!plic_irq_valid(local_irq)) {
		return;
	}

	if (prio > AS32X601_PLIC_PRIO_MAX) {
		prio = AS32X601_PLIC_PRIO_MAX;
	}

	as32x601_plic_prio[local_irq] = prio;
	plic_apply(local_irq, prio, riscv_plic_irq_is_enabled(irq) ? ENABLE : DISABLE);
}

unsigned int riscv_plic_get_irq(void)
{
	uint32_t irq = PLIC->AXI_PLIC_ID;
	return irq;
}

void riscv_plic_irq_complete(uint32_t irq)
{
	uint32_t local_irq = plic_local_irq(irq);

	if (!plic_irq_valid(local_irq)) {
		return;
	}

	PLIC->AXI_PLIC_ID = local_irq;
}

const struct device *riscv_plic_get_dev(void)
{
	return DEVICE_DT_INST_GET(0);
}

int riscv_plic_irq_set_affinity(uint32_t irq, uint32_t cpumask)
{
	ARG_UNUSED(irq);
	ARG_UNUSED(cpumask);
	return -ENOTSUP;
}

void riscv_plic_irq_set_pending(uint32_t irq)
{
	ARG_UNUSED(irq);
}

static void as32x601_plic_mext_dispatch(uint32_t irq)
{
	if (!plic_irq_valid(irq)) {
		return;
	}

	as32x601_plic_exti.IDBuffer[as32x601_plic_exti.Index++] = (uint8_t)irq;

	switch (irq) {
	case TIM0_IRQn:
		TIM0_IRQ_Handler();
		break;
	case GPIOA_IRQn:
		GPIOA_IRQ_Handler();
		break;
	case GPIOB_IRQn:
		GPIOB_IRQ_Handler();
		break;
	case GPIOE_IRQn:
		GPIOE_IRQ_Handler();
		break;
	case GPIOG_IRQn:
		GPIOG_IRQ_Handler();
		break;
	case MAC_IRQn:
		MAC_IRQ_Handler();
		break;
	case USART0_IRQn:
		USART0_IRQ_Handler();
		break;
	default:
		break;
	}
	PLIC->AXI_PLIC_ID = (uint32_t)as32x601_plic_exti.IDBuffer[--as32x601_plic_exti.Index];
}

void as32x601_plic_handle_mext(void)
{
	uint32_t irq = riscv_plic_get_irq();

	if (irq == 0U) {
		return;
	}

	as32x601_plic_mext_dispatch(irq);
}

static int as32x601_plic_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	for (uint32_t irq = 1; irq <= AS32X601_PLIC_MAX_IRQ; irq++) {
		as32x601_plic_prio[irq] = 0U;
		as32x601_plic_enabled[irq] = false;
		plic_apply(irq, 0U, DISABLE);
	}

	as32x601_plic_exti.Index = 0U;

	return 0;
}

static const struct as32x601_plic_cfg as32x601_plic_cfg_0 = {
	.num_irqs = DT_INST_PROP(0, riscv_ndev),
};

DEVICE_DT_INST_DEFINE(0, as32x601_plic_init, NULL, NULL, &as32x601_plic_cfg_0,
		      PRE_KERNEL_1, CONFIG_INTC_INIT_PRIORITY, NULL);
