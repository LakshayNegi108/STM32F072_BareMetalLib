################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/BML_DRIVERS/Src/BML_DRIVER.c \
../Libraries/BML_DRIVERS/Src/i2c.c 

OBJS += \
./Libraries/BML_DRIVERS/Src/BML_DRIVER.o \
./Libraries/BML_DRIVERS/Src/i2c.o 

C_DEPS += \
./Libraries/BML_DRIVERS/Src/BML_DRIVER.d \
./Libraries/BML_DRIVERS/Src/i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/BML_DRIVERS/Src/%.o Libraries/BML_DRIVERS/Src/%.su: ../Libraries/BML_DRIVERS/Src/%.c Libraries/BML_DRIVERS/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I"E:/STM codes/STM32F072 SPACE/I2C library/Libraries/BML_DRIVERS/Inc" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libraries-2f-BML_DRIVERS-2f-Src

clean-Libraries-2f-BML_DRIVERS-2f-Src:
	-$(RM) ./Libraries/BML_DRIVERS/Src/BML_DRIVER.d ./Libraries/BML_DRIVERS/Src/BML_DRIVER.o ./Libraries/BML_DRIVERS/Src/BML_DRIVER.su ./Libraries/BML_DRIVERS/Src/i2c.d ./Libraries/BML_DRIVERS/Src/i2c.o ./Libraries/BML_DRIVERS/Src/i2c.su

.PHONY: clean-Libraries-2f-BML_DRIVERS-2f-Src

