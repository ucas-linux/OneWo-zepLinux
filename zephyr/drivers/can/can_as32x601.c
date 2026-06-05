/* SPDX-License-Identifier: Apache-2.0 */

#define DT_DRV_COMPAT ansilic_as32x601_canfd

#include <errno.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/irq.h>
#include <zephyr/arch/riscv/irq.h>
#include <zephyr/sys/util.h>

#include <as32x601_canfd.h>
#include <as32x601_smu.h>
#include <as32x601_plic.h>
#include <soc.h>

LOG_MODULE_REGISTER(can_as32x601, CONFIG_CAN_LOG_LEVEL);

/* AS32 硬件资源上限 */
#define AS32X601_CAN_NUM_TX_BUF   3U   /* TB0..TB2 */
#define AS32X601_CAN_NUM_FILTERS  3U   /* 3 组接收滤波器 */

/* TX 完成轮询上限（每次 k_busy_wait(1us)），约 50ms 足够最慢的 64B FD 帧 */
#define AS32X601_CAN_TX_POLL_MAX   50000U

/*
 * Value of SR.ESTAT[1:0] (read via CANFD_GetStatus(regs, CANFD_SR_ESTAT)) that
 * means the node is Bus-Off. The HAL headers only label the field as
 * "Error Status" without enumerating the codes; on this IP the encoding is
 * 00=Configuration, 01=Error-Active, 10=Bus-Off, 11=Error-Passive.
 */
#define AS32X601_ESTAT_BUSOFF     0x2U

struct as32x601_can_config {
	/*
	 * MUST be the first member: the CAN subsystem casts dev->config to
	 * struct can_driver_config to read min_bitrate/max_bitrate (see
	 * can_get_bitrate_min/max()). Omitting it makes can_set_bitrate() read
	 * garbage and reject valid bitrates with -ENOTSUP.
	 */
	struct can_driver_config common;
	CANFD_TypeDef *regs;
	uint32_t reg_addr;
	uint32_t irq;
	uint32_t irq_prio;
	const struct pinctrl_dev_config *pcfg;
};

struct as32x601_can_filter {
	bool used;
	struct can_filter filter;
	can_rx_callback_t cb;
	void *cb_arg;
};

struct as32x601_can_data {
	struct k_mutex lock;
	bool started;
	can_mode_t mode;
	struct can_timing timing;
	struct can_timing timing_data;
	struct as32x601_can_filter filters[AS32X601_CAN_NUM_FILTERS];
	can_state_change_callback_t state_cb;
	void *state_cb_arg;
	struct k_sem tx_sem[AS32X601_CAN_NUM_TX_BUF];
};

static int as32x601_can_enable_clock(uint32_t reg_addr)
{
	switch (reg_addr) {
	case AS32X601_CANFD0_BASE: CANFD0_CLK_ENABLE(); CANFD0_RESET(); CANFD0_SET(); return 0;
	case AS32X601_CANFD1_BASE: CANFD1_CLK_ENABLE(); CANFD1_RESET(); CANFD1_SET(); return 0;
	case AS32X601_CANFD2_BASE: CANFD2_CLK_ENABLE(); CANFD2_RESET(); CANFD2_SET(); return 0;
	case AS32X601_CANFD3_BASE: CANFD3_CLK_ENABLE(); CANFD3_RESET(); CANFD3_SET(); return 0;
	default: return -EINVAL;
	}
}

static int as32x601_can_get_capabilities(const struct device *dev, can_mode_t *cap)
{
	ARG_UNUSED(dev);
	*cap = CAN_MODE_LOOPBACK | CAN_MODE_LISTENONLY | CAN_MODE_FD;
	return 0;
}

static int as32x601_can_set_mode(const struct device *dev, can_mode_t mode)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;

	if (mode & ~(CAN_MODE_LOOPBACK | CAN_MODE_LISTENONLY | CAN_MODE_FD)) {
		return -ENOTSUP;
	}

	k_mutex_lock(&data->lock, K_FOREVER);

	if (data->started) {
		k_mutex_unlock(&data->lock);
		return -EBUSY;
	}

	if (mode & CAN_MODE_LOOPBACK) {
		CANFD_SetLoopback(cfg->regs);
	} else {
		CANFD_CloseLoopback(cfg->regs);
	}
	if (mode & CAN_MODE_LISTENONLY) {
		CANFD_SetSnoop(cfg->regs);
	} else {
		CANFD_CloseSnoop(cfg->regs);
	}

	/*
	 * CAN_MODE_FD intentionally needs no register write here: this controller
	 * selects the CAN-FD protocol per frame at TX time (FDF/BRS in the TX
	 * buffer), not via a controller-wide enable. Do not add an FD switch here.
	 */

	data->mode = mode;
	k_mutex_unlock(&data->lock);
	return 0;
}

