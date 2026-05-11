/*
 * uart_driver.h
 *
 *  Created on: May 11, 2026
 *      Author: elash
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>
#include "stm32f446_rcc.h"

#define GPIOA_MODER  *((volatile uint32_t*) 0x40020000)
#define GPIOA_ODR    *((volatile uint32_t*) 0x40020014)
#define GPIOA_AFRL   *((volatile uint32_t*) 0x40020020)

#define USART2_SR    *((volatile uint32_t*) 0x40004400)
#define USART2_DR    *((volatile uint32_t*) 0x40004404)
#define USART2_BRR   *((volatile uint32_t*) 0x40004408)
#define USART2_CR1   *((volatile uint32_t*) 0x4000440C)

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(char* str);
void uart_send_int(int16_t val);


#endif /* UART_DRIVER_H_ */
