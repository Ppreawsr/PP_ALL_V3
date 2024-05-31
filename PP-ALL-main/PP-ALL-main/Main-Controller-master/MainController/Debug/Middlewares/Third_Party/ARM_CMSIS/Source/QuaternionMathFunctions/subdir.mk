################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.c 

OBJS += \
./Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.o 

C_DEPS += \
./Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/%.o Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/%.su Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/%.cyclo: ../Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/%.c Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/Include -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/BasicMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/BayesFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/CommonTables" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/ComplexMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/ControllerFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/DistanceFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/FastMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/FilteringFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/InterpolationFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/MatrixFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/QuaternionMathFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/StatisticsFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/SupportFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/SVMFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/TransformFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source/WindowFunctions" -I"C:/Users/ppreawsr/OneDrive/Documents/GitHub/PP_ALL_V3/PP-ALL-main/PP-ALL-main/Main-Controller-master/MainController/Source" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-Source-2f-QuaternionMathFunctions

clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-Source-2f-QuaternionMathFunctions:
	-$(RM) ./Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.cyclo ./Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.d ./Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.o ./Middlewares/Third_Party/ARM_CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-Source-2f-QuaternionMathFunctions

