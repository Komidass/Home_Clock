################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Clock.c \
../DIO_Program.c \
../EEPROM.c \
../EXTI_program.c \
../KBD_program.c \
../LCD_program.c \
../PWM_T0.c \
../croutine.c \
../event_groups.c \
../heap_1.c \
../list.c \
../main.c \
../port.c \
../queue.c \
../tasks.c \
../timers.c 

OBJS += \
./Clock.o \
./DIO_Program.o \
./EEPROM.o \
./EXTI_program.o \
./KBD_program.o \
./LCD_program.o \
./PWM_T0.o \
./croutine.o \
./event_groups.o \
./heap_1.o \
./list.o \
./main.o \
./port.o \
./queue.o \
./tasks.o \
./timers.o 

C_DEPS += \
./Clock.d \
./DIO_Program.d \
./EEPROM.d \
./EXTI_program.d \
./KBD_program.d \
./LCD_program.d \
./PWM_T0.d \
./croutine.d \
./event_groups.d \
./heap_1.d \
./list.d \
./main.d \
./port.d \
./queue.d \
./tasks.d \
./timers.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


