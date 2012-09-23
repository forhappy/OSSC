INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	${OSSC_SOURCE_DIR}/src/
	)


ADD_EXECUTABLE(abort_multipart_upload abort_multipart_upload.c)
TARGET_LINK_LIBRARIES(abort_multipart_upload ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(complete_multipart_upload complete_multipart_upload.c)
TARGET_LINK_LIBRARIES(complete_multipart_upload ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(initiate_multipart_upload initiate_multipart_upload.c)
TARGET_LINK_LIBRARIES(initiate_multipart_upload ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(list_multipart_upload list_multipart_upload.c)
TARGET_LINK_LIBRARIES(list_multipart_upload ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(list_parts list_parts.c)
TARGET_LINK_LIBRARIES(list_parts ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(upload_part upload_part.c)
TARGET_LINK_LIBRARIES(upload_part ossmodules ossutil ossauth osscore curl)


