/* SPDX-License-Identifier: Apache-2.0 */
/* AS32X601 Core10100 MAC Ethernet 驱动 */

#define DT_DRV_COMPAT ansilic_as32x601_eth

#define LOG_MODULE_NAME eth_as32x601
#define LOG_LEVEL CONFIG_ETHERNET_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_pkt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/irq.h>
#include <zephyr/arch/riscv/irq.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/sys/util.h>
#include <string.h>

#include <as32x601_smu.h>
#include "eth_as32x601_mac.h"

/* 环/缓冲参数（与参考工程一致） */
#define ETH_RX_RING_SIZE    8U
#define ETH_TX_RING_SIZE    4U
#define ETH_BUF1_SIZE       1024U
#define ETH_BUF2_SIZE       512U
#define ETH_FRAME_BUF_SIZE  (ETH_BUF1_SIZE + ETH_BUF2_SIZE)
#define ETH_MAX_FRAME_LEN   1518U
#define ETH_DESC_STRIDE     64U

/* PBL=1: 防止 TX/RX DMA 并发时总线冻结 */
#define ETH_BUS_PBL         MAC_BUS_MODE_PBL_0
#define ETH_DESC_SKIP_LW    12U
#define ETH_DESC_SKIP_BM    (ETH_DESC_SKIP_LW << MAC_BUS_MODE_DSL_Pos)

#define ETH_TX_STATUS_CLEAR (MAC_STATUS_TI  | MAC_STATUS_TPS | MAC_STATUS_TU  | \
                             MAC_STATUS_UNF | MAC_STATUS_ETI | MAC_STATUS_NIS | \
                             MAC_STATUS_AIS)

#define ETH_DIAG_ENABLE_TX_IRQ 0
#define MDIO_TIMEOUT        10000U
#define RX_THREAD_STACK_SZ  1024
#define RX_THREAD_PRIO      K_PRIO_PREEMPT(1)
#define RX_BATCH_BUDGET     4U

/* 带填充的描述符槽（保证 64 字节对齐） */
typedef struct {
    volatile mac_tx_desc_t desc;
    uint8_t pad[ETH_DESC_STRIDE - sizeof(mac_tx_desc_t)];
} eth_tx_slot_t;

typedef struct {
    volatile mac_rx_desc_t desc;
    uint8_t pad[ETH_DESC_STRIDE - sizeof(mac_rx_desc_t)];
} eth_rx_slot_t;

BUILD_ASSERT(sizeof(eth_tx_slot_t) == ETH_DESC_STRIDE,
             "eth_tx_slot_t size mismatch");
BUILD_ASSERT(sizeof(eth_rx_slot_t) == ETH_DESC_STRIDE,
             "eth_rx_slot_t size mismatch");

/* 驱动实例数据 */
struct eth_as32x601_data {
    struct net_if *iface;
    uint8_t mac_addr[6];
    const struct pinctrl_dev_config *pcfg;

    eth_tx_slot_t tx_ring[ETH_TX_RING_SIZE] __aligned(64);
    eth_rx_slot_t rx_ring[ETH_RX_RING_SIZE] __aligned(64);
    uint8_t tx_buf[ETH_TX_RING_SIZE][ETH_FRAME_BUF_SIZE] __aligned(4);
    uint8_t rx_buf[ETH_RX_RING_SIZE][ETH_FRAME_BUF_SIZE] __aligned(4);

    uint32_t tx_head;
    uint32_t rx_tail;

    struct k_sem rx_sem;
    struct k_thread rx_thread;
    K_KERNEL_STACK_MEMBER(rx_stack, RX_THREAD_STACK_SZ);
};

/* 获取 MAC 寄存器指针（基址来自 DT） */
static inline volatile mac_regs_t *get_mac(void)
{
    return (volatile mac_regs_t *)DT_INST_REG_ADDR(0);
}

/* ─── MDIO 辅助函数 ─────────────────────────────────── */

static void mdio_set_prescaler(uint16_t div)
{
    volatile mac_regs_t *mac = get_mac();

    mac->MDIO_CTRL0 &= ~GENMASK(15, 0);
    mac->MDIO_CTRL0 |= (uint32_t)div;
}

