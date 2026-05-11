#include <stdint.h>

#define RCC_AHB1ENR *((volatile uint32_t*) 0x40023830)
#define RCC_APB1ENR *((volatile uint32_t*) 0x40023840)

#define GPIOA_MODER  *((volatile uint32_t*) 0x40020000)
#define GPIOA_ODR    *((volatile uint32_t*) 0x40020014)
#define GPIOA_AFRL   *((volatile uint32_t*) 0x40020020)


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


#define USART2_SR    *((volatile uint32_t*) 0x40004400)
#define USART2_DR    *((volatile uint32_t*) 0x40004404)
#define USART2_BRR   *((volatile uint32_t*) 0x40004408)
#define USART2_CR1   *((volatile uint32_t*) 0x4000440C)

void delay(volatile uint32_t count) {
    while(count--);
}


void uart_send_char(char c) {
	    while(!(USART2_SR & (0x1 << 7)));  // wait until TXE = 1 (transmit buffer empty)
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


uint8_t i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr){    // keep as I2C confirmation method

	I2C1_CR1 |= (1<<8); // generate start bit (SDA LOW while SCL HIGH)
	I2C1_CR1 |= (1<<10);  // enable acknowledge
	while((I2C1_SR1 & (0x1)) == 0);  // wait until start condition is generated (CPU faster then I2C hardware)
	I2C1_DR = (dev_addr<<1) | (0); // write 7 bit address and add LSB of 0 (0 is write)
	while((I2C1_SR1 & (0X2)) == 0);  // wait for ADDR to confirm address was sent and acknowledge
	uint16_t temp = I2C1_SR1 | I2C1_SR2; // clear ADDR (read status registers)

	while((I2C1_SR1 & (1<<7)) == 0);  // while TxE is is 0 (Data register is empty, waiting for address to finish sending)
	I2C1_DR = reg_addr;
	while((I2C1_SR1 & 0x4) == 0);  // wait for byte the finish transmitting

	I2C1_CR1 |= (1<<8);  // generate repeated start
	while((I2C1_SR1 & (0x1)) == 0);
	I2C1_DR = (dev_addr<<1) | 1;  // read mode for address
	while((I2C1_SR1 & (0X2)) == 0);  // wait for ADDR to confirm address was sent and acknowledge
	temp = I2C1_SR1 | I2C1_SR2;  // clear ADDR

	I2C1_CR1 &= ~(1<<10);  //disable ACK after byte was sent (STM sends NACK after byte)
	I2C1_CR1 |= (1<<9);  // Generate STOP
	while((I2C1_SR1 & (1<<6)) == 0); // while RxNE (data register) is empty (gets byte at once from shift register)
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
	while((I2C1_SR1 & (1<<2)) == 0); // while BTF (bytes) are fully transferred
	I2C1_CR1 |= (1<<9);  // Generate STOP (stop generating start)

}


int main(void)
{

	RCC_AHB1ENR |= (3);
	RCC_APB1ENR |= (1 << 17);
	RCC_APB1ENR |= (1<<21);


	GPIOA_MODER &= ~(0x3 << 4);
	GPIOA_MODER |=  (0x2 << 4);
	GPIOA_AFRL  &= ~(0xF << 8);
	GPIOA_AFRL  |=  (0x7 << 8);


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


	USART2_BRR = 0x8B; //16Mhz / 115200
	USART2_CR1  |=  (0x1 << 13);  // UE: USART enable
	USART2_CR1  |=  (0x1 << 3);   // TE: transmitter enable





	uint8_t who_am_i = i2c_read_reg(0x68, 0x75);
	if(who_am_i != 0x68){
	    uart_send_string("I2C ERROR\r\n");
	    while(1);  // halt
	}
	else{
		uart_send_string("I2C working with address 0x68\r\n");
	}


	i2c_write_reg(0x68, 0x6B, 0x00);

	while(1){

	uint8_t accel_buf[6];
	uint8_t gyro_buf[6];

	i2c_read_burst(0x68, 0x3B, accel_buf, 6);
	i2c_read_burst(0x68, 0x43, gyro_buf, 6);

	int16_t ax = (accel_buf[0] << 8) | accel_buf[1];
	int16_t ay = (accel_buf[2] << 8) | accel_buf[3];
	int16_t az = (accel_buf[4] << 8) | accel_buf[5];

	int16_t gx = (gyro_buf[0] << 8) | gyro_buf[1];
	int16_t gy = (gyro_buf[2] << 8) | gyro_buf[3];
	int16_t gz = (gyro_buf[4] << 8) | gyro_buf[5];

	uart_send_int(ax); uart_send_char(',');
	uart_send_int(ay); uart_send_char(',');
	uart_send_int(az); uart_send_char(',');
	uart_send_int(gx); uart_send_char(',');
	uart_send_int(gy); uart_send_char(',');
	uart_send_int(gz);
	uart_send_string("\r\n");

	delay(3200000);

	}

}
