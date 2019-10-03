################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/VGA.c \
../src/platform.c \
../src/vdma.c \
../src/vga_display.c 

OBJS += \
./src/VGA.o \
./src/platform.o \
./src/vdma.o \
./src/vga_display.o 

C_DEPS += \
./src/VGA.d \
./src/platform.d \
./src/vdma.d \
./src/vga_display.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../FSBL_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


