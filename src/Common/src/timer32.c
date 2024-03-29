/*****************************************************************************
 *   timer32.c:  32-bit Timer C file for NXP LPC1xxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11xx.h"
#include "timer32.h"

volatile uint32_t timer32_0_counter = 0;
volatile uint32_t timer32_1_counter = 0;
volatile uint32_t timer32_0_capture = 0;
volatile uint32_t timer32_1_capture = 0;
volatile uint32_t timer32_0_period = 0;
volatile uint32_t timer32_1_period = 0;

/*****************************************************************************
** Function name:		delay32Ms
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second			
** 						
** Returned value:		None
**
*****************************************************************************/
void delay32Ms(uint8_t timer_num, uint32_t delayInMs)
{
  if (timer_num == 0)
  { 
    if((LPC_SYSCON->SYSAHBCLKCTRL & (1<<9)) ==0)
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);
    /* setup timer #0 for delay */
    LPC_TMR32B0->TCR = 0x02;		/* reset timer */
    LPC_TMR32B0->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR32B0->MR0 = delayInMs * (SystemCoreClock / 1000);
    LPC_TMR32B0->IR  = 0xff;		/* reset all interrrupts */
    LPC_TMR32B0->MCR = 0x04;		/* stop timer on match */
    LPC_TMR32B0->TCR = 0x01;		/* start timer */

    /* wait until delay time has elapsed */
    while (LPC_TMR32B0->TCR & 0x01);
  }
  else if (timer_num == 1)
  {
    if((LPC_SYSCON->SYSAHBCLKCTRL & (1<<10)) ==0)
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
    /* setup timer #1 for delay */
    LPC_TMR32B1->TCR = 0x02;		/* reset timer */
    LPC_TMR32B1->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR32B1->MR0 = delayInMs * (SystemCoreClock / 1000);
    LPC_TMR32B1->IR  = 0xff;		/* reset all interrrupts */
    LPC_TMR32B1->MCR = 0x04;		/* stop timer on match */
    LPC_TMR32B1->TCR = 0x01;		/* start timer */

    /* wait until delay time has elapsed */
    while (LPC_TMR32B1->TCR & 0x01);
  }
  return;
}

/*****************************************************************************
** Function name:		delay32Ms
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second			
** 						
** Returned value:		None
**
*****************************************************************************/
void delay32Us(uint8_t timer_num, uint32_t delayInUs)
{
  if (timer_num == 0)
  { 
    if((LPC_SYSCON->SYSAHBCLKCTRL & (1<<9)) ==0)
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);
    /* setup timer #0 for delay */
    LPC_TMR32B0->TCR = 0x02;		/* reset timer */
    LPC_TMR32B0->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR32B0->MR0 = delayInUs * SystemCoreClock;
    LPC_TMR32B0->IR  = 0xff;		/* reset all interrrupts */
    LPC_TMR32B0->MCR = 0x04;		/* stop timer on match */
    LPC_TMR32B0->TCR = 0x01;		/* start timer */

    /* wait until delay time has elapsed */
    while (LPC_TMR32B0->TCR & 0x01);
  }
  else if (timer_num == 1)
  {
    if((LPC_SYSCON->SYSAHBCLKCTRL & (1<<10)) ==0)
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
    /* setup timer #1 for delay */
    LPC_TMR32B1->TCR = 0x02;		/* reset timer */
    LPC_TMR32B1->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR32B1->MR0 = delayInUs * SystemCoreClock;
    LPC_TMR32B1->IR  = 0xff;		/* reset all interrrupts */
    LPC_TMR32B1->MCR = 0x04;		/* stop timer on match */
    LPC_TMR32B1->TCR = 0x01;		/* start timer */

    /* wait until delay time has elapsed */
    while (LPC_TMR32B1->TCR & 0x01);
  }
  return;
}


