################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engine/lib/poly/Poly.cpp 

OBJS += \
./engine/lib/poly/Poly.o 

CPP_DEPS += \
./engine/lib/poly/Poly.d 


# Each subdirectory must supply rules for building sources it contributes
engine/lib/poly/%.o: ../engine/lib/poly/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_MAIN=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