static void mdio_enable(bool en)
{
    volatile mac_regs_t *mac = get_mac();

    if (en) {
        mac->MDIO_CTRL0 |= MAC_MDIO_CTRL0_MDCE;
    } else {
        mac->MDIO_CTRL0 &= ~MAC_MDIO_CTRL0_MDCE;
    }
}

static int mdio_write(uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
    volatile mac_regs_t *mac = get_mac();
    uint32_t val;
    uint32_t to = MDIO_TIMEOUT;

    val  = ((uint32_t)data     << MAC_MDIO_CTRL1_RDATA_Pos);
    val |= MAC_MDIO_CTRL1_TURN_1;
    val |= ((uint32_t)reg_addr << MAC_MDIO_CTRL1_REG_ADDR_Pos);
    val |= ((uint32_t)phy_addr << MAC_MDIO_CTRL1_PHY_ADDR_Pos);
    val |= MAC_MDIO_CTRL1_START0;
    val |= MAC_MDIO_CTRL1_RDWR_0;
    mac->MDIO_CTRL1 = val;

    while (to-- != 0U) {
        if (mac->MDIO_CTRL1 & MAC_MDIO_CTRL1_ACK) {
            return 0;
        }
    }
    return -ETIMEDOUT;
}

static int mdio_read(uint8_t phy_addr, uint8_t reg_addr, uint16_t *out)
{
    volatile mac_regs_t *mac = get_mac();
    uint32_t val;
    uint32_t to = MDIO_TIMEOUT;

    val  = MAC_MDIO_CTRL1_TURN_1;
    val |= ((uint32_t)reg_addr  << MAC_MDIO_CTRL1_REG_ADDR_Pos);
    val |= ((uint32_t)phy_addr  << MAC_MDIO_CTRL1_PHY_ADDR_Pos);
    val |= MAC_MDIO_CTRL1_START0;
    val |= MAC_MDIO_CTRL1_RDWR_1;
    mac->MDIO_CTRL1 = val;

    while (to-- != 0U) {
        if (mac->MDIO_CTRL1 & MAC_MDIO_CTRL1_ACK) {
            *out = (uint16_t)(mac->MDIO_CTRL1 & MAC_MDIO_CTRL1_RDATA);
            return 0;
        }
    }
    return -ETIMEDOUT;
}

/* ─── PHY 初始化（DP83867IR，移植自 eth.c） ─────────── */

static int phy_dp83867_init(void)
{
    uint8_t phy_addr = 0xFFU;
    uint16_t val;
    uint16_t bmcr;
    uint32_t to;

    mdio_set_prescaler(80U);
    mdio_enable(true);

    for (uint8_t a = 0U; a <= 31U; a++) {
        if (mdio_read(a, 1U, &val) == 0 && val != 0xFFFFU) {
            phy_addr = a;
            break;
        }
    }
    if (phy_addr == 0xFFU) {
        LOG_ERR("No PHY device found");
        return -ENODEV;
    }

    /* 软复位 */
    mdio_read(phy_addr, 0U, &val);
    val |= BIT(15);
    mdio_write(phy_addr, 0U, val);

    to = 100000U;
    while (to-- != 0U) {
        mdio_read(phy_addr, 0U, &val);
        if (!(val & BIT(15))) {
            break;
        }
    }

    if (to == 0U) {
        LOG_WRN("PHY reset timeout, continuing anyway");
    }

    /* 配置 100M 全双工 */
    mdio_read(phy_addr, 0U, &bmcr);
    bmcr &= (uint16_t)~(BIT(13) | BIT(12));
    bmcr |= (uint16_t)(BIT(8) | BIT(13));
    mdio_write(phy_addr, 0U, bmcr);

    return 0;
}

/* ─── 描述符环初始化 ────────────────────────────────── */

