/* SPDX-License-Identifier: Apache-2.0 */
#define DT_DRV_COMPAT ansilic_as32x601_qspi_nor

#include <errno.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include <as32x601_qspi.h>   /* vendor HAL: QSPI_*, QSPI_TypeDef */
#include <as32x601_smu.h>    /* QSPI_CLK_ENABLE */

LOG_MODULE_REGISTER(flash_as32x601_qspi, CONFIG_FLASH_LOG_LEVEL);

/* S25FL512S 特有（v1 验证序列，24 位地址） */
#define S25FL_CMD_QPP   0x32   /* Quad Page Program, 3-byte addr */
#define S25FL_CMD_SE    0xD8   /* Sector Erase 256KiB, 3-byte addr */
#define S25FL_CMD_READ  0x03   /* Read Data, 3-byte addr (mmap) */
#define S25FL_CMD_RDCR  0x35   /* Read Configuration Register-1 */
#define S25FL_CMD_WREN  0x06   /* Write Enable */
#define S25FL_CMD_WRR   0x01   /* Write Register (SR1 + CR1) */
#define S25FL_CMD_RDSR1 0x05   /* Read Status Register-1 */
#define S25FL_CR1_QUAD  BIT(1) /* 0x02 */
#define S25FL_SR1_WIP   BIT(0) /* 0x01, Write In Progress */

struct flash_as32x601_qspi_config {
	QSPI_TypeDef *qspi;             /* DT 一致性保留；HAL 实际用全局 QSPI 基址 */
	uint8_t *mmap_base;
	const struct pinctrl_dev_config *pcfg;
	uint32_t flash_size;
	uint32_t sector_size;
	uint32_t page_size;
	uint8_t prescaler;
	uint8_t cs_high_time;
};

struct flash_as32x601_qspi_data {
#if defined(CONFIG_MULTITHREADING)
	struct k_mutex lock;
#endif
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
	struct flash_pages_layout layout;
#endif
};

static const struct flash_parameters flash_as32x601_qspi_parameters = {
	.write_block_size = 1,
	.erase_value = 0xFF,
};

/* ---- 加锁助手（CONFIG_MULTITHREADING=n 时编译为空） ---- */
static inline void qspi_lock(struct flash_as32x601_qspi_data *d)
{
#if defined(CONFIG_MULTITHREADING)
	k_mutex_lock(&d->lock, K_FOREVER);
#else
	ARG_UNUSED(d);
#endif
}

static inline void qspi_unlock(struct flash_as32x601_qspi_data *d)
{
#if defined(CONFIG_MULTITHREADING)
	k_mutex_unlock(&d->lock);
#else
	ARG_UNUSED(d);
#endif
}

/* ---- 私有原语：统一返回 0 / -EIO ---- */

/* 轮询控制器 BUSY，等价 BasicProject delay_ms(10)×1000（~10s 上限） */
static int qspi_wait_not_busy(void)
{
	uint32_t count = 0;

	while (QSPI_GetFlagStatus(QSPI_FLAG_BUSY) == SET) {
		if (count++ > 1000U) {
			return -EIO;
		}
		k_busy_wait(10000); /* 10 ms */
	}
	return 0;
}

static int qspi_wait_fifo_bytes(uint8_t bytes)
{
	uint32_t count = 0;

	while (QSPI_GetFIFOLevel() < bytes) {
		if (count++ > 100000U) {
			return -EIO;
		}
		k_busy_wait(10);
	}
	return 0;
}

/* 轮询 flash WIP（RDSR1 bit0）清零，端口自 BasicProject QSPIFlash_GetFlashStatus
 * 的硬件自动轮询。控制器 BUSY 只表示移位完成，而 WRR/擦/写的 flash 内部周期需靠
 * WIP 才能确知完成——必须在这些写操作之后调用。
 */
