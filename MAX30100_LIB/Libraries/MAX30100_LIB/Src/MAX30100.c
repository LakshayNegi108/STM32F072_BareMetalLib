/*
 Arduino-MAX30100 oximetry / heart rate integrated sensor library
 Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <i2c.h>

#include "MAX30100.h"
#include "BML_DEF.h"
BML_StatusTypeDef ret;

bool MAX30100_begin(I2C_TypeDef *i2c, uint32_t timing) {
//    Wire.begin();
//    Wire.setClock(I2C_BUS_SPEED);

	I2C_INIT(i2c, timing);

//	ret = I2C_DeviceReady(i2c, MAX30100_I2C_ADDRESS, 100);
//	if (ret == BML_OK) {
//		print("Success\n");
//	} else {
//		print("Device not found\n");
//		return false;
//	}

	if (MAX30100_getPartId() != EXPECTED_PART_ID) {
		return false;
	}

	MAX30100_setMode(DEFAULT_MODE);
	MAX30100_setLedsPulseWidth(DEFAULT_PULSE_WIDTH);
	MAX30100_setSamplingRate(DEFAULT_SAMPLING_RATE);
	MAX30100_setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
	MAX30100_setHighresModeEnabled(true);

	return true;
}

void MAX30100_setMode(Mode mode) {
	writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100_setLedsPulseWidth(LEDPulseWidth ledPulseWidth) {
	uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
	writeRegister(MAX30100_REG_SPO2_CONFIGURATION,
			(previous & 0xfc) | ledPulseWidth);
}

void MAX30100_setSamplingRate(SamplingRate samplingRate) {
	uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
	writeRegister(MAX30100_REG_SPO2_CONFIGURATION,
			(previous & 0xe3) | (samplingRate << 2));
}

void MAX30100_setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent) {
	writeRegister(MAX30100_REG_LED_CONFIGURATION,
			redLedCurrent << 4 | irLedCurrent);
}

void MAX30100_setHighresModeEnabled(bool enabled) {
	uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
	if (enabled) {
		writeRegister(MAX30100_REG_SPO2_CONFIGURATION,
				previous | MAX30100_SPC_SPO2_HI_RES_EN);
	} else {
		writeRegister(MAX30100_REG_SPO2_CONFIGURATION,
				previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
	}
}

void MAX30100_update() {
	readFifoData();
}

bool MAX30100_getRawValues(uint16_t *ir, uint16_t *red) {
	if (!readoutsBuffer.isEmpty()) {
		SensorReadout readout = readoutsBuffer.pop();

		*ir = readout.ir;
		*red = readout.red;

		return true;
	} else {
		return false;
	}
}

BML_StatusTypeDef MAX30100_resetFifo() {
	ret = MAX30100_writeRegister(MAX30100_REG_FIFO_WRITE_POINTER, 0);
	if (ret != BML_OK) {
		return BML_ERROR;
	}
	ret = MAX30100_writeRegister(MAX30100_REG_FIFO_READ_POINTER, 0);
	if (ret != BML_OK) {
		return BML_ERROR;
	}
	ret = MAX30100_writeRegister(MAX30100_REG_FIFO_OVERFLOW_COUNTER, 0);
	if (ret != BML_OK) {
		return BML_ERROR;
	}
}

uint8_t MAX30100_readRegister(uint8_t address) {
	uint8_t rdata = 0;
	ret = I2C_MasterByteRead(I2C1, MAX30100_I2C_ADDRESS, address, &rdata, 100);
	if (ret == BML_OK) {
		return rdata;
	} else {
		return BML_ERROR;
	}
}

BML_StatusTypeDef MAX30100_writeRegister(uint8_t address, uint8_t data) {
	ret = I2C_MasterByteWrite(I2C1, MAX30100_I2C_ADDRESS, address, data, 100);
	if (ret == BML_OK) {
		return rdata;
	} else {
		return BML_ERROR;
	}
}

BML_StatusTypeDef MAX30100_burstRead(uint8_t baseAddress, uint8_t *buffer,
		uint8_t length) {
	ret = I2C_MasterMultiRead(I2C1, MAX30100_I2C_ADDRESS, baseAddress, length,
			buffer, 100);
	if (ret == BML_OK) {
		return rdata;
	} else {
		return BML_ERROR;
	}
}

void MAX30100_readFifoData() {
	uint8_t buffer[MAX30100_FIFO_DEPTH * 4];
	uint8_t toRead;

	toRead = (readRegister(MAX30100_REG_FIFO_WRITE_POINTER)
			- readRegister(MAX30100_REG_FIFO_READ_POINTER))
			& (MAX30100_FIFO_DEPTH - 1);

	if (toRead) {
		burstRead(MAX30100_REG_FIFO_DATA, buffer, 4 * toRead);

		for (uint8_t i = 0; i < toRead; ++i) {
			// Warning: the values are always left-aligned
		readoutsBuffer.push( {
					.ir=(uint16_t)((buffer[i*4] << 8) | buffer[i*4 + 1]),
					.red=(uint16_t)((buffer[i*4 + 2] << 8) | buffer[i*4 + 3])});
	}
}
}

void MAX30100_startTemperatureSampling() {
uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
modeConfig |= MAX30100_MC_TEMP_EN;

writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

bool MAX30100_isTemperatureReady() {
return !(readRegister(MAX30100_REG_MODE_CONFIGURATION) & MAX30100_MC_TEMP_EN);
}

float MAX30100_retrieveTemperature() {
int8_t tempInteger = readRegister(MAX30100_REG_TEMPERATURE_DATA_INT);
float tempFrac = readRegister(MAX30100_REG_TEMPERATURE_DATA_FRAC);

return tempFrac * 0.0625 + tempInteger;
}

void MAX30100_shutdown() {
uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
modeConfig |= MAX30100_MC_SHDN;

writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

void MAX30100_resume() {
uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
modeConfig &= ~MAX30100_MC_SHDN;

writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

uint8_t MAX30100_getPartId() {
return readRegister(0xff);
}
