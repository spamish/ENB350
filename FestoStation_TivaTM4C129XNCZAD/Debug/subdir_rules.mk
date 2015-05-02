################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DK_TM4C129X.obj: ../DK_TM4C129X.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x" --include_path="c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="DK_TM4C129X.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

frame.obj: ../frame.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x" --include_path="c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="frame.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

kentec320x240x16_ssd2119.obj: ../kentec320x240x16_ssd2119.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x" --include_path="c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="kentec320x240x16_ssd2119.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x" --include_path="c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: ../uartecho.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"c:/ti/xdctools_3_30_03_47_core/xs" --xdcpath="c:/ti/tirtos_tivac_2_10_01_38/packages;c:/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages;c:/ti/tirtos_tivac_2_10_01_38/products/ndk_2_24_01_18/packages;c:/ti/tirtos_tivac_2_10_01_38/products/uia_2_00_02_39/packages;c:/ti/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C129XNCZAD -r release -c "c:/ti/ccsv6/tools/compiler/arm_5.1.6" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path=\"c:/ti/ccsv6/tools/compiler/arm_5.1.6/include\" --include_path=\"C:/ti/TivaWare_C_Series-2.0.1.11577\" --include_path=\"C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x\" --include_path=\"c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c\" -g --gcc --define=ccs=\"ccs\" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd


