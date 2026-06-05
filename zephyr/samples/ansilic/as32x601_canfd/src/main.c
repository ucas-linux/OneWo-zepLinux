/* SPDX-License-Identifier: Apache-2.0 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(canfd_sample, LOG_LEVEL_INF);

/*
 * AS32X601 CANFD3 真实总线收发 demo（行为对齐已验证的 IAR 工程）：
 *   - 周期发送扩展帧 ID 0x147，载荷 8/16/32/64 字节（演示 FD 载荷增长），FD+BRS。
 *   - 接收过滤同一 ID 0x147（精确匹配），收到后回显并打印。
 * 注意：发送与接收使用同一 ID 0x147。真实总线（非 loopback）下本节点不会收到自己发的帧；
 * 回显假设对端是受控的单帧伙伴。若两端都跑本 demo 互相回显会形成回环放大——
 * 验证时建议对端用不同 payload 以便区分自发/对端帧。
 */

#define CANFD_NODE     DT_NODELABEL(canfd3)
#define CANFD_TX_ID    0x147U  /* 扩展帧 ID，与 IAR demo 一致 */

static const struct device *const can_dev = DEVICE_DT_GET(CANFD_NODE);

static const uint8_t send_data[64] = {
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18, 0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38, 0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,
	0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58, 0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,
	0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78, 0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,
};

CAN_MSGQ_DEFINE(rx_msgq, 4);

static void tx_callback(const struct device *dev, int error, void *user_data)
{
	ARG_UNUSED(dev); ARG_UNUSED(user_data);
	if (error != 0) {
		LOG_WRN("tx failed: %d", error);
	}
}

static int send_frame(uint8_t nbytes)
{
	struct can_frame frame = {0};

	frame.id = CANFD_TX_ID;
	frame.flags = CAN_FRAME_IDE | CAN_FRAME_FDF | CAN_FRAME_BRS;
	frame.dlc = can_bytes_to_dlc(nbytes);
	memcpy(frame.data, send_data, nbytes);

	return can_send(can_dev, &frame, K_MSEC(100), tx_callback, NULL);
}

int main(void)
{
	struct can_filter filter = {
		.flags = CAN_FILTER_IDE,
		.id = CANFD_TX_ID,
		.mask = CAN_EXT_ID_MASK,
	};
	struct can_frame rx;
	int ret;

	if (!device_is_ready(can_dev)) {
		LOG_ERR("CAN device not ready");
		return 0;
	}

	ret = can_set_mode(can_dev, CAN_MODE_FD);
	if (ret != 0) {
		LOG_ERR("set FD mode failed: %d", ret);
		return 0;
	}

	ret = can_set_bitrate(can_dev, 500000);
	if (ret != 0) {
		LOG_ERR("set nominal bitrate failed: %d", ret);
		return 0;
	}

	/* 数据段降到 1M：30MHz 下 1M=30 个时间量子（2M 仅 15 个），余量充足、无需 TDC。 */
	ret = can_set_bitrate_data(can_dev, 1000000);
	if (ret != 0) {
		LOG_ERR("set data bitrate failed: %d", ret);
		return 0;
	}

	ret = can_add_rx_filter_msgq(can_dev, &rx_msgq, &filter);
	if (ret < 0) {
		LOG_ERR("add rx filter failed: %d", ret);
		return 0;
	}

	if (can_start(can_dev) != 0) {
		LOG_ERR("CAN start failed");
		return 0;
	}
	LOG_INF("CANFD3 started: nominal 500k / data 1M, FD mode");

	send_frame(8);

	while (1) {
		/* 发送错误经 tx_callback 异步上报，这里不再检查返回值 */
		send_frame(16);
		send_frame(32);

		if (k_msgq_get(&rx_msgq, &rx, K_NO_WAIT) == 0) {
			uint8_t n = can_dlc_to_bytes(rx.dlc);

			can_send(can_dev, &rx, K_MSEC(100), tx_callback, NULL);

			printk("canfd rx (id=0x%x, %u bytes):", rx.id, n);
			for (uint8_t i = 0; i < n; i++) {
				printk(" %02x", rx.data[i]);
			}
			printk("\n");
		}

		send_frame(64);
		k_msleep(500);  /* 每轮间隔 500ms */
	}
	return 0;
}
