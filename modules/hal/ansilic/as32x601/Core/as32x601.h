/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version:         V1.0
 * Date:			2024/01/15
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/01/15     1.0     build this module
 * Description:     This file contains all the data types and state definitions are about the firmware library.
 */
#ifndef AS32X601_H
#define AS32X601_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief  FDevelop test version definition.
 */
// #define AS32x601_MCU
#define AS32x601_FPGA

#define UNUSED(X) (void)X /* To avoid gcc/g++ warnings */

/*
 * @brief  Redister IO definition.
 */
#ifdef __cplusplus
#define __I volatile /* Define the read only permission */
#else
#define __I volatile const /* Define the read only permission */
#endif
#define __O  volatile /* Define the write only permission */
#define __IO volatile /* Define read/write permissions */

/*
 * @brief  Function parameter checking and data assertion definition.
 */
// #include <assert.h>
#define assert_param(expr) ((void)0)
#define assert(expr)       ((void)0)

/*
 * @defgroup  Read/write Specifies the address and read/write register definition.
 */
#define set_32bit_addr(addr, offset, value) (*(volatile uint32_t *)((addr) + (offset)) = value)                     
#define get_32bit_addr(addr, offset) (*(volatile uint32_t *)((addr) + (offset)))
    
#define SET_BIT(REG, BIT)                   ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)                 ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)                  ((REG) & (BIT))

#define CLEAR_REG(REG)                      ((REG) = (0x0))

#define WRITE_REG(REG, VAL)                 ((REG) = (VAL))

#define READ_REG(REG)                       ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define SET_BIT32(REG, BIT)                 ((REG) |= (uint32_t)(BIT))

#define CLEAR_BIT32(REG, BIT)               ((REG) &= ~(uint32_t)(BIT))

#define READ_BIT32(REG, BIT)                ((REG) & (uint32_t)(BIT))

#define CLEAR_REG32(REG)                    ((REG) = (uint32_t)(0x0))

#define WRITE_REG32(REG, VAL)               ((REG) = (uint32_t)(VAL))

#define READ_REG32(REG)                     ((uint32_t)(REG))

#define MODIFY_REG32(REG, MASK, POS, VAL)   (WRITE_REG32((REG), (((REG) & (~((uint32_t)(MASK)))) | (((uint32_t)(VAL)) << (POS)))))

/*
 * @defgroup  Data type definitions used by the firmware library.
 */
typedef const int64_t sc64; /* Read Only */
typedef const int32_t sc32; /* Read Only */
typedef const int16_t sc16; /* Read Only */
typedef const int8_t sc8;   /* Read Only */

typedef __IO uint64_t vu64;
typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef __IO int64_t vs64;
typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t vs8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

/*
 * @brief  Status enum used by the firmware library.
 */
typedef enum 
{
    ERROR   = 0,
    SUCCESS = !ERROR
} ErrorStatus;

typedef enum 
{
    DISABLE = 0,
    ENABLE  = !DISABLE
} FunctionalState, EnState;

typedef enum 
{
    RESET = 0,
    SET   = !RESET
} FlagStatus, ITStatus;

/*
 * @brief  Status parameter check definition.
 */
#define IS_ErrorStatus_STATE(STATE)     (((STATE) == ERROR) || ((STATE) == SUCCESS))
#define IS_FunctionalState_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
#define IS_EnState_STATE(STATE)         (((STATE) == DISABLE) || ((STATE) == ENABLE))
#define IS_FlagStatus_STATE(STATE)      (((STATE) == RESET) || ((STATE) == SET))
#define IS_ITStatus_STATE(STATE)        (((STATE) == RESET) || ((STATE) == SET))

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE*****/
