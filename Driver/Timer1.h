#ifndef _TIMER1_H_
#define _TIMER1_H_

#include <avr/interrupt.h>
#define EN1 PD2
#define DIR1 PD3
#define STEP1 PD4

#define BUTTON_PORT PORTC

#define FORWARD PC0
#define BACKWARD PC1
#define CW PC2
#define CCW PC3

void Timer1_Init(uint16_t devider);
void Timer1_RotateSteps(long steps);
void Timer1_LinearSteps(long steps);
#endif
