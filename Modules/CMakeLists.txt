set(GIT_DIR_LOOKUP_POLICY ALLOW_LOOKING_ABOVE_CMAKE_SOURCE_DIR)
add_subdirectory(etl SYSTEM)	# etl::etl

option(SISPS_ENABLE_CODE_COVERAGE "Enable code coverage reporting" OFF)
if(SISPS_ENABLE_CODE_COVERAGE)
	set(HDC1080_CODE_COVERAGE ON CACHE BOOL "Enable coverage reporting for HDC1080")
	set(TMP116_CODE_COVERAGE ON CACHE BOOL "Enable coverage reporting for TMP116")
	set(SM72445_CODE_COVERAGE ON CACHE BOOL "Enable coverage reporting for SM72445")
endif()

add_subdirectory(HDC1080) # HDC1080::HDC1080
add_subdirectory(TMP116) # TMP116::TMP116
add_subdirectory(SM72445) # SM72445::SM72445