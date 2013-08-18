/*****************************************************************************
 *   timer32.h:  Header file for NXP LPC1xxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER32_H
#define __TIMER32_H

/* The test is either MAT_OUT or CAP_IN. Default is MAT_OUT. */


#define EMC0	4
#define EMC1	6
#define EMC2	8
#define EMC3	10

#define MATCH0	(1<<0)
#define MATCH1	(1<<1)
#define MATCH2	(1<<2)
#define MATCH3	(1<<3)

//#define TIME_INTERVAL	(SystemCoreClock/100 - 1)
/* depending on the SystemFrequency and SystemCoreClock setting,
if SystemFrequency = 60Mhz, SystemCoreClock = 1/4 SystemFrequency,
10mSec = 150.000-1 counts */

typedef void (*TIMER32_INT_HANDLER)(void); 

extern void delay32Ms(uint8_t timer_num, uint32_t delayInMs);
extern void delay32Us(uint8_t timer_num, uint32_t delayInUs);
extern void CT32B0_IRQHandler(void);
extern void CT32B1_IRQHandler(void);
extern void enable_timer32(uint8_t timer_num);
extern void disable_timer32(uint8_t timer_num);
extern void reset_timer32(uint8_t timer_num);
extern void reset_run_timer32(uint8_t timer_num);
extern unsigned int timer32_get_tc(uint8_t timer_num);
#define TIMER_MATCH_STOP   4
#define TIMER_MATCH_INT    1
#define Timer_MATCH_RESET  2
extern void init_timer32(uint8_t timer_num, uint32_t perscaler,uint32_t timerInterval,uint32_t flag);
extern void init_timer32PWM(uint8_t timer_num, uint32_t period, uint8_t match_enable);
extern void setMatch_timer32PWM (uint8_t timer_num, uint8_t match_nr, uint32_t value);
extern void timer32HandlerRegist(unsigned int index,TIMER32_INT_HANDLER handler);




#endif /* end __TIMER32_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
