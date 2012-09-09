INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	${OSSC_SOURCE_DIR}/include/ossc/modules/
	${OSSC_SOURCE_DIR}/src/util/
	)


MACRO(build_modules_tests testlist libs)
IF (BUILD_TESTS)
	FOREACH(t ${testlist})
		ADD_EXECUTABLE(${t} ${t}.c)
		SET_PROPERTY(TARGET ${t}
			APPEND
			PROPERTY LINK_FLAGS "-L ${OSSC_BINARY_DIR}/src/modules/ -static -lossmodules -L${OSSC_BINARY_DIR}/src/util/ -static -lossutil -L${OSSC_BINARY_DIR}/src/lib/ -static -lossauth -L${OSSC_BINARY_DIR}/src/ -lossc"
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
	access-control-list-t
	bucket-t
	complete-multipart-upload-request-t
	complete-multipart-upload-result-t
	copy-object-request-t
	copy-object-result-t
	generate-presigned-url-request-t
	get-object-request-t
	grant-t
	initiate-multipart-upload-request-t
	initiate-multipart-upload-result-t
	list-multipart-uploads-request-t
	list-objects-request-t
	list-parts-request-t
	multipart-upload-t
	multipart-upload-listing-t
	object-t
	object-listing-t
	object-metadata-t
	object-summary-t
	part-etag-t
	part-listing-t
	part-summary-t
	put-object-result-t
	response-header-overrides-t
	upload-part-request-t
	upload-part-result-t
	generate-authentication-t
	time-t
	)

build_modules_tests("${tests}" "-lossmodules -lossutil -lossauth -lossc")
ADD_EXECUTABLE(client-t client-t.c)
TARGET_LINK_LIBRARIES(client-t ossmodules ossutil ossauth ossc curl)
