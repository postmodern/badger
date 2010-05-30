cmake_minimum_required(VERSION 2.6)
project(BADGER C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(InstallPaths)
include(CheckIncludeFiles)

option(DEBUG "Enable debugging" off)
option(ENCODERS_XOR "Enable the XOR packet encoder" on)
option(ENCODERS_AES "Enable the AES packet encoder" on)

option(SERVICES_SYS "Enables the 'sys' service" on)
option(SERVICES_FS "Enables the 'fs' service" on)
option(SERVICES_FFI "Enables the 'ffi' service" off)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

if(UNIX)
	set(LINK_FLAGS -static)
	
	if(!DEBUG)
		set(LINK_FLAGS ${LINK_FLAGS} -s)
	endif(!DEBUG)
endif(UNIX)

set(BADGER_LIBS badger_core msgpack zmq uuid pthread stdc++)

if(SERVICES_SYS)
	set(BADGER_LIBS badger_sys ${BADGER_LIBS})
endif(SERVICES_SYS)

if(SERVICES_FS)
	set(BADGER_LIBS badger_fs ${BADGER_LIBS})
endif(SERVICES_FS)

if(SERVICES_FFI)
	check_include_file(ffi.h HAS_FFI)

	if(HAS_FFI)
		set(BADGER_LIBS ffi ${BADGER_LIBS})
	endif(HAS_FFI)
endif(SERVICES_FFI)

set(BADGER_SRC
	src/private/util.c src/slist.c src/data.c
	src/function.c src/service.c src/caller.c src/private/caller.c
	src/private/packet.c src/private/response.c
	src/private/server.c src/encoders.c src/server.c
	src/badger.c
)
if(ENCODERS_AES)
	set(BADGER_SRC src/private/aes.c ${BADGER_SRC})
endif(ENCODERS_AES)

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

add_library(badger_core STATIC ${BADGER_SRC})
set_target_properties(
	badger_core PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

if(SERVICES_SYS)
	add_library(badger_sys STATIC src/services/sys.c)
	set_target_properties(
		badger_sys PROPERTIES
		VERSION ${LIBRARY_VERSION}
		SOVERSION ${LIBRARY_SOVERSION}
	)
endif(SERVICES_SYS)

if(SERVICES_FS)
	add_library(badger_fs STATIC src/services/fs.c)
	set_target_properties(
		badger_fs PROPERTIES
		VERSION ${LIBRARY_VERSION}
		SOVERSION ${LIBRARY_SOVERSION}
	)
endif(SERVICES_FS)

add_executable(badger src/programs/badger.c)
set_target_properties(
	badger PROPERTIES
	LINK_FLAGS ${LINK_FLAGS}
)
target_link_libraries(badger ${BADGER_LIBS})

install(
	TARGETS badger_core
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)

if(SERVICES_SYS)
	install(
		TARGETS badger_sys
		ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	)
endif(SERVICES_SYS)

if(SERVICES_FS)
	install(
		TARGETS badger_fs
		ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	)
endif(SERVICES_FS)

install(
	TARGETS badger
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory(test)
