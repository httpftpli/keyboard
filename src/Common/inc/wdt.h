/*****************************************************************************
 *   wdt.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __WDT_H 
#define __WDT_H

#define WDEN		0x00000001
#define WDRESET		0x00000002
#define WDTOF		0x00000004
#define WDINT		0x00000008

extern void WDT_IRQHandler(void);
extern void WDTInit( unsigned int wdtInterVal );
extern void WDTFeed( void );

#endif /* end __WDT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