/*
 * Bias of segment/SJW register values relative to the actual tq count.
 *
 * Verified against modules/hal/.../as32x601_canfd.h enum literals:
 *   CANFD_APBTR_TS1_11tp == 0x0A (= 10), CANFD_APBTR_TS2_4tp == 0x03 (= 3),
 *   CANFD_APBTR_SJW_2tp == 0x01 (= 1) and the matching DP enums.
 * Each enum encodes (tq_count - 1), so the register field stores count - 1.
 */
#define AS32X601_TS_BIAS   1U
#define AS32X601_SJW_BIAS  1U

/*
 * Map the cached Zephyr can_timing into the AS32 HAL init struct and program
 * the controller. Called by start() (Task 7); kept here so timing mapping is
 * fully self-contained.
 *
 * Zephyr semantics: TS1(HAL) = prop_seg + phase_seg1, TS2(HAL) = phase_seg2,
 * SJW(HAL) = sjw. The sync segment (1 tq) is implicit in hardware.
 */
static void as32x601_can_apply_timing(struct as32x601_can_data *data,
				      CANFD_TypeDef *regs)
{
	CANFD_InitTypeDef init;
	uint32_t n_brp = data->timing.prescaler - 1U;
	uint32_t n_ts1 = data->timing.prop_seg + data->timing.phase_seg1 - AS32X601_TS_BIAS;
	uint32_t n_ts2 = data->timing.phase_seg2 - AS32X601_TS_BIAS;
	uint32_t n_sjw = data->timing.sjw - AS32X601_SJW_BIAS;
	uint32_t d_brp = data->timing_data.prescaler - 1U;
	uint32_t d_ts1 = data->timing_data.prop_seg + data->timing_data.phase_seg1 - AS32X601_TS_BIAS;
	uint32_t d_ts2 = data->timing_data.phase_seg2 - AS32X601_TS_BIAS;
	uint32_t d_sjw = data->timing_data.sjw - AS32X601_SJW_BIAS;

	CANFD_StructInit(&init);
	init.CANFD_SRR = CANFD_SRR_RESET; /* 显式保持复位态，不依赖 StructInit 默认值 */

	/* Arbitration (nominal) phase */
	init.CANFD_APBRPR      = (CANFDAPBRPR_TypeDef)n_brp;
	init.CANFD_APBTR_APTS1 = (CANFDAPBTRts1_TypeDef)n_ts1;
	init.CANFD_APBTR_APTS2 = (CANFDAPBTRts2_TypeDef)n_ts2;
	init.CANFD_APBTR_APSJW = (CANFDAPBTRsjw_TypeDef)n_sjw;

	/* Data (FD) phase */
	init.CANFD_DPBRPR      = (CANFDDPBRPR_TypeDef)d_brp;
	init.CANFD_DPBTR_DPTS1 = (CANFDDPBTRts1_TypeDef)d_ts1;
	init.CANFD_DPBTR_DPTS2 = (CANFDDPBTRts2_TypeDef)d_ts2;
	init.CANFD_DPBTR_DPSJW = (CANFDDPBTRsjw_TypeDef)d_sjw;

	LOG_DBG("nominal: brp=%u ts1=%u ts2=%u sjw=%u", n_brp, n_ts1, n_ts2, n_sjw);
	LOG_DBG("data:    brp=%u ts1=%u ts2=%u sjw=%u", d_brp, d_ts1, d_ts2, d_sjw);

	CANFD_Init(regs, &init);
}

