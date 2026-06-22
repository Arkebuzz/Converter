################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADC_Interrupt.obj" \
"./ALG_DataExchange.obj" \
"./ALG_DataProcessing.obj" \
"./ALG_MainCycleCalc.obj" \
"./ALG_ParamManager.obj" \
"./ALG_Protections.obj" \
"./ALG_Regulation.obj" \
"./ALG_StateMachine.obj" \
"./ALG_StructuresInit.obj" \
"./ALG_Synchronizer.obj" \
"./ExternalControls.obj" \
"./Filters.obj" \
"./GlobalDataInit.obj" \
"./InitFunctions.obj" \
"./MemFunctions.obj" \
"./PCON_C28.obj" \
"./SPIFlashFunctions.obj" \
"./TimedProt.obj" \
"./pid.obj" \
"./inc/F28M35x_Adc.obj" \
"./inc/F28M35x_CodeStartBranch.obj" \
"./inc/F28M35x_CpuTimers.obj" \
"./inc/F28M35x_DMA.obj" \
"./inc/F28M35x_DefaultIsr.obj" \
"./inc/F28M35x_GlobalVariableDefs.obj" \
"./inc/F28M35x_Gpio.obj" \
"./inc/F28M35x_Ipc.obj" \
"./inc/F28M35x_Ipc_Lite.obj" \
"./inc/F28M35x_Ipc_Util.obj" \
"./inc/F28M35x_PieCtrl.obj" \
"./inc/F28M35x_PieVect.obj" \
"./inc/F28M35x_SysCtrl.obj" \
"./inc/F28M35x_usDelay.obj" \
"../inc/F28M35x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/cmd/F28M35x_generic_C28_RAM.cmd" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include inc/subdir_vars.mk
-include subdir_rules.mk
-include inc/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
blinky_dc_c28.out 

EXE_OUTPUTS__QUOTED += \
"blinky_dc_c28.out" 


# All Target
all: blinky_dc_c28.out

# Tool invocations
blinky_dc_c28.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'C2000 Linker - building target: "$@"'
	"C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -g --diag_warning=225 --no_fast_branch -z -m"blinky_dc_c28.map" --stack_size=0x300 --warn_sections -i"C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/lib" -i"C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/include" --reread_libs --xml_link_info="blinky_dc_c28_linkInfo.xml" --entry_point=code_start --rom_model -o "blinky_dc_c28.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADC_Interrupt.obj" "ALG_DataExchange.obj" "ALG_DataProcessing.obj" "ALG_MainCycleCalc.obj" "ALG_ParamManager.obj" "ALG_Protections.obj" "ALG_Regulation.obj" "ALG_StateMachine.obj" "ALG_StructuresInit.obj" "ALG_Synchronizer.obj" "ExternalControls.obj" "Filters.obj" "GlobalDataInit.obj" "InitFunctions.obj" "MemFunctions.obj" "PCON_C28.obj" "SPIFlashFunctions.obj" "TimedProt.obj" "pid.obj" "inc\F28M35x_Adc.obj" "inc\F28M35x_CodeStartBranch.obj" "inc\F28M35x_CpuTimers.obj" "inc\F28M35x_DMA.obj" "inc\F28M35x_DefaultIsr.obj" "inc\F28M35x_GlobalVariableDefs.obj" "inc\F28M35x_Gpio.obj" "inc\F28M35x_Ipc.obj" "inc\F28M35x_Ipc_Lite.obj" "inc\F28M35x_Ipc_Util.obj" "inc\F28M35x_PieCtrl.obj" "inc\F28M35x_PieVect.obj" "inc\F28M35x_SysCtrl.obj" "inc\F28M35x_usDelay.obj" 
	-$(RM) "InitFunctions.d" "inc\F28M35x_Adc.d" "inc\F28M35x_CpuTimers.d" "inc\F28M35x_DMA.d" "inc\F28M35x_DefaultIsr.d" "inc\F28M35x_GlobalVariableDefs.d" "inc\F28M35x_Gpio.d" "inc\F28M35x_Ipc.d" "inc\F28M35x_Ipc_Lite.d" "inc\F28M35x_Ipc_Util.d" "inc\F28M35x_PieCtrl.d" "inc\F28M35x_PieVect.d" "inc\F28M35x_SysCtrl.d" 
	-$(RM) "ADC_Interrupt.d" "ALG_DataExchange.d" "ALG_DataProcessing.d" "ALG_MainCycleCalc.d" "ALG_ParamManager.d" "ALG_Protections.d" "ALG_Regulation.d" "ALG_StateMachine.d" "ALG_StructuresInit.d" "ALG_Synchronizer.d" "ExternalControls.d" "Filters.d" "GlobalDataInit.d" "MemFunctions.d" "PCON_C28.d" "SPIFlashFunctions.d" "TimedProt.d" "pid.d" 
	-$(RM) "inc\F28M35x_CodeStartBranch.d" "inc\F28M35x_usDelay.d" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

