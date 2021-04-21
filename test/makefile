################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include test/subdir.mk
-include engine/renderer/3d/subdir.mk
-include engine/renderer/2d/subdir.mk
-include engine/lib/texture/subdir.mk
-include engine/lib/sprite/subdir.mk
-include engine/lib/linal/vector/subdir.mk
-include engine/lib/linal/matrix/subdir.mk
-include engine/lib/linal/complex/subdir.mk
-include engine/lib/buffer/subdir.mk
-include engine/lib/arith/fixed/subdir.mk
-include engine/lib/arith/subdir.mk
-include engine/lib/3d/subdir.mk
-include emulator/linux/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: muonium-engine

# Tool invocations
muonium-engine: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o"muonium-engine" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) muonium-engine
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