static void ring_init(struct eth_as32x601_data *data)
{
    for (uint32_t i = 0U; i < ETH_RX_RING_SIZE; i++) {
        data->rx_ring[i].desc.buffer_1     = data->rx_buf[i];
        data->rx_ring[i].desc.buffer_2     = (uint32_t)(data->rx_buf[i] + ETH_BUF1_SIZE);
        data->rx_ring[i].desc.descriptor_1 =
            ((i == (ETH_RX_RING_SIZE - 1U)) ? MAC_RDES1_RER : 0U)
            | ((ETH_BUF2_SIZE & MAC_RDES1_RBS2_MASK) << MAC_RDES1_RBS2_OFFSET)
            | ((ETH_BUF1_SIZE & MAC_RDES1_RBS1_MASK) << MAC_RDES1_RBS1_OFFSET);
        data->rx_ring[i].desc.descriptor_0 = MAC_RDES0_OWN;
    }
    for (uint32_t i = 0U; i < ETH_TX_RING_SIZE; i++) {
        data->tx_ring[i].desc.buffer_1     = data->tx_buf[i];
        data->tx_ring[i].desc.buffer_2     = (uint32_t)(data->tx_buf[i] + ETH_BUF1_SIZE);
        data->tx_ring[i].desc.descriptor_1 =
            ((i == (ETH_TX_RING_SIZE - 1U)) ? MAC_TDES1_TER : 0U);
        data->tx_ring[i].desc.descriptor_0 = 0U;
    }
    data->tx_head = 0U;
    data->rx_tail = 0U;
}

/* ─── MAC 硬件初始化 ────────────────────────────────── */

static void mac_hw_init(struct eth_as32x601_data *data)
{
    volatile mac_regs_t *mac = get_mac();
    uint32_t tmp;

    mac->BUS_MODE |= MAC_BUS_MODE_SWR;
    while (mac->BUS_MODE & MAC_BUS_MODE_SWR) {
    }

    tmp  = mac->BUS_MODE;
    tmp &= ~(MAC_BUS_MODE_BAR | GENMASK(6, 2) | MAC_BUS_MODE_BLE |
             GENMASK(13, 8) | GENMASK(19, 17) | BIT(20));
    tmp |= ETH_BUS_PBL | MAC_BUS_MODE_BAR | ETH_DESC_SKIP_BM;
    mac->BUS_MODE = tmp;

    tmp  = mac->OPT_MODE;
    tmp &= ~(MAC_OPT_MODE_PB  | MAC_OPT_MODE_PR | MAC_OPT_MODE_PM |
             MAC_OPT_MODE_FD  | MAC_OPT_MODE_TR | MAC_OPT_MODE_SF |
             MAC_OPT_MODE_TTM | MAC_OPT_MODE_RA);
    tmp |= MAC_OPT_MODE_FD | MAC_OPT_MODE_SF | MAC_OPT_MODE_PR |
           MAC_OPT_MODE_PM | MAC_OPT_MODE_TTM;
    mac->OPT_MODE = tmp;

    mac->TIMER_CTRL = 0U;

    ring_init(data);
    eth_cache_flush();

    mac->RDES_BASE_ADDR = (uint32_t)&data->rx_ring[0].desc;
    mac->TDES_BASE_ADDR = (uint32_t)&data->tx_ring[0].desc;

    mac->INT_ENABLE = MAC_INT_ENABLE_RIE | MAC_INT_ENABLE_NIE;
#if ETH_DIAG_ENABLE_TX_IRQ
    mac->INT_ENABLE |= MAC_INT_ENABLE_TIE;
#endif
    mac->OPT_MODE  |= (MAC_OPT_MODE_SR | MAC_OPT_MODE_ST);

    LOG_INF("MAC RX init: INT=0x%08x RDES=0x%08x",
            mac->INT_ENABLE, mac->RDES_BASE_ADDR);
}

/* ─── 网络接口 iface init 回调 ──────────────────────── */

static void eth_as32x601_iface_init(struct net_if *iface)
{
    const struct device *dev = net_if_get_device(iface);
    struct eth_as32x601_data *data = dev->data;

    data->iface = iface;
    net_if_set_link_addr(iface, data->mac_addr, sizeof(data->mac_addr),
                         NET_LINK_ETHERNET);
    ethernet_init(iface);
}

/* ─── 前向声明 ──────────────────────────────────────── */
static int  eth_as32x601_send(const struct device *dev, struct net_pkt *pkt);
static void eth_as32x601_isr(const struct device *dev);
static void eth_as32x601_rx_thread(void *arg1, void *arg2, void *arg3);

static const struct ethernet_api eth_as32x601_api = {
    .iface_api.init = eth_as32x601_iface_init,
    .send           = eth_as32x601_send,
};

/* ─── 设备 init ─────────────────────────────────────── */

