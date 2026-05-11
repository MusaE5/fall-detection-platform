/*
 * i2c_driver.c
 *
 *  Created on: May 11, 2026
 *      Author: elash
 */

#include "i2c_driver.h"

void i2c_init(void){
	RCC_AHB1ENR |= (1<<1);
	RCC_APB1ENR |= (1<<21);

	GPIOB_MODER &= ~(0xF0000);
	GPIOB_MODER |=  (0xA0000);

	GPIOB_OTYPER |= (0x3<<8);

	GPIOB_OSPEEDR |= (0xF0000);

	GPIOB_AFRH &= ~(0xFF);
	GPIOB_AFRH |= (0x44);

	I2C1_CR1 |= (1<<15);
	I2C1_CR1 &= ~(1<<15);

	I2C1_CR2 &= ~(0x3F);
	I2C1_CR2 |=  (0x10);

	I2C1_CCR &= ~(0xFFF);
	I2C1_CCR |= (0x50);

	I2C1_TRISE &= ~(0x3F);
	I2C1_TRISE |=  (0x11);

	I2C1_CR1 |= (0x1);
}


uint8_t i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr){

	I2C1_CR1 |= (1<<8);
	I2C1_CR1 |= (1<<10);
	while((I2C1_SR1 & (0x1)) == 0);
	I2C1_DR = (dev_addr<<1) | (0);
	while((I2C1_SR1 & (0X2)) == 0);
	uint16_t temp = I2C1_SR1 | I2C1_SR2;

	while((I2C1_SR1 & (1<<7)) == 0);
	I2C1_DR = reg_addr;
	while((I2C1_SR1 & 0x4) == 0);

	I2C1_CR1 |= (1<<8);
	while((I2C1_SR1 & (0x1)) == 0);
	I2C1_DR = (dev_addr<<1) | 1;
	while((I2C1_SR1 & (0X2)) == 0);
	temp = I2C1_SR1 | I2C1_SR2;

	I2C1_CR1 &= ~(1<<10);
	I2C1_CR1 |= (1<<9);
	while((I2C1_SR1 & (1<<6)) == 0);
	return (uint8_t) I2C1_DR;

}


void i2c_read_burst(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t len){
	I2C1_CR1 |= (1<<8);
	I2C1_CR1 |= (1<<10);
	while((I2C1_SR1 & (0x1)) == 0);
	I2C1_DR = (dev_addr<<1)|0;
	while((I2C1_SR1 & (0X2)) == 0);
	uint16_t temp = I2C1_SR1 | I2C1_SR2;

	while((I2C1_SR1 & (1<<7)) == 0);
	I2C1_DR = reg_addr;
	while((I2C1_SR1 & 0x4) == 0);

	I2C1_CR1 |= (1<<8);
	while((I2C1_SR1 & (0x1)) == 0);
	I2C1_DR = (dev_addr<<1) | 1;
	while((I2C1_SR1 & (0X2)) == 0);
	temp = I2C1_SR1 | I2C1_SR2;

	for(uint8_t i = 0; i<len; i++){
		if(i == len-1){
			I2C1_CR1 &= ~(1<<10);
			I2C1_CR1 |= (1<<9);
		}

		while((I2C1_SR1 & (1<<6)) == 0);
		*(buf + i) = I2C1_DR;
	}

}



void i2c_write_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data){
	I2C1_CR1 |= (1<<8);
	while((I2C1_SR1 & 0x1) == 0);
	I2C1_DR = (dev_addr<<1) | 0;
	while((I2C1_SR1 & (0x2)) == 0);
	uint16_t temp = I2C1_SR1 | I2C1_SR2;

	while((I2C1_SR1 & (1<<7)) == 0);
	I2C1_DR = reg_addr;
	while((I2C1_SR1 & 0x4) == 0);

	I2C1_DR = data;
	while((I2C1_SR1 & (1<<2)) == 0);
	I2C1_CR1 |= (1<<9);

}