/******************************************************************************
** Function name:		CT32B0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
TIMER32_INT_HANDLER handler0 = (void *)0,handler1 = (void *)0;

void timer32HandlerRegist(unsigned int index,TIMER32_INT_HANDLER handler){
  if(0==index){
    handler0 = handler;
  }else if(1==index){
    handler1 = handler;
  }
}

void CT32B0_IRQHandler(void)
{
  if ( LPC_TMR32B0->IR & 0x01 )
  {
	LPC_TMR32B0->IR = 1;				/* clear interrupt flag */
	timer32_0_counter++;
        if(handler0!=(void *)0){
          handler0();
        }
  }
  if ( LPC_TMR32B0->IR & (0x1<<4) )
  {
	LPC_TMR32B0->IR = 0x1<<4;			/* clear interrupt flag */
	timer32_0_capture++;
  }
  return;
}

/******************************************************************************
** Function name:		CT32B1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void CT32B1_IRQHandler(void)
{
  if ( LPC_TMR32B1->IR & 0x01 )
  {
	LPC_TMR32B1->IR = 1;			/* clear interrupt flag */
	timer32_1_counter++;
  }
  if ( LPC_TMR32B1->IR & (0x1<<4) )
  {
	LPC_TMR32B1->IR = 0x1<<4;			/* clear interrupt flag */
	timer32_1_capture++;
  }
  return;
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void enable_timer32(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_TMR32B0->TCR = 1;
  }
  else
  {
    LPC_TMR32B1->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void disable_timer32(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_TMR32B0->TCR = 0;
  }
  else
  {
    LPC_TMR32B1->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void reset_timer32(uint8_t timer_num)
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
    regVal = LPC_TMR32B0->TCR;
    regVal |= 0x02;
    LPC_TMR32B0->TCR = regVal;
  }
  else
  {
    regVal = LPC_TMR32B1->TCR;
    regVal |= 0x02;
    LPC_TMR32B1->TCR = regVal;
  }
  return;
}

void reset_run_timer32(uint8_t timer_num)
{
  
  volatile static int i;
  if ( timer_num == 0 )
  {
    LPC_TMR32B0->TC = 0x03;
    i++;
    i++;
    i++;    
    LPC_TMR32B0->TCR = 0x01;
  }
  else if( timer_num == 1 )
  {
    LPC_TMR32B1->TCR = 0x03;
    i++;
    i++;
    i++;    
    LPC_TMR32B1->TCR = 0x01;
  }
  return;
}


