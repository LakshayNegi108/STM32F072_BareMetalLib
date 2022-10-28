/*
 * i2c.h
 *
 *  Created on: Oct 23, 2022
 *      Author: hp
 */

#ifndef BML_DRIVERS_INC_I2C_H_
#define BML_DRIVERS_INC_I2C_H_

#include "stm32f072xb.h"

#define STANDARD_MODE	(uint32_t)0x2000090E
#define FAST_MODE		(uint32_t)0x0000020B

typedef struct{
	I2C_TypeDef *i2c;
	uint32_t	TIMING;
}I2C_CONFIG;


void I2C_INIT(I2C_TypeDef *i2c, uint32_t timing);
void I2C_byteWrite(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint8_t data);
void I2C_byteRead(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint8_t *data);
void I2C_MultiWrite(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint32_t n,
		uint8_t *data);
void I2C_MultiRead(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint32_t n,
		uint8_t *data);

#endif /* BML_DRIVERS_INC_I2C_H_ */
