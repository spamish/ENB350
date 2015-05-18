################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DK_TM4C129X.obj: ../DK_TM4C129X.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/tirtos_tivac_2_12_01_33/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="DK_TM4C129X.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

FestoStation.obj: ../FestoStation.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/tirtos_tivac_2_12_01_33/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="FestoStation.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: ../FestoStation.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_31_00_24_core/xs" --xdcpath="C:/ti/tirtos_tivac_2_12_01_33/packages;C:/ti/tirtos_tivac_2_12_01_33/products/bios_6_41_04_54/packages;C:/ti/tirtos_tivac_2_12_01_33/products/ndk_2_24_02_31/packages;C:/ti/tirtos_tivac_2_12_01_33/products/uia_2_00_02_39/packages;C:/ti/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C129XNCZAD -r release -c "C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path=\"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include\" --include_path=\"C:/ti/tirtos_tivac_2_12_01_33/products/TivaWare_C_Series-2.1.0.12573c\" -g --gcc --define=ccs=\"ccs\" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd

frame.obj: ../frame.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/tirtos_tivac_2_12_01_33/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="frame.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

kentec320x240x16_ssd2119.obj: ../kentec320x240x16_ssd2119.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/tirtos_tivac_2_12_01_33/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="kentec320x240x16_ssd2119.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