unsigned int timer32_get_tc(uint8_t timer_num){
  unsigned int val;
  val = (timer_num==0)? LPC_TMR32B0->TC:LPC_TMR32B1->TC;
  return val;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		None
**
******************************************************************************/

#define TIMER_MATCH_STOP   4
#define TIMER_MATCH_INT    1
#define Timer_MATCH_RESET  2
void init_timer32(uint8_t timer_num, uint32_t perscaler, uint32_t timerInterval,uint32_t flag)
{
  if ( timer_num == 0 )
  {
    /* Some of the I/O pins need to be clearfully planned if
    you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);
    //LPC_IOCON->PIO1_5 &= ~0x07;	/*  Timer0_32 I/O config */
    //LPC_IOCON->PIO1_5 |= 0x02;	/* Timer0_32 CAP0 */
    //LPC_IOCON->PIO1_6 &= ~0x07;
    //LPC_IOCON->PIO1_6 |= 0x02;	/* Timer0_32 MAT0 */
    //LPC_IOCON->PIO1_7 &= ~0x07;
    //LPC_IOCON->PIO1_7 |= 0x02;	/* Timer0_32 MAT1 */
    //LPC_IOCON->PIO0_1 &= ~0x07;	
    //LPC_IOCON->PIO0_1 |= 0x02;	/* Timer0_32 MAT2 */
#ifdef __JTAG_DISABLED
    LPC_IOCON->R_PIO0_11 &= ~0x07;	
    LPC_IOCON->R_PIO0_11 |= 0x03;	/* Timer0_32 MAT3 */
#endif

    timer32_0_counter = 0;
    timer32_0_capture = 0;
    LPC_TMR32B0->PR = perscaler-1;
    LPC_TMR32B0->MR0 = timerInterval;
#if TIMER_MATCH
	//LPC_TMR32B0->EMR &= ~(0xFF<<4);
	//LPC_TMR32B0->EMR |= ((0x3<<4)|(0x3<<6)|(0x3<<8)|(0x3<<10));	/* MR0/1/2/3 Toggle */
#else
	/* Capture 0 on rising edge, interrupt enable. */
	//LPC_TMR32B0->CCR = (0x1<<0)|(0x1<<2);
#endif
    
    LPC_TMR32B0->MCR = flag & 0x07;			/* Interrupt and Reset on MR0  oneshot*/

    /* Enable the TIMER0 Interrupt */
    NVIC_EnableIRQ(TIMER_32_0_IRQn);
  }
  else if ( timer_num == 1 )
  {
    /* Some of the I/O pins need to be clearfully planned if
    you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
#ifdef __JTAG_DISABLED
    //LPC_IOCON->R_PIO1_0  &= ~0x07;	/*  Timer1_32 I/O config */
    //LPC_IOCON->R_PIO1_0  |= 0x03;	/* Timer1_32 CAP0 */
    //LPC_IOCON->R_PIO1_1  &= ~0x07;	
    //LPC_IOCON->R_PIO1_1  |= 0x03;	/* Timer1_32 MAT0 */
    //LPC_IOCON->R_PIO1_2 &= ~0x07;
    //LPC_IOCON->R_PIO1_2 |= 0x03;	/* Timer1_32 MAT1 */
    //LPC_IOCON->SWDIO_PIO1_3  &= ~0x07;
    //LPC_IOCON->SWDIO_PIO1_3  |= 0x03;	/* Timer1_32 MAT2 */
#endif
    //LPC_IOCON->PIO1_4 &= ~0x07;
    //LPC_IOCON->PIO1_4 |= 0x02;		/* Timer0_32 MAT3 */

    timer32_1_counter = 0;
    timer32_1_capture = 0;
    LPC_TMR32B1->PR = perscaler-1;
    LPC_TMR32B1->MR0 = timerInterval;
#if TIMER_MATCH
	//LPC_TMR32B1->EMR &= ~(0xFF<<4);
	//LPC_TMR32B1->EMR |= ((0x3<<4)|(0x3<<6)|(0x3<<8)|(0x3<<10));	/* MR0/1/2 Toggle */
#else
	/* Capture 0 on rising edge, interrupt enable. */
	//LPC_TMR32B1->CCR = (0x1<<0)|(0x1<<2);
#endif
    LPC_TMR32B1->MCR = flag & 0x07;	/* Interrupt and Reset on MR0 */

    /* Enable the TIMER1 Interrupt */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);
  }
  return;
}
/******************************************************************************
** Function name:		init_timer32PWM
**
** Descriptions:		Initialize timer as PWM
**
** parameters:			timer number, period and match enable:
**										match_enable[0] = PWM for MAT0
**										match_enable[1] = PWM for MAT1
**										match_enable[2] = PWM for MAT2
**			
** Returned value:		None
**
******************************************************************************/
void init_timer32PWM(uint8_t timer_num, uint32_t period, uint8_t match_enable)
{
	
	disable_timer32(timer_num);
	if (timer_num == 1)
	{
	    /* Some of the I/O pins need to be clearfully planned if
	    you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
	    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);

		/* Setup the external match register */
		LPC_TMR32B1->EMR = (1<<EMC3)|(1<<EMC2)|(2<<EMC1)|(1<<EMC0)|(1<<3)|(match_enable);

		/* Setup the outputs */
		/* If match0 is enabled, set the output */
		if (match_enable & 0x01)
		{
		  	LPC_IOCON->R_PIO1_1  &= ~0x07;	
  			LPC_IOCON->R_PIO1_1  |= 0x03;		/* Timer1_32 MAT0 */
		}
		/* If match1 is enabled, set the output */
		if (match_enable & 0x02)
		{
		  LPC_IOCON->R_PIO1_2 &= ~0x07;
		  LPC_IOCON->R_PIO1_2 |= 0x03;		/* Timer1_32 MAT1 */
		}
		/* If match2 is enabled, set the output */
		if (match_enable & 0x04)
		{
		  LPC_IOCON->SWDIO_PIO1_3   &= ~0x07;
		  LPC_IOCON->SWDIO_PIO1_3   |= 0x03;		/* Timer1_32 MAT2 */
		}
		/* If match3 is enabled, set the output */
		if (match_enable & 0x08)
		{
		  LPC_IOCON->PIO1_4           &= ~0x07;
		  LPC_IOCON->PIO1_4           |= 0x02;		/* Timer1_32 MAT3 */
		}

//#ifdef __JTAG_DISABLED
//	  PIO1_0_JTAG_TMS  &= ~0x07;	/*  Timer1_32 I/O config */
//	  PIO1_0_JTAG_TMS  |= 0x03;		/* Timer1_32 CAP0 */
//#endif

		/* Enable the selected PWMs and enable Match3 */
		LPC_TMR32B1->PWMC = (1<<3)|(match_enable);

		/* Setup the match registers */
		/* set the period value to a global variable */
		timer32_1_period = period;
		LPC_TMR32B1->MR3 = timer32_1_period;
		LPC_TMR32B1->MR0	= timer32_1_period/2;
		LPC_TMR32B1->MR1	= timer32_1_period/2;
		LPC_TMR32B1->MR2	= timer32_1_period/2;
		
		/* */
    	LPC_TMR32B1->MCR = 1<<10;				/* Reset on MR3 */
	}
	else
	{
	    /* Some of the I/O pins need to be clearfully planned if
	    you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
	    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

		/* Setup the external match register */
		LPC_TMR32B0->EMR = (1<<EMC3)|(2<<EMC2)|(1<<EMC1)|(1<<EMC0)|(1<<3)|(match_enable);

		/* Setup the outputs */
		/* If match0 is enabled, set the output */
		if (match_enable & 0x01)
		{
//	 		LPC_IOCON->PIO1_6           &= ~0x07;
//		  	LPC_IOCON->PIO1_6           |= 0x02;		/* Timer0_32 MAT0 */
		}
		/* If match1 is enabled, set the output */
		if (match_enable & 0x02)
		{
			LPC_IOCON-> PIO1_7           &= ~0x07;
		  	LPC_IOCON->PIO1_7           |= 0x02;		/* Timer0_32 MAT1 */
		}
		/* If match2 is enabled, set the output */
		if (match_enable & 0x04)
		{
		  LPC_IOCON->PIO0_1           &= ~0x07;	
		  LPC_IOCON->PIO0_1           |= 0x02;		/* Timer0_32 MAT2 */
		}
		/* If match3 is enabled, set the output */
		if (match_enable & 0x08)
		{
		  LPC_IOCON->R_PIO0_11 &= ~0x07;	
		  LPC_IOCON->R_PIO0_11 |= 0x03;		/* Timer0_32 MAT3 */
		}

		/* Enable the selected PWMs and enable Match3 */
		LPC_TMR32B0->PWMC = (1<<3)|(match_enable);

		/* Setup the match registers */
		/* set the period value to a global variable */
		timer32_0_period = period;
		LPC_TMR32B0->MR3 = timer32_0_period;
		LPC_TMR32B0->MR0	= timer32_0_period;	///2;
		LPC_TMR32B0->MR1	= timer32_0_period/2;
		LPC_TMR32B0->MR2	= timer32_0_period/2;

    	LPC_TMR32B0->MCR = 1<<10;				/* Reset on MR3 */
	}


}
/******************************************************************************
** Function name:		pwm32_setMatch
**
** Descriptions:		Set the pwm32 match values
**
** parameters:			timer number, match numner and the value
**
** Returned value:		None
**
******************************************************************************/
void setMatch_timer32PWM (uint8_t timer_num, uint8_t match_nr, uint32_t value)
{
	if (timer_num)
	{
		switch (match_nr)
		{
			case 0:
				LPC_TMR32B1->MR0 = value;
				break;
			case 1:
				LPC_TMR32B1->MR1 = value;
				break;
			case 2:
				LPC_TMR32B1->MR2 = value;
				break;
			case 3:
				LPC_TMR32B1->MR3 = value;
				break;
			default:
				break;
		}	

	}
	else
	{
		switch (match_nr)
		{
			case 0:
				LPC_TMR32B0->MR0 = value;
				break;
			case 1:
				LPC_TMR32B0->MR1 = value;
				break;
			case 2:
				LPC_TMR32B0->MR2 = value;
				break;
			case 3:
				LPC_TMR32B0->MR3 = value;
				break;
			default:
				break;
		}	
	}

}

/******************************************************************************
**                            End Of File
******************************************************************************/
