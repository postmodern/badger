cmake_minimum_required(VERSION 2.6)
project(BADGER C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

if(UNIX)
	include(FindPkgConfig)
endif(UNIX)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

find_library(MSGPACK msgpack)
find_library(ZMQ zmq)

option(DEBUG "Enable debugging" on)
option(XOR_ENCODER "Enable the XOR packet encoder" on)

if(DEBUG)
	if(UNIX)
		add_definitions("-ggdb")
	endif(UNIX)
endif(DEBUG)

include(CheckIncludeFiles)
configure_file(config.h.cmake ${BADGER_SOURCE_DIR}/include/badger/config.h)

include_directories(${BADGER_SOURCE_DIR}/include)
include_directories(${BADGER_SOURCE_DIR}/src)

if(UNIX)
	add_definitions("-Wall")
endif(UNIX)

add_library(
	badger-static STATIC
	src/private/aes.c
	src/util.c src/slist.c src/mempool.c src/func.c src/service.c
	src/encoders.c src/server.c src/private/server.c
	src/badger.c
)
set_target_properties(
	badger-static PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(badger src/programs/server.c)
target_link_libraries(badger ${MSGPACK} ${ZMQ} badger-static)

install(
	TARGETS badger-static
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS badger
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory(test)
