################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engine/lib/linal/matrix/Matrix2.cpp \
../engine/lib/linal/matrix/Matrix3.cpp \
../engine/lib/linal/matrix/Matrix4.cpp 

OBJS += \
./engine/lib/linal/matrix/Matrix2.o \
./engine/lib/linal/matrix/Matrix3.o \
./engine/lib/linal/matrix/Matrix4.o 

CPP_DEPS += \
./engine/lib/linal/matrix/Matrix2.d \
./engine/lib/linal/matrix/Matrix3.d \
./engine/lib/linal/matrix/Matrix4.d 


# Each subdirectory must supply rules for building sources it contributes
engine/lib/linal/matrix/%.o: ../engine/lib/linal/matrix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dtest_main=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


