################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Drivers/Modbus_Client.c \
../Core/Drivers/Modbus_ClientParser.c \
../Core/Drivers/Modbus_ClientRequest.c 

OBJS += \
./Core/Drivers/Modbus_Client.o \
./Core/Drivers/Modbus_ClientParser.o \
./Core/Drivers/Modbus_ClientRequest.o 

C_DEPS += \
./Core/Drivers/Modbus_Client.d \
./Core/Drivers/Modbus_ClientParser.d \
./Core/Drivers/Modbus_ClientRequest.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Drivers/%.o Core/Drivers/%.su Core/Drivers/%.cyclo: ../Core/Drivers/%.c Core/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/admin/STM32CubeIDE/workspace_1.16.0/Modbus_TCP_Client/Core/Drivers" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/BSP/Components/dp83848 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Drivers

clean-Core-2f-Drivers:
	-$(RM) ./Core/Drivers/Modbus_Client.cyclo ./Core/Drivers/Modbus_Client.d ./Core/Drivers/Modbus_Client.o ./Core/Drivers/Modbus_Client.su ./Core/Drivers/Modbus_ClientParser.cyclo ./Core/Drivers/Modbus_ClientParser.d ./Core/Drivers/Modbus_ClientParser.o ./Core/Drivers/Modbus_ClientParser.su ./Core/Drivers/Modbus_ClientRequest.cyclo ./Core/Drivers/Modbus_ClientRequest.d ./Core/Drivers/Modbus_ClientRequest.o ./Core/Drivers/Modbus_ClientRequest.su

.PHONY: clean-Core-2f-Drivers

