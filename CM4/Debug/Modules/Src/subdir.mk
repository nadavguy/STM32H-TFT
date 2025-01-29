################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Src/SharedMemoryAgent.c 

OBJS += \
./Modules/Src/SharedMemoryAgent.o 

C_DEPS += \
./Modules/Src/SharedMemoryAgent.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Src/%.o Modules/Src/%.su Modules/Src/%.cyclo: ../Modules/Src/%.c Modules/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -DUSE_NUCLEO_64 -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/BSP/STM32H7xx_Nucleo -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"/Users/nadavguy/STM32CubeIDE/workspace_1.17.0/STM32H-TFT/Common/Inc" -I"/Users/nadavguy/STM32CubeIDE/workspace_1.17.0/STM32H-TFT/CM4/Modules/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Modules-2f-Src

clean-Modules-2f-Src:
	-$(RM) ./Modules/Src/SharedMemoryAgent.cyclo ./Modules/Src/SharedMemoryAgent.d ./Modules/Src/SharedMemoryAgent.o ./Modules/Src/SharedMemoryAgent.su

.PHONY: clean-Modules-2f-Src

