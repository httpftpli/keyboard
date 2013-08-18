
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "gpio.h"
#include "ssp.h"

/* statistics of all the interrupts */
volatile uint32_t interruptRxStat0 = 0;
volatile uint32_t interruptOverRunStat0 = 0;
volatile uint32_t interruptRxTimeoutStat0 = 0;

volatile uint32_t interruptRxStat1 = 0;
volatile uint32_t interruptOverRunStat1 = 0;
volatile uint32_t interruptRxTimeoutStat1 = 0;

/*****************************************************************************
** Function name:		SSP0_IRQHandler
**
** Descriptions:		SSP port is used for SPI communication.
**						SSP interrupt handler
**						The algorithm is, if RXFIFO is at least half full, 
**						start receive until it's empty; if TXFIFO is at least
**						half empty, start transmit until it's full.
**						This will maximize the use of both FIFOs and performance.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP0_IRQHandler(void) 
{
  uint32_t regValue;

  regValue = LPC_SSP0->MIS;
  if ( regValue & SSPMIS_RORMIS )	/* Receive overrun interrupt */
  {
	interruptOverRunStat0++;
	LPC_SSP0->ICR = SSPICR_RORIC;	/* clear interrupt */
  }
  if ( regValue & SSPMIS_RTMIS )	/* Receive timeout interrupt */
  {
	interruptRxTimeoutStat0++;
	LPC_SSP0->ICR = SSPICR_RTIC;	/* clear interrupt */
  }

  /* please be aware that, in main and ISR, CurrentRxIndex and CurrentTxIndex
  are shared as global variables. It may create some race condition that main
  and ISR manipulate these variables at the same time. SSPSR_BSY checking (polling)
  in both main and ISR could prevent this kind of race condition */
  if ( regValue & SSPMIS_RXMIS )	/* Rx at least half full */
  {
	interruptRxStat0++;		/* receive until it's empty */		
  }
  return;
}

/*****************************************************************************
** Function name:		SSP1_IRQHandler
**
** Descriptions:		SSP port is used for SPI communication.
**						SSP interrupt handler
**						The algorithm is, if RXFIFO is at least half full, 
**						start receive until it's empty; if TXFIFO is at least
**						half empty, start transmit until it's full.
**						This will maximize the use of both FIFOs and performance.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP1_IRQHandler(void) 
{
  uint32_t regValue;

  regValue = LPC_SSP1->MIS;
  if ( regValue & SSPMIS_RORMIS )	/* Receive overrun interrupt */
  {
	interruptOverRunStat1++;
	LPC_SSP1->ICR = SSPICR_RORIC;	/* clear interrupt */
  }
  if ( regValue & SSPMIS_RTMIS )	/* Receive timeout interrupt */
  {
	interruptRxTimeoutStat1++;
	LPC_SSP1->ICR = SSPICR_RTIC;	/* clear interrupt */
  }

  /* please be aware that, in main and ISR, CurrentRxIndex and CurrentTxIndex
  are shared as global variables. It may create some race condition that main
  and ISR manipulate these variables at the same time. SSPSR_BSY checking (polling)
  in both main and ISR could prevent this kind of race condition */
  if ( regValue & SSPMIS_RXMIS )	/* Rx at least half full */
  {
	interruptRxStat1++;		/* receive until it's empty */		
  }
  return;
}

/*****************************************************************************
** Function name:		SSP_IOConfig
**
** Descriptions:		SSP port initialization routine
**				
** parameters:			None
** Returned value:		None 
** 
*****************************************************************************/
void SSP0_IOConfig( void )
{
 
	LPC_SYSCON->PRESETCTRL |= (0x1<<0);
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<11);
	LPC_SYSCON->SSP0CLKDIV = 0x2;			/* Divided by 2 */
	LPC_IOCON->PIO0_8 &= ~0x07;		/*  SSP I/O config */
	LPC_IOCON->PIO0_8 |= 0x01;		/* SSP MISO */
	LPC_IOCON->PIO0_9 &= ~0x07;	
	LPC_IOCON->PIO0_9 |= 0x01;		/* SSP MOSI */
        
        LPC_IOCON->SCK_LOC = 0x01;            //clk             
        LPC_IOCON->PIO2_11 = 0x09;       /*SSP SLK PULL DOWN */          
	//LPC_IOCON->PIO0_6 &= ~0x07;	
	//LPC_IOCON->PIO0_6 = 0x02;
	LPC_IOCON->PIO0_2 &= ~0x1f;	
	//LPC_IOCON->PIO0_2 |= 0x11;	 /* SSP SSEL  PULL UP */
        LPC_IOCON->PIO0_2 |= 0x10;	 /* SSP SSEL gpio PULL UP */
 		
}

/*****************************************************************************
** Function name:		SSP_Init
**
** Descriptions:		SSP port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP0_Init( )
{
  volatile uint8_t i, Dummy;
#define SPI_8BIT 0x07
#define SPI_CPOL_LOW 0
#define SPI_CPHA_FIRST 0

        /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
	LPC_SSP0->CPSR = 0x2;
        
	/*SCR is 5 */
	LPC_SSP0->CR0 = SPI_8BIT<<0 | SPI_CPOL_LOW <<6 | SPI_CPHA_FIRST<<7 | 47<<8;

	for ( i = 0; i < FIFOSIZE; i++ )
	{
	  Dummy = LPC_SSP0->DR;		/* clear the RxFIFO */
	}

	/* Enable the SSP Interrupt */
	NVIC_EnableIRQ(SSP0_IRQn);
	
	/* Device select as master, SSP Enabled */
#if LOOPBACK_MODE
	LPC_SSP0->CR1 = SSPCR1_LBM | SSPCR1_SSE;
#else
	/* Master mode */
	LPC_SSP0->CR1 = SSPCR1_SSE;
#endif
	/* Set SSPINMS registers to enable interrupts */
	/* enable all error related interrupts */
	LPC_SSP0->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;
}

/*****************************************************************************
** Function name:		SSP_Send
**
** Descriptions:		Send a block of data to the SSP port, the 
**						first parameter is the buffer pointer, the 2nd 
**						parameter is the block length.
**
** parameters:			port #, buffer pointer, and the block length
** Returned value:		None
** 
*****************************************************************************/
void SSP0_Send(void *buf, uint32_t Length )
{
  uint32_t i;
  volatile uint8_t Dummy;
    
  for ( i = 0; i < Length; i++ )
  {		
	  /* Move on only TX FIFO not full. */
	  while ( (LPC_SSP0->SR & SSPSR_TNF) != SSPSR_TNF );
	  LPC_SSP0->DR = ((unsigned char *)buf)[i];
  }
  while ( LPC_SSP0->SR & SSPSR_BSY );
}

void SSP0_Receive(void *buf, uint32_t length )
{ 
  volatile unsigned char dumy ;
  while(LPC_SSP0->SR&SSPSR_RNE){
    dumy = LPC_SSP0->DR;
  }
  for(int i = 0;i<length;i++){
    LPC_SSP0->DR = 0;
  }
  while(LPC_SSP0->SR&SSPSR_BSY);
  for(int i = 0;i<length;i++){
    ((unsigned char *)buf)[i] = LPC_SSP0->DR;
  }
}
 


