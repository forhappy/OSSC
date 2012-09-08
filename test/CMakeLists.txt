INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	${OSSC_SOURCE_DIR}/src/util/
	)


MACRO(build_modules_tests testlist libs)
IF (BUILD_TESTS)
	FOREACH(t ${testlist})
		ADD_EXECUTABLE(${t} ${t}.c)
		SET_PROPERTY(TARGET ${t}
			APPEND
			PROPERTY LINK_FLAGS "-L ${OSSC_BINARY_DIR}/src/modules/ -static -lossmodules"
			)
		FOREACH(l ${libs})
			TARGET_LINK_LIBRARIES(${t} ${l})
		ENDFOREACH(l ${libs})
	ENDFOREACH(t ${tests})
ENDIF(BUILD_TESTS)
ENDMACRO(build_modules_tests testlist libs)

#ADD_SUBDIRECTORY(bucket)
set(tests
	abort-multipart-upload-request-t
	access_control_list-t
	bucket-t)

build_modules_tests("${tests}" "-lossmodules")
