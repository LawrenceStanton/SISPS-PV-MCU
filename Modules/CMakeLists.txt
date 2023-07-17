add_subdirectory(etl SYSTEM)	# etl::etl

if(SISPS_ENABLE_CODE_COVERAGE)
	set(HDC1080_CODE_COVERAGE ON CACHE BOOL "Enable coverage reporting for HDC1080")
	set(TMP116_CODE_COVERAGE ON CACHE BOOL "Enable coverage reporting for TMP116")
endif()

add_subdirectory(HDC1080) # HDC1080::HDC1080
add_subdirectory(TMP116) # TMP116::TMP116
