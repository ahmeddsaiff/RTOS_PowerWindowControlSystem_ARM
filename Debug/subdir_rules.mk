################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject" --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject/FreeRTOS/Source/include" --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject/MCAL/UART" --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject/MCAL/GPIO" --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject/MCAL" --include_path="C:/Users/ahmed/OneDrive/Desktop/Labs-RTOS-SherifHammad/PowerWindowControlSystemProject/Common" --include_path="C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


