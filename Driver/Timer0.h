#ifndef _TIMER0_H_
#define _TIMER0_H_

#include <avr/interrupt.h>

typedef unsigned long Time;

void Timer0_Init(void);
Time Timer0_GetTime(void);
void Timer0_StartTimer(Time* StartTime);
uint8_t Timer0_TimeIsOut(Time* StartTime,Time Delay);

#endif
