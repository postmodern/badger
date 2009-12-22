cmake_minimum_required(VERSION 2.6)
project(RONIN_RAT C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

option(USE_FFI "Enables the use of the Foreign Function Interface (FFI)" ON)

if(UNIX)
	find_library(BERT BERT-static)
endif(UNIX)

configure_file(config.h.cmake ${RONIN_RAT_SOURCE_DIR}/include/ronin/rat/config.h)

include_directories(${RONIN_RAT_SOURCE_DIR}/include)

if(UNIX)
	add_definitions("-Wall")
endif(UNIX)

add_library(
	ronin_rat STATIC
	src/rat.c src/slist.c src/func.c src/service.c
)
set_target_properties(
	ronin_rat PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(ronin_rat_server src/programs/server.c)
target_link_libraries(ronin_rat_server BERT-static ronin_rat)

if(USE_FFI)
	add_library(
		ronin_rat_ffi STATIC src/ffi/func.c
	)

	if(UNIX)
		find_library(FFI ffi)

		include_directories(${FFI_INCLUDE_DIRS})
		target_link_libraries(ronin_rat_server ${FFI_LIBRARIES})
	endif(UNIX)

	target_link_libraries(ronin_rat_server ronin_rat_ffi)
endif(USE_FFI)

install(
	TARGETS ronin_rat
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS ronin_rat_server
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

if(USE_FFI)
	install(
		TARGETS ronin_rat_ffi
		ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	)
endif(USE_FFI)

add_subdirectory(test)
