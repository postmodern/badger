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

include(CheckIncludeFiles)
configure_file(config.h.cmake ${BADGER_SOURCE_DIR}/include/badger/config.h)

include_directories(${BADGER_SOURCE_DIR}/include)
include_directories(${BADGER_SOURCE_DIR}/src)

if(UNIX)
	add_definitions("-Wall")
endif(UNIX)

add_library(
	badger STATIC
	src/util.c src/slist.c src/mempool.c src/func.c
	src/service.c src/server.c src/private/server.c
	src/badger.c
)
set_target_properties(
	badger PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(badger_server src/programs/server.c)
if(UNIX)
	set_target_properties(
		badger_server PROPERTIES
		LINK_FLAGS "-static"
	)
endif(UNIX)

target_link_libraries(badger_server ${MSGPACK} ${ZMQ} badger)

install(
	TARGETS badger
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS badger_server
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory(test)
