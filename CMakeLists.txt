CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(CheckIncludeFiles)

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
	CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)
ENDIF(UNIX)

# Environment
CHECK_INCLUDE_FILES(stdbool.h HAVE_STDBOOL_H)
CHECK_INCLUDE_FILES(errno.h HAVE_ERRNO_H)

# Check environment
CONFIGURE_FILE(config.h.cmake.in config.h)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)

# vim:tabstop=4:shiftwidth=4

