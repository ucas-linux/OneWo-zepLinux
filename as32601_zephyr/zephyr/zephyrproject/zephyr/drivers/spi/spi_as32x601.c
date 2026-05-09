/* SPDX-License-Identifier: Apache-2.0 */

#define DT_DRV_COMPAT ansilic_as32x601_spi

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(spi_as32x601, CONFIG_SPI_LOG_LEVEL);

#include <as32x601_smu.h>
#include <as32x601_spi.h>

#include "spi_context.h"

struct spi_as32x601_config {
	SPI_TypeDef *spi;
	uint32_t clock_freq;
	const struct pinctrl_dev_config *pcfg;
};

struct spi_as32x601_data {
	struct spi_context ctx;
};

static uint8_t as32x601_spi_clkdiv(uint32_t pclk, uint32_t freq)
{
	uint32_t div = pclk / freq;

	if (div < 2U) {
		div = 2U;
	}
	/* round up to next power-of-2 divider supported by hardware */
	/* hardware divides by 2*(clkdiv+1), so clkdiv = div/2 - 1  */
	return (uint8_t)((div / 2U) - 1U);
}

static uint8_t as32x601_spi_motorola_mode(uint16_t operation)
{
	bool cpol = (operation & SPI_MODE_CPOL) != 0U;
	bool cpha = (operation & SPI_MODE_CPHA) != 0U;

	if (!cpol && !cpha) {
		return SPI_Motorola_Mode0;
	} else if (!cpol && cpha) {
		return SPI_Motorola_Mode1;
	} else if (cpol && !cpha) {
		return SPI_Motorola_Mode2;
	} else {
		return SPI_Motorola_Mode3;
	}
}

static int spi_as32x601_configure(const struct device *dev,
				  const struct spi_config *config)
{
	const struct spi_as32x601_config *cfg = dev->config;
	struct spi_as32x601_data *data = dev->data;
	SPI_InitTypeDef init;

	if (spi_context_configured(&data->ctx, config)) {
		return 0;
	}

	if (SPI_WORD_SIZE_GET(config->operation) != 8U) {
		return -ENOTSUP;
	}

	if (config->operation & SPI_OP_MODE_SLAVE) {
		return -ENOTSUP;
	}

	SPI_StructInit(&init);
	init.SPI_CLKDiv       = as32x601_spi_clkdiv(cfg->clock_freq, config->frequency);
	init.SPI_Mode         = SPI_Mode_Master;
	init.SPI_FrameSize    = SPI_FrameSize_8b;
	init.SPI_MotorolaMode = as32x601_spi_motorola_mode(config->operation);
	init.SPI_TransMode    = SPI_TransBToB;

	SPI_Cmd(cfg->spi, DISABLE);
	SPI_Init(cfg->spi, &init);
	SPI_ResetFIFO(cfg->spi, SPI_ResetFIFO_TxandRx);
	SPI_Cmd(cfg->spi, ENABLE);

	data->ctx.config = config;
	return 0;
}

static void spi_as32x601_send_recv(SPI_TypeDef *spi,
				   const uint8_t *tx, uint8_t *rx,
				   size_t len)
{
	for (size_t i = 0U; i < len; i++) {
		uint8_t tx_byte = (tx != NULL) ? tx[i] : 0x00U;

		while (SPI_GetFlagStatus(spi, SPI_Flag_TXF) == SET) {
		}

		if (i == len - 1U) {
			SPI_SendDataLast(spi, tx_byte);
		} else {
			SPI_SendData(spi, tx_byte);
		}

		while (SPI_GetFlagStatus(spi, SPI_Flag_RXE) == SET) {
		}

		uint8_t rx_byte = (uint8_t)SPI_ReceiveData(spi);

		if (rx != NULL) {
			rx[i] = rx_byte;
		}
	}
}

static int spi_as32x601_transceive(const struct device *dev,
				   const struct spi_config *config,
				   const struct spi_buf_set *tx_bufs,
				   const struct spi_buf_set *rx_bufs)
{
	const struct spi_as32x601_config *cfg = dev->config;
	struct spi_as32x601_data *data = dev->data;
	int ret;

	spi_context_lock(&data->ctx, false, NULL, NULL, config);

	ret = spi_as32x601_configure(dev, config);
	if (ret != 0) {
		spi_context_release(&data->ctx, ret);
		return ret;
	}

	spi_context_cs_control(&data->ctx, true);

	size_t tx_count = (tx_bufs != NULL) ? tx_bufs->count : 0U;
	size_t rx_count = (rx_bufs != NULL) ? rx_bufs->count : 0U;
	size_t buf_count = MAX(tx_count, rx_count);

	for (size_t i = 0U; i < buf_count; i++) {
		const uint8_t *tx = NULL;
		uint8_t *rx = NULL;
		size_t len = 0U;

		if (i < tx_count && tx_bufs->buffers[i].buf != NULL) {
			tx  = tx_bufs->buffers[i].buf;
			len = tx_bufs->buffers[i].len;
		}
		if (i < rx_count && rx_bufs->buffers[i].buf != NULL) {
			rx  = rx_bufs->buffers[i].buf;
			if (rx_bufs->buffers[i].len > len) {
				len = rx_bufs->buffers[i].len;
			}
		}

		if (len > 0U) {
			spi_as32x601_send_recv(cfg->spi, tx, rx, len);
		}
	}

	spi_context_cs_control(&data->ctx, false);
	spi_context_release(&data->ctx, 0);
	return 0;
}

static int spi_as32x601_release(const struct device *dev,
				const struct spi_config *config)
{
	struct spi_as32x601_data *data = dev->data;

	spi_context_unlock_unconditionally(&data->ctx);
	return 0;
}

static int spi_as32x601_init(const struct device *dev)
{
	const struct spi_as32x601_config *cfg = dev->config;
	struct spi_as32x601_data *data = dev->data;
	int ret;

	SPI4_CLK_ENABLE();

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret < 0) {
		return ret;
	}

	ret = spi_context_cs_configure_all(&data->ctx);
	if (ret < 0) {
		return ret;
	}

	spi_context_unlock_unconditionally(&data->ctx);
	return 0;
}

static DEVICE_API(spi, spi_as32x601_api) = {
	.transceive = spi_as32x601_transceive,
	.release    = spi_as32x601_release,
};

#define SPI_AS32X601_INIT(inst)							\
	PINCTRL_DT_INST_DEFINE(inst);						\
	static const struct spi_as32x601_config spi_as32x601_config_##inst = {	\
		.spi        = (SPI_TypeDef *)DT_INST_REG_ADDR(inst),		\
		.clock_freq = DT_INST_PROP(inst, clock_frequency),		\
		.pcfg       = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),		\
	};									\
	static struct spi_as32x601_data spi_as32x601_data_##inst = {		\
		SPI_CONTEXT_INIT_LOCK(spi_as32x601_data_##inst, ctx),		\
		SPI_CONTEXT_INIT_SYNC(spi_as32x601_data_##inst, ctx),		\
		SPI_CONTEXT_CS_GPIOS_INITIALIZE(DT_DRV_INST(inst), ctx)	\
	};									\
	DEVICE_DT_INST_DEFINE(inst, spi_as32x601_init, NULL,			\
		&spi_as32x601_data_##inst, &spi_as32x601_config_##inst,		\
		POST_KERNEL, CONFIG_SPI_INIT_PRIORITY,				\
		&spi_as32x601_api);

DT_INST_FOREACH_STATUS_OKAY(SPI_AS32X601_INIT)
