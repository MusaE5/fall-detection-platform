#include "i2c_driver.h"
#include "uart_driver.h"


void delay(volatile uint32_t count) {
    while(count--);
}


int main(void)
{

	uart_init();
	i2c_init();

	uint8_t who_am_i = i2c_read_reg(0x68, 0x75);
	if(who_am_i != 0x68){
	    uart_send_string("I2C ERROR\r\n");
	    while(1);
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
