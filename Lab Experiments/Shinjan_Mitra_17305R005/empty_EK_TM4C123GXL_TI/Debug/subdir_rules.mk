################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
EK_TM4C123GXL.obj: ../EK_TM4C123GXL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/shinjan/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/shinjan/Workspaces/CS684-2017/Experiments/empty_EK_TM4C123GXL_TI" --include_path="/home/shinjan/Workspaces/CS684-2017/Experiments/empty_EK_TM4C123GXL_TI" --include_path="/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b" --include_path="/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="/home/shinjan/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="EK_TM4C123GXL.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

empty.obj: ../empty.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/shinjan/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/shinjan/Workspaces/CS684-2017/Experiments/empty_EK_TM4C123GXL_TI" --include_path="/home/shinjan/Workspaces/CS684-2017/Experiments/empty_EK_TM4C123GXL_TI" --include_path="/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b" --include_path="/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="/home/shinjan/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="empty.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-881623425:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-881623425-inproc

build-881623425-inproc: ../empty.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/home/shinjan/ti/xdctools_3_32_00_06_core/xs" --xdcpath="/home/shinjan/ti/tirtos_tivac_2_16_00_08/packages;/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages;/home/shinjan/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C123GH6PM -r release -c "/home/shinjan/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"/home/shinjan/Workspaces/CS684-2017/Experiments/empty_EK_TM4C123GXL_TI\" --include_path=\"/home/shinjan/Workspaces/CS684-2017/Experiments/empty_EK_TM4C123GXL_TI\" --include_path=\"/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b\" --include_path=\"/home/shinjan/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix\" --include_path=\"/home/shinjan/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include\" --define=ccs=\"ccs\" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-881623425 ../empty.cfg
configPkg/compiler.opt: build-881623425
configPkg/: build-881623425


