project(Lib)

set(LIBRARY ${PROJECT_NAME})

add_subdirectory(HDC1080)
add_subdirectory(TMP116)
add_subdirectory(SM72445)

add_library(${LIBRARY} INTERFACE)

target_link_libraries(${LIBRARY} INTERFACE
	SISPS::HDC1080
	SISPS::TMP116
	SISPS::SM72445
)

add_library(SISPS::${LIBRARY} ALIAS ${LIBRARY})