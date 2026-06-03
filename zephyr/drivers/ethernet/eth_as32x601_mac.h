/* SPDX-License-Identifier: Apache-2.0 */
/*
 * AS32X601 Core10100 MAC 寄存器定义
 * 基址：0x50000000，PLIC IRQ 24
 *
 * 包含：MAC 寄存器结构体（mac_regs_t）、DMA 描述符类型、
 *        寄存器位域宏、eth_cache_flush() 缓存刷新宏。
 *
 * 位域数值来自 as32x601_eth_regs.h（ANSILIC vendor SDK）。
 */
#ifndef ETH_AS32X601_MAC_H
#define ETH_AS32X601_MAC_H

#include <zephyr/types.h>
#include <zephyr/sys/util.h>

/* MAC 寄存器布局（基址 0x50000000） */
typedef struct {
    volatile uint32_t BUS_MODE;          /* 0x00 */
    volatile uint32_t RESERVED0;         /* 0x04 */
    volatile uint32_t TX_POLL;           /* 0x08 */
    volatile uint32_t RESERVED1;         /* 0x0C */
    volatile uint32_t RX_POLL;           /* 0x10 */
    volatile uint32_t RESERVED2;         /* 0x14 */
    volatile uint32_t RDES_BASE_ADDR;    /* 0x18 */
    volatile uint32_t RESERVED3;         /* 0x1C */
    volatile uint32_t TDES_BASE_ADDR;    /* 0x20 */
    volatile uint32_t RESERVED4;         /* 0x24 */
    volatile uint32_t STATUS;            /* 0x28 */
    volatile uint32_t RESERVED5;         /* 0x2C */
    volatile uint32_t OPT_MODE;          /* 0x30 */
    volatile uint32_t RESERVED6;         /* 0x34 */
    volatile uint32_t INT_ENABLE;        /* 0x38 */
    volatile uint32_t RESERVED7;         /* 0x3C */
    volatile uint32_t FRAME_CNT;         /* 0x40 */
    volatile uint32_t RESERVED8;         /* 0x44 */
    volatile uint32_t MDIO_CTRL0;        /* 0x48 */
    volatile uint32_t RESERVED9;         /* 0x4C */
    volatile uint32_t MDIO_CTRL1;        /* 0x50 */
    volatile uint32_t RESERVED10;        /* 0x54 */
    volatile uint32_t TIMER_CTRL;        /* 0x58 */
} mac_regs_t;

/* BUS_MODE 位域 */
#define MAC_BUS_MODE_SWR        BIT(0)
#define MAC_BUS_MODE_BAR        BIT(1)
#define MAC_BUS_MODE_DSL_Pos    2U
#define MAC_BUS_MODE_BLE        BIT(7)
#define MAC_BUS_MODE_PBL_Pos    8U
#define MAC_BUS_MODE_PBL_0      (0x1U << MAC_BUS_MODE_PBL_Pos)  /* PBL=1 */

/* STATUS 位域（来自 as32x601_eth_regs.h 验证的位位置） */
#define MAC_STATUS_TI           BIT(0)
#define MAC_STATUS_TPS          BIT(1)
#define MAC_STATUS_TU           BIT(2)
#define MAC_STATUS_UNF          BIT(5)
#define MAC_STATUS_RI           BIT(6)
#define MAC_STATUS_RU           BIT(7)
#define MAC_STATUS_ETI          BIT(10)
#define MAC_STATUS_AIS          BIT(15)
#define MAC_STATUS_NIS          BIT(16)

/* OPT_MODE 位域（来自 as32x601_eth_regs.h 验证的位位置） */
#define MAC_OPT_MODE_SR         BIT(1)
#define MAC_OPT_MODE_PB         BIT(3)
#define MAC_OPT_MODE_PR         BIT(6)
#define MAC_OPT_MODE_PM         BIT(7)
#define MAC_OPT_MODE_FD         BIT(9)
#define MAC_OPT_MODE_ST         BIT(13)
#define MAC_OPT_MODE_TR         BIT(15)
#define MAC_OPT_MODE_SF         BIT(21)
#define MAC_OPT_MODE_TTM        BIT(22)
#define MAC_OPT_MODE_RA         BIT(30)

/* INT_ENABLE 位域 */
#define MAC_INT_ENABLE_TIE      BIT(0)
#define MAC_INT_ENABLE_RIE      BIT(6)
#define MAC_INT_ENABLE_NIE      BIT(16)

/* MDIO_CTRL0 位域 */
#define MAC_MDIO_CTRL0_MDC_DIV_Pos  0U
#define MAC_MDIO_CTRL0_MDCE         BIT(16)

/* MDIO_CTRL1 位域 */
#define MAC_MDIO_CTRL1_RDATA_Pos    0U
#define MAC_MDIO_CTRL1_RDATA        GENMASK(15, 0)
#define MAC_MDIO_CTRL1_TURN_1       (0x2U << 16U)
#define MAC_MDIO_CTRL1_REG_ADDR_Pos 18U
#define MAC_MDIO_CTRL1_PHY_ADDR_Pos 23U
#define MAC_MDIO_CTRL1_RDWR_0       (0x1U << 28U)  /* 写 */
#define MAC_MDIO_CTRL1_RDWR_1       (0x2U << 28U)  /* 读 */
#define MAC_MDIO_CTRL1_START0       BIT(30)
#define MAC_MDIO_CTRL1_ACK          BIT(31)

/* TX 描述符类型 */
typedef struct {
    uint32_t        descriptor_0;
    uint32_t        descriptor_1;
    const uint8_t  *buffer_1;
    uint32_t        buffer_2;
} mac_tx_desc_t;

/* RX 描述符类型 */
typedef struct {
    uint32_t  descriptor_0;
    uint32_t  descriptor_1;
    uint8_t  *buffer_1;
    uint32_t  buffer_2;
} mac_rx_desc_t;

/* TX 描述符位域 */
#define MAC_TDES0_OWN           BIT(31)
#define MAC_TDES1_LS            BIT(30)
#define MAC_TDES1_FS            BIT(29)
#define MAC_TDES1_TER           BIT(25)
#define MAC_TDES1_TBS1_OFFSET   0U
#define MAC_TDES1_TBS1_MASK     0x7FFU
#define MAC_TDES1_TBS2_OFFSET   11U
#define MAC_TDES1_TBS2_MASK     0x7FFU

/* RX 描述符位域 */
#define MAC_RDES0_OWN           BIT(31)
#define MAC_RDES0_FL_OFFSET     16U
#define MAC_RDES0_FL_MASK       0x3FFFU
#define MAC_RDES1_RER           BIT(25)
#define MAC_RDES1_RBS1_OFFSET   0U
#define MAC_RDES1_RBS1_MASK     0x7FFU
#define MAC_RDES1_RBS2_OFFSET   11U
#define MAC_RDES1_RBS2_MASK     0x7FFU

/* AS32X601 Core10100 MAC DMA 访问前必须调用的缓存刷新。
 * 使用 AS32X601 CPU 自定义 RISC-V 扩展指令（opcode 0x0F, funct3=5）
 * 等价于参考工程中的 ClearCache()（定义于 as32x601_core.h）。
 * memory clobber 防止编译器跨此调用重排内存访问。 */
#define eth_cache_flush() __asm volatile(".insn r 0x0F,5,0,x0,x0,x0" ::: "memory")

#endif /* ETH_AS32X601_MAC_H */
