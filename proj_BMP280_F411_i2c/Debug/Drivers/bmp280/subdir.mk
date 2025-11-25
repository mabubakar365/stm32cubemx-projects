################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/bmp280/bmp280.c 

OBJS += \
./Drivers/bmp280/bmp280.o 

C_DEPS += \
./Drivers/bmp280/bmp280.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/bmp280/%.o Drivers/bmp280/%.su Drivers/bmp280/%.cyclo: ../Drivers/bmp280/%.c Drivers/bmp280/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/bmp280 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-bmp280

clean-Drivers-2f-bmp280:
	-$(RM) ./Drivers/bmp280/bmp280.cyclo ./Drivers/bmp280/bmp280.d ./Drivers/bmp280/bmp280.o ./Drivers/bmp280/bmp280.su

.PHONY: clean-Drivers-2f-bmp280

