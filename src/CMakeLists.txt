INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/
	${OSSC_SOURCE_DIR}/src
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	)

MACRO(set_modules_properties target)
SET_PROPERTY(TARGET ${target}
	PROPERTY COMPILE_FLAGS "${MODULES_COMPILE_FLAGS}")
SET_PROPERTY(TARGET ${target}
	PROPERTY LINK_FLAGS "${MODULES_LINK_FLAGS}")
ENDMACRO(set_modules_properties target)

SET(SRC
	oss_client.c)
ADD_LIBRARY(ossc STATIC ${SRC})
ADD_DEPENDENCIES(ossc ossmodules ossutil ossauth)
TARGET_LINK_LIBRARIES(ossc ossmodules ossutil ossauth)

ADD_SUBDIRECTORY(modules)
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(lib)

