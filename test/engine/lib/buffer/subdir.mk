################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engine/lib/buffer/DepthBuffer.cpp \
../engine/lib/buffer/FrameBuffer.cpp 

OBJS += \
./engine/lib/buffer/DepthBuffer.o \
./engine/lib/buffer/FrameBuffer.o 

CPP_DEPS += \
./engine/lib/buffer/DepthBuffer.d \
./engine/lib/buffer/FrameBuffer.d 


# Each subdirectory must supply rules for building sources it contributes
engine/lib/buffer/%.o: ../engine/lib/buffer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dtest_main=main -I"/mnt/2E94A04F94A01AFB/Projets/muonium-engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


