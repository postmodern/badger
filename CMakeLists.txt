cmake_minimum_required(VERSION 2.6)
project(BADGER C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

if(UNIX)
	include(FindPkgConfig)
endif(UNIX)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

option(USE_FFI "Enables the use of the Foreign Function Interface (FFI)" ON)

if(UNIX)
	find_library(BERT BERT-static)
endif(UNIX)

configure_file(config.h.cmake ${BADGER_SOURCE_DIR}/include/badger/config.h)

include_directories(${BADGER_SOURCE_DIR}/include)

if(UNIX)
	add_definitions("-Wall")
endif(UNIX)

add_library(
	badger STATIC
	src/badger.c src/slist.c src/func.c src/service.c
)
set_target_properties(
	badger PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(badger_server src/programs/server.c)
target_link_libraries(badger_server BERT-static badger)

if(USE_FFI)
	add_library(
		badger_ffi STATIC src/ffi/func.c
	)

	if(UNIX)
		pkg_search_module(FFI libffi)

		include_directories(${FFI_INCLUDE_DIRS})
		target_link_libraries(badger_server ${FFI_LIBRARIES})
	endif(UNIX)

	target_link_libraries(badger_server badger_ffi)
endif(USE_FFI)

install(
	TARGETS badger
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS badger_server
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

if(USE_FFI)
	install(
		TARGETS badger_ffi
		ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	)
endif(USE_FFI)

add_subdirectory(test)