static int as32x601_can_start(const struct device *dev)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;

	k_mutex_lock(&data->lock, K_FOREVER);

	if (data->started) {
		k_mutex_unlock(&data->lock);
		return -EALREADY;
	}

	as32x601_can_apply_timing(data, cfg->regs);

	/*
	 * Re-open the hardware acceptance filters AFTER CANFD_Init(): that call
	 * writes SRR=RESET (software reset), which clears the RX filter registers
	 * (AFR/AFMR/AFIR). Filters registered via add_rx_filter() before start()
	 * would otherwise be lost, leaving AFR=0 => every frame rejected => no RXOK
	 * interrupt ever fires. Mask 0 accepts everything; the exact (id,mask) check
	 * is done in software in the ISR. (IAR avoids this by calling FilterInit
	 * after CANFD_Init; in the Zephyr model add_rx_filter may run before start.)
	 */
	for (int i = 0; i < AS32X601_CAN_NUM_FILTERS; i++) {
		if (data->filters[i].used) {
			CANFD_FilterInit(cfg->regs, (uint32_t)i, 0x00000000U, 0x00000000U);
		}
	}

	CANFD_AutoRetransConfig(cfg->regs, ENABLE);
	CANFD_ITConfig(cfg->regs, CANFD_IT_ERXOK, ENABLE);
	CANFD_Enable(cfg->regs); /* sets SRR.CEN -> re-enables after a prior stop() */
	CANFD_ClearITPendingBit(cfg->regs, CANFD_CLEAR_ALL);

	data->started = true;
	k_mutex_unlock(&data->lock);
	return 0;
}

static int as32x601_can_stop(const struct device *dev)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;

	k_mutex_lock(&data->lock, K_FOREVER);

	if (!data->started) {
		k_mutex_unlock(&data->lock);
		return -EALREADY;
	}

	CANFD_ITConfig(cfg->regs, CANFD_IT_ERXOK, DISABLE);
	/*
	 * Truly disable the controller so the node leaves the bus, as required by
	 * the can_stop() contract. The HAL exposes no CANFD_Disable(), so clear the
	 * CAN-Enable bit directly (CANFD_Enable() sets SRR.CEN; we clear it here).
	 * Do NOT use CANFD_SetBusOff(): that issues a bus-off *recovery* request and
	 * keeps SRR.CEN set, leaving the node attached to the bus.
	 */
	cfg->regs->SRR &= ~((uint32_t)CANFD_SRR_CEN);

	data->started = false;
	k_mutex_unlock(&data->lock);
	return 0;
}

static int as32x601_can_set_timing(const struct device *dev, const struct can_timing *t)
{
	struct as32x601_can_data *data = dev->data;

	if (data->started) {
		return -EBUSY;
	}
	k_mutex_lock(&data->lock, K_FOREVER);
	data->timing = *t;
	k_mutex_unlock(&data->lock);
	return 0;
}

static int as32x601_can_set_timing_data(const struct device *dev, const struct can_timing *t)
{
	struct as32x601_can_data *data = dev->data;

	if (data->started) {
		return -EBUSY;
	}
	k_mutex_lock(&data->lock, K_FOREVER);
	data->timing_data = *t;
	k_mutex_unlock(&data->lock);
	return 0;
}
/*
 * 简化的同步发送：无论 cb 是否为空都阻塞到发送完成（或超时），
 * 完成后就地调用 cb。单帧在途、与已验证的 IAR 发送方式一致。
 */
static int as32x601_can_send(const struct device *dev, const struct can_frame *frame,
			     k_timeout_t timeout, can_tx_callback_t cb, void *user_data)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;
	CANFD_TXFrameTypeDef txf;
	uint8_t buf[CAN_MAX_DLEN] = {0};
	uint8_t nbytes;
	const int tb = TB1; /* demo 用 TB1，与 IAR 一致 */

	if (!data->started) {
		return -ENETDOWN;
	}
	if ((frame->flags & CAN_FRAME_FDF) && !(data->mode & CAN_MODE_FD)) {
		return -ENOTSUP;
	}

	if (k_sem_take(&data->tx_sem[tb], timeout) != 0) {
		return -EAGAIN;
	}

	nbytes = can_dlc_to_bytes(frame->dlc);
	if (nbytes > sizeof(buf)) {
		k_sem_give(&data->tx_sem[tb]);
		return -EINVAL;
	}
	memcpy(buf, frame->data, nbytes);

	if (frame->flags & CAN_FRAME_FDF) {
		txf.CANFD_TX_PROTOCOL = (frame->flags & CAN_FRAME_BRS) ? CANFD_BRS : CANFD;
	} else {
		txf.CANFD_TX_PROTOCOL = CAN;
	}
	txf.CANFD_TX_FORMAT   = (frame->flags & CAN_FRAME_IDE) ? EXTENDED : STANDARD;
	txf.CANFD_TX_TYPE     = (frame->flags & CAN_FRAME_RTR) ? REMOTE : DATA;
	txf.CANFD_Frame_ID    = frame->id;

	k_mutex_lock(&data->lock, K_FOREVER);
	CANFD_FrameInit(cfg->regs, tb, &txf);
	CANFD_Transmit(cfg->regs, tb, buf, nbytes);
	k_mutex_unlock(&data->lock);

	/* 等待发送完成：GetSendCmpStatus==SET 表示该 TB 仍在发送 */
	{
		bool tx_ok;
		uint32_t guard = AS32X601_CAN_TX_POLL_MAX;

		while (CANFD_GetSendCmpStatus(cfg->regs, (1U << tb)) == SET && guard-- > 0U) {
			k_busy_wait(1);   /* 1us/次，让 guard 成为真实的超时上限 */
		}
		tx_ok = (CANFD_GetSendCmpStatus(cfg->regs, (1U << tb)) != SET);

		k_sem_give(&data->tx_sem[tb]);

		if (cb != NULL) {
			cb(dev, tx_ok ? 0 : -EIO, user_data);
		}
		return tx_ok ? 0 : -EIO;
	}
}
/*
 * ISR: drain the RX FIFO, software-match every received frame against the
 * registered filters and invoke their callbacks.
 *
 * Hardware acceptance filtering is left fully permissive (AFMR=0 in
 * add_rx_filter, see there); exact matching is done here in software so that
 * the Zephyr per-callback (id, mask) semantics are honoured precisely.
 *
 * Read-index advance: CANFD_GetReceiveData() already advances the FIFO read
 * index itself (it writes RFSR.IRI internally after copying the data words).
 * We therefore MUST NOT call CANFD_SetRxIncReadIndex() again, or every other
 * message would be skipped. The while loop is additionally bounded by a guard
 * (FIFO depth 64 + 1) as defensive protection against a non-advancing read.
 */
