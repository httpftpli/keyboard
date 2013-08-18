#include "LPC11xx.h"
#include "gpio.h"
#include "timer16.h"

typedef struct __gpiogroup{
  unsigned int port;
  unsigned int bit;
}GPIOGROUP;


GPIOGROUP keyScanOutput[8] ={
  {2,0},{2,1},{2,2},{2,3},
  {2,4},{2,5},{2,6},{2,7}
};

GPIOGROUP keyScanInput[8] ={
  {1,4},{1,5},{3,1},{3,2},
  {1,8},{1,9},{1,10},{1,11}
};

inline unsigned int port2IRQn(unsigned int port){
  return 31-port;
}

void keyScanInit(void){
   init_timer16(1,1 * (SystemCoreClock / 1000) ); //1ms per interrupt
   enable_timer16(1);
   for(int i=0;i<(sizeof(keyScanInput)/sizeof(GPIOGROUP));i++){
      GPIOSetDir(keyScanInput[i].port, keyScanInput[i].bit,0 ); //inpit             
   }
   
   for(int i=0;i<(sizeof(keyScanOutput)/sizeof(GPIOGROUP));i++){
      GPIOSetDir(keyScanOutput[i].port, keyScanOutput[i].bit,1 );
   }
   
   for(int i=0;i<(sizeof(keyScanOutput)/sizeof(GPIOGROUP));i++){
     GPIOSetValue(keyScanOutput[i].port, keyScanOutput[i].bit, 1);
   }
}





void keyScanOutputTest(void){
  for(int i=0;i<(sizeof(keyScanOutput)/sizeof(GPIOGROUP));i++){
    GPIOSetValue(keyScanOutput[i].port, keyScanOutput[i].bit, 0);
  }
}
  

static void leyScanOutputReset(void){
  for(int i=0;i<(sizeof(keyScanOutput)/sizeof(GPIOGROUP));i++){
    GPIOSetValue(keyScanOutput[i].port, keyScanOutput[i].bit, 1);
  }
}


#define TIMEINTERVAL_2KEY        500 //500ms
#define TIMEINTERVAL_DEBOUNCE    10  //10ms
#define TIMEINTERVAL_KEYHOLD     100  //10ms

#define NOKEY                   0
#define KEYTODEBOUNCE           1
#define KEYPUSHED               2
#define KEYHOLD                 3


signed int keyScan(void){
  static unsigned short keycode;
  static unsigned int keystate = NOKEY;
  static unsigned int timermark;
  unsigned short keycodenow;
  for(int i=0;i<(sizeof(keyScanOutput)/sizeof(GPIOGROUP));i++){
    leyScanOutputReset();
    GPIOSetValue(keyScanOutput[i].port, keyScanOutput[i].bit, 0);
    delayUs(0, 20);
    for(int j=0;j<(sizeof(keyScanInput)/sizeof(GPIOGROUP));j++){
        unsigned int bitval = GPIOGetBitValue(keyScanInput[j].port, keyScanInput[j].bit);
        if(0==bitval){
         keycodenow = (i<<8)|j;
         goto KEYPROCESS;
        }
    }
  }
  keystate = NOKEY;
  return -1;
  
KEYPROCESS:
    switch(keystate){
    case NOKEY:
      keystate = KEYTODEBOUNCE;
      keycode = keycodenow;
      timermark = timer16_1_counter;
      break;
    case KEYTODEBOUNCE:
      if(keycodenow==keycode){
        if((timer16_1_counter-timermark)>=TIMEINTERVAL_DEBOUNCE){
          keystate = KEYPUSHED;
          keycode = keycodenow;
          timermark = timer16_1_counter;
          return keycode;
        }
      }else{
        keystate = NOKEY;
      }
      break; 
    case KEYPUSHED:
      if(keycodenow==keycode){
        if((timer16_1_counter-timermark)>=TIMEINTERVAL_2KEY){
          keystate = KEYHOLD;
          keycode = keycodenow;
          timermark = timer16_1_counter;
          return keycode;
        }
      }else {
        keystate = NOKEY;
      }
      break;
    case KEYHOLD:
      if(keycodenow==keycode){
        if((timer16_1_counter-timermark)>=TIMEINTERVAL_KEYHOLD){
          keystate = KEYHOLD;
          keycode = keycodenow;
          timermark = timer16_1_counter;
          return keycode;
        }
      }else {
        keystate = NOKEY;
      }
      break;
    default:
      break;
    }  

return -1;      
}   
   

