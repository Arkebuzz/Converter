################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
modbus/rtu/%.obj: ../modbus/rtu/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'ARM Compiler - building file: "$<"'
	"" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/rtu" --include_path="/products/ndk_2_25_00_09/packages/ti/ndk/inc" --include_path="/products/ndk_2_25_00_09/packages/ti/ndk/inc/bsd" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/tcp" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/include" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/port" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3" --include_path="/products/MWare_v202a" --include_path="/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="/include" --include_path="/products/ndk_2_25_00_09/packages/ti/ndk/inc/nettools" --define=ccs --define=MWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="modbus/rtu/$(basename $(<F)).d_raw" --obj_directory="modbus/rtu" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


