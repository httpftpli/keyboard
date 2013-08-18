#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "include.h"
#include "timer16.h"
#include "timer32.h"
#include "clkconfig.h"
#include "wdt.h"
#include "touch.h"


#define MSG_TYPE_KEY    1
#define MSG_TYPE_TOUCH  2
#define MSG_TYPE_KEYRESET 4


#pragma pack(1)
typedef struct __message{
  unsigned char type;
  unsigned char magic;
  unsigned short keycode;
  unsigned int tscval;
} MSG;
#pragma pack()

MSG msg = {.magic = 0x55};

unsigned int ctrlbeep=0;

volatile int keybeepin = 0;

#define BEEPIN_GPIO      0
#define BEEPIN_GPIOBIT   6

#define BEEPOUT_GPIO     3
#define BEEPOUT_GPIOBIT  0

#define LCDBLIN_GPIO     1
#define LCDBLIN_GPIOBIT  2 

#define LCDBL_GPIO     1
#define LCDBL_GPIOBIT  1




static void beeptimerhandle (void){
  GPIOSetValue( BEEPOUT_GPIO, BEEPOUT_GPIOBIT,0);
  keybeepin = 0;
}
  

void beepInit(void){
  GPIOSetDir(BEEPOUT_GPIO, BEEPOUT_GPIOBIT, 1);//beepout pin
  GPIOSetValue(BEEPOUT_GPIO, BEEPOUT_GPIOBIT, 0);
  init_timer32(0, 1 ,70 * (SystemCoreClock / 1000),TIMER_MATCH_STOP|TIMER_MATCH_INT|Timer_MATCH_RESET); 
  timer32HandlerRegist(0 ,beeptimerhandle);
}
  

void beep(void){
   GPIOSetValue( BEEPOUT_GPIO, BEEPOUT_GPIOBIT,1);
   keybeepin = 1;
   reset_timer32(0);
   enable_timer32(0);
}


volatile unsigned short xsam,ysam;


int main()
{
   int send = 0;
   SystemInit ();   //arm core clk 24mhz
   GPIOInit();
   WDT_CLK_Setup(WDTCLK_SRC_WDT_OSC); //250khz watchdog clock
   UARTInit(57600);
   touchInit();
   msg.type = MSG_TYPE_KEYRESET;
   UARTSend(&msg, sizeof(msg));
   beepInit();
   GPIOSetDir(BEEPIN_GPIO, BEEPIN_GPIOBIT, 0);//beepin pin 
   
   LPC_IOCON->R_PIO1_1 = 1<<0 | 2<<3 | 1<<7 ;
   GPIOSetDir(LCDBL_GPIO, LCDBL_GPIOBIT, 1);  //LCD BLACK pin OUT
   GPIOSetValue(LCDBL_GPIO, LCDBL_GPIOBIT, 1); //LCD BLACKLIGTH OFF
   keyScanInit();
   
#ifdef NDEBUG
   WDTInit(125000);
#endif
   unsigned int keycode =-1;
   while(1){
#ifdef NDEBUG
      WDTFeed();
#endif
      keycode = keyScan();
      if(keycode!=-1){
        msg.type |= MSG_TYPE_KEY;
        msg.keycode = (unsigned short)keycode;
        beep();
        send = 1;
      } 
           
      if(!GPIOGetBitValue(BEEPIN_GPIO, BEEPIN_GPIOBIT)){
         GPIOSetValue( BEEPOUT_GPIO, BEEPOUT_GPIOBIT,1);
      }else if(keybeepin==0){
         GPIOSetValue( BEEPOUT_GPIO, BEEPOUT_GPIOBIT,0);
      }
      
      if(!GPIOGetBitValue(LCDBLIN_GPIO,LCDBLIN_GPIOBIT)){
        GPIOSetValue(LCDBL_GPIO, LCDBL_GPIOBIT, 0); //LCD BLACKLIGTH ON 
      }else{
        GPIOSetValue(LCDBL_GPIO, LCDBL_GPIOBIT, 1); //LCD BLACKLIGTH OFF
      }
      touchSampleExec();
      ////////////////////////////  
#if 0
      static unsigned int timerbuf[256];
      static int timerbufindex = 0;
      static unsigned int oldtimer ;
#endif
      ////////////////////////////
      if(touched==1){
       ////////////////////////////    
        #if 0
        timerbuf[timerbufindex++] = timer16_1_counter-oldtimer ;
        oldtimer = timer16_1_counter;
        if(timerbufindex == 256){
          timerbufindex =0 ;
        }
        #endif
        ////////////////////////////          
        touched=0; 
        msg.type |= MSG_TYPE_TOUCH;
        msg.tscval = touchpointY<<16 | touchpointX;
        send = 1;
      }
      if(1==send){
        UARTSend(&msg,sizeof msg);
        send =0;
        msg.type = 0;
      }   
  }
}





