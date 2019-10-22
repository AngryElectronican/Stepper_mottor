#include "Timer1.h"
#include <avr/io.h>
#include <inttypes.h>

volatile long steps=0;
volatile uint8_t move_flag=0;
ISR(TIMER1_COMPA_vect){
    if(steps>0){    // Angular
      PORTD|=(1<<DIR1);
      steps--;
      PORTD^=1<<STEP1;
    }else if(steps<0){
      PORTD&=~(1<<DIR1);
      steps++;
      PORTD^=1<<STEP1;
    }
     switch(move_flag){   // Linear
       case 1:       // Forward
         PORTD|=(1<<DIR1);
         PORTD^=1<<STEP1;
       break;
       case 2:
         PORTD&=~(1<<DIR1);
         PORTD^=1<<STEP1;
       break;
       default:
         asm("NOP");
       break;
     }
}
void Timer1_Init(uint16_t devider){
    TCCR1B|=(1<<WGM12); //CTC
    TCCR1B|=(1<<CS10);  //no prescaler
    OCR1A=devider;       // 5000 Hz
    TIMSK1|=(1<<OCIE1A);
}
void Timer1_Disable(void){
    TIMSK1&=~(1<<OCIE1A);
}
void Timer1_Enable(void){
    TIMSK1|=(1<<OCIE1A);
}
void Timer1_RotateSteps(long steps_r){
    steps=steps_r;
}
void Timer1_LinearMove(uint8_t dir){
    move_flag=dir;
}
