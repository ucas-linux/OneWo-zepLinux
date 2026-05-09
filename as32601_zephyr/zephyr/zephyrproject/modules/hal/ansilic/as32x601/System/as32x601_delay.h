#ifndef _AS32X601_DELAY_H_
#define _AS32X601_DELAY_H_

#include "as32x601.h"
#include "as32x601_core.h"

/*
 * @brief Function declaration
 */
uint64_t read_cputick(void);
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);

#endif

/*****END OF FILE*****/
