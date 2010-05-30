cmake_minimum_required(VERSION 2.6)
project(BADGER C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)
include(CheckIncludeFiles)

option(DEBUG "Enable debugging" off)
option(XOR_ENCODER "Enable the XOR packet encoder" on)
option(AES_ENCODER "Enable the AES packet encoder" on)

option(SYS_SERVICE "Enables the 'sys' service" on)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

if(UNIX)
	set(LINK_FLAGS -static)
	
	if(!DEBUG)
		set(LINK_FLAGS ${LINK_FLAGS} -s)
	endif(!DEBUG)
endif(UNIX)

set(BADGER_LIBS msgpack zmq uuid pthread stdc++)
if(SYS_SERVICE)
	set(BADGER_LIBS badger_sys ${BADGER_LIBS})
endif(SYS_SERVICE)

set(BADGER_SRC	src/private/util.c src/slist.c
		src/func.c src/service.c src/caller.c src/private/caller.c
		src/private/packet.c src/private/response.c
		src/private/server.c src/encoders.c src/server.c
		src/badger.c
)
if(AES_ENCODER)
	set(BADGER_SRC src/private/aes.c ${BADGER_SRC})
endif(AES_ENCODER)

configure_file(config.h.cmake ${BADGER_SOURCE_DIR}/include/badger/config.h)

include_directories(${BADGER_SOURCE_DIR}/include)
include_directories(${BADGER_SOURCE_DIR}/src)

if(UNIX)
	add_definitions("-Wall")

	if(DEBUG)
		add_definitions("-g")
	else(DEBUG)
		add_definitions("-O3")
	endif(DEBUG)
endif(UNIX)

add_library(badger-static STATIC ${BADGER_SRC})
set_target_properties(
	badger-static PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_library(badger_sys STATIC src/services/sys.c)
set_target_properties(
	badger_sys PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(badger src/programs/server.c)
set_target_properties(
	badger PROPERTIES
	LINK_FLAGS ${LINK_FLAGS}
)
target_link_libraries(badger badger-static ${BADGER_LIBS})

install(
	TARGETS badger-static
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS badger
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory(test)
