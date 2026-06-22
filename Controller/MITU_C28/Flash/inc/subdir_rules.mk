################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
inc/%.obj: ../inc/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'C2000 Compiler - building file: "$<"'
	"C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/include" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_C28" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_C28/inc" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" --advice:performance=all -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="inc/$(basename $(<F)).d_raw" --obj_directory="inc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

inc/%.obj: ../inc/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'C2000 Compiler - building file: "$<"'
	"C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/Program Files/ti/CCS2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/include" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_C28" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_C28/inc" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" --advice:performance=all -g --define="_FLASH" --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="inc/$(basename $(<F)).d_raw" --obj_directory="inc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


