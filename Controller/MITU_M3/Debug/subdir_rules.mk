################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'ARM Compiler - building file: "$<"'
	"C:/ti/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/user/ekb/Controller/MITU_M3/modbus/rtu" --include_path="C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages/ti/ndk/inc" --include_path="C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages/ti/ndk/inc/bsd" --include_path="C:/Users/user/ekb/Controller/MITU_M3/modbus/tcp" --include_path="C:/Users/user/ekb/Controller/MITU_M3/modbus/include" --include_path="C:/Users/user/ekb/Controller/MITU_M3/modbus" --include_path="C:/Users/user/ekb/Controller/MITU_M3/port" --include_path="C:/Users/user/ekb/Controller/MITU_M3" --include_path="C:/ti/tirtos_c2000_2_16_00_08/products/MWare_v202a" --include_path="C:/ti/tirtos_c2000_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="C:/ti/ti-cgt-arm_16.9.0.LTS/include" --include_path="C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages/ti/ndk/inc/nettools" --define=ccs --define=MWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-634965199:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-634965199-inproc

build-634965199-inproc: ../MITU_M3.cfg
	@echo 'XDCtools - building file: "$<"'
	"C:/ti/xdctools_3_32_00_06_core/xs" --xdcpath="C:/ti/tirtos_c2000_2_16_00_08/packages;C:/ti/tirtos_c2000_2_16_00_08/products/tidrivers_c2000_2_16_00_08/packages;C:/ti/tirtos_c2000_2_16_00_08/products/bios_6_45_01_29/packages;C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages;C:/ti/tirtos_c2000_2_16_00_08/products/uia_2_00_05_50/packages;C:/ti/xdctools_3_32_00_06_core;C:/ti/xdctools_3_32_01_22_core;C:/ti/xdctools_3_32_00_06_core/packages/xdc;C:/ti/ccs2051/ccs/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.concertoM3:F28M35H52C1 -r release -c "C:/ti/ti-cgt-arm_16.9.0.LTS" --compileOptions "-mv7M3 --code_state=16 --float_support=vfplib -me --include_path=\"C:/Users/user/ekb/Controller/MITU_M3/modbus/rtu\" --include_path=\"C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages/ti/ndk/inc\" --include_path=\"C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages/ti/ndk/inc/bsd\" --include_path=\"C:/Users/user/ekb/Controller/MITU_M3/modbus/tcp\" --include_path=\"C:/Users/user/ekb/Controller/MITU_M3/modbus/include\" --include_path=\"C:/Users/user/ekb/Controller/MITU_M3/modbus\" --include_path=\"C:/Users/user/ekb/Controller/MITU_M3/port\" --include_path=\"C:/Users/user/ekb/Controller/MITU_M3\" --include_path=\"C:/ti/tirtos_c2000_2_16_00_08/products/MWare_v202a\" --include_path=\"C:/ti/tirtos_c2000_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix\" --include_path=\"C:/ti/ti-cgt-arm_16.9.0.LTS/include\" --include_path=\"C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages/ti/ndk/inc/nettools\" --define=ccs --define=MWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-634965199 ../MITU_M3.cfg
configPkg/compiler.opt: build-634965199
configPkg: build-634965199


