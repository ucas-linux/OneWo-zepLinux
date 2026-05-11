/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2024 Ansilic Technology Co., Ltd */

#include <zephyr/drivers/counter.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util.h>

#include <as32x601_plic.h>
#include <as32x601_smu.h>

#define DT_DRV_COMPAT ansilic_as32x601_timer

/* Register offsets */
#define TIM_OFF_CR1   0x00
#define TIM_OFF_DIER  0x0C
#define TIM_OFF_SR    0x10
#define TIM_OFF_CNT   0x28
#define TIM_OFF_PSC   0x2C
#define TIM_OFF_ARR   0x30

#define TIM_REG(base, off)  (*(volatile uint32_t *)((uintptr_t)(base) + (off)))

struct as32x601_timer_config {
	mm_reg_t base;
};

struct as32x601_timer_data {
	counter_top_callback_t top_cb;
	void *top_user_data;
	bool hw_initialized;
};

static void as32x601_timer_hw_init(const struct device *dev)
{
	const struct as32x601_timer_config *cfg = dev->config;
	struct as32x601_timer_data *data = dev->data;
	PLIC_InitTypeDef plic_init;

	if (data->hw_initialized) {
		return;
	}

	HTIM0_CLK_ENABLE();
	HTIM0_SET();

	TIM_REG(cfg->base, TIM_OFF_CR1)  = 0;
	TIM_REG(cfg->base, TIM_OFF_DIER) = 0;
	TIM_REG(cfg->base, TIM_OFF_SR)   = 0;

	PLIC_StructInit(&plic_init);
	plic_init.PLIC_IRQChannel = TIM0_IRQn;
	plic_init.PLIC_IRQPriority = 1U;
	plic_init.PLIC_IRQChannelCmd = ENABLE;
	PLIC_Init(&plic_init);

	data->hw_initialized = true;
}

static int as32x601_timer_start(const struct device *dev)
{
	const struct as32x601_timer_config *cfg = dev->config;

	as32x601_timer_hw_init(dev);

	TIM_REG(cfg->base, TIM_OFF_CR1) |= BIT(0);
	return 0;
}

static int as32x601_timer_stop(const struct device *dev)
{
	const struct as32x601_timer_config *cfg = dev->config;

	as32x601_timer_hw_init(dev);

	TIM_REG(cfg->base, TIM_OFF_CR1) &= ~BIT(0);
	return 0;
}

static int as32x601_timer_get_value(const struct device *dev, uint32_t *ticks)
{
	const struct as32x601_timer_config *cfg = dev->config;

	as32x601_timer_hw_init(dev);

	*ticks = TIM_REG(cfg->base, TIM_OFF_CNT);
	return 0;
}

static int as32x601_timer_set_top_value(const struct device *dev,
					const struct counter_top_cfg *top_cfg)
{
	const struct as32x601_timer_config *cfg = dev->config;
	struct as32x601_timer_data *data = dev->data;

	as32x601_timer_hw_init(dev);

	/* Stop counter while reconfiguring */
	TIM_REG(cfg->base, TIM_OFF_CR1) &= ~BIT(0);

	data->top_cb = top_cfg->callback;
	data->top_user_data = top_cfg->user_data;

	TIM_REG(cfg->base, TIM_OFF_PSC) = 0;
	TIM_REG(cfg->base, TIM_OFF_ARR) = top_cfg->ticks;
	TIM_REG(cfg->base, TIM_OFF_SR)  = 0;        /* clear any pending UIF */
	TIM_REG(cfg->base, TIM_OFF_DIER) |= BIT(0); /* UIE: update interrupt enable */

	/* Counter remains stopped; caller must invoke counter_start() */
	return 0;
}

static uint32_t as32x601_timer_get_top_value(const struct device *dev)
{
	const struct as32x601_timer_config *cfg = dev->config;

	return TIM_REG(cfg->base, TIM_OFF_ARR);
}

static void as32x601_timer_isr(const struct device *dev)
{
	const struct as32x601_timer_config *cfg = dev->config;
	struct as32x601_timer_data *data = dev->data;

	if (TIM_REG(cfg->base, TIM_OFF_SR) & BIT(0)) {
		TIM_REG(cfg->base, TIM_OFF_SR) &= ~BIT(0); /* clear UIF */
		if (data->top_cb != NULL) {
			data->top_cb(dev, data->top_user_data);
		}
	}
}

static int as32x601_timer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	return 0;
}

/* Called from intc_as32x601_plic.c switch-case dispatch */
void TIM0_IRQ_Handler(void)
{
	as32x601_timer_isr(DEVICE_DT_INST_GET(0));
}

static const struct counter_driver_api as32x601_timer_api = {
	.start         = as32x601_timer_start,
	.stop          = as32x601_timer_stop,
	.get_value     = as32x601_timer_get_value,
	.set_top_value = as32x601_timer_set_top_value,
	.get_top_value = as32x601_timer_get_top_value,
};

#define AS32X601_TIMER_INIT(n)                                                  \
	static const struct as32x601_timer_config as32x601_timer_cfg_##n = {    \
		.base = DT_INST_REG_ADDR(n),                                    \
	};                                                                      \
                                                                                \
	static struct as32x601_timer_data as32x601_timer_data_##n;             \
                                                                                \
	DEVICE_DT_INST_DEFINE(n,                                                \
		as32x601_timer_init, NULL,                                      \
		&as32x601_timer_data_##n, &as32x601_timer_cfg_##n,             \
		PRE_KERNEL_1, CONFIG_COUNTER_INIT_PRIORITY,                     \
		&as32x601_timer_api);

DT_INST_FOREACH_STATUS_OKAY(AS32X601_TIMER_INIT)
