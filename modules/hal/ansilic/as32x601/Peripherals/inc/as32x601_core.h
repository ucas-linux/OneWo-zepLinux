/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	  as32x601_core.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			      2024/08/20
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/08/20     1.0     build this module
 * Description:     This file contains all the functions prototypes for the core configuration.
 */

#ifndef AS32X601_CORE_H
#define AS32X601_CORE_H

#ifdef __cplusplus
extern "C" {
#endif
  
/*
 * @brief  Clock source frequency definition
 */
#define CLK_FIRC_HZ (16000000U) /* 16MHz FIRC */
#define CLK_OSC_HZ  (20000000U) /* 20MHz OSC */

/*
 * @brief  Interrupt nesting support switch definition(1:on,0:off).
 */
#define INTNEST 0

/*
 * @brief  Dual core lockstep switch definition(1:on,0:off).
 */
#define DCLS 1

/*
 * @brief  CSR register bits definition
 */
#define MIE_SSIE     (0x00000002U)
#define MIE_MSIE     (0x00000008U)
#define MIE_STIE     (0x00000020U)
#define MIE_MTIE     (0x00000080U)
#define MIE_SEIE     (0x00000200U)
#define MIE_MEIE     (0x00000800U)
#define MSTATUS_UIE  (0x00000001U)
#define MSTATUS_SIE  (0x00000002U)
#define MSTATUS_HIE  (0x00000004U)
#define MSTATUS_MIE  (0x00000008U)
#define MSTATUS_UPIE (0x00000010U)
#define MSTATUS_SPIE (0x00000020U)
#define MSTATUS_HPIE (0x00000040U)
#define MSTATUS_MPIE (0x00000080U)
#define MSTATUS_SPP  (0x00000100U)
#define MSTATUS_HPP  (0x00000600U)
#define MSTATUS_MPP  (0x00001800U)
#define MSTATUS_FS   (0x00006000U)
#define MSTATUS_XS   (0x00018000U)
#define MSTATUS_MPRV (0x00020000U)
#define MSTATUS_SUM  (0x00040000U)
#define MSTATUS_MXR  (0x00080000U)
#define MSTATUS_TVM  (0x00100000U)
#define MSTATUS_TW   (0x00200000U)
#define MSTATUS_TSR  (0x00400000U)

/*
 * @brief  Read CSR register definition
 */
#define read_csr(reg) ({ unsigned long __tmp; \
                        __asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
                        __tmp; })

/*
 * @brief  Write CSR register definition
 */
#define write_csr(reg, val)  __asm volatile ("csrw " #reg ", %0" :: "r"(val));

/*
 * @brief  Clear chche definition
 */
#define clear_cache() __asm volatile(".insn r 0x0F,5,0,x0,x0,x0")

/*
 * @brief Function declaration
 */
void SystemInit(void);
void SYS_BusClkInit(void);
void ClearCache(void);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
