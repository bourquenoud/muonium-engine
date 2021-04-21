################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../emulator/linux/PolyLoader.cpp \
../emulator/linux/emulator_main.cpp \
../emulator/linux/old.cpp 

OBJS += \
./emulator/linux/PolyLoader.o \
./emulator/linux/emulator_main.o \
./emulator/linux/old.o 

CPP_DEPS += \
./emulator/linux/PolyLoader.d \
./emulator/linux/emulator_main.d \
./emulator/linux/old.d 


# Each subdirectory must supply rules for building sources it contributes
emulator/linux/%.o: ../emulator/linux/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dtest_main=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


