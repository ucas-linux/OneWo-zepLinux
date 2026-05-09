#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>

#define ADC_VREF_MV  3300U
#define ADC_MAX_VAL  4095U
#define ADC_CHANNELS 8U

static const struct spi_dt_spec adc_spi = SPI_DT_SPEC_GET(
	DT_NODELABEL(adc128s),
	SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER,
	0);

static uint16_t adc_read_channel(uint8_t ch)
{
	uint8_t tx[2] = {(uint8_t)(ch << 3), 0x00U};
	uint8_t rx[2] = {0U, 0U};
	struct spi_buf tx_buf = { .buf = tx, .len = 2U };
	struct spi_buf rx_buf = { .buf = rx, .len = 2U };
	struct spi_buf_set tx_set = { .buffers = &tx_buf, .count = 1U };
	struct spi_buf_set rx_set = { .buffers = &rx_buf, .count = 1U };

	spi_transceive_dt(&adc_spi, &tx_set, &rx_set);

	return (uint16_t)(((uint16_t)(rx[0] & 0x0FU) << 8U) | rx[1]);
}

int main(void)
{
	if (!spi_is_ready_dt(&adc_spi)) {
		printk("SPI device not ready\n");
		return -1;
	}

	while (1) {
		for (uint8_t ch = 0U; ch < ADC_CHANNELS; ch++) {
			uint16_t raw = adc_read_channel(ch);
			uint32_t mv = (uint32_t)raw * ADC_VREF_MV / ADC_MAX_VAL;

			printk("CH%u: %u.%03uV  ", ch, mv / 1000U, mv % 1000U);
		}
		printk("\n");
		k_msleep(500);
	}

	return 0;
}
