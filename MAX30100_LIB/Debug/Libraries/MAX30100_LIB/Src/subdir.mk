################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/MAX30100_LIB/Src/MAX30100.c 

OBJS += \
./Libraries/MAX30100_LIB/Src/MAX30100.o 

C_DEPS += \
./Libraries/MAX30100_LIB/Src/MAX30100.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/MAX30100_LIB/Src/%.o Libraries/MAX30100_LIB/Src/%.su: ../Libraries/MAX30100_LIB/Src/%.c Libraries/MAX30100_LIB/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I"E:/STM codes/STM32F072 SPACE/MAX30100_LIB/Libraries/BML_DRIVERS/Inc" -I"E:/STM codes/STM32F072 SPACE/MAX30100_LIB/Libraries/MAX30100_LIB/Inc" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libraries-2f-MAX30100_LIB-2f-Src

clean-Libraries-2f-MAX30100_LIB-2f-Src:
	-$(RM) ./Libraries/MAX30100_LIB/Src/MAX30100.d ./Libraries/MAX30100_LIB/Src/MAX30100.o ./Libraries/MAX30100_LIB/Src/MAX30100.su

.PHONY: clean-Libraries-2f-MAX30100_LIB-2f-Src

