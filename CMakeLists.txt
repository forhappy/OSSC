CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(CMakeDependentOption)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckVariableExists)
INCLUDE(CheckCCompilerFlag)

PROJECT(OSSC)

# Default build type
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: Debug Release"
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# CMAKE_BUILD_TYPE can only be one of Debug or Release
SET(BUILD_TYPE_OK FALSE)
FOREACH(build_type "Debug" "Release")
	IF (${CMAKE_BUILD_TYPE} STREQUAL ${build_type})
		SET(BUILD_TYPE_OK TRUE)
	ENDIF (${CMAKE_BUILD_TYPE} STREQUAL ${build_type})
ENDFOREACH(build_type)

IF (NOT BUILD_TYPE_OK)
	MESSAGE(FATAL_ERROR "CMAKE_BUILD_TYPE can only be one of \"Debug\" or \"Release\"")
ENDIF (NOT BUILD_TYPE_OK)

IF (${OSSC_BINARY_DIR} STREQUAL ${OSSC_SOURCE_DIR})
	MESSAGE(FATAL_ERROR "In-tree-compile is not prefered.")
ENDIF (${OSSC_BINARY_DIR} STREQUAL ${OSSC_SOURCE_DIR})


# Build types
SET(CMAKE_C_FLAGS "-std=c99 -Wall"
	CACHE
	STRING "Flags used by the compiler during all build types." FORCE)
SET(CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE
	STRING "Flags used by the compiler during debug builds." FORCE)
SET(CMAKE_C_FLAGS_RELEASE "-O2" CACHE
	STRING "Flags used by the compiler during release builds." FORCE)

IF(UNIX)
	ADD_DEFINITIONS("-D_GNU_SOURCE")
ENDIF(UNIX)

# Environment
CHECK_INCLUDE_FILES(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILES(alloca.h HAVE_ALLOCA_H)

IF (HAVE_ALLOCA_H)
	SET(HAVE_ALLOCA TRUE)
ENDIF (HAVE_ALLOCA_H)

CHECK_INCLUDE_FILES(stdbool.h HAVE_STDBOOL_H)
CHECK_INCLUDE_FILES(errno.h HAVE_ERRNO_H)
CHECK_INCLUDE_FILES(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES(thread_db.h HAVE_THREAD_DB_H)
CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)

SET(THREADDB_LIBS "")
IF (HAVE_LINUX_THREAD_DB)
	CHECK_INCLUDE_FILES(thread_db.h HAVE_THREAD_DB)
	IF (NOT HAVE_THREAD_DB)
		MESSAGE(STATUS "no thread_db.h")
		SET(HAVE_THREAD_DB FALSE)
	ELSE (NOT HAVE_THREAD_DB)
		CHECK_MACRO(thread_db.h TD_VERSION HAVE_TD_VERSION)
		SET(CMAKE_REQUIRED_LIBRARIES "-lthread_db")
		CHECK_THREADDB_FUNCTION_EXISTS(td_ta_new have_td_ta_new)
		IF (NOT have_td_ta_new)
			MESSAGE(STATUS "no td_ta_new in libthread_db")
			SET(HAVE_THREAD_DB FALSE)
		ELSE(NOT have_td_ta_new)
			SET(CMAKE_REQUIRED_LIBRARIES "-lthread_db")
			CHECK_THREADDB_FUNCTION_EXISTS(td_thr_tls_get_addr HAVE_TD_THR_TLS_GET_ADDR)
			LIST(APPEND THREADDB_LIBS	"-lthread_db")
		ENDIF (NOT have_td_ta_new)
	ENDIF (NOT HAVE_THREAD_DB)
ENDIF (HAVE_LINUX_THREAD_DB)


# Check environment
CONFIGURE_FILE(config.h.cmake.in config.h)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)

# vim:tabstop=4:shiftwidth=4

