
#ifndef __TOUCH_H__
#define __TOUCH_H__
extern volatile int touched;
extern unsigned short touchpointX ,touchpointY;
extern volatile int samplecounter;

extern  void touchInit();
extern unsigned short touchXsample();
extern unsigned short touchYsample();
extern void  touchSample(unsigned short *x,unsigned short *y);
extern void touchSampleExec();

#endif