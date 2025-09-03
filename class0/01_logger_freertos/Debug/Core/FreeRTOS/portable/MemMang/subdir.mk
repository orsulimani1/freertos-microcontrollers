################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/FreeRTOS/portable/MemMang/heap_5.c 

OBJS += \
./Core/FreeRTOS/portable/MemMang/heap_5.o 

C_DEPS += \
./Core/FreeRTOS/portable/MemMang/heap_5.d 


# Each subdirectory must supply rules for building sources it contributes
Core/FreeRTOS/portable/MemMang/%.o Core/FreeRTOS/portable/MemMang/%.su Core/FreeRTOS/portable/MemMang/%.cyclo: ../Core/FreeRTOS/portable/MemMang/%.c Core/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Core/FreeRTOS/portable/GCC/ARM_CM4F -I"C:/src/freertos-microcontrollers/class0/01_logger_FREE_RTOS_manual/00_logger_freertos/Core/FreeRTOS/include" -I"C:/src/freertos-microcontrollers/class0/01_logger_FREE_RTOS_manual/00_logger_freertos/Drivers/custom_drivers/rtc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-FreeRTOS-2f-portable-2f-MemMang

clean-Core-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./Core/FreeRTOS/portable/MemMang/heap_5.cyclo ./Core/FreeRTOS/portable/MemMang/heap_5.d ./Core/FreeRTOS/portable/MemMang/heap_5.o ./Core/FreeRTOS/portable/MemMang/heap_5.su

.PHONY: clean-Core-2f-FreeRTOS-2f-portable-2f-MemMang

