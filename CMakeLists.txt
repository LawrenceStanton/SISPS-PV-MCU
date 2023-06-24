cmake_minimum_required(VERSION 3.24)

project(SISPS-PV)

set(TARGET_EXECUTABLE ${CMAKE_PROJECT_NAME})
set(TARGET_TEST_EXECUTABLE ${CMAKE_PROJECT_NAME}-Test)
set(LOCAL_TEST_EXECUTABLE ${CMAKE_PROJECT_NAME}-LocalTest)

enable_language(C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(CMake/mcu.cmake)

set(APPLICATION_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(STM32CUBEG0_INCLUDE_DIRECTORIES
	# HAL Driver Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Include
	# FreeRTOS Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/include
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0
)

file(GLOB_RECURSE APPLICATION_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp
)

file(GLOB STM32CUBEG0_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/*_hal*.c

	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/*.c
	
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
)

# Workaround - Broken template files should not be compiled.
list(FILTER STM32CUBEG0_SOURCES EXCLUDE REGEX ".*_template.c")

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/CubeMX/startup_stm32g070xx.s)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/CubeMX/STM32G070CBTx_FLASH.ld)

add_subdirectory(Modules/etl)

add_executable(${TARGET_EXECUTABLE} 
	${APPLICATION_SOURCES} 
	${STM32CUBEG0_SOURCES}
	${STARTUP_SCRIPT}
)

target_compile_definitions(${TARGET_EXECUTABLE} PRIVATE
	USE_HAL_DRIVER
	STM32G070xx
)

target_include_directories(${TARGET_EXECUTABLE} PRIVATE
	${APPLICATION_INCLUDE_DIRECTORIES}
)
target_include_directories(${TARGET_EXECUTABLE} SYSTEM PRIVATE
	${STM32CUBEG0_INCLUDE_DIRECTORIES}
)

target_compile_options(${TARGET_EXECUTABLE} PRIVATE
	${CPU_PARAMETERS}
	-Wall
	-Wpedantic
	-Og
	-g3
	-ggdb
)

target_link_libraries(
	${TARGET_EXECUTABLE}
	etl
)

target_link_options(${TARGET_EXECUTABLE} PRIVATE
	-T${LINKER_SCRIPT}
	${CPU_PARAMETERS}
	-Wl,--start-group
	-lc
	-lm
	-lstdc++
	-lsupc++
	-Wl,--end-group
	-Wl,--print-memory-usage
)

add_custom_command(TARGET ${TARGET_EXECUTABLE} POST_BUILD
	COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_EXECUTABLE}>
)

add_custom_command(TARGET ${TARGET_EXECUTABLE} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_EXECUTABLE}> ${TARGET_EXECUTABLE}.hex
	COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_EXECUTABLE}> ${TARGET_EXECUTABLE}.bin
)

add_custom_command(TARGET ${TARGET_EXECUTABLE} POST_BUILD
	COMMAND ${CMAKE_OBJDUMP} -D -C $<TARGET_FILE:${TARGET_EXECUTABLE}> > ${TARGET_EXECUTABLE}.s)