static int qspi_wait_wip_clear(void)
{
	QSPI_CmdTypeDef cmd;
	uint32_t count = 0;

	QSPI_ClearFlag(QSPI_CLEAR_CPMF);
	QSPI_SetDataLength(1 - 1);
	QSPI_SetPollingInterval(0xff);
	QSPI_SetPollingStaMatch(0x00);          /* 匹配 WIP=0 */
	QSPI_PollingStaMask(S25FL_SR1_WIP);

	QSPI_CmdStructInit(&cmd);
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.ADMODE = QSPI_AddrMode_None;
	cmd.ADSIZE = QSPI_AddrSize_8b;
	cmd.DMODE = QSPI_DataMode_1Line;
	cmd.DUMMY = 0;
	cmd.FMODE = QSPI_FunMode_AutoPoll;
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.INST = S25FL_CMD_RDSR1;
	QSPI_CmdConfig(&cmd);

	while (QSPI_GetFlagStatus(QSPI_FLAG_PMF) == RESET) {
		if (count++ > 1000U) {
			return -EIO;
		}
		k_busy_wait(10000); /* 10 ms */
	}
	QSPI_ClearFlag(QSPI_CLEAR_CPMF);
	return qspi_wait_not_busy();
}

/* 复刻 BasicProject 每次操作前的控制器配置 */
static void qspi_controller_init_regs(const struct flash_as32x601_qspi_config *cfg)
{
	QSPI_InitTypeDef init;

	QSPI_StructInit(&init);
	init.PRESCALER = cfg->prescaler;
	/* QSPI_CSHT_n 编码为 (n-1)<<3；DT cs-high-time=7 → QSPI_CSHT_7 (0x30) */
	init.CSHT = (uint32_t)(cfg->cs_high_time - 1) << 3;
	init.CKMODE = QSPI_CKMode_0;
	QSPI_Init(&init);
	QSPI_Cmd(ENABLE);
}

/* WREN（1 线，无地址无数据），等价 QSPIFLASH_QSPIFlashEn(ENABLE) */
static void qspi_write_enable(void)
{
	QSPI_CmdTypeDef cmd;

	QSPI_CmdStructInit(&cmd);
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.ADMODE = QSPI_AddrMode_None;
	cmd.ADSIZE = QSPI_AddrSize_8b;
	cmd.DMODE = QSPI_DataMode_None;
	cmd.DUMMY = 0;
	cmd.FMODE = QSPI_FunMode_WriteIndirect;
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.INST = S25FL_CMD_WREN;
	QSPI_CmdConfig(&cmd);
}

/* RDCR 0x35：读 CR1（1 线，1 字节，ReadIndirect） */
static int qspi_read_cr1(uint8_t *cr1)
{
	QSPI_CmdTypeDef cmd;

	QSPI_SetDataLength(1 - 1);
	QSPI_CmdStructInit(&cmd);
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.ADMODE = QSPI_AddrMode_None;
	cmd.ADSIZE = QSPI_AddrSize_8b;
	cmd.DMODE = QSPI_DataMode_1Line;
	cmd.DUMMY = 0;
	cmd.FMODE = QSPI_FunMode_ReadIndirect;
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.INST = S25FL_CMD_RDCR;
	QSPI_CmdConfig(&cmd);

	/*
	 * For indirect reads the AS32X601 QSPI controller may keep BUSY set
	 * until the received byte is drained from DR. This follows the vendor
	 * BSP read path, which reads DR before waiting for the controller to
	 * become idle.
	 */
	*cr1 = (uint8_t)QSPI_ReadData();

	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}
	return 0;
}

/* WREN + WRR[SR1=0, CR1|QUAD]，写非易失 CR1 开 QUAD。等价 BasicProject
 * QSPIFLASH_OpenQUADMode 的 WRR 段（控制器配置已由 init 完成，不重复）。
 */
