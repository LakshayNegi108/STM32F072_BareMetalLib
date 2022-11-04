#include <main.h>

#include "BML_DEF.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "adc.h"

BML_StatusTypeDef ret;

#define SCL_PORT	PORTB
#define SCL_PIN		8

#define SDA_PORT	PORTB
#define SDA_PIN		9

int main(void){
	RCC_CONFIG_48MHZ();
	uart_print_config(9600);
	print("Working\n");

	adc_en(1000);
	adc_config(sample_time_7, CONT_off, SCANDIR_off, AUTOFF_off, WAIT_off, DISCEN_on, OVRMOD_off);

	gpio_config(PORTA, 5, OUTPUT_MODE, LOW_SPEED, DI_PUPD, PHPL);
	gpio_config(ADC14_PORT, ADC14_PIN, ANALOG_MODE, HIGH_SPEED, DI_PUPD, PHPL);

	gpio_config(SCL_PORT, SCL_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
	gpio_config(SDA_PORT, SDA_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);

	gpio_altfn(SCL_PORT, SCL_PIN, I2C1_SCL_PB8);
	gpio_altfn(SDA_PORT, SDA_PIN, I2C1_SDA_PB9);

	I2C_INIT(I2C1, STANDARD_MODE);

	uint16_t adcVal = 0;

	ret = I2C_DeviceReady(I2C1, 0x57, 100);
	if(ret == BML_OK){
		print("Device Found\n");
	}
	else{
		print("Device not found\n");
	}

	while(1){
		adc_read(&adcVal, 1, 100);
		print("ADC Value: %d\n", adcVal);
		gpio_toggle(PORTA, 5);
		Delay(100);
//		HAL_Delay(1000);
//		for(int i = 1000000; i > 0; i--);
	}

	return 0;
}
