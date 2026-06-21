################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
PCON_C28/F28M35x_Adc.obj: ../PCON_C28/F28M35x_Adc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_Adc.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_CodeStartBranch.obj: ../PCON_C28/F28M35x_CodeStartBranch.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_CodeStartBranch.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_CpuTimers.obj: ../PCON_C28/F28M35x_CpuTimers.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_CpuTimers.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_DefaultIsr.obj: ../PCON_C28/F28M35x_DefaultIsr.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_DefaultIsr.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_GlobalVariableDefs.obj: ../PCON_C28/F28M35x_GlobalVariableDefs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_GlobalVariableDefs.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_Gpio.obj: ../PCON_C28/F28M35x_Gpio.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_Gpio.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_Ipc.obj: ../PCON_C28/F28M35x_Ipc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_Ipc.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_Ipc_Lite.obj: ../PCON_C28/F28M35x_Ipc_Lite.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_Ipc_Lite.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_Ipc_Util.obj: ../PCON_C28/F28M35x_Ipc_Util.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_Ipc_Util.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_PieCtrl.obj: ../PCON_C28/F28M35x_PieCtrl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_PieCtrl.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_PieVect.obj: ../PCON_C28/F28M35x_PieVect.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_PieVect.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_SysCtrl.obj: ../PCON_C28/F28M35x_SysCtrl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_SysCtrl.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/F28M35x_usDelay.obj: ../PCON_C28/F28M35x_usDelay.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/F28M35x_usDelay.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/InitFunctions.obj: ../PCON_C28/InitFunctions.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/InitFunctions.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/MemFunctions.obj: ../PCON_C28/MemFunctions.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/MemFunctions.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/PCON_C28.obj: ../PCON_C28/PCON_C28.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/PCON_C28.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PCON_C28/pid.obj: ../PCON_C28/pid.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.6/include" --include_path="C:/Users/VKD/workspace_v5_5/PCON_C28" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="PCON_C28/pid.pp" --obj_directory="PCON_C28" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


