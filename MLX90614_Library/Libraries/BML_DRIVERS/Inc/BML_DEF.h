/*
 * BML_DEF.h
 *
 *  Created on: Oct 25, 2022
 *      Author: hp
 */

#ifndef BML_DRIVERS_INC_BML_DEF_H_
#define BML_DRIVERS_INC_BML_DEF_H_

#include "stm32f0xx_hal.h"
#include "BML_DRIVER.h"
#include "string.h"

typedef enum
{
  BML_OK       = 0x00U,
  BML_ERROR    = 0x01U,
  BML_BUSY     = 0x02U,
  BML_TIMEOUT  = 0x03U
} BML_StatusTypeDef;

#define		HIGH	1
#define		LOW		0

#define	I2C_Delay(x)								HAL_Delay(x);
#define I2C_Print(fmt, len)				USART_WRITE(USART4, fmt, len);

#endif /* BML_DRIVERS_INC_BML_DEF_H_ */
