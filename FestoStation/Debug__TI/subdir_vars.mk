################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../DK_TM4C129X.cmd 

CFG_SRCS += \
../FestoStation.cfg 

C_SRCS += \
../DK_TM4C129X.c \
../FestoStation.c \
../frame.c \
../kentec320x240x16_ssd2119.c 

OBJS += \
./DK_TM4C129X.obj \
./FestoStation.obj \
./frame.obj \
./kentec320x240x16_ssd2119.obj 

C_DEPS += \
./DK_TM4C129X.pp \
./FestoStation.pp \
./frame.pp \
./kentec320x240x16_ssd2119.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"DK_TM4C129X.pp" \
"FestoStation.pp" \
"frame.pp" \
"kentec320x240x16_ssd2119.pp" 

OBJS__QUOTED += \
"DK_TM4C129X.obj" \
"FestoStation.obj" \
"frame.obj" \
"kentec320x240x16_ssd2119.obj" 

C_SRCS__QUOTED += \
"../DK_TM4C129X.c" \
"../FestoStation.c" \
"../frame.c" \
"../kentec320x240x16_ssd2119.c" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 


