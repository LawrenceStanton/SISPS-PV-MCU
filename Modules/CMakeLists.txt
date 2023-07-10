add_subdirectory(etl SYSTEM)	# etl::etl

set(HDC1080_CODE_COVERAGE OFF CACHE BOOL "Enable coverage reporting for HDC1080")
add_subdirectory(HDC1080) # HDC1080::HDC1080