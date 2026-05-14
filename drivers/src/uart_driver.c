/*
 * uart_driver.c
 *
 *  Created on: May 11, 2026
 *      Author: elash
 */
#include "uart_driver.h"

void uart_init(void){
    RCC_AHB1ENR |= (1<<0);
    RCC_APB1ENR |= (1<<17);

    GPIOA_MODER &= ~(0x3 << 4);
    GPIOA_MODER |=  (0x2 << 4);
    GPIOA_AFRL  &= ~(0xF << 8);
    GPIOA_AFRL  |=  (0x7 << 8);

    USART2_BRR = 0x16D;  //42MHz clock for FreeRTOS
    USART2_CR1 |= (1<<13);
    USART2_CR1 |= (1<<3);
}

void uart_send_char(char c) {
	    while(!(USART2_SR & (0x1 << 7)));
	    USART2_DR = c;
	}


void uart_send_string(char* str) {
	   while(*str) {
	       uart_send_char(*str);
	       str++;
	   }
	}

void uart_send_int(int16_t val){
	if(val == 0){
	    uart_send_char('0');
	    return;
	}

	uint16_t num = abs(val);
	char buff[6];
	uint8_t counter = 5;

	while(num>0){
		char digit = '0' + (num % 10);
		buff[counter] = digit;
		num /= 10;
		counter--;
	}


	if(val<0){
		uart_send_char('-');
	}

	counter++;
	while(counter<=5){
		uart_send_char(buff[counter]);
		counter++;
	}

}
