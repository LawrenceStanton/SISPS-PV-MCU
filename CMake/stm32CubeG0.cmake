# Configures the STM32CubeG0 HAL and FreeRTOS libraries from the perspective of ${CMAKE_SOURCE_DIR}.
# It is expected the https://github.com/STMicroelectronics/STM32CubeG0 is cloned to ${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0.

# Generates static libraries:
# - SISPS::HAL
# - SISPS::FreeRTOS

set(STM32CUBEG0_HAL_INCLUDE_DIRECTORIES
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Include
	${CMAKE_SOURCE_DIR}/Inc/HAL
)
set(STM32CUBEG0_FREERTOS_INCLUDE_DIRECTORIES
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/include
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Core/Include
)

file(GLOB STM32CUBEG0_HAL_SOURCES
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/*_hal*.c
)

file(GLOB STM32CUBEG0_FREERTOS_SOURCES
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/*.c
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
	${CMAKE_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
)

# Workaround - Broken template files should not be compiled.
list(FILTER STM32CUBEG0_HAL_SOURCES EXCLUDE REGEX ".*_template.c")


# HAL Library
add_library(STM32CubeG0_HAL STATIC
${STM32CUBEG0_HAL_SOURCES}
)

set(STM32CUBEG0_HAL_COMPILE_DEFINITIONS
	USE_HAL_DRIVER
	STM32G070xx
)

target_compile_definitions(STM32CubeG0_HAL PUBLIC
	${STM32CUBEG0_HAL_COMPILE_DEFINITIONS}
)

target_include_directories(STM32CubeG0_HAL SYSTEM
	PUBLIC ${STM32CUBEG0_HAL_INCLUDE_DIRECTORIES}
)

add_library(SISPS::HAL ALIAS STM32CubeG0_HAL)

# FreeRTOS Library
add_library(STM32CubeG0_FREERTOS STATIC
	${STM32CUBEG0_FREERTOS_SOURCES}
)

target_compile_definitions(STM32CubeG0_FREERTOS PUBLIC
	${HAL_COMPILE_DEFINITIONS}
)

target_include_directories(STM32CubeG0_FREERTOS SYSTEM
	PUBLIC ${STM32CUBEG0_FREERTOS_INCLUDE_DIRECTORIES}
	PRIVATE ${APPLICATION_INCLUDE_DIRECTORIES}
)

add_library(SISPS::FreeRTOS ALIAS STM32CubeG0_FREERTOS)