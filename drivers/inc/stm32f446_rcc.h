/*
 * stm32f446_rcc.h
 *
 *  Created on: May 11, 2026
 *      Author: elash
 */

#ifndef STM32F446_RCC_H_
#define STM32F446_RCC_H_

#include <stdint.h>

#define RCC_AHB1ENR *((volatile uint32_t*) 0x40023830)
#define RCC_APB1ENR *((volatile uint32_t*) 0x40023840)

#endif /* STM32F446_RCC_H_ */
