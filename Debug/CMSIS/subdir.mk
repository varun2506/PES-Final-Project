################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__REDLIB__ -DSDK_DEBUGCONSOLE=1 -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\board" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\source" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\drivers" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\CMSIS" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\utilities" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\freertos" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


