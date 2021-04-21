################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engine/lib/3d/Camera.cpp \
../engine/lib/3d/LightSun.cpp \
../engine/lib/3d/Poly.cpp 

OBJS += \
./engine/lib/3d/Camera.o \
./engine/lib/3d/LightSun.o \
./engine/lib/3d/Poly.o 

CPP_DEPS += \
./engine/lib/3d/Camera.d \
./engine/lib/3d/LightSun.d \
./engine/lib/3d/Poly.d 


# Each subdirectory must supply rules for building sources it contributes
engine/lib/3d/%.o: ../engine/lib/3d/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dtest_main=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


