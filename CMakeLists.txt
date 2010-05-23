cmake_minimum_required(VERSION 2.6)
project(BADGER C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

if(UNIX)
	include(FindPkgConfig)
endif(UNIX)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

if(UNIX)
	find_library(MSGPACK msgpack)
	find_library(ZMQ zmq)
endif(UNIX)

configure_file(config.h.cmake ${BADGER_SOURCE_DIR}/include/badger/config.h)

include_directories(${BADGER_SOURCE_DIR}/include)

if(UNIX)
	add_definitions("-Wall")
endif(UNIX)

add_library(
	badger STATIC
	src/badger.c src/slist.c src/mempool.c src/func.c
	src/service.c src/server.c
)
set_target_properties(
	badger PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(badger_server src/programs/server.c)
target_link_libraries(badger_server msgpack zmq badger)

install(
	TARGETS badger
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS badger_server
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory(test)