static int qspi_enable_quad(void)
{
	QSPI_CmdTypeDef cmd;

	qspi_write_enable();
	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}

	QSPI_CmdStructInit(&cmd);
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.ADMODE = QSPI_AddrMode_None;
	cmd.ADSIZE = QSPI_AddrSize_24b;
	cmd.DMODE = QSPI_DataMode_1Line;
	cmd.DUMMY = 0;
	cmd.FMODE = QSPI_FunMode_WriteIndirect;
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.INST = S25FL_CMD_WRR;
	QSPI_CmdConfig(&cmd);

	QSPI_SetDataLength(2 - 1);
	QSPI_SetData((uint32_t)S25FL_CR1_QUAD << 8); /* SR1=0x00, CR1=QUAD */

	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}
	/* WRR 写非易失寄存器需 ms 级，必须等 WIP 清零后才能可靠读回校验 */
	return qspi_wait_wip_clear();
}

/* SE 0xD8：擦单个 256KiB sector（1 线指令 + 24 位地址，无数据） */
static int qspi_sector_erase(const struct flash_as32x601_qspi_config *cfg, uint32_t addr)
{
	QSPI_CmdTypeDef cmd;

	qspi_controller_init_regs(cfg);
	qspi_write_enable();
	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}

	QSPI_CmdStructInit(&cmd);
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.ADMODE = QSPI_AddrMode_1Line;
	cmd.ADSIZE = QSPI_AddrSize_24b;
	cmd.DMODE = QSPI_DataMode_None;
	cmd.DUMMY = 0;
	cmd.FMODE = QSPI_FunMode_WriteIndirect;
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.INST = S25FL_CMD_SE;
	QSPI_CmdConfig(&cmd);

	QSPI_SetAddr(addr);

	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}
	/* 擦除是 flash 内部长周期（秒级），必须等 WIP 清零 */
	return qspi_wait_wip_clear();
}

/* READ 0x03：间接读，复刻 BasicProject QSPIFLASH_ReadIndirect。 */
static int qspi_read_indirect(const struct flash_as32x601_qspi_config *cfg,
			      uint32_t addr, uint8_t *buf, size_t len)
{
	QSPI_CmdTypeDef cmd;

	qspi_controller_init_regs(cfg);
	QSPI_SetDataLength(len - 1);

	QSPI_CmdStructInit(&cmd);
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.ADMODE = QSPI_AddrMode_1Line;
	cmd.ADSIZE = QSPI_AddrSize_24b;
	cmd.DMODE = QSPI_DataMode_1Line;
	cmd.DUMMY = 0;
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.INST = S25FL_CMD_READ;
	cmd.FMODE = QSPI_FunMode_ReadIndirect;
	QSPI_CmdConfig(&cmd);

	k_busy_wait(10000);
	QSPI_SetAddr(addr);
	k_busy_wait(10000);

	for (size_t i = 0; i < len; i += 4) {
		size_t chunk = MIN((size_t)4, len - i);
		uint32_t value;

		if (qspi_wait_fifo_bytes(chunk) != 0) {
			return -EIO;
		}

		value = QSPI_ReadData();
		memcpy(&buf[i], &value, chunk);
		__asm volatile("fence.i");
	}

	/*
	 * Match the vendor BSP's QSPIFLASH_ReadIndirect(): after draining the
	 * requested bytes from DR it returns directly. Waiting on BUSY here can
	 * falsely time out after a successful indirect read on this controller.
	 */
	return 0;
}

/* QPP 0x32：4 线页编程，24 位地址；len ≤ 256 且 4 字节对齐（QSPI_SetData 写 32 位字）。
 * 复刻 BasicProject QSPIFLASH_QUADProgramRom 的单页路径，写后等 WIP 清零。
 */
