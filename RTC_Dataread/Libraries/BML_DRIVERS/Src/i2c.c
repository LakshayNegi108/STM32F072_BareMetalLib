/*
 * i2c.c
 *
 *  Created on: Oct 23, 2022
 *      Author: hp
 */
#include "i2c.h"

void I2C_INIT(I2C_TypeDef *i2c, uint32_t timing) {
	if (i2c == I2C1) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	} else if (i2c == I2C2) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	}
	i2c->CR1 = 0; /* software reset I2C1 */
	i2c->TIMINGR = timing; /* 100 KHz, peripheral clock is 8/48 MHz */
//	i2c->CR1 = 0x00000001; /* enable I2C1 module */
	i2c->CR1 |= I2C_CR1_PE;
}

void I2C_byteWrite(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint8_t data) {
	while (i2c->ISR & I2C_ISR_BUSY)
		; /* wait until bus not busy */

//	i2c->CR2 = 0x02002000 | (2 << 16) /* generate start, autoend, byte count 2 */
//	| (saddr << 1); /* and send slave address */
	i2c->CR2 |= I2C_CR2_START | I2C_CR2_AUTOEND;
	i2c->CR2 |= 2 << 16;
	i2c->CR2 |= (saddr << 1);


	while (!(i2c->ISR & I2C_ISR_TXIS))
		; /* wait until TXIS is set */
	i2c->TXDR = maddr; /* send register address */

	while (!(i2c->ISR & I2C_ISR_TXIS))
		; /* wait until TXIS is set */
	i2c->TXDR = data; /* send data */

	while (!(i2c->ISR & I2C_ISR_STOPF))
		; /* wait until stop flag is set */

	i2c->ICR = I2C_ICR_STOPCF; /* clear stop flag */
//	i2c->CR2 = 0x02000000; /* clear configuration register 2 */
	i2c->CR2 = 0;
	i2c->CR2 |= I2C_CR2_AUTOEND;
}

void I2C_byteRead(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint8_t *data) {
	while (i2c->ISR & I2C_ISR_BUSY)
		; /* wait until bus not busy */

	i2c->CR2 |= I2C_CR2_START; /* generate start and send slave address */
	i2c->CR2 |= 1 << 16;
	i2c->CR2 |= (saddr << 1);
	while (!(i2c->ISR & I2C_ISR_TXIS))
		; /* wait until start flag is set */

	i2c->TXDR = maddr; /* send memory address */
	while (!(i2c->ISR & I2C_ISR_TC))
		; /* wait until transfer complete */

	/* generate restart, send slave address, read 1 byte, set for auto end */
//	i2c->CR2 = 0x02002400;
	i2c->CR2 = I2C_CR2_START | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND;
	i2c->CR2 |= (1 << 16) | (saddr << 1);

	while (!(i2c->ISR & I2C_ISR_RXNE))
		; /* Wait until RXNE flag is set */
	*data = i2c->RXDR; /* Read data from DR */

	while (!(i2c->ISR & I2C_ISR_STOPF))
		; /* wait until stop flag is set */

	i2c->ICR = I2C_ISR_STOPF; /* clear stop flag */
	i2c->CR2 = 0; /* clear configuration register 2 */
	i2c->CR2 = I2C_CR2_AUTOEND;
}

void I2C_MultiWrite(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint32_t n,
		uint8_t *data) {
	int i;

	while (i2c->ISR & I2C_ISR_BUSY)
		; /* wait until bus not busy */

//	i2c->CR2 = 0;
//	i2c->CR2 |= ((n + 1) << 16) | (saddr << 1);
//	i2c->CR2 |= I2C_CR2_START | I2C_CR2_AUTOEND;
	i2c->CR2 = I2C_CR2_START | I2C_CR2_AUTOEND | ((n + 1) << 16) | (saddr << 1);/* set byte count and send slave address */
																				/* generate start, set auto end, */

	while (!(i2c->ISR & I2C_ISR_TXIS))
		; /* wait until TXIS is set */
	i2c->TXDR = maddr; /* send memory address */

	for (i = 0; i < n; i++) /* send n bytes of data */
	{
		while (!(i2c->ISR & I2C_ISR_TXIS))
			; /* wait until TXIS is set */
		i2c->TXDR = *data++; /* send a byte of data */
	}

	while (!(i2c->ISR & I2C_ISR_STOPF))
		; /* wait until stop flag is set */

	i2c->ICR = I2C_ISR_STOPF; /* clear stop flag */
//	i2c->CR2 = 0; /* clear configuration register 2 */
	i2c->CR2 |= I2C_CR2_AUTOEND;
}

void I2C_MultiRead(I2C_TypeDef *i2c, uint8_t saddr, uint8_t maddr, uint32_t n,
		uint8_t *data) {
	while (i2c->ISR & I2C_ISR_BUSY)
		; /* wait until bus not busy */

	i2c->CR2 = 	I2C_CR2_START | (1 << 16) | (saddr << 1);	/* generate start and send slave address */

	while (!(i2c->ISR & I2C_ISR_TXIS))
		; /* wait until transmitter is ready */
	i2c->TXDR = maddr; /* send memory address */
	while (!(i2c->ISR & I2C_ISR_TC))
		; /* wait until transfer complete */

	/* generate restart, send slave address, repeat read n bytes, set for auto end */
	i2c->CR2 = I2C_CR2_START | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND | ((n & 0xFF) << 16) | (saddr << 1);

	for (; n > 0; n--) {
		while (!(i2c->ISR & I2C_ISR_RXNE))
			; /* Wait until RXNE flag is set */
		*data++ = i2c->RXDR; /* Read data from DR */
	}

	while (!(i2c->ISR & I2C_ISR_STOPF))
		; /* wait until stop flag is set */

	i2c->ICR = I2C_ISR_STOPF; /* clear stop flag */
//	i2c->CR2 = 0; /* clear configuration register 2 */
	i2c->CR2 = I2C_CR2_AUTOEND;
}
