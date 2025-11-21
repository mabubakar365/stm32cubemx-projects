################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/max30100/max30100.c 

OBJS += \
./Drivers/max30100/max30100.o 

C_DEPS += \
./Drivers/max30100/max30100.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/max30100/%.o Drivers/max30100/%.su Drivers/max30100/%.cyclo: ../Drivers/max30100/%.c Drivers/max30100/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/max30100 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-max30100

clean-Drivers-2f-max30100:
	-$(RM) ./Drivers/max30100/max30100.cyclo ./Drivers/max30100/max30100.d ./Drivers/max30100/max30100.o ./Drivers/max30100/max30100.su

.PHONY: clean-Drivers-2f-max30100

