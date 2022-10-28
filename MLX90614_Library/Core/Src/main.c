#include "main.h"
#include "BML_DRIVER.h"
#include "MLX90614.h"

BML_StatusTypeDef status;

#define SCL_PORT	PORTB
#define SCL_PIN		8

#define SDA_PORT	PORTB
#define SDA_PIN		9

void testCase1() {

	float Emiss = 0, minTemp = 0, maxTemp = 0;
	status = mlx90614_getEmissivity(&Emiss);
	if (status == BML_OK) {
		print("Emissivity = %f\n\r", Emiss);
	}
	status = mlx90614_getMax(&maxTemp);
	if (status == BML_OK) {
		print("Max Temp = %f\n\r", maxTemp);
	}
	status = mlx90614_getMin(&minTemp);
	if (status == BML_OK) {
		print("Min Temp = %f\n\r", minTemp);
	}
}

void testCase2() {

	float temp = 0, ambient = 0;

	mlx90614_setUnit(MLX90614_UNIT_C);
	status = mlx90614_getObject1(&temp);
	if (status == BML_OK) {
		print("Temp in Celsius: = 	%f		", temp);
	}
	status = mlx90614_getAmbient(&ambient);
	if (status == BML_OK) {
		print("Ambient in Celsius: = 	%f\n\r", ambient);
	}

	mlx90614_setUnit(MLX90614_UNIT_F);
	status = mlx90614_getObject1(&temp);
	if (status == BML_OK) {
		print("Temp in fahrenheit : =	%f		", temp);
	}
	status = mlx90614_getAmbient(&ambient);
	if (status == BML_OK) {
		print("Ambient in fahrenheit : = %f\n\r", ambient);
	}

	mlx90614_setUnit(MLX90614_UNIT_K);
	status = mlx90614_getObject1(&temp);
	if (status == BML_OK) {
		print("Temp in Kelvin: = 	%f		", temp);
	}
	status = mlx90614_getAmbient(&ambient);
	if (status == BML_OK) {
		print("Ambient in Kelvin: = 	%f\n\r", ambient);
	}
}

int main(void) {

	RCC_CONFIG_48MHZ();
	USART_INIT(USART4, PORTC, PORTC, 10, 11, 9600);
	USART_WRITE(USART4, "Working\n", 9);

	gpio_config(SCL_PORT, SCL_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
	gpio_config(SDA_PORT, SDA_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);

	gpio_altfn(SCL_PORT, SCL_PIN, I2C1_SCL_PB8);
	gpio_altfn(SDA_PORT, SDA_PIN, I2C1_SDA_PB9);

	status = mlx90614_init();
	if (status != BML_OK) {
		print("Error in Init\n\r");
		while (1)
			;
	}

	testCase1();

	while (1) {
		testCase2();
		HAL_Delay(500);
	}

	return 0;
}

//#include "main.h"
//#include "BML_DRIVER.h"
//#include "i2c.h"
//
//#define SLAVE_ADDR 0x5A    /* 1101 000.    DS3231 */
//#define SCL_PORT	PORTB
//#define SCL_PIN		8
//
//#define SDA_PORT	PORTB
//#define SDA_PIN		9
//
//int bcdToDec(uint8_t val) {
//	return (int) (((val / 16) * 10) + (val % 16));
//}
//
//I2C_CONFIG i2c1;
//BML_StatusTypeDef status;
//
//void i2c_config() {
//	i2c1.i2c = I2C1;
//	i2c1.TIMING = STANDARD_MODE;
//}
//
//void I2C_devCheck() {
//	status = I2C_DeviceReady(i2c1.i2c, SLAVE_ADDR, 100);
//
//	if (status == BML_BUSY) {
//		USART_WRITE(USART4, "BUSY\n", 5);
//	} else if (status == BML_OK) {
//		USART_WRITE(USART4, "Ready\n", 6);
//	} else if (status == BML_ERROR) {
//		USART_WRITE(USART4, "Not Ready\n", 10);
//	}
//}
//
//int main() {
//	RCC_CONFIG_48MHZ();
//	USART_INIT(USART4, PORTC, PORTC, 10, 11, 9600);
//	USART_WRITE(USART4, "Working\n", 9);
//
//	gpio_config(SCL_PORT, SCL_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
//	gpio_config(SDA_PORT, SDA_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
//
//	gpio_altfn(SCL_PORT, SCL_PIN, I2C1_SCL_PB8);
//	gpio_altfn(SDA_PORT, SDA_PIN, I2C1_SDA_PB9);
//
//	i2c_config();
//
//	I2C_INIT(i2c1.i2c, i2c1.TIMING);
//	I2C_devCheck();
//
//	uint8_t tempData[3] = { 0 };
//	uint16_t temp = 0;
//	float tempC = 0;
//
//	while (1) {
//		status = I2C_MasterMultiRead(i2c1.i2c, SLAVE_ADDR, 0x07, 3,
//				&tempData[0], 100);
//		if (status == BML_BUSY) {
//			USART_WRITE(USART4, "Busy\n", 5);
//		} else if (status == BML_TIMEOUT) {
//			USART_WRITE(USART4, "Timeout\n", 8);
//		} else if (status == BML_OK) {
//			temp = (tempData[0] | (tempData[1] << 8));
//			tempC = temp;
//			if (tempC == 0) {
//				print("NAN\n");
//			}
//			tempC *= .02;
//			tempC -= 273.15;
//			print("Temp: %f\n\r", tempC);
//		}
//
//		HAL_Delay(100);
//	}
//	return 0;
//}
