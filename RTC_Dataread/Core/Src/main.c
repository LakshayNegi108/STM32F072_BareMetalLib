#include "main.h"
#include "BML_DRIVER.h"
#include "i2c.h"

#define SLAVE_ADDR 0x68    /* 1101 000.    DS3231 */
#define SCL_PORT	PORTB
#define SCL_PIN		8

#define SDA_PORT	PORTB
#define SDA_PIN		9

int bcdToDec(uint8_t val) {
	return (int) (((val / 16) * 10) + (val % 16));
}

I2C_CONFIG i2c1;

void i2c_config(){
	i2c1.i2c = I2C1;
	i2c1.TIMING = FAST_MODE;
}

int main() {
	RCC_CONFIG_48MHZ();
	USART_INIT(USART4, PORTC, PORTC, 10, 11, 9600);

	gpio_config(SCL_PORT, SCL_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
	gpio_config(SDA_PORT, SDA_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);

	gpio_altfn(SCL_PORT, SCL_PIN, I2C1_SCL_PB8);
	gpio_altfn(SDA_PORT, SDA_PIN, I2C1_SDA_PB9);

	i2c_config();

	I2C_INIT(i2c1.i2c, i2c1.TIMING);

	/*                            Sec   Min    HR   DAY   DATE  Month  YR   */
//	uint8_t timeDateToSet[15] = { 0x00, 0x44, 0x04, 0x00, 0x23, 0x10, 0x22, 0 };
	uint8_t timeDateToSet[15] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0 };
	/* 2017 September 26, Tuesday, 10:58:55 */
//	       I2C_MultiWrite(I2C1, SLAVE_ADDR, 0, 7, timeDateToSet);
	uint8_t data[7];

	USART_WRITE(USART4, "Working\n", 9);


	while (1) {
//		gpio_toggle(PORTA, 5);
		I2C_MultiRead(i2c1.i2c, SLAVE_ADDR, 0x00, 7, &data);
		print("%d:%d:%d ,%d, %d, %d, %d\n\r", bcdToDec(data[2]), bcdToDec(data[1]), bcdToDec(data[0]), bcdToDec(data[3]), bcdToDec(data[4]), bcdToDec(data[5]), bcdToDec(data[6]));

		HAL_Delay(500);
	}
	return 0;
}
