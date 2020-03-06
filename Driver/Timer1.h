#ifndef _TIMER1_H_
#define _TIMER1_H_

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define STEP 200
#define STEP_DEVIDER 8
#define DEVIDER FOSC/STEP/STEP_DEVIDER/2

#include <avr/interrupt.h>

#define EN1 PD3
#define DIR1 PD4
#define STEP1 PD5

#define BUTTON_PORT PORTC

#define FORWARD PC0
#define BACKWARD PC1
#define CW PC2
#define CCW PC3

void Timer1_Init(uint16_t devider);
void Timer1_Disable(void);
void Timer1_Enable(void);
void Timer1_RotateSteps(long steps_r);
void Timer1_LinearMove(uint8_t steps_l);
#endif
