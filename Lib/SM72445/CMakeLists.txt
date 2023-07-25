project(SM72445_Lib)

enable_language(CXX)

set(LIBRARY ${PROJECT_NAME})

add_library(${LIBRARY} STATIC
	Src/SM72445_I2C.cpp
)

target_include_directories(${LIBRARY} PUBLIC
	Inc
)

target_link_libraries(${LIBRARY} PUBLIC
	SM72445::SM72445
	SISPS::HAL
)

add_library(SISPS::SM72445 ALIAS ${LIBRARY})