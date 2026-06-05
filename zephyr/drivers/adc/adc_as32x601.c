/* SPDX-License-Identifier: Apache-2.0 */

#define DT_DRV_COMPAT ansilic_as32x601_adc

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include <as32x601_adc.h>
#include <as32x601_smu.h>
#include <soc.h>
#include <zephyr/drivers/pinctrl.h>

LOG_MODULE_REGISTER(adc_as32x601, CONFIG_ADC_LOG_LEVEL);

#define AS32X601_ADC_MAX_CHANNELS       21U
#define AS32X601_ADC_REGULAR_SEQ_MAX    16U
#define AS32X601_ADC_TIMEOUT_US         100000U
#define AS32X601_ADC_POLL_INTERVAL_US   10U

struct as32x601_adc_config {
	ADC_TypeDef *regs;
	uint32_t reg_addr;
	uint32_t clock_divide;
	uint32_t sample_time;
	const struct pinctrl_dev_config *pcfg;
};

struct as32x601_adc_channel {
	bool configured;
	uint8_t input;
};

struct as32x601_adc_data {
	struct k_mutex lock;
	struct as32x601_adc_channel channels[AS32X601_ADC_MAX_CHANNELS];
};

static uint32_t as32x601_adc_resolution_to_hal(uint8_t resolution)
{
	switch (resolution) {
	case 12:
		return ADC_RESOLUTION_12BIT;
	case 10:
		return ADC_RESOLUTION_10BIT;
	case 8:
		return ADC_RESOLUTION_8BIT;
	case 6:
		return ADC_RESOLUTION_6BIT;
	default:
		return ADC_RESOLUTION_MAX;
	}
}

static int as32x601_adc_enable_clock(uint32_t reg_addr)
{
	switch (reg_addr) {
	case AS32X601_ADC0_BASE:
		ADC0_CLK_ENABLE();
		return 0;
	case AS32X601_ADC1_BASE:
		ADC1_CLK_ENABLE();
		return 0;
	default:
		return -EINVAL;
	}
}

static uint32_t as32x601_adc_channel_to_hal(uint8_t channel_id)
{
	return channel_id;
}

static uint8_t as32x601_adc_count_channels(uint32_t mask)
{
	return POPCOUNT(mask);
}

static int as32x601_adc_channel_setup(const struct device *dev,
				      const struct adc_channel_cfg *channel_cfg)
{
	struct as32x601_adc_data *data = dev->data;
	uint8_t channel_id = channel_cfg->channel_id;

	if (channel_id >= AS32X601_ADC_MAX_CHANNELS) {
		return -EINVAL;
	}

	if (channel_cfg->differential) {
		return -ENOTSUP;
	}

	if (channel_cfg->gain != ADC_GAIN_1) {
		return -ENOTSUP;
	}

	switch (channel_cfg->reference) {
	case ADC_REF_VDD_1:
		break;
	default:
		return -ENOTSUP;
	}

	if (channel_cfg->acquisition_time != ADC_ACQ_TIME_DEFAULT) {
		return -ENOTSUP;
	}

	k_mutex_lock(&data->lock, K_FOREVER);
	data->channels[channel_id].configured = true;
	data->channels[channel_id].input = channel_id;
	k_mutex_unlock(&data->lock);

	return 0;
}

static void as32x601_adc_apply_config(const struct as32x601_adc_config *cfg,
				      uint32_t resolution, uint8_t sequence_len)
{
	ADC_InitTypeDef init;

	ADC_StructInit(&init);
	init.ADC_ClockDivide = cfg->clock_divide;
	init.ADC_Resolution = resolution;
	init.ADC_Alignment = ADC_DATA_ALIGN_RIGHT;
	init.ADC_Symbol = ADC_DATA_SELDO_UNSIGNED;
	init.ADC_RegularTrigger = ADC_TRIGGER_INTERNAL;
	init.ADC_InjectTrigger = ADC_TRIGGER_INTERNAL;
	init.ADC_RegularEnable = ENABLE;
	init.ADC_InjectEnable = DISABLE;
	init.ADC_DmaEnable = DISABLE;
	init.ADC_SelvihdlsRef = ADC_SAR_INPUT_SINGLE_ENDED;
	init.ADC_ScanModeEn = (sequence_len > 1U) ? ENABLE : DISABLE;
	init.ADC_ContinuousModeEn = DISABLE;
	init.ADC_RegularDiscontinuousModeEn = DISABLE;
	init.ADC_InjectDiscontinuousModeEn = DISABLE;
	init.ADC_InjectAutoModeEn = DISABLE;
	init.ADC_IntervalModeEn = DISABLE;
	init.ADC_RegularDiscontinuousNum = 0U;
	init.ADC_RegularSequenceLength = sequence_len;
	init.ADC_InjectSequenceLength = 1U;
	init.ADC_PowerEn = ENABLE;
	init.ADC_PowerRegain = ENABLE;

	ADC_Init(cfg->regs, &init);
}

static int as32x601_adc_configure_slots(const struct device *dev,
					const struct adc_sequence *sequence,
					uint8_t sequence_len)
{
	const struct as32x601_adc_config *cfg = dev->config;
	struct as32x601_adc_data *data = dev->data;
	uint8_t slot = 0U;

	for (uint8_t channel_id = 0U; channel_id < AS32X601_ADC_MAX_CHANNELS; channel_id++) {
		ADC_ChannelTypeDef channel_cfg;

		if ((sequence->channels & BIT(channel_id)) == 0U) {
			continue;
		}

		if (!data->channels[channel_id].configured) {
			return -EINVAL;
		}

		if (slot >= AS32X601_ADC_REGULAR_SEQ_MAX || slot >= sequence_len) {
			return -EINVAL;
		}

		ADC_InitChannelStruct(&channel_cfg);
		channel_cfg.ADC_Channel = as32x601_adc_channel_to_hal(data->channels[channel_id].input);
		channel_cfg.ADC_SampleTime = cfg->sample_time;
		channel_cfg.ADC_InterruptEn = DISABLE;

		ADC_ConfigChannel(cfg->regs, slot, &channel_cfg);
		slot++;
	}

	return 0;
}

