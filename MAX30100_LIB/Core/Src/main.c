#include "main.h"

#include "BML_DEF.h"
#include "rcc.h"
#include "uart.h"
#include "i2c.h"
#include "BML_DRIVER.h"

BML_StatusTypeDef ret;

#define SCL_PORT	PORTB
#define SCL_PIN		8

#define SDA_PORT	PORTB
#define SDA_PIN		9

//#define MAX30100_I2C_ADDRESS	0x57

I2C_TYPE i2c1;

void i2cConfig(){
	i2c1.i2c = I2C1;
	i2c1.TIMING = STANDARD_MODE;
}

//uint8_t MAX30100_readRegister(uint8_t address)
//{
//	uint8_t rdata = 0;
//    ret = I2C_MasterByteRead(I2C1, MAX30100_I2C_ADDRESS, address, &rdata, 100);
//    if(ret == BML_OK){
//    	 return rdata;
//    }
//    else{
//    	print("Error Reading\n");
//    	return -1;
//    }
//}
//
//void MAX30100_writeRegister(uint8_t address, uint8_t data)
//{
//    I2C_MasterByteWrite(I2C1, MAX30100_I2C_ADDRESS, address, data, 100);
//}
//
//void MAX30100_burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length)
//{
////    Wire.beginTransmission(MAX30100_I2C_ADDRESS);
////    Wire.write(baseAddress);
////    Wire.endTransmission(false);
////    Wire.requestFrom((uint8_t)MAX30100_I2C_ADDRESS, length);
////
////    uint8_t idx = 0;
////    while (Wire.available()) {
////        buffer[idx++] = Wire.read();
////    }
//	I2C_MasterMultiRead(I2C1, MAX30100_I2C_ADDRESS, baseAddress, length, buffer, 100);
//}


int main(void){

	RCC_CONFIG_48MHZ();
	uart_print_config(9600);
	USART_WRITE(USART4, "Working\n\r", 9, 100);

	gpio_config(SCL_PORT, SCL_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
	gpio_config(SDA_PORT, SDA_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);

	gpio_altfn(SCL_PORT, SCL_PIN, I2C1_SCL_PB8);
	gpio_altfn(SDA_PORT, SDA_PIN, I2C1_SDA_PB9);

//	I2C_INIT(I2C1, i2c1.TIMING);
//
//	ret = I2C_DeviceReady(I2C1, MAX30100_I2C_ADDRESS, 100);
//	if(ret == BML_OK){
//		print("Success\n");
//	}
//	else{
//		print("Device not found\n");
//	}

	int8_t ID = MAX30100_readRegister(0xFF);
	print("ID: %x\n", ID);
	if(ID == 0x11){
		print("ID OK\n");
	}
	else{
		print("Wrong ID\n");
	}

	for(;;){

	}

	return 0;
}
