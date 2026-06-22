################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'C2000 Compiler - building file: "$<"'
	"C:/ti/ti-cgt-c2000_16.9.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ti-cgt-c2000_16.9.0.LTS/include" --include_path="C:/Users/user/ekb/Controller/MITU_C28" --include_path="C:/Users/user/ekb/Controller/MITU_C28/inc" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" --advice:performance=all --define=_FLASH -g --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'C2000 Compiler - building file: "$<"'
	"C:/ti/ti-cgt-c2000_16.9.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --vcu_support=vcu0 -O1 --opt_for_speed=5 --include_path="C:/ti/ti-cgt-c2000_16.9.0.LTS/include" --include_path="C:/Users/user/ekb/Controller/MITU_C28" --include_path="C:/Users/user/ekb/Controller/MITU_C28/inc" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_common/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v202/F28M35x_headers/include" --advice:performance=all --define=_FLASH -g --diag_warning=225 --no_fast_branch --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


