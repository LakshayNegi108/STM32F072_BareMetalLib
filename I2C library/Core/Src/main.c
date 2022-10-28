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

void i2c_config() {
	i2c1.i2c = I2C1;
	i2c1.TIMING = STANDARD_MODE;
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

	USART_WRITE(USART4, "Working\n", 9);

	USART_WRITE(USART4, "Standard Mode\n", 14);

	for (int i = 0; i < 128; i++) {
		if (I2C_Scanner(i2c1.i2c, i)) {
			print("Device Address found at: 0x%X \r\n", i);
		}
		I2C_DI(i2c1.i2c);
		I2C_EN(i2c1.i2c);
	}

	I2C_DEINIT(i2c1.i2c);
	i2c1.TIMING = FAST_MODE;
	I2C_INIT(i2c1.i2c, i2c1.TIMING);

	USART_WRITE(USART4, "Fast Mode\n", 10);

	for (int i = 0; i < 128; i++) {
		if (I2C_Scanner(i2c1.i2c, i)) {
			print("Device Address found at: 0x%X \r\n", i);
		}
		I2C_DI(i2c1.i2c);
		I2C_EN(i2c1.i2c);
	}

	I2C_DEINIT(i2c1.i2c);
	i2c1.TIMING = 0x00000A17;		// 200Khz Fast Mode
//	i2c1.TIMING = 0x00000212;		// 300Khz Fast Mode
	I2C_INIT(i2c1.i2c, i2c1.TIMING);

	USART_WRITE(USART4, "Custom Mode\n", 12);

	for (int i = 0; i < 128; i++) {
		if (I2C_Scanner(i2c1.i2c, i)) {
			print("Device Address found at: 0x%X \r\n", i);
		}
		I2C_DI(i2c1.i2c);
		I2C_EN(i2c1.i2c);
	}

	while (1) {
//		I2C_MasterMultiRead(i2c1.i2c, SLAVE_ADDR, 0x00, 7, &data);
//		print("%d:%d:%d ,%d, %d, %d, %d\n\r", bcdToDec(data[2]), bcdToDec(data[1]), bcdToDec(data[0]), bcdToDec(data[3]), bcdToDec(data[4]), bcdToDec(data[5]), bcdToDec(data[6]));

		HAL_Delay(500);
	}
	return 0;
}
