################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Clock.o: C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/inc/Clock.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LaunchPad.o: C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/inc/LaunchPad.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SPI.o: C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/inc/SPI.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

TExaS.o: C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/inc/TExaS.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Timer.o: C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/inc/Timer.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/rober/OneDrive/Documents/College/319H/MSPM0_ValvanoWare/MadMines/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


