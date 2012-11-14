################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Taggers/subdir.mk
-include src/Selections/subdir.mk
-include src/ReconstructionModules/subdir.mk
-include src/RecoObjects/subdir.mk
-include src/Readers/subdir.mk
-include src/Python/subdir.mk
-include src/Printers/subdir.mk
-include src/HitFit/subdir.mk
-include src/HistHelpers/subdir.mk
-include src/Analysers/subdir.mk
-include src/subdir.mk
-include bin/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: AnalysisSoftware

# Tool invocations
AnalysisSoftware: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ -o "AnalysisSoftware" $(OBJS) $(USER_OBJS) $(LIBS) -L/usr/lib -L/usr/local/lib -L"/software/root/current/lib" -L"/storage/Workspace/Analysis/TopQuarkAnalysis/Release" -Xlinker -L/usr/local/lib `/software/root/current/bin/root-config --libs --glibs --cflags --ldflags --auxlibs --auxcflags` -pthread -lm -ldl
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) AnalysisSoftware
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
