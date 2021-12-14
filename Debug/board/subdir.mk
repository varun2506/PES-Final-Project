################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__REDLIB__ -DSDK_DEBUGCONSOLE=1 -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\board" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\source" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\drivers" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\CMSIS" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\utilities" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\freertos" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


