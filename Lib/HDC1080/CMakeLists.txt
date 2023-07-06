project(HDC1080_Lib)

enable_language(CXX)

set(LIBRARY ${PROJECT_NAME})

add_library(${LIBRARY} STATIC
	Src/HDC1080_I2C.cpp
)

target_include_directories(${LIBRARY} PUBLIC
	Inc
)

target_link_libraries(${LIBRARY} PUBLIC
	HDC1080::HDC1080
	SISPS::HAL
)

add_library(SISPS::HDC1080 ALIAS ${LIBRARY})