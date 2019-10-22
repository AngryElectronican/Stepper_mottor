#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define STEP 200
#define STEP_DEVIDER 8
#define DEVIDER FOSC/STEP/STEP_DEVIDER/2

#include <avr/interrupt.h>
#include "USART.h"
#include "Timer0.h"
#include "Timer1.h"
unsigned char data[10]={0};



Time timer1=0;
int main(){

  DDRC&=~((1<<FORWARD)|(1<<BACKWARD)|(1<<CW)|(1<<CCW));
  PORTC|=(1<<FORWARD)|(1<<BACKWARD)|(1<<CW)|(1<<CCW);
  
  DDRD|=(1<<STEP1)|(1<<DIR1)|(1<<EN1)|(1<<PD1);
  PORTD&=~(1<<STEP1);
  PORTD|=1<<EN1;

  USART_Init(MYUBRR);
  Timer1_Init(DEVIDER);
  Timer0_Init();
  Timer0_StartTimer(&timer1);
  
  sei();
  for(int i=0;;){
    if(USART_Available()){
      char temp=USART_Receive();
      if(temp=='\n' || temp=='\r'){
        USART_Transmit(counter+'0');
        i=0;
        Timer1_RotateSteps(0);
        Timer1_Disable();
        Timer1_RotateSteps(atoi(data));
        Timer1_Enable();
        for(int j=0;j<10;data[j++]=0);
        }else if((temp>='0' && temp<='9')||temp=='-'|| temp=='+'){ // write to data if correct
         data[i++]=temp;
        }
    }
    if(Timer0_TimeIsOut(&timer1,100)){
      static uint8_t prev_flags[]={0,0};
      uint8_t curr_flags[4];
      static int32_t counter=0;
      for(uint8_t j=0;j<4;j++){
          curr_flags[j]=(!((PINC>>j)&1));//Read !pin
        }
        if(curr_flags[0]==1 && prev_flags[0]==0){ // Angular Buttons
          long rotation_step=0;
          if(counter%7==0){
            rotation_step=58;
            }else{
              rotation_step=57;
            }
          Timer1_Disable();
          Timer1_RotateSteps(rotation_step);
          Timer1_Enable();
          counter++;
        }else if (curr_flags[1]==1 && prev_flags[1]==0){
          long rotation_step=0;
          if((counter-1)%7==0){
            rotation_step=58;
            }else{
              rotation_step=57;
            }
          Timer1_Disable();
          Timer1_RotateSteps(-57);
          Timer1_Enable();
          counter--;
        }
        if(curr_flags[2]==1){ //Linear Buttons
          Timer1_LinearMove(1);
        }else if(curr_flags[3]==1){
          Timer1_LinearMove(2);
        }else{
          Timer1_LinearMove(0);
        }
      prev_flags[0]=curr_flags[0];
      prev_flags[1]=curr_flags[1];
      DDRB|=1<<PB5;
      PORTB^=1<<PB5;          //Blink
      Timer0_StartTimer(&timer1);
    }
  }
}