static void as32x601_can_isr(const struct device *dev)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;
	uint32_t guard = 65U; /* RX FIFO depth (64) + 1, anti-deadlock bound */

	if (CANFD_GetITStatus(cfg->regs, CANFD_FLAG_RXOK) == RESET) {
		return;
	}

	while (CANFD_GetRxFillLevel(cfg->regs) > 0U && guard-- > 0U) {
		CANFD_RXInfoTypeDef info = CANFD_GetReceiveInfo(cfg->regs);
		struct can_frame frame = {0};
		uint8_t rxbuf[CAN_MAX_DLEN] = {0};
		uint8_t nbytes;

		/* GetReceiveData copies the data words AND advances the read index. */
		nbytes = (uint8_t)CANFD_GetReceiveData(cfg->regs, rxbuf);

		frame.id = info.CANFD_CurrentFrame_ID;
		frame.dlc = can_bytes_to_dlc(nbytes);
		if (info.CANFD_CurrentMessage_FORMAT == EXTENDED) {
			frame.flags |= CAN_FRAME_IDE;
		}
		if (info.CANFD_CurrentMessage_PROTOCOL == CANFD ||
		    info.CANFD_CurrentMessage_PROTOCOL == CANFD_BRS) {
			frame.flags |= CAN_FRAME_FDF;
		}
		if (info.CANFD_CurrentMessage_PROTOCOL == CANFD_BRS) {
			frame.flags |= CAN_FRAME_BRS;
		}
		if (info.CANFD_CurrentMessage_TYPE == REMOTE) {
			frame.flags |= CAN_FRAME_RTR;
		}
		memcpy(frame.data, rxbuf, MIN(nbytes, sizeof(frame.data)));

		for (int i = 0; i < AS32X601_CAN_NUM_FILTERS; i++) {
			struct as32x601_can_filter *f = &data->filters[i];
			can_rx_callback_t cb = f->cb;

			if (f->used && cb != NULL &&
			    ((frame.id ^ f->filter.id) & f->filter.mask) == 0U) {
				cb(dev, &frame, f->cb_arg);
			}
		}
	}

	CANFD_ClearITPendingBit(cfg->regs, CANFD_CLEAR_RXOK);
}

static int as32x601_can_add_rx_filter(const struct device *dev, can_rx_callback_t cb,
				      void *user_data, const struct can_filter *filter)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;
	int id = -ENOSPC;

	k_mutex_lock(&data->lock, K_FOREVER);
	for (int i = 0; i < AS32X601_CAN_NUM_FILTERS; i++) {
		if (!data->filters[i].used) {
			data->filters[i].used = true;
			data->filters[i].filter = *filter;
			data->filters[i].cb = cb;
			data->filters[i].cb_arg = user_data;
			id = i;
			break;
		}
	}
	if (id >= 0) {
		/*
		 * Open the hardware acceptance filter fully (mask = 0 => no bit is
		 * required to match => accept every frame); the exact (id, mask)
		 * check is performed in software in the ISR. AFMR=0 is safe: this
		 * HAL's assert_param() is a no-op build, so the Afm>0 assert in
		 * CANFD_FilterInit() never fires.
		 */
		CANFD_FilterInit(cfg->regs, (uint32_t)id, 0x00000000U, 0x00000000U);
	}
	k_mutex_unlock(&data->lock);
	return id;
}