static int eth_as32x601_init(const struct device *dev)
{
    struct eth_as32x601_data *data = dev->data;
    int ret;

    MAC_CLK_ENABLE();

    ret = pinctrl_apply_state(data->pcfg, PINCTRL_STATE_DEFAULT);
    if (ret < 0) {
        LOG_ERR("pinctrl apply failed: %d", ret);
        return ret;
    }

    ret = phy_dp83867_init();
    if (ret < 0) {
        return ret;
    }

    mac_hw_init(data);

    k_sem_init(&data->rx_sem, 0, 1);

    z_riscv_irq_priority_set(DT_INST_IRQN(0), DT_INST_IRQ(0, priority), 0);
    irq_enable(DT_INST_IRQN(0));

    k_thread_create(&data->rx_thread, data->rx_stack,
                    K_KERNEL_STACK_SIZEOF(data->rx_stack),
                    eth_as32x601_rx_thread, (void *)dev, NULL, NULL,
                    RX_THREAD_PRIO, 0, K_NO_WAIT);
    k_thread_name_set(&data->rx_thread, "eth_as32x601_rx");

    LOG_INF("AS32X601 Ethernet driver initialized");
    return 0;
}

/* ─── TX / ISR / RX 线程 ───────────────────────────── */

static int eth_as32x601_send(const struct device *dev, struct net_pkt *pkt)
{
    struct eth_as32x601_data *data = dev->data;
    volatile mac_regs_t *mac = get_mac();
    volatile mac_tx_desc_t *desc;
    uint32_t len = net_pkt_get_len(pkt);
    uint32_t b1, b2;
    uint32_t to = 200000U;

    if (len == 0U || len > ETH_MAX_FRAME_LEN) {
        LOG_ERR("MAC TX invalid len=%u", len);
        return -EINVAL;
    }

    /* 等待当前描述符空闲（惰性回收：OWN 由 DMA 发完自动清） */
    eth_cache_flush();
    do {
        desc = &data->tx_ring[data->tx_head].desc;
        if (!(desc->descriptor_0 & MAC_TDES0_OWN)) {
            break;
        }
    } while (to-- != 0U);

    if (to == 0U) {
        LOG_ERR("MAC TX timeout head=%u d0=0x%08x d1=0x%08x STATUS=0x%08x OPT=0x%08x",
                data->tx_head, desc->descriptor_0, desc->descriptor_1,
                mac->STATUS, mac->OPT_MODE);
        return -ETIMEDOUT;
    }

    /* 停止 TX DMA，填充缓冲区，再重启 */
    mac->OPT_MODE &= ~MAC_OPT_MODE_ST;

    b1 = (len > ETH_BUF1_SIZE) ? ETH_BUF1_SIZE : len;
    b2 = (len > ETH_BUF1_SIZE) ? (len - ETH_BUF1_SIZE) : 0U;

    if (net_pkt_read(pkt, data->tx_buf[data->tx_head], len)) {
        mac->OPT_MODE |= MAC_OPT_MODE_ST;
        LOG_ERR("MAC TX net_pkt_read failed len=%u", len);
        return -EIO;
    }
    eth_cache_flush();

    desc->descriptor_1 =
        MAC_TDES1_FS | MAC_TDES1_LS
        | ((data->tx_head == (ETH_TX_RING_SIZE - 1U)) ? MAC_TDES1_TER : 0U)
        | ((b2 & MAC_TDES1_TBS2_MASK) << MAC_TDES1_TBS2_OFFSET)
        | ((b1 & MAC_TDES1_TBS1_MASK) << MAC_TDES1_TBS1_OFFSET);
    desc->descriptor_0 = MAC_TDES0_OWN;
    eth_cache_flush();

    mac->STATUS = ETH_TX_STATUS_CLEAR;
    mac->TX_POLL = 1U;
    mac->OPT_MODE |= MAC_OPT_MODE_ST;

    data->tx_head = (data->tx_head + 1U) % ETH_TX_RING_SIZE;
    return 0;
}

static void eth_as32x601_isr(const struct device *dev)
{
    struct eth_as32x601_data *data = dev->data;
    volatile mac_regs_t *mac = get_mac();
    uint32_t status = mac->STATUS;

    if (status & MAC_STATUS_RI) {
        eth_cache_flush();
        mac->STATUS = MAC_STATUS_RI | MAC_STATUS_NIS;
        k_sem_give(&data->rx_sem);
    } else if (status & MAC_STATUS_TI) {
        mac->STATUS = MAC_STATUS_TI | MAC_STATUS_NIS;
    } else if (status != 0U) {
        mac->STATUS = status;
    }
}

