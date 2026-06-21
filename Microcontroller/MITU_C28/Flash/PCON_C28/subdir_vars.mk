################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PCON_C28/MemFunctions.cpp \
../PCON_C28/PCON_C28.cpp \
../PCON_C28/pid.cpp 

CMD_SRCS += \
../PCON_C28/F28M35x_Headers_nonBIOS.cmd 

ASM_SRCS += \
../PCON_C28/F28M35x_CodeStartBranch.asm \
../PCON_C28/F28M35x_usDelay.asm 

C_SRCS += \
../PCON_C28/F28M35x_Adc.c \
../PCON_C28/F28M35x_CpuTimers.c \
../PCON_C28/F28M35x_DefaultIsr.c \
../PCON_C28/F28M35x_GlobalVariableDefs.c \
../PCON_C28/F28M35x_Gpio.c \
../PCON_C28/F28M35x_Ipc.c \
../PCON_C28/F28M35x_Ipc_Lite.c \
../PCON_C28/F28M35x_Ipc_Util.c \
../PCON_C28/F28M35x_PieCtrl.c \
../PCON_C28/F28M35x_PieVect.c \
../PCON_C28/F28M35x_SysCtrl.c \
../PCON_C28/InitFunctions.c 

OBJS += \
./PCON_C28/F28M35x_Adc.obj \
./PCON_C28/F28M35x_CodeStartBranch.obj \
./PCON_C28/F28M35x_CpuTimers.obj \
./PCON_C28/F28M35x_DefaultIsr.obj \
./PCON_C28/F28M35x_GlobalVariableDefs.obj \
./PCON_C28/F28M35x_Gpio.obj \
./PCON_C28/F28M35x_Ipc.obj \
./PCON_C28/F28M35x_Ipc_Lite.obj \
./PCON_C28/F28M35x_Ipc_Util.obj \
./PCON_C28/F28M35x_PieCtrl.obj \
./PCON_C28/F28M35x_PieVect.obj \
./PCON_C28/F28M35x_SysCtrl.obj \
./PCON_C28/F28M35x_usDelay.obj \
./PCON_C28/InitFunctions.obj \
./PCON_C28/MemFunctions.obj \
./PCON_C28/PCON_C28.obj \
./PCON_C28/pid.obj 

ASM_DEPS += \
./PCON_C28/F28M35x_CodeStartBranch.pp \
./PCON_C28/F28M35x_usDelay.pp 

CPP_DEPS += \
./PCON_C28/MemFunctions.pp \
./PCON_C28/PCON_C28.pp \
./PCON_C28/pid.pp 

C_DEPS += \
./PCON_C28/F28M35x_Adc.pp \
./PCON_C28/F28M35x_CpuTimers.pp \
./PCON_C28/F28M35x_DefaultIsr.pp \
./PCON_C28/F28M35x_GlobalVariableDefs.pp \
./PCON_C28/F28M35x_Gpio.pp \
./PCON_C28/F28M35x_Ipc.pp \
./PCON_C28/F28M35x_Ipc_Lite.pp \
./PCON_C28/F28M35x_Ipc_Util.pp \
./PCON_C28/F28M35x_PieCtrl.pp \
./PCON_C28/F28M35x_PieVect.pp \
./PCON_C28/F28M35x_SysCtrl.pp \
./PCON_C28/InitFunctions.pp 

CPP_DEPS__QUOTED += \
"PCON_C28\MemFunctions.pp" \
"PCON_C28\PCON_C28.pp" \
"PCON_C28\pid.pp" 

C_DEPS__QUOTED += \
"PCON_C28\F28M35x_Adc.pp" \
"PCON_C28\F28M35x_CpuTimers.pp" \
"PCON_C28\F28M35x_DefaultIsr.pp" \
"PCON_C28\F28M35x_GlobalVariableDefs.pp" \
"PCON_C28\F28M35x_Gpio.pp" \
"PCON_C28\F28M35x_Ipc.pp" \
"PCON_C28\F28M35x_Ipc_Lite.pp" \
"PCON_C28\F28M35x_Ipc_Util.pp" \
"PCON_C28\F28M35x_PieCtrl.pp" \
"PCON_C28\F28M35x_PieVect.pp" \
"PCON_C28\F28M35x_SysCtrl.pp" \
"PCON_C28\InitFunctions.pp" 

OBJS__QUOTED += \
"PCON_C28\F28M35x_Adc.obj" \
"PCON_C28\F28M35x_CodeStartBranch.obj" \
"PCON_C28\F28M35x_CpuTimers.obj" \
"PCON_C28\F28M35x_DefaultIsr.obj" \
"PCON_C28\F28M35x_GlobalVariableDefs.obj" \
"PCON_C28\F28M35x_Gpio.obj" \
"PCON_C28\F28M35x_Ipc.obj" \
"PCON_C28\F28M35x_Ipc_Lite.obj" \
"PCON_C28\F28M35x_Ipc_Util.obj" \
"PCON_C28\F28M35x_PieCtrl.obj" \
"PCON_C28\F28M35x_PieVect.obj" \
"PCON_C28\F28M35x_SysCtrl.obj" \
"PCON_C28\F28M35x_usDelay.obj" \
"PCON_C28\InitFunctions.obj" \
"PCON_C28\MemFunctions.obj" \
"PCON_C28\PCON_C28.obj" \
"PCON_C28\pid.obj" 

ASM_DEPS__QUOTED += \
"PCON_C28\F28M35x_CodeStartBranch.pp" \
"PCON_C28\F28M35x_usDelay.pp" 

C_SRCS__QUOTED += \
"../PCON_C28/F28M35x_Adc.c" \
"../PCON_C28/F28M35x_CpuTimers.c" \
"../PCON_C28/F28M35x_DefaultIsr.c" \
"../PCON_C28/F28M35x_GlobalVariableDefs.c" \
"../PCON_C28/F28M35x_Gpio.c" \
"../PCON_C28/F28M35x_Ipc.c" \
"../PCON_C28/F28M35x_Ipc_Lite.c" \
"../PCON_C28/F28M35x_Ipc_Util.c" \
"../PCON_C28/F28M35x_PieCtrl.c" \
"../PCON_C28/F28M35x_PieVect.c" \
"../PCON_C28/F28M35x_SysCtrl.c" \
"../PCON_C28/InitFunctions.c" 

ASM_SRCS__QUOTED += \
"../PCON_C28/F28M35x_CodeStartBranch.asm" \
"../PCON_C28/F28M35x_usDelay.asm" 

CPP_SRCS__QUOTED += \
"../PCON_C28/MemFunctions.cpp" \
"../PCON_C28/PCON_C28.cpp" \
"../PCON_C28/pid.cpp" 