static void as32x601_can_remove_rx_filter(const struct device *dev, int filter_id)
{
	struct as32x601_can_data *data = dev->data;

	if (filter_id < 0 || filter_id >= AS32X601_CAN_NUM_FILTERS) {
		return;
	}
	k_mutex_lock(&data->lock, K_FOREVER);
	data->filters[filter_id].used = false;
	data->filters[filter_id].cb = NULL;
	k_mutex_unlock(&data->lock);
}
static int as32x601_can_get_state(const struct device *dev, enum can_state *state,
				  struct can_bus_err_cnt *err_cnt)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;
	uint32_t tec = CANFD_GetTXErrorCount(cfg->regs);
	uint32_t rec = CANFD_GetRXErrorCount(cfg->regs);

	if (err_cnt) {
		err_cnt->tx_err_cnt = (uint8_t)tec;
		err_cnt->rx_err_cnt = (uint8_t)rec;
	}
	if (state) {
		/*
		 * Read the hardware error state from SR.ESTAT[1:0] instead of relying
		 * on the TX error counter: TEC is an 8-bit counter (CANFD_GetTXErrorCount
		 * returns <=255), so a ">=256" test can never fire and would be dead code.
		 * ESTAT == AS32X601_ESTAT_BUSOFF (binary 10) indicates the node is bus-off.
		 */
		uint32_t estat = CANFD_GetStatus(cfg->regs, CANFD_SR_ESTAT);

		if (!data->started) {
			*state = CAN_STATE_STOPPED;
		} else if (estat == AS32X601_ESTAT_BUSOFF) {
			*state = CAN_STATE_BUS_OFF;
		} else if (tec > 127U || rec > 127U) {
			*state = CAN_STATE_ERROR_PASSIVE;
		} else if (tec > 95U || rec > 95U) {
			*state = CAN_STATE_ERROR_WARNING;
		} else {
			*state = CAN_STATE_ERROR_ACTIVE;
		}
	}
	return 0;
}
static void as32x601_can_set_state_change_callback(const struct device *dev,
				can_state_change_callback_t cb, void *user_data)
{
	/*
	 * 保存回调备用。注意：当前驱动尚未接错误状态变化中断，
	 * 该回调暂不会被触发（错误/bus-off 状态可经 can_get_state 主动查询）。
	 * 后续接 CANFD 错误中断后在此通知。
	 */
	struct as32x601_can_data *data = dev->data;
	data->state_cb = cb;
	data->state_cb_arg = user_data;
}
static int as32x601_can_get_core_clock(const struct device *dev, uint32_t *rate)
{
	ARG_UNUSED(dev);
	/*
	 * CANCLKx2 is, as its name says, twice the actual CAN bit-timing clock:
	 * the controller's prescaler divides CANCLKx2/2 (confirmed against the
	 * validated IAR config, where CANCLKx2=80MHz yields a 40MHz timing clock
	 * matching its BRP/TSEG register values). Report the real timing clock so
	 * can_calc_timing() programs the prescaler/segments correctly.
	 */
	*rate = SMU_ClocksStruct.CANCLKx2_Frequency / 2U;
	return 0;
}
static int as32x601_can_get_max_filters(const struct device *dev, bool ide)
{ ARG_UNUSED(dev); ARG_UNUSED(ide); return AS32X601_CAN_NUM_FILTERS; }

