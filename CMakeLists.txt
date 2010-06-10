cmake_minimum_required(VERSION 2.6)
project(BADGER C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(InstallPaths)
include(CheckIncludeFiles)
if(UNIX)
	include(FindPkgConfig)
endif(UNIX)

option(DEBUG "Enable debugging" off)
option(PACK "Enable UPX packing of the badger executable" off)

option(ENCODERS_XOR "Enable the XOR packet encoder" on)
option(ENCODERS_AES "Enable the AES packet encoder" on)

option(SERVICES_SYS "Enables the 'sys' service" on)
option(SERVICES_FS "Enables the 'fs' service" on)
option(SERVICES_FFI "Enables the 'ffi' service" off)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

if(UNIX)
	pkg_search_module(ZMQ REQUIRED libzmq)

	set(LINK_FLAGS -static)
	
	if(!DEBUG)
		set(LINK_FLAGS ${LINK_FLAGS} -s)

		if(PACK)
			include(FindSelfPackers)
		endif(PACK)
	endif(!DEBUG)
endif(UNIX)

set(BADGER_LIBS badger_core msgpack zmq uuid)
if(UNIX)
	set(BADGER_LIBS ${BADGER_LIBS} pthread stdc++)
endif(UNIX)

if(SERVICES_SYS)
	set(BADGER_LIBS badger_sys ${BADGER_LIBS})
endif(SERVICES_SYS)

if(SERVICES_FS)
	set(BADGER_LIBS badger_fs ${BADGER_LIBS})
endif(SERVICES_FS)

if(SERVICES_FFI)
	if(UNIX)
		pkg_search_module(FFI REQUIRED libffi)

		set(BADGER_LIBS dl ${BADGER_LIBS})
	endif(UNIX)

	include_directories(${FFI_INCLUDEDIR})
	set(BADGER_LIBS badger_ffi ffi ${BADGER_LIBS})
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

if(SERVICES_FFI)
	add_library(
		badger_ffi STATIC
		src/private/ffi/types.c
		src/private/ffi/value.c
		src/private/ffi/function.c
		src/private/ffi/library.c
		src/services/ffi.c
	)
	set_target_properties(
		badger_ffi PROPERTIES
		VERSION ${LIBRARY_VERSION}
		SOVERSION ${LIBRARY_SOVERSION}
	)
endif(SERVICES_FFI)

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

if(FFI_SERVICES)
	install(
		TARGETS badger_ffi
		ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	)
endif(FFI_SERVICES)

install(
	TARGETS badger
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory(test)
