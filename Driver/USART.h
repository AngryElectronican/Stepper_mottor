#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <inttypes.h>

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
uint8_t USART_Available();
unsigned char USART_Receive(void);

#endif
