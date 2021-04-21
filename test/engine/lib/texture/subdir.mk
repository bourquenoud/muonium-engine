################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engine/lib/texture/Colour.cpp \
../engine/lib/texture/Texture.cpp 

OBJS += \
./engine/lib/texture/Colour.o \
./engine/lib/texture/Texture.o 

CPP_DEPS += \
./engine/lib/texture/Colour.d \
./engine/lib/texture/Texture.d 


# Each subdirectory must supply rules for building sources it contributes
engine/lib/texture/%.o: ../engine/lib/texture/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dtest_main=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


