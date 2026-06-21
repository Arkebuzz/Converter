################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'ARM Compiler - building file: "$<"'
	"" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/rtu" --include_path="/products/ndk_2_25_00_09/packages/ti/ndk/inc" --include_path="/products/ndk_2_25_00_09/packages/ti/ndk/inc/bsd" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/tcp" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/include" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/port" --include_path="C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3" --include_path="/products/MWare_v202a" --include_path="/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="/include" --include_path="/products/ndk_2_25_00_09/packages/ti/ndk/inc/nettools" --define=ccs --define=MWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-634965199:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-634965199-inproc

build-634965199-inproc: ../MITU_M3.cfg
	@echo 'XDCtools - building file: "$<"'
	"/xs" --xdcpath="C:/ti/xdctools_3_32_00_06_core;C:/ti/xdctools_3_32_01_22_core;C:/ti/xdctools_3_32_00_06_core/packages/xdc;C:/Program Files/ti/CCS2051/ccs/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.concertoM3:F28M35H52C1 -r release -c --compileOptions "-mv7M3 --code_state=16 --float_support=vfplib -me --include_path=\"C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/rtu\" --include_path=\"/products/ndk_2_25_00_09/packages/ti/ndk/inc\" --include_path=\"/products/ndk_2_25_00_09/packages/ti/ndk/inc/bsd\" --include_path=\"C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/tcp\" --include_path=\"C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus/include\" --include_path=\"C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/modbus\" --include_path=\"C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3/port\" --include_path=\"C:/Users/Arkebuzz/Documents/Практика/workspace_v6_2 TEM2/MITU_M3\" --include_path=\"/products/MWare_v202a\" --include_path=\"/products/bios_6_45_01_29/packages/ti/sysbios/posix\" --include_path=\"/include\" --include_path=\"/products/ndk_2_25_00_09/packages/ti/ndk/inc/nettools\" --define=ccs --define=MWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-634965199 ../MITU_M3.cfg
configPkg/compiler.opt: build-634965199
configPkg: build-634965199


