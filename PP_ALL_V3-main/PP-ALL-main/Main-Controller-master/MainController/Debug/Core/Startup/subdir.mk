################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g474retx.s 

OBJS += \
./Core/Startup/startup_stm32g474retx.o 

S_DEPS += \
./Core/Startup/startup_stm32g474retx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/BasicMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/BayesFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/CommonTables" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/ComplexMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/ControllerFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/DistanceFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/FastMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/FilteringFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/InterpolationFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/MatrixFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/QuaternionMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/StatisticsFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/SupportFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/SVMFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/TransformFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source/WindowFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP_ALL_V3-main/PP-ALL-main/Main-Controller-master/MainController/Source" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g474retx.d ./Core/Startup/startup_stm32g474retx.o

.PHONY: clean-Core-2f-Startup

