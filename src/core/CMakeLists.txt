INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/
	${OSSC_SOURCE_DIR}/src
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	)

SET(SRC
	client_bucket_operation.c
	client_generate_url.c
	client_multipart_upload_operation.c
	client_object_group_operation.c
	client_object_operation.c
	oss_client.c
	oss_helper.c
	oss_curl_callback.c
	)

ADD_LIBRARY(osscore STATIC ${SRC})
ADD_DEPENDENCIES(osscore ossmodules ossutil ossauth)
TARGET_LINK_LIBRARIES(osscore ossmodules ossutil ossauth)