/* PLIC 分发入口：as32x601_plic_mext_dispatch() 中 MAC_IRQn case 调用此函数 */
void MAC_IRQ_Handler(void)
{
    eth_as32x601_isr(DEVICE_DT_INST_GET(0));
}

static void eth_as32x601_rx_thread(void *arg1, void *arg2, void *arg3)
{
    const struct device *dev = arg1;
    struct eth_as32x601_data *data = dev->data;
    volatile mac_regs_t *mac = get_mac();

    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        k_sem_take(&data->rx_sem, K_FOREVER);
        uint32_t processed = 0U;

        /* 循环处理所有可用帧 */
        while (1) {
            volatile mac_rx_desc_t *desc = &data->rx_ring[data->rx_tail].desc;
            struct net_pkt *pkt = NULL;
            uint32_t fl, b1, b2;

            eth_cache_flush();
            if (desc->descriptor_0 & MAC_RDES0_OWN) {
                break;
            }

            fl = (desc->descriptor_0 >> MAC_RDES0_FL_OFFSET) & MAC_RDES0_FL_MASK;
            fl = (fl > 4U) ? (fl - 4U) : 0U;   /* 去掉 4 字节 CRC */

            if (fl == 0U || fl > ETH_FRAME_BUF_SIZE) {
                LOG_ERR("MAC RX invalid len=%u tail=%u d0=0x%08x",
                        fl, data->rx_tail, desc->descriptor_0);
                goto recycle;
            }

            pkt = net_pkt_rx_alloc_with_buffer(data->iface, fl,
                                               AF_UNSPEC, 0, K_NO_WAIT);
            if (!pkt) {
                LOG_ERR("RX: alloc failed");
                goto recycle;
            }

            b1 = (fl > ETH_BUF1_SIZE) ? ETH_BUF1_SIZE : fl;
            b2 = (fl > ETH_BUF1_SIZE) ? (fl - ETH_BUF1_SIZE) : 0U;

            if (net_pkt_write(pkt, data->rx_buf[data->rx_tail], b1)) {
                LOG_ERR("MAC RX net_pkt_write b1 failed len=%u", b1);
                net_pkt_unref(pkt);
                goto recycle;
            }
            if (b2 > 0U) {
                if (net_pkt_write(pkt,
                                  data->rx_buf[data->rx_tail] + ETH_BUF1_SIZE,
                                  b2)) {
                    LOG_ERR("MAC RX net_pkt_write b2 failed len=%u", b2);
                    net_pkt_unref(pkt);
                    goto recycle;
                }
            }

            if (net_recv_data(data->iface, pkt) < 0) {
                net_pkt_unref(pkt);
            }

recycle:
            desc->descriptor_1 =
                ((data->rx_tail == (ETH_RX_RING_SIZE - 1U)) ? MAC_RDES1_RER : 0U)
                | ((ETH_BUF2_SIZE & MAC_RDES1_RBS2_MASK) << MAC_RDES1_RBS2_OFFSET)
                | ((ETH_BUF1_SIZE & MAC_RDES1_RBS1_MASK) << MAC_RDES1_RBS1_OFFSET);
            desc->descriptor_0 = MAC_RDES0_OWN;
            eth_cache_flush();

            mac->STATUS = MAC_STATUS_RU | MAC_STATUS_AIS;
            mac->RX_POLL = 1U;

            data->rx_tail = (data->rx_tail + 1U) % ETH_RX_RING_SIZE;
            processed++;
            if (processed >= RX_BATCH_BUDGET) {
                processed = 0U;
                k_yield();
            }
        }
    }
}

/* ─── 设备实例注册 ─────────────────────────────────── */

PINCTRL_DT_INST_DEFINE(0);

static struct eth_as32x601_data eth_data_0 = {
    .pcfg     = PINCTRL_DT_INST_DEV_CONFIG_GET(0),
    .mac_addr = DT_INST_PROP(0, local_mac_address),
};

ETH_NET_DEVICE_DT_INST_DEFINE(0,
    eth_as32x601_init, NULL,
    &eth_data_0, NULL,
    CONFIG_ETH_INIT_PRIORITY,
    &eth_as32x601_api,
    NET_ETH_MTU);
