################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../serial.cpp \
../ui_serial_avr.cpp \
../ui_serial_gen.cpp 

OBJS += \
./serial.o \
./ui_serial_avr.o \
./ui_serial_gen.o 

CPP_DEPS += \
./serial.d \
./ui_serial_avr.d \
./ui_serial_gen.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__RPI__ -I"../../uav_core/uav_core" -I"../../uav_rpi" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


