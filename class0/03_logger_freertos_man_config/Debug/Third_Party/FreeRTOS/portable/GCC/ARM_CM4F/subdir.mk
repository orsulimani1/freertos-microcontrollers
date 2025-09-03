################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/%.o Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/%.su Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/%.cyclo: ../Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/%.c Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/src/freertos-microcontrollers/class0/03_logger_freertos_man_config/Third_Party/FreeRTOS/include" -I"C:/src/freertos-microcontrollers/class0/03_logger_freertos_man_config/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Third_Party-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F

clean-Third_Party-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.cyclo ./Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.d ./Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.o ./Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-Third_Party-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F

