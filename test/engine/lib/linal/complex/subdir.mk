################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engine/lib/linal/complex/Complex.cpp \
../engine/lib/linal/complex/Quaternion.cpp 

OBJS += \
./engine/lib/linal/complex/Complex.o \
./engine/lib/linal/complex/Quaternion.o 

CPP_DEPS += \
./engine/lib/linal/complex/Complex.d \
./engine/lib/linal/complex/Quaternion.d 


# Each subdirectory must supply rules for building sources it contributes
engine/lib/linal/complex/%.o: ../engine/lib/linal/complex/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dtest_main=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


