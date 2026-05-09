#include "as32x601_delay.h"

/* Private function prototypes -----------------------------------------------*/
static uint8_t fac_us = 0;		/* us delay multiplier */

/*
 * Function:        read_cputick
 * Description:     Read CPU count value.
 * Param:           None.
 * Return:          CPU count value.
 */
uint64_t read_cputick(void)
{
    uint64_t cycle;

    cycle = read_csr(time);
    cycle += (uint64_t)(read_csr(timeh)) << 32;
    return cycle;
}

/*
 * Function:        delay_init
 * Description:     Initialize delay function.
 * Param:           SYSCLK: system clock.
 * Return:          None.
 */
void delay_init(uint8_t SYSCLK)
{
    fac_us = SYSCLK;
}

/*
 * Function:        delay_us
 * Description:     Microsecond delay.
 * Param:           nus: the number of microseconds of delay.
 * Return:          None.
 */
void delay_us(uint32_t nus)
{
    uint64_t ticks;
    uint64_t told, tnow, tcnt = 0;
    uint64_t reload = 0xffffffffffffffff;
    ticks = (uint64_t)(nus * fac_us);		/* The required number of beats */

    told = read_cputick();				/* Counter value upon entry */
    while(1)
    {
        tnow = read_cputick();	
        if(tnow != told)
        {	    
            if(told < tnow)
            {
                tcnt += tnow - told;	/* Please note that time is an increasing counter. */
            }
            else
            {
                tcnt += reload - told + tnow;
            }
            told = tnow;

            if(tcnt >= ticks)break;		/* If the time exceeds/equals the time to be delayed, exit. */
        }  
    }
}

/*
 * Function:        delay_ms
 * Description:     Millisecond delay.
 * Param:           nms: the number of Milliseconds of delay.
 * Return:          None.
 */
void delay_ms(uint32_t nms)
{
    delay_us((uint32_t)(nms*1000));
}

/*****END OF FILE*****/
