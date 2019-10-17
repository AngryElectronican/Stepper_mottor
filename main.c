#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/interrupt.h>

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
byte USART_available();

unsigned char data[10]={0};
long steps=0;

#define EN PD2
#define DIR PD3
#define STEP PD4

ISR(TIMER1_COMPA_vect){
    if(steps>0){
      PORTD|=(1<<DIR);
      steps--;
      PORTD^=1<<STEP;
    }else if(steps<0){
      PORTD&=~(1<<DIR);
      steps++;
      PORTD^=1<<STEP;
      }
    }

int main(){
  //TCCR0A=0;
  char temp=0;
  DDRD|=(1<<PD4)|(1<<PD2)|(1<<PD3);
  TCCR1B|=(1<<WGM12);
  TCCR1B|=(1<<CS10);
  TIMSK1|=(1<<OCIE1A);
  OCR1A=0x2FF0;
  PORTD|=1<<PD2;
  USART_Init(MYUBRR);
  sei();
  for(int i=0;;){
  if(USART_available()){
    temp=USART_Receive();
    //USART_Transmit(temp);
    if(temp=='\n' || temp=='\r'){
      i=0;
      steps=0;
      steps=atoi(data);
      for(int j=0;j<10;data[j++]=0){
        ;}
      }else if((temp>='0' && temp<='9')||temp=='-'|| temp=='+'){
       data[i++]=temp;
      }
    }
  }
}

void USART_Init(unsigned int ubrr)
{
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void USART_Transmit(unsigned char data)
{
while (!(UCSR0A & (1<<UDRE0)))
;
UDR0 = data;
}

byte USART_available(){
  if(!(UCSR0A & (1<<RXC0))){
    return 1;
  }
  else{
    return 0;
  }
  }
unsigned char USART_Receive(void)
{
while (!(UCSR0A & (1<<RXC0)))
;
return UDR0;
}