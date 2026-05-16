/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_addrmap.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/01/15
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/01/15     1.0     build this module
 * Description:     This file contains all address definitions related to the firmware library.
 */

#ifndef  AS32X601_ADDRMAP_H
#define  AS32X601_ADDRMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @defgroup Address definition
 */
#define AXI_Bus0_BASE		((uint32_t)0x00000000)
#define BOOT_ROM_BASE		((uint32_t)AXI_Bus0_BASE + 0x00000000)
#define P_FLASH_Main_BASE	((uint32_t)AXI_Bus0_BASE + 0x01000000)
#define P_FLASH_Info_BASE	((uint32_t)AXI_Bus0_BASE + 0x01200000)
#define D_FLASH_Main_BASE	((uint32_t)AXI_Bus0_BASE + 0x02000000)
#define D_FLASH_Info_BASE	((uint32_t)AXI_Bus0_BASE + 0x02080000)
#define QSPI_FLASH_BASE		((uint32_t)AXI_Bus0_BASE + 0x10000000)
#define AXI_Bus1_BASE		((uint32_t)0x20000000)
#define SRAM0_BASE			((uint32_t)AXI_Bus1_BASE + 0x00000000)
#define SRAM1_BASE			((uint32_t)AXI_Bus1_BASE + 0x00020000)
#define SRAM2_BASE			((uint32_t)AXI_Bus1_BASE + 0x00040000)
#define SRAM3_BASE			((uint32_t)AXI_Bus1_BASE + 0x00060000)
#define AXi_Bus2_BASE		((uint32_t)0x30000000)
#define AXILite4_Bus0_BASE	((uint32_t)0x30000000)
#define MPU0_IBUS_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00000000)
#define MPU1_DBUS_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00000400)
#define MPU2_PBUS_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00000800)
#define MPU3_DMA0_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00000C00)
#define MPU4_DMA1_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00001000)
#define FCU_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00002000)
#define CMU0_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00003000)
#define CMU1_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00003400)
#define CMU2_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00003800)
#define CMU3_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00003C00)
#define DMA0_CFG_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00004000)
#define DMA1_CFG_BASE		((uint32_t)AXILite4_Bus0_BASE + 0x00004400)
#define SMU_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00006000)
#define CLINT_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00010000)
#define PLIC_BASE			((uint32_t)AXILite4_Bus0_BASE + 0x00020000)
#define APB_BusS_BASE		((uint32_t)0x30100000)
#define BACKUP_RAM_BASE		((uint32_t)APB_BusS_BASE + 0x00000000)
#define RTC_BASE			((uint32_t)APB_BusS_BASE + 0x00001000)
#define GPIOH_BASE			((uint32_t)APB_BusS_BASE + 0x00002000)
#define PMU_BASE			((uint32_t)APB_BusS_BASE + 0x00003000)
#define IWDG_BASE			((uint32_t)APB_BusS_BASE + 0x00004000)
#define AXI_Bus3_BASE		((uint32_t)0x40000000)
#define AXILite4_Bus1_BASE	((uint32_t)0x41000000)
#define TIM0_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00000000)
#define CANFD0_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00010000)
#define CANFD1_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00020000)
#define ADC0_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00001000)
#define ADC1_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00002000)
#define DAC_CMP0_BASE		((uint32_t)AXILite4_Bus1_BASE + 0x00003000)
#define GPIOA_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00004000)
#define GPIOB_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00005000)
#define GPIOC_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00006000)
#define DSE_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00100000)
#define SPACC_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00100000)
#define PKA_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00140000)
#define TRNG_BASE           ((uint32_t)AXILite4_Bus1_BASE + 0x00150000)
#define REG_BASE			((uint32_t)AXILite4_Bus1_BASE + 0x00160000)
#define AXILite4_Bus2_BASE	((uint32_t)0x42000000)
#define TIM1_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00000000)
#define CANFD2_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00010000)
#define CANFD3_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00020000)
#define DAC_CMP1_BASE		((uint32_t)AXILite4_Bus2_BASE + 0x00002000)
#define GPIOD_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00003000)
#define GPIOE_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00004000)
#define GPIOF_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00005000)
#define GPIOG_BASE			((uint32_t)AXILite4_Bus2_BASE + 0x00006000)
#define FLASH_CTRL_BASE		((uint32_t)AXILite4_Bus2_BASE + 0x00100000)
#define QSPI_CTRL_BASE		((uint32_t)AXILite4_Bus2_BASE + 0x00100800)
#define APBBus0_BASE		((uint32_t)0x50000000)
#define MAC_BASE			((uint32_t)APBBus0_BASE + 0x00000000)
#define SPI0_BASE			((uint32_t)APBBus0_BASE + 0x00001000)
#define SPI1_BASE			((uint32_t)APBBus0_BASE + 0x00002000)
#define SPI2_BASE			((uint32_t)APBBus0_BASE + 0x00003000)
#define IIC_IIS0_BASE		((uint32_t)APBBus0_BASE + 0x00004000)
#define IIC_IIS1_BASE		((uint32_t)APBBus0_BASE + 0x00005000)
#define USART0_BASE			((uint32_t)APBBus0_BASE + 0x00006000)
#define USART1_BASE			((uint32_t)APBBus0_BASE + 0x00007000)
#define USART2_BASE			((uint32_t)APBBus0_BASE + 0x00008000)
#define USART3_BASE			((uint32_t)APBBus0_BASE + 0x00009000)
#define TIM2_BASE			((uint32_t)APBBus0_BASE + 0x0000A000)
#define TIM3_BASE			((uint32_t)APBBus0_BASE + 0x0000B000)
#define TIM4_BASE			((uint32_t)APBBus0_BASE + 0x0000C000)
#define WWDG0_BASE			((uint32_t)APBBus0_BASE + 0x0000D000)
#define APBBus1_BASE		((uint32_t)0x51000000)
#define SPI3_BASE			((uint32_t)APBBus1_BASE + 0x00000000)
#define SPI4_BASE			((uint32_t)APBBus1_BASE + 0x00001000)
#define SPI5_BASE			((uint32_t)APBBus1_BASE + 0x00002000)
#define IIC_IIS2_BASE		((uint32_t)APBBus1_BASE + 0x00003000)
#define IIC_IIS3_BASE		((uint32_t)APBBus1_BASE + 0x00004000)
#define USART4_BASE			((uint32_t)APBBus1_BASE + 0x00005000)
#define USART5_BASE			((uint32_t)APBBus1_BASE + 0x00006000)
#define USART6_BASE			((uint32_t)APBBus1_BASE + 0x00007000)
#define USART7_BASE			((uint32_t)APBBus1_BASE + 0x00008000)
#define TIM5_BASE			((uint32_t)APBBus1_BASE + 0x00009000)
#define TIM6_BASE			((uint32_t)APBBus1_BASE + 0x0000A000)
#define TIM7_BASE			((uint32_t)APBBus1_BASE + 0x0000B000)
#define WWDG1_BASE			((uint32_t)APBBus1_BASE + 0x0000C000)
#define CRC_BASE			((uint32_t)APBBus1_BASE + 0x0000D000)

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
