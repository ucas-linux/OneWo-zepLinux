/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define ADC_SAMPLE_INTERVAL_MS 500

static const struct adc_dt_spec adc_channels[] = {
	ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
	ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
};

int main(void)
{
	uint16_t sample_buffer[ARRAY_SIZE(adc_channels)];
	int ret;

	printk("AS32X601 ADC1 demo start\n");

	for (size_t i = 0; i < ARRAY_SIZE(adc_channels); i++) {
		if (!adc_is_ready_dt(&adc_channels[i])) {
			printk("ADC channel %u device not ready\n",
			       adc_channels[i].channel_id + 1U);
			return -ENODEV;
		}

		ret = adc_channel_setup_dt(&adc_channels[i]);
		if (ret != 0) {
			printk("ADC channel %u setup failed (%d)\n",
			       adc_channels[i].channel_id + 1U, ret);
			return ret;
		}
	}

	while (1) {
		struct adc_sequence sequence = {.options = NULL};

		ret = adc_sequence_init_dt(&adc_channels[0], &sequence);
		if (ret != 0) {
			printk("ADC sequence init failed (%d)\n", ret);
			return ret;
		}

		sequence.channels = BIT(adc_channels[0].channel_id) |
				    BIT(adc_channels[1].channel_id);
		sequence.buffer = sample_buffer;
		sequence.buffer_size = sizeof(sample_buffer);

		ret = adc_read(adc_channels[0].dev, &sequence);
		if (ret != 0) {
			printk("ADC read failed (%d)\n", ret);
		} else {
			printk("ADC1 channel 15 raw: %u, channel 16 raw: %u\n",
			       sample_buffer[0], sample_buffer[1]);
		}

		k_msleep(ADC_SAMPLE_INTERVAL_MS);
	}

	return 0;
}
