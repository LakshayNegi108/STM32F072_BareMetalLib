#include <gpio.h>
#include "main.h"
#include "BML_DEF.h"
#include "adc.h"
#include "uart.h"
//#include "i2c.h"
#include "rcc.h"

BML_StatusTypeDef ret;

void pinConfig() {
	gpio_config(ADC14_PORT, ADC14_PIN, ANALOG_MODE, LOW_SPEED, DI_PUPD, PHPL);
	gpio_config(ADC15_PORT, ADC15_PIN, ANALOG_MODE, LOW_SPEED, DI_PUPD, PHPL);
//		gpio_config(ADC4_PORT, ADC4_PIN, ANALOG_MODE, LOW_SPEED, DI_PUPD, PHPL);

	gpio_config(PORTC, 8, INPUT_MODE, LOW_SPEED, DI_PUPD, PHPL);
	gpio_config(PORTC, 9, INPUT_MODE, LOW_SPEED, DI_PUPD, PHPL);
}

int main() {
	RCC_CONFIG_48MHZ();
	uart_print_config(9600);
	USART_WRITE(USART4, "Working \n\r", 10, 100);
	ret = adc_en(100);
	if (ret != BML_OK) {
		USART_WRITE(USART4, "Error: 1\n\r", 10, 100);
	}
	pinConfig();
	adc_config(sample_time_7, CONT_off, SCANDIR_off, AUTOFF_off, WAIT_off,
	DISCEN_off, OVRMOD_off);
	uint16_t ADC_result[2];
	uint8_t rem1 = 0, rem2 = 0;
	while (1) {
		if ((gpio_read(PORTC, 8) == HIGH) && (gpio_read(PORTC, 9) == HIGH)) {
			ret = adc_read(&ADC_result[0], 2, 10);
			if (ret != BML_OK) {
				USART_WRITE(USART4, "Error: 2\n\r", 10, 100);
			}
			print("ADC value: %d\n\r", ADC_result[0]);
//			print(", %d\n\r", ADC_result[1]);
		}
		else{
			if(gpio_read(PORTC, 8) == LOW)
				USART_WRITE(USART4, "Error in L0-\n\r", 14, 100);

			if(gpio_read(PORTC, 9) == LOW)
				USART_WRITE(USART4, "Error in L0+\n\r", 14, 100);
		}
		rem1 = gpio_read(PORTC, 8);
		rem2 = gpio_read(PORTC, 9);
		print("%d, %d\n\r", rem1, rem2);

//		HAL_Delay(200);
	}
	return 0;
}
