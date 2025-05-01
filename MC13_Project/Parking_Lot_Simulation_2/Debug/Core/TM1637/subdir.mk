################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TM1637/tm1637.c 

OBJS += \
./Core/TM1637/tm1637.o 

C_DEPS += \
./Core/TM1637/tm1637.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TM1637/%.o Core/TM1637/%.su Core/TM1637/%.cyclo: ../Core/TM1637/%.c Core/TM1637/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/denni/OneDrive/Desktop/MC10_Project/MC13_Project/Parking_Lot_Simulation_2/Core/TM1637" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TM1637

clean-Core-2f-TM1637:
	-$(RM) ./Core/TM1637/tm1637.cyclo ./Core/TM1637/tm1637.d ./Core/TM1637/tm1637.o ./Core/TM1637/tm1637.su

.PHONY: clean-Core-2f-TM1637

