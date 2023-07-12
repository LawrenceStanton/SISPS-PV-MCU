project(TMP116_Lib)

enable_language(CXX)

set(LIBRARY ${PROJECT_NAME})

add_library(${LIBRARY} STATIC
	Src/TMP116_I2C.cpp
)

target_include_directories(${LIBRARY} PUBLIC
	Inc
)

target_link_libraries(${LIBRARY} PUBLIC
	TMP116::TMP116
	SISPS::HAL
)

add_library(SISPS::TMP116 ALIAS ${LIBRARY})
