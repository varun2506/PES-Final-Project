################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/LEDs.c \
../source/UART.c \
../source/cbfifo.c \
../source/cli.c \
../source/extra_switch.c \
../source/freertos_hello.c \
../source/i2c.c \
../source/mma8451.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/test_cbfifo.c \
../source/touch.c 

OBJS += \
./source/LEDs.o \
./source/UART.o \
./source/cbfifo.o \
./source/cli.o \
./source/extra_switch.o \
./source/freertos_hello.o \
./source/i2c.o \
./source/mma8451.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/test_cbfifo.o \
./source/touch.o 

C_DEPS += \
./source/LEDs.d \
./source/UART.d \
./source/cbfifo.d \
./source/cli.d \
./source/extra_switch.d \
./source/freertos_hello.d \
./source/i2c.d \
./source/mma8451.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/test_cbfifo.d \
./source/touch.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__REDLIB__ -DSDK_DEBUGCONSOLE=1 -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\board" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\source" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\drivers" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\CMSIS" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\utilities" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\freertos" -I"C:\Users\mvaru\Documents\MCUXpressoIDE_11.2.0_4120\Project\Accelerometer_freertos\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


