#include "stdint.h"
#include "ssp.h"
#include "gpio.h"
#include "timer32.h"
#include "lpc11xx.h"


#define ADS7843_CS_GPIO             0
#define ADS7843_CS_GPIOBIT          2
#define ADS7843_INT_GPIO            2
#define ADS7843_INT_GPIOBIT         10

#define ADS7843_CTRL_X_SAMPLE   0xd0   //0b11010000
#define ADS7843_CTRL_Y_SAMPLE   0x90   //0b10010000

#define NARRAY(array) (sizeof((array))/sizeof((array[0])))


int debugbufx[500], debugbufy[500];
int debugbufindex = 0,debugbufindex1 = 0;
int npoint = 0;

volatile unsigned int touched = 0;

unsigned short touchpointX ,touchpointY;

void bubbleSortAscend(int *buf, unsigned int nNum) {
   int j, k;
   int flag = nNum;
   unsigned int tmp;
   while (flag > 0) {
      k = flag;
      flag = 0;
      for (j = 1; j < k; j++){
         if (buf[j - 1] > buf[j]) {
            tmp = buf[j-1];
            buf[j - 1] = buf[j];
            buf[j] = tmp;
            flag = j;
         }
      }
   }
}

int sum(int *buf,unsigned int nNum){
   unsigned int sum = 0;
   for (int i=0;i<nNum;i++) {
      sum += buf[i];
   }
   return sum;
}


static void  touchTimerInit();

void touchInit(){
  SSP0_IOConfig(); 
  GPIOSetDir(ADS7843_CS_GPIO,ADS7843_CS_GPIOBIT,1);  //out
  GPIOSetDir(ADS7843_INT_GPIO,ADS7843_INT_GPIOBIT,0); //in put
  GPIOSetInterrupt(ADS7843_INT_GPIO,ADS7843_INT_GPIOBIT, 0, 0, 0); //signal fall edge 
  //GPIOIntEnable(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT);
  GPIOSetValue(ADS7843_CS_GPIO,ADS7843_CS_GPIOBIT,1); //output hight
  SSP0_Init();   //spi_clk is 1mhz
  touchTimerInit();
  
}

unsigned short touchXsample(){
  unsigned char rcvbuf[2];
  unsigned char sendval = ADS7843_CTRL_X_SAMPLE;
  GPIOSetValue(ADS7843_CS_GPIO,ADS7843_CS_GPIOBIT, 0);
  SSP0_Send(&sendval, 1); 
  SSP0_Receive(rcvbuf, sizeof rcvbuf);
  GPIOSetValue(ADS7843_CS_GPIO,ADS7843_CS_GPIOBIT, 1);
  unsigned short val = (((unsigned short)rcvbuf[0])&0x7f)<<5;
  return val | (rcvbuf[1])>>3;
}


unsigned short touchYsample(){
  unsigned char rcvbuf[2];
  unsigned char sendval = ADS7843_CTRL_Y_SAMPLE;
  GPIOSetValue(ADS7843_CS_GPIO,ADS7843_CS_GPIOBIT, 0);
  SSP0_Send(&sendval, 1);
  SSP0_Receive(rcvbuf, sizeof rcvbuf );
  unsigned short val = (((unsigned short)rcvbuf[0])&0x7f)<<5;
  return val | (rcvbuf[1])>>3;
}


void  touchSample(int *x, int *y){
     int xsampleval[5],ysampleval[5];
     //GPIOSetValue(3, 0, 1);  //to debug
     for(int i=0;i<5;i++){
        xsampleval[i] = touchXsample();
     }
     bubbleSortAscend(xsampleval,NARRAY(xsampleval));
     for(int i=0;i<5;i++){
        ysampleval[i] = touchYsample();
     }
     bubbleSortAscend(ysampleval,NARRAY(ysampleval));    
     //GPIOSetValue(3, 0, 0); //to debug
     *y =  sum(ysampleval+1,NARRAY(ysampleval)-2)/(NARRAY(ysampleval)-2);
     *x =  sum(xsampleval+1,NARRAY(xsampleval)-2)/(NARRAY(xsampleval)-2);
     //debugbufx[debugbufindex] = *x;
     //debugbufy[debugbufindex++] = *y;
     
}

static void touchTimerInit(){
  init_timer32(1, (SystemCoreClock/1000/1000),0,0); //TC :1us
  enable_timer32(1);
}

void touchTimerReset(){
  reset_run_timer32(1);
}

unsigned int touchGetTime(){
  return timer32_get_tc(1);
}


