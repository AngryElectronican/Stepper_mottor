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

uint8_t counter=0, flag=0;

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
  TIMSK1|=(1<<OCIE1A);
  sei();
  for(int i=0;;){

    if(USART_Available()){
      temp=USART_Receive();
      
      if(temp=='\n' || temp=='\r'){
        USART_Transmit(counter+'0');
        i=0;
        steps=0;
        TIMSK1&=~(1<<OCIE1A);
        if(counter==0){
          steps=2*atoi(data)+2;
        }else{
          steps=2*atoi(data);
        }
        TIMSK1|=(1<<OCIE1A);
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
          steps=57*2;
        }else if (curr_flags[1]==1 && prev_flags[1]==0){
          steps=-57*2;
        }
        if(curr_flags[2]==1){
          flag=1;
        }else if(curr_flags[3]==1){
          flag=2;
        }else{
          flag=0;
        }
      DDRB|=1<<PB5;
      PORTB^=1<<PB5;
      for(uint8_t j;j<4;prev_flags[j]=curr_flags[j])j++;// prev=curr
      Timer0_StartTimer(&timer1);
    }
  }
}
