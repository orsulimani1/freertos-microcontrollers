################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/custom_drivers/rtc/rtc_driver.c \
../Drivers/custom_drivers/rtc/rtc_driver_api.c \
../Drivers/custom_drivers/rtc/rtc_internal.c 

OBJS += \
./Drivers/custom_drivers/rtc/rtc_driver.o \
./Drivers/custom_drivers/rtc/rtc_driver_api.o \
./Drivers/custom_drivers/rtc/rtc_internal.o 

C_DEPS += \
./Drivers/custom_drivers/rtc/rtc_driver.d \
./Drivers/custom_drivers/rtc/rtc_driver_api.d \
./Drivers/custom_drivers/rtc/rtc_internal.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/custom_drivers/rtc/%.o Drivers/custom_drivers/rtc/%.su Drivers/custom_drivers/rtc/%.cyclo: ../Drivers/custom_drivers/rtc/%.c Drivers/custom_drivers/rtc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"C:/src/freertos-microcontrollers/class0/00_logger/Drivers/custom_drivers/rtc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-custom_drivers-2f-rtc

clean-Drivers-2f-custom_drivers-2f-rtc:
	-$(RM) ./Drivers/custom_drivers/rtc/rtc_driver.cyclo ./Drivers/custom_drivers/rtc/rtc_driver.d ./Drivers/custom_drivers/rtc/rtc_driver.o ./Drivers/custom_drivers/rtc/rtc_driver.su ./Drivers/custom_drivers/rtc/rtc_driver_api.cyclo ./Drivers/custom_drivers/rtc/rtc_driver_api.d ./Drivers/custom_drivers/rtc/rtc_driver_api.o ./Drivers/custom_drivers/rtc/rtc_driver_api.su ./Drivers/custom_drivers/rtc/rtc_internal.cyclo ./Drivers/custom_drivers/rtc/rtc_internal.d ./Drivers/custom_drivers/rtc/rtc_internal.o ./Drivers/custom_drivers/rtc/rtc_internal.su

.PHONY: clean-Drivers-2f-custom_drivers-2f-rtc

