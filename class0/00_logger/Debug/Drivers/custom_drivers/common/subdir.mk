################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/custom_drivers/common/itm_debug.c 

OBJS += \
./Drivers/custom_drivers/common/itm_debug.o 

C_DEPS += \
./Drivers/custom_drivers/common/itm_debug.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/custom_drivers/common/%.o Drivers/custom_drivers/common/%.su Drivers/custom_drivers/common/%.cyclo: ../Drivers/custom_drivers/common/%.c Drivers/custom_drivers/common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"C:/src/freertos-microcontrollers/class0/00_logger/Drivers/custom_drivers/rtc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-custom_drivers-2f-common

clean-Drivers-2f-custom_drivers-2f-common:
	-$(RM) ./Drivers/custom_drivers/common/itm_debug.cyclo ./Drivers/custom_drivers/common/itm_debug.d ./Drivers/custom_drivers/common/itm_debug.o ./Drivers/custom_drivers/common/itm_debug.su

.PHONY: clean-Drivers-2f-custom_drivers-2f-common

