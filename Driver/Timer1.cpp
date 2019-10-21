#include "Timer1.h"
#include <avr/io.h>
#include <inttypes.h>

volatile long steps=0;

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

    // switch(flag){   // Linear
    //   case 0:
    //     asm("NOP");
    //   break;
    //   case 1:       // Forward
    //     PORTD|=(1<<DIR1);
    //     PORTD^=1<<STEP1;
    //   break;
    //   case 2:
    //     PORTD&=~(1<<DIR1);
    //     PORTD^=1<<STEP1;
    //   break;
    // }
}
void Timer1_Init(uint16_t devider){
    TCCR1B|=(1<<WGM12); //CTC
    TCCR1B|=(1<<CS10);  //no prescaler
    OCR1A=devider;       // 5000 Hz
}
void Timer1_RotateSteps(long steps){
    asm("NOP");
}
void Timer1_LinearSteps(long steps){
    asm("NOP");
}
