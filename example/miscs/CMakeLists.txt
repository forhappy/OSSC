INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/util
	${OSSC_SOURCE_DIR}/include/ossc/modules/
	${OSSC_SOURCE_DIR}/src/util/
	${OSSC_SOURCE_DIR}/src/lib/
	${OSSC_SOURCE_DIR}/src/
	)

ADD_EXECUTABLE(generate_presigned_url_with_expiration generate_presigned_url_with_expiration.c)
TARGET_LINK_LIBRARIES(generate_presigned_url_with_expiration osscore_static curl)

ADD_EXECUTABLE(generate_presigned_url_with_method generate_presigned_url_with_method.c)
TARGET_LINK_LIBRARIES(generate_presigned_url_with_method osscore_static curl)

ADD_EXECUTABLE(sync_upload sync_upload.c)
TARGET_LINK_LIBRARIES(sync_upload ossextra_static osscore_static curl)

ADD_EXECUTABLE(sync_download sync_download.c)
TARGET_LINK_LIBRARIES(sync_download ossextra_static osscore_static curl)

