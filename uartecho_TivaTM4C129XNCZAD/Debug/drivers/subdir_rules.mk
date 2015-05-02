################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/frame.obj: ../drivers/frame.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x" --include_path="c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="drivers/frame.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/kentec320x240x16_ssd2119.obj: ../drivers/kentec320x240x16_ssd2119.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/examples/boards/dk-tm4c129x" --include_path="c:/ti/tirtos_tivac_2_10_01_38/products/TivaWare_C_Series-2.1.0.12573c" -g --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --preproc_with_compile --preproc_dependency="drivers/kentec320x240x16_ssd2119.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