static void as32x601_adc_clear_slots(const struct as32x601_adc_config *cfg, uint8_t slot_count)
{
	for (uint8_t slot = 0U; slot < slot_count; slot++) {
		ADC_ClearFlag(cfg->regs, ADC_FLAG_EOC, slot);
	}
}

static int as32x601_adc_read_slots(const struct as32x601_adc_config *cfg, uint16_t *buffer,
				   uint8_t sequence_len)
{
	for (uint8_t slot = 0U; slot < sequence_len; slot++) {
		uint32_t elapsed_us = 0U;
		uint16_t value;

		while (ADC_GetFlagStatus(cfg->regs, ADC_FLAG_EOC, slot) != SET) {
			if (elapsed_us >= AS32X601_ADC_TIMEOUT_US) {
				as32x601_adc_clear_slots(cfg, sequence_len);
				return -ETIMEDOUT;
			}

			k_busy_wait(AS32X601_ADC_POLL_INTERVAL_US);
			elapsed_us += AS32X601_ADC_POLL_INTERVAL_US;
		}

		(void)ADC_GetSeqResult(cfg->regs, slot, &value);
		ADC_ClearFlag(cfg->regs, ADC_FLAG_EOC, slot);
		buffer[slot] = value;
	}

	return 0;
}

static int as32x601_adc_read(const struct device *dev, const struct adc_sequence *sequence)
{
	const struct as32x601_adc_config *cfg = dev->config;
	struct as32x601_adc_data *data = dev->data;
	uint32_t resolution;
	uint8_t slot_count;
	int ret;

	if (sequence->options != NULL) {
		return -ENOTSUP;
	}

	if (sequence->oversampling != 0U) {
		return -ENOTSUP;
	}

	resolution = as32x601_adc_resolution_to_hal(sequence->resolution);
	if (resolution == ADC_RESOLUTION_MAX) {
		return -EINVAL;
	}

	if ((sequence->channels & ~BIT_MASK(AS32X601_ADC_MAX_CHANNELS)) != 0U) {
		return -EINVAL;
	}

	slot_count = as32x601_adc_count_channels(sequence->channels);
	if (slot_count == 0U || slot_count > AS32X601_ADC_REGULAR_SEQ_MAX) {
		return -EINVAL;
	}

	if (sequence->buffer == NULL) {
		return -EINVAL;
	}

	if (sequence->buffer_size < (sizeof(uint16_t) * slot_count)) {
		return -ENOMEM;
	}

	k_mutex_lock(&data->lock, K_FOREVER);

	as32x601_adc_apply_config(cfg, resolution, slot_count);

	ret = as32x601_adc_configure_slots(dev, sequence, slot_count);
	if (ret == 0) {
		as32x601_adc_clear_slots(cfg, slot_count);
		ADC_SoftRegularConvert(cfg->regs);
		ret = as32x601_adc_read_slots(cfg, sequence->buffer, slot_count);
	}

	k_mutex_unlock(&data->lock);

	return ret;
}

#ifdef CONFIG_ADC_ASYNC
static int as32x601_adc_read_async(const struct device *dev,
				   const struct adc_sequence *sequence,
				   struct k_poll_signal *async)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(sequence);
	ARG_UNUSED(async);

	return -ENOTSUP;
}
#endif /* CONFIG_ADC_ASYNC */

static int as32x601_adc_init(const struct device *dev)
{
	const struct as32x601_adc_config *cfg = dev->config;
	struct as32x601_adc_data *data = dev->data;
	int ret;

	ret = as32x601_adc_enable_clock(cfg->reg_addr);
	if (ret != 0) {
		return ret;
	}

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret < 0 && ret != -ENOENT) {
		return ret;
	}

	k_mutex_init(&data->lock);
	as32x601_adc_apply_config(cfg, ADC_RESOLUTION_12BIT, 1U);

	return 0;
}

static DEVICE_API(adc, as32x601_adc_api) = {
	.channel_setup = as32x601_adc_channel_setup,
	.read = as32x601_adc_read,
#ifdef CONFIG_ADC_ASYNC
	.read_async = as32x601_adc_read_async,
#endif
};

#define AS32X601_ADC_INIT(inst)                                                                   \
	PINCTRL_DT_INST_DEFINE(inst);                                                              \
	static struct as32x601_adc_data as32x601_adc_data_##inst;                                  \
	static const struct as32x601_adc_config as32x601_adc_config_##inst = {                     \
		.regs = (ADC_TypeDef *)DT_INST_REG_ADDR(inst),                                     \
		.reg_addr = DT_INST_REG_ADDR(inst),                                                \
		.clock_divide = ADC_CLK_DIVIDE_4,                                                  \
		.sample_time = ADC_SPT_CLK_3,                                                      \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                                     \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(inst, as32x601_adc_init, NULL, &as32x601_adc_data_##inst,            \
			      &as32x601_adc_config_##inst, POST_KERNEL, CONFIG_ADC_INIT_PRIORITY,  \
			      &as32x601_adc_api);

DT_INST_FOREACH_STATUS_OKAY(AS32X601_ADC_INIT)