static DEVICE_API(can, as32x601_can_api) = {
	.get_capabilities = as32x601_can_get_capabilities,
	.start = as32x601_can_start,
	.stop = as32x601_can_stop,
	.set_mode = as32x601_can_set_mode,
	.set_timing = as32x601_can_set_timing,
	.set_timing_data = as32x601_can_set_timing_data,
	.send = as32x601_can_send,
	.add_rx_filter = as32x601_can_add_rx_filter,
	.remove_rx_filter = as32x601_can_remove_rx_filter,
	.get_state = as32x601_can_get_state,
	.set_state_change_callback = as32x601_can_set_state_change_callback,
	.get_core_clock = as32x601_can_get_core_clock,
	.get_max_filters = as32x601_can_get_max_filters,
	.timing_min = { .sjw = 1, .prop_seg = 0, .phase_seg1 = 1, .phase_seg2 = 1, .prescaler = 1 },
	/* 上限源自 HAL 寄存器段位宽（寄存器存 count-1）：
	 * 仲裁段 TS1(=prop+phase_seg1)≤61, TS2≤128, SJW≤128（phase_seg1 取 60 给 prop_seg 留 1）。
	 */
	.timing_max = { .sjw = 128, .prop_seg = 0, .phase_seg1 = 60, .phase_seg2 = 128, .prescaler = 256 },
	.timing_data_min = { .sjw = 1, .prop_seg = 0, .phase_seg1 = 1, .phase_seg2 = 1, .prescaler = 1 },
	/* 数据段 TS1≤16, TS2≤8, SJW≤8（phase_seg1 取 15 给 prop_seg 留 1）。 */
	.timing_data_max = { .sjw = 8, .prop_seg = 0, .phase_seg1 = 15, .phase_seg2 = 8, .prescaler = 256 },
};

static int as32x601_can_init(const struct device *dev)
{
	const struct as32x601_can_config *cfg = dev->config;
	struct as32x601_can_data *data = dev->data;
	int ret;

	k_mutex_init(&data->lock);
	for (int i = 0; i < AS32X601_CAN_NUM_TX_BUF; i++) {
		k_sem_init(&data->tx_sem[i], 1, 1);
	}

	ret = as32x601_can_enable_clock(cfg->reg_addr);
	if (ret < 0) {
		LOG_ERR("unknown CANFD base 0x%x", cfg->reg_addr);
		return ret;
	}

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret < 0) {
		LOG_ERR("pinctrl apply failed: %d", ret);
		return ret;
	}

	z_riscv_irq_priority_set(cfg->irq, cfg->irq_prio, 0);
	irq_enable(cfg->irq);

	LOG_INF("AS32X601 CANFD @0x%x initialized", cfg->reg_addr);
	return 0;
}

#define AS32X601_CAN_INIT(inst)                                                          \
	PINCTRL_DT_INST_DEFINE(inst);                                                    \
	static struct as32x601_can_data as32x601_can_data_##inst;                        \
	static const struct as32x601_can_config as32x601_can_config_##inst = {           \
		.common = CAN_DT_DRIVER_CONFIG_INST_GET(inst, 0, 8000000),               \
		.regs = (CANFD_TypeDef *)DT_INST_REG_ADDR(inst),                         \
		.reg_addr = DT_INST_REG_ADDR(inst),                                      \
		.irq = DT_INST_IRQN(inst),                                               \
		.irq_prio = DT_INST_IRQ(inst, priority),                                 \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                            \
	};                                                                               \
	DEVICE_DT_INST_DEFINE(inst, as32x601_can_init, NULL, &as32x601_can_data_##inst,  \
			      &as32x601_can_config_##inst, POST_KERNEL,                  \
			      CONFIG_CAN_INIT_PRIORITY, &as32x601_can_api);

DT_INST_FOREACH_STATUS_OKAY(AS32X601_CAN_INIT)

/*
 * Global ISR trampolines invoked from the PLIC dispatch
 * (intc_as32x601_plic.c). Each is compiled only when its instance is enabled
 * in the devicetree, so disabled instances produce no undefined symbol and the
 * matching PLIC switch case (guarded identically) never references a missing
 * handler.
 */
#if DT_NODE_HAS_STATUS(DT_NODELABEL(canfd0), okay)
void CANFD0_IRQ_Handler(void)
{
	as32x601_can_isr(DEVICE_DT_GET(DT_NODELABEL(canfd0)));
}
#endif
#if DT_NODE_HAS_STATUS(DT_NODELABEL(canfd1), okay)
void CANFD1_IRQ_Handler(void)
{
	as32x601_can_isr(DEVICE_DT_GET(DT_NODELABEL(canfd1)));
}
#endif
#if DT_NODE_HAS_STATUS(DT_NODELABEL(canfd2), okay)
void CANFD2_IRQ_Handler(void)
{
	as32x601_can_isr(DEVICE_DT_GET(DT_NODELABEL(canfd2)));
}
#endif
#if DT_NODE_HAS_STATUS(DT_NODELABEL(canfd3), okay)
void CANFD3_IRQ_Handler(void)
{
	as32x601_can_isr(DEVICE_DT_GET(DT_NODELABEL(canfd3)));
}
#endif
