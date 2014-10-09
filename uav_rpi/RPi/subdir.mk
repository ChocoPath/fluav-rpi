################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ifc_loader.cpp \
../ini_file.cpp \
../loader.cpp \
../rpi_pilots.cpp \
../rpi_system.cpp 

OBJS += \
./ifc_loader.o \
./ini_file.o \
./loader.o \
./rpi_pilots.o \
./rpi_system.o 

CPP_DEPS += \
./ifc_loader.d \
./ini_file.d \
./loader.d \
./rpi_pilots.d \
./rpi_system.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__RPI__ -I"../" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


