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

uint8_t counter=0;

Time timer1=0;
int main(){

  DDRC&=~((1<<FORWARD)|(1<<BACKWARD)|(1<<CW)|(1<<CCW));
  PORTC|=(1<<FORWARD)|(1<<BACKWARD)|(1<<CW)|(1<<CCW);
  char temp=0;
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
      temp=USART_Receive();
      
      if(temp=='\n' || temp=='\r'){
        USART_Transmit(counter+'0');
        i=0;
        Timer1_RotateSteps(0);
        Timer1_Disable();
        if(counter==0){
          Timer1_RotateSteps(2*atoi(data)+2);
        }else{
          Timer1_RotateSteps(2*atoi(data));
        }
        Timer1_Enable();
        counter++;
        if(counter>=7) counter=0;
        for(int j=0;j<10;data[j++]=0);
        }else if((temp>='0' && temp<='9')||temp=='-'|| temp=='+'){
         data[i++]=temp;
        }
    }
    if(Timer0_TimeIsOut(&timer1,100)){
      static uint8_t prev_flags[]={0,0,0,0};
      uint8_t curr_flags[4];
      for(uint8_t j=0;j<4;j++){
          curr_flags[j]=(!((PINC>>j)&1));//Read !pin
        }
        
        if(curr_flags[0]==1 && prev_flags[0]==0){
          Timer1_Disable();
          Timer1_RotateSteps(57*2);
          Timer1_Enable();
        }else if (curr_flags[1]==1 && prev_flags[1]==0){
          Timer1_Disable();
          Timer1_RotateSteps(-57*2);
          Timer1_Enable();
        }
        for(uint8_t k=0;k<4;k++){
          USART_Transmit(curr_flags[k]+'0');
          }
          USART_Transmit('\n');
          for(uint8_t k=0;k<4;k++){
          USART_Transmit(prev_flags[k]+'0');
          }
          USART_Transmit('\n');
          USART_Transmit('\n');
          prev_flags[0]=curr_flags[0];
          prev_flags[1]=curr_flags[1];
        if(curr_flags[2]==1){
          Timer1_LinearMove(1);
        }else if(curr_flags[3]==1){
          Timer1_LinearMove(2);
        }else{
          Timer1_LinearMove(0);
        }
      DDRB|=1<<PB5;
      PORTB^=1<<PB5;
      Timer0_StartTimer(&timer1);
    }
  }
}
