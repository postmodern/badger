cmake_minimum_required(VERSION 2.6)
project(RONIN_RAT C)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

option(USE_FFI "Enables the use of the Foreign Function Interface (FFI)" ON)

configure_file(config.h.cmake ${RONIN_RAT_SOURCE_DIR}/include/ronin/rat/config.h)

include_directories(${RONIN_RAT_SOURCE_DIR}/include)

add_definitions("-Wall")
add_library(
	ronin_rat STATIC src/func.c
)

add_executable(ronin_rat_server src/programs/server.c)
target_link_libraries(ronin_rat_server ronin_rat)

if (USE_FFI)
	if (UNIX)
		include(FindPkgConfig)
		pkg_check_modules(FFI REQUIRED libffi>=3.0.8)

		include_directories(${FFI_INCLUDE_DIRS})
		target_link_libraries(ronin_rat_server ${FFI_LIBRARIES})
	endif (UNIX)
endif (USE_FFI)

add_subdirectory(test)
