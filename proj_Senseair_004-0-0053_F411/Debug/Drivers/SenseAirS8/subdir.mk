################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SenseAirS8/modbus_crc.c \
../Drivers/SenseAirS8/s8_uart.c \
../Drivers/SenseAirS8/utils.c 

OBJS += \
./Drivers/SenseAirS8/modbus_crc.o \
./Drivers/SenseAirS8/s8_uart.o \
./Drivers/SenseAirS8/utils.o 

C_DEPS += \
./Drivers/SenseAirS8/modbus_crc.d \
./Drivers/SenseAirS8/s8_uart.d \
./Drivers/SenseAirS8/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SenseAirS8/%.o Drivers/SenseAirS8/%.su Drivers/SenseAirS8/%.cyclo: ../Drivers/SenseAirS8/%.c Drivers/SenseAirS8/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/bmp280 -I../Drivers/SenseAirS8 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SenseAirS8

clean-Drivers-2f-SenseAirS8:
	-$(RM) ./Drivers/SenseAirS8/modbus_crc.cyclo ./Drivers/SenseAirS8/modbus_crc.d ./Drivers/SenseAirS8/modbus_crc.o ./Drivers/SenseAirS8/modbus_crc.su ./Drivers/SenseAirS8/s8_uart.cyclo ./Drivers/SenseAirS8/s8_uart.d ./Drivers/SenseAirS8/s8_uart.o ./Drivers/SenseAirS8/s8_uart.su ./Drivers/SenseAirS8/utils.cyclo ./Drivers/SenseAirS8/utils.d ./Drivers/SenseAirS8/utils.o ./Drivers/SenseAirS8/utils.su

.PHONY: clean-Drivers-2f-SenseAirS8

