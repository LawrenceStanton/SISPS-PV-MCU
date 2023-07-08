cmake_minimum_required(VERSION 3.24)

project(SISPS-PV)

set(TARGET_EXECUTABLE ${CMAKE_PROJECT_NAME})
set(LOCAL_TEST_EXECUTABLE ${CMAKE_PROJECT_NAME}-LocalTest)

enable_language(C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(CMake/mcu.cmake)

if(CMAKE_CROSSCOMPILING)
	# Cross Compilation Required Options
	set(CROSSCOMPILE_CFLAGS
		-fdata-sections
		-ffunction-sections
		--specs=nano.specs
	)
	set(CROSSCOMPILE_CXXFLAGS
		-fno-rtti
		-fno-exceptions
		-fno-threadsafe-statics
	)

	set(CROSSCOMPILE_OPTIMISATION_FLAGS_DEBUG
		-Og
		-g3
		-ggdb
	)

	set(CROSSCOMPILE_OPTIMISATION_FLAGS_RELEASE
		-O3
		-g0
	)

	add_compile_options(
		${CPU_PARAMETERS}
		${CROSSCOMPILE_CFLAGS}
		"$<$<COMPILE_LANGUAGE:CXX>:${CROSSCOMPILE_CXXFLAGS}>"
		"$<$<CONFIG:Debug>:${CROSSCOMPILE_OPTIMISATION_FLAGS_DEBUG}>"
		"$<$<CONFIG:Release>:${CROSSCOMPILE_OPTIMISATION_FLAGS_RELEASE}>"
	)

	add_link_options(
		-Wl,--gc-section
	)

endif()

set(APPLICATION_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Lib/Inc
)

set(STM32CUBEG0_HAL_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Inc/HAL
)
set(STM32CUBEG0_FREERTOS_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/include
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/CMSIS/Core/Include
)

file(GLOB_RECURSE APPLICATION_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/app_freertos.c
)

list(FILTER APPLICATION_SOURCES EXCLUDE REGEX "main.c$")	# main.cpp is used instead of main.c

file(GLOB STM32CUBEG0_HAL_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/*_hal*.c
)

file(GLOB_RECURSE TEST_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp
)

# Workaround - Broken template files should not be compiled.
list(FILTER STM32CUBEG0_HAL_SOURCES EXCLUDE REGEX ".*_template.c")

file(GLOB STM32CUBEG0_FREERTOS_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/STM32CubeG0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
)

include(CMake/scripts.cmake)

if(NOT CMAKE_CROSSCOMPILING)
	# Google Test
	include(FetchContent)
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest
		GIT_TAG origin/main
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	enable_testing()
endif()

add_subdirectory(Modules)

if(CMAKE_CROSSCOMPILING)

	include(CMake/STM32CubeG0.cmake)

	# HAL Library
	add_library(STM32CubeG0_HAL STATIC
		${STM32CUBEG0_HAL_SOURCES}
	)

	target_compile_definitions(STM32CubeG0_HAL PUBLIC
		${HAL_COMPILE_DEFINITIONS}
	)

	target_include_directories(STM32CubeG0_HAL SYSTEM
		PUBLIC ${STM32CUBEG0_HAL_INCLUDE_DIRECTORIES}
	)

	target_compile_options(STM32CubeG0_HAL PRIVATE
		# ${CPU_PARAMETERS}
		# -Wall
		# -Wpedantic
		# -Og
		# -g3
		# -ggdb
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

	target_compile_options(STM32CubeG0_FREERTOS PRIVATE
		# ${CPU_PARAMETERS}
		# -Wall
		# -Wpedantic
		# -Og
		# -g3
		# -ggdb
	)

	add_library(SISPS::FreeRTOS ALIAS STM32CubeG0_FREERTOS)

	# Local Library
	add_subdirectory(Lib)

	# TARGET EXECUTABLE

	add_executable(${TARGET_EXECUTABLE} 
		${APPLICATION_SOURCES}
		${STARTUP_SCRIPT}
	)

	target_compile_definitions(${TARGET_EXECUTABLE} PRIVATE
		${HAL_COMPILE_DEFINITIONS}
	)

	target_include_directories(${TARGET_EXECUTABLE} PRIVATE
		${APPLICATION_INCLUDE_DIRECTORIES}
	)
	target_include_directories(${TARGET_EXECUTABLE} SYSTEM PRIVATE
		# ${STM32CUBEG0_HAL_INCLUDE_DIRECTORIES}
		# ${STM32CUBEG0_FREERTOS_INCLUDE_DIRECTORIES}
	)

	target_compile_options(${TARGET_EXECUTABLE} PRIVATE
		# ${CPU_PARAMETERS}
		# -Wall
		# -Wpedantic
		# -O0
		# -g3
		# -ggdb
	)

	target_link_libraries(
		${TARGET_EXECUTABLE} PRIVATE
		STM32CubeG0_HAL
		STM32CubeG0_FREERTOS
		SISPS::Lib
		etl
	)

	target_link_options(${TARGET_EXECUTABLE} PRIVATE
		-T${LINKER_SCRIPT}
		${CPU_PARAMETERS}
		-Wl,-Map=${TARGET_EXECUTABLE}.map
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
		COMMENT "Generating ${TARGET_EXECUTABLE}.hex and ${TARGET_EXECUTABLE}.bin"
		COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_EXECUTABLE}> ${TARGET_EXECUTABLE}.hex
		COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_EXECUTABLE}> ${TARGET_EXECUTABLE}.bin
	)

	add_custom_command(TARGET ${TARGET_EXECUTABLE} POST_BUILD
		COMMENT "Generating ${TARGET_EXECUTABLE} Disassembly"
		COMMAND ${CMAKE_OBJDUMP} -D -C $<TARGET_FILE:${TARGET_EXECUTABLE}> > ${TARGET_EXECUTABLE}.s
	)

	add_subdirectory(Test)

endif()