static int qspi_page_program(const struct flash_as32x601_qspi_config *cfg,
			     uint32_t addr, const uint8_t *buf, size_t len)
{
	QSPI_CmdTypeDef cmd;
	const uint32_t *words = (const uint32_t *)buf;

	qspi_controller_init_regs(cfg);
	qspi_write_enable();
	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}

	QSPI_CmdStructInit(&cmd);
	cmd.IMODE = QSPI_InstMode_1Line;
	cmd.ADMODE = QSPI_AddrMode_1Line;
	cmd.ADSIZE = QSPI_AddrSize_24b;
	cmd.ABMODE = QSPI_AltByteMode_None;
	cmd.ABSIZE = QSPI_AltByteSize_8b;
	cmd.DUMMY = 0;
	cmd.DMODE = QSPI_DataMode_4Lines;
	cmd.FMODE = QSPI_FunMode_WriteIndirect;
	cmd.INST = S25FL_CMD_QPP;
	QSPI_CmdConfig(&cmd);

	QSPI_SetAddr(addr);
	QSPI_SetDataLength(len - 1);
	for (size_t i = 0; i < len; i += 4) {
		QSPI_SetData(words[i / 4]);
	}

	if (qspi_wait_not_busy() != 0) {
		return -EIO;
	}
	/* 页编程是 flash 内部写周期，必须等 WIP 清零 */
	return qspi_wait_wip_clear();
}

static int flash_as32x601_qspi_read(const struct device *dev, off_t off,
				    void *data, size_t len)
{
	const struct flash_as32x601_qspi_config *cfg = dev->config;
	struct flash_as32x601_qspi_data *d = dev->data;
	int ret;

	if (len == 0) {
		return 0;
	}
	if ((off < 0) || ((uint32_t)off + len > cfg->flash_size)) {
		return -EINVAL;
	}

	qspi_lock(d);
	ret = qspi_read_indirect(cfg, (uint32_t)off, data, len);
	qspi_unlock(d);
	return ret;
}

static int flash_as32x601_qspi_write(const struct device *dev, off_t off,
				     const void *data, size_t len)
{
	const struct flash_as32x601_qspi_config *cfg = dev->config;
	struct flash_as32x601_qspi_data *d = dev->data;
	const uint8_t *p = data;
	uint32_t addr = (uint32_t)off;
	int ret = 0;

	if (len == 0) {
		return 0;
	}
	if ((off < 0) || ((uint32_t)off + len > cfg->flash_size)) {
		return -EINVAL;
	}
	/* v1：QPP 经 QSPI_SetData 以 32 位字写入，要求 4 字节对齐 */
	if ((addr % 4 != 0) || (len % 4 != 0)) {
		return -EINVAL;
	}

	qspi_lock(d);
	while (len > 0) {
		uint32_t page_off = addr % cfg->page_size;
		size_t chunk = MIN(len, (size_t)(cfg->page_size - page_off));

		ret = qspi_page_program(cfg, addr, p, chunk);
		if (ret != 0) {
			break;
		}
		addr += chunk;
		p += chunk;
		len -= chunk;
	}
	qspi_unlock(d);
	return ret;
}

static int flash_as32x601_qspi_erase(const struct device *dev, off_t off, size_t size)
{
	const struct flash_as32x601_qspi_config *cfg = dev->config;
	struct flash_as32x601_qspi_data *d = dev->data;
	uint32_t addr = (uint32_t)off;
	int ret = 0;

	if (size == 0) {
		return 0;
	}
	if ((off < 0) || ((uint32_t)off + size > cfg->flash_size)) {
		return -EINVAL;
	}
	if ((addr % cfg->sector_size != 0) || (size % cfg->sector_size != 0)) {
		return -EINVAL;
	}

	qspi_lock(d);
	while (size > 0) {
		ret = qspi_sector_erase(cfg, addr);
		if (ret != 0) {
			break;
		}
		addr += cfg->sector_size;
		size -= cfg->sector_size;
	}
	qspi_unlock(d);
	return ret;
}

