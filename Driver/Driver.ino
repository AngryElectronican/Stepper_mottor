#include "Modbus.h"
#include "Timer1.h"

int main(void){
  ModRTU_Init();
  Timer1_Init(DEVIDER);
  while(1){
    ModRTU_Handler();
    if(ModRTU_get_memory_bits(0,0) && ModRTU_get_memory_bits(0,1)){
      DDRB|=1<<PB5;
      PORTB&=~(1<<PB5);
      Timer1_LinearMove(0);
    }else if(ModRTU_get_memory_bits(0,0)){
      DDRB|=1<<PB5;
      PORTB|=1<<PB5;
      Timer1_LinearMove(1);
    }else if (ModRTU_get_memory_bits(0,1)){
      DDRB|=1<<PB5;
      PORTB|=1<<PB5;
      Timer1_LinearMove(2);
    }else{
      DDRB|=1<<PB5;
      PORTB&=~(1<<PB5);
      Timer1_LinearMove(0);
    }
  }
}
