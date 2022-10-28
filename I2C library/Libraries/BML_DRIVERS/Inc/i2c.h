/*
 * i2c.h
 *
 *  Created on: Oct 23, 2022
 *      Author: hp
 */

#ifndef BML_DRIVERS_INC_I2C_H_
#define BML_DRIVERS_INC_I2C_H_

#include "stm32f072xb.h"
#include "stdbool.h"

#define STANDARD_MODE	(uint32_t)0x2000090E
#define FAST_MODE		(uint32_t)0x0000020B

typedef struct{
	I2C_TypeDef *i2c;
	uint32_t	TIMING;
}I2C_CONFIG;

void I2C_EN(I2C_TypeDef *i2c);
void I2C_DI(I2C_TypeDef *i2c);
void I2C_INIT(I2C_TypeDef *i2c, uint32_t timing);
bool I2C_Scanner(I2C_TypeDef *i2c, uint8_t saddr);
void I2C_MasterByteWrite(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint8_t data);
void I2C_MasterByteRead(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint8_t *data);
void I2C_MasterMultiWrite(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint32_t n,
		uint8_t *data);
void I2C_MasterMultiRead(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint32_t n,
		uint8_t *data);

#endif /* BML_DRIVERS_INC_I2C_H_ */
