/*
 * i2c_driver.h
 *
 *  Created on: May 11, 2026
 *      Author: elash
 */

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include <stdint.h>
#include "stm32f446_rcc.h"


#define GPIOB_MODER *((volatile uint32_t*) 0x40020400)
#define GPIOB_OTYPER *((volatile uint32_t*) 0x40020404)
#define GPIOB_OSPEEDR *((volatile uint32_t*) 0x40020408)
#define GPIOB_AFRH  *((volatile uint32_t*) 0x40020424)


#define I2C1_CR1 *((volatile uint32_t*) 0x40005400)
#define I2C1_CR2 *((volatile uint32_t*) 0x40005404)
#define I2C1_CCR *((volatile uint32_t*) 0x4000541C)
#define I2C1_TRISE *((volatile uint32_t*) 0x40005420)
#define I2C1_DR *((volatile uint32_t*) 0x40005410)
#define I2C1_SR1 *((volatile uint32_t*) 0x40005414)
#define I2C1_SR2 *((volatile uint32_t*) 0x40005418)


void i2c_init(void);
uint8_t i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr);
void i2c_read_burst(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t len);
void i2c_write_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);



#endif /* I2C_DRIVER_H_ */