#if 0
void touchSampleExec(){
#define NO_TOUCH      0
#define TOUCHED_RELEASE  3
#define TOUCH_SAMPLE_BEGIN  5
#define TOUCH_DEBOUNCE    7
  static int samplecounter = 0,stat = NO_TOUCH;
  static unsigned int sumx = 0,sumy = 0;
  static unsigned short lastx = 0,lasty = 0;

  switch(stat){
    case NO_TOUCH:
      if(!GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
          touchSample(&lastx,&lasty);
          sumx += lastx;
          sumy += lasty;
          samplecounter++;
          stat = TOUCHE_SAMPLE_BEGIN;
          touchTimerReset();   //复位定时器
      }
      break;
  case TOUCHE_SAMPLE_BEGIN:
      if(!GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
          touchSample(&lastx,&lasty);
          sumx += lastx;
          sumy += lasty;
          samplecounter++;
          stat = TOUCHE_SAMPLE_BEGIN;
          if(touchGetTime()>50000){ //100ms
             touchpointX = (sumx)/(samplecounter);
             touchpointY = (sumy)/(samplecounter);
             sumx = 0;
             sumy = 0;
             samplecounter = 0;
             touched = 1;
             touchTimerReset();            
          }
      }else{
        {
        if(samplecounter>=5){
          touchpointX = (sumx-lastx)/(samplecounter-1);
          touchpointY = (sumy-lasty)/(samplecounter-1);          
          touched = 1;
        }else{
          npoint = samplecounter;
        }
        }
        sumx = 0;
        sumy = 0;
        samplecounter = 0;
        stat = NO_TOUCH; 
      }
  default:
      break;
  }
}
#endif

void touchSampleExec(){
#define NO_TOUCH            0
#define TOUCHED     3
#define TOUCH_SAMPLE_BEGIN  5
#define TOUCH_DEBOUNCE      7
#define TOUCH_FROZE       4
  
#define TOUCH_SAMPLE_NUM   7
  static int samplecounter = 0,stat = NO_TOUCH;  
  static int xbuf[TOUCH_SAMPLE_NUM+2],ybuf[TOUCH_SAMPLE_NUM+2];  
  
  switch(stat){
    case NO_TOUCH:
      if(!GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
         stat = TOUCH_DEBOUNCE;
         touchTimerReset();
      }
      break;
  case TOUCH_DEBOUNCE:
    if(GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
        samplecounter = 0;
        stat = NO_TOUCH;
    }else{
      if(touchGetTime()>16000){
        samplecounter = 0;
        stat = TOUCH_SAMPLE_BEGIN;
      }
    }
    break;
  case TOUCH_SAMPLE_BEGIN:
     if(GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
        samplecounter = 0;
        stat = NO_TOUCH;
    }else{
      touchSample(xbuf+samplecounter,ybuf+samplecounter);     
      samplecounter ++;
      if(samplecounter >= TOUCH_SAMPLE_NUM+1){
         samplecounter --;
         bubbleSortAscend(xbuf, samplecounter);
         bubbleSortAscend(ybuf, samplecounter);
         if(((xbuf[TOUCH_SAMPLE_NUM-2-1]-xbuf[2])<10)&&
            ((ybuf[TOUCH_SAMPLE_NUM-2-1]-ybuf[2])<10)){
           touchpointX = sum(xbuf+2,samplecounter-4)/(samplecounter-4);
           touchpointY = sum(ybuf+2,samplecounter-4)/(samplecounter-4);        
           touched = 1;
           stat = TOUCHED;
          }
         samplecounter = 0;                              
         touchTimerReset();        
      }     
    }
    break;
  case TOUCHED:
     if(GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
        samplecounter = 0;
        stat = NO_TOUCH;
    }else{
      //touchTimerReset();
      touchSample(xbuf+samplecounter,ybuf+samplecounter);     
      samplecounter ++;
      if((samplecounter >= TOUCH_SAMPLE_NUM+1)){ 
          samplecounter --;
         bubbleSortAscend(xbuf, samplecounter);
         bubbleSortAscend(ybuf, samplecounter);
          if(((xbuf[TOUCH_SAMPLE_NUM-2-1]-xbuf[2])<10)&&
            ((ybuf[TOUCH_SAMPLE_NUM-2-1]-ybuf[2])<10)){
            touchpointX = sum(xbuf+2,samplecounter-4)/(samplecounter-4);
            touchpointY = sum(ybuf+2,samplecounter-4)/(samplecounter-4); 
            touched = 1;
            stat = TOUCH_FROZE;
         }
         samplecounter = 0;         
         //touchTimerReset();
      }
    }
    break;
  case TOUCH_FROZE:
    if(GPIOGetBitValue(ADS7843_INT_GPIO, ADS7843_INT_GPIOBIT)){
        samplecounter = 0;
        stat = NO_TOUCH;
    }else{
      if(touchGetTime()>50000){
        stat = TOUCHED;
        samplecounter = 0;
        touchTimerReset();
      }
    }
    break;
  default:
    break;
  }
}
    