static const struct flash_parameters *
flash_as32x601_qspi_get_parameters(const struct device *dev)
{
	ARG_UNUSED(dev);
	return &flash_as32x601_qspi_parameters;
}

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
static void flash_as32x601_qspi_page_layout(const struct device *dev,
					    const struct flash_pages_layout **layout,
					    size_t *layout_size)
{
	struct flash_as32x601_qspi_data *d = dev->data;

	*layout = &d->layout;
	*layout_size = 1;
}
#endif

static int flash_as32x601_qspi_init(const struct device *dev)
{
	const struct flash_as32x601_qspi_config *cfg = dev->config;
	struct flash_as32x601_qspi_data *d = dev->data;
	uint8_t cr1;
	int ret;

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret < 0) {
		LOG_ERR("pinctrl apply failed (%d)", ret);
		return ret;
	}

	/* 一次性：使能 QSPI 外设时钟 + 解锁(CR=0) + 清标志。 */
	QSPI_CLK_ENABLE();
	QSPI->CR = 0x0000U;
	QSPI_ClearFlag(QSPI_CLEAR_CTCF | QSPI_CLEAR_CTOF | QSPI_CLEAR_CPMF);

	qspi_controller_init_regs(cfg);

	ret = qspi_read_cr1(&cr1);
	if (ret != 0) {
		LOG_ERR("read CR1 failed (%d)", ret);
		return ret;
	}

	if ((cr1 & S25FL_CR1_QUAD) == 0) {
		LOG_INF("enabling QUAD mode");
		ret = qspi_enable_quad();
		if (ret != 0) {
			return ret;
		}
		ret = qspi_read_cr1(&cr1);
		if (ret != 0) {
			return ret;
		}
		if ((cr1 & S25FL_CR1_QUAD) == 0) {
			LOG_ERR("QUAD enable verify failed (CR1=0x%02x)", cr1);
			return -EIO;
		}
	}

#if defined(CONFIG_MULTITHREADING)
	k_mutex_init(&d->lock);
#endif
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
	d->layout.pages_count = cfg->flash_size / cfg->sector_size;
	d->layout.pages_size = cfg->sector_size;
#else
	ARG_UNUSED(d);
#endif
	LOG_INF("AS32X601 QSPI NOR ready (%u MiB, sector %u KiB)",
		cfg->flash_size >> 20, cfg->sector_size >> 10);
	return 0;
}

static const struct flash_driver_api flash_as32x601_qspi_api = {
	.read = flash_as32x601_qspi_read,
	.write = flash_as32x601_qspi_write,
	.erase = flash_as32x601_qspi_erase,
	.get_parameters = flash_as32x601_qspi_get_parameters,
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
	.page_layout = flash_as32x601_qspi_page_layout,
#endif
};

#define FLASH_AS32X601_QSPI_INIT(n)						\
	PINCTRL_DT_INST_DEFINE(n);						\
	static const struct flash_as32x601_qspi_config cfg_##n = {		\
		.qspi = (QSPI_TypeDef *)DT_INST_REG_ADDR_BY_NAME(n, ctrl),	\
		.mmap_base = (uint8_t *)DT_INST_REG_ADDR_BY_NAME(n, mmap),	\
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(n),			\
		.flash_size = DT_INST_PROP(n, size),				\
		.sector_size = DT_INST_PROP(n, sector_size),			\
		.page_size = DT_INST_PROP(n, page_size),			\
		.prescaler = DT_INST_PROP(n, prescaler),			\
		.cs_high_time = DT_INST_PROP(n, cs_high_time),			\
	};									\
	static struct flash_as32x601_qspi_data data_##n;			\
	DEVICE_DT_INST_DEFINE(n, flash_as32x601_qspi_init, NULL,		\
		&data_##n, &cfg_##n, POST_KERNEL,				\
		90, &flash_as32x601_qspi_api);

DT_INST_FOREACH_STATUS_OKAY(FLASH_AS32X601_QSPI_INIT)
