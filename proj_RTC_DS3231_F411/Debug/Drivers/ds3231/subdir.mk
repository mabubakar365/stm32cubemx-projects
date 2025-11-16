################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ds3231/ds3231.c 

OBJS += \
./Drivers/ds3231/ds3231.o 

C_DEPS += \
./Drivers/ds3231/ds3231.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ds3231/%.o Drivers/ds3231/%.su Drivers/ds3231/%.cyclo: ../Drivers/ds3231/%.c Drivers/ds3231/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/ds3231 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ds3231

clean-Drivers-2f-ds3231:
	-$(RM) ./Drivers/ds3231/ds3231.cyclo ./Drivers/ds3231/ds3231.d ./Drivers/ds3231/ds3231.o ./Drivers/ds3231/ds3231.su

.PHONY: clean-Drivers-2f-ds3231

