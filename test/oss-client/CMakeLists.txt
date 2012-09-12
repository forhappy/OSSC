INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	${OSSC_SOURCE_DIR}/include/ossc/modules/
	${OSSC_SOURCE_DIR}/src/util/
	${OSSC_SOURCE_DIR}/lib/
	)

ADD_EXECUTABLE(client_get_bucket_acl-t client_get_bucket_acl-t.c)
TARGET_LINK_LIBRARIES(client_get_bucket_acl-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_set_bucket_acl-t client_set_bucket_acl-t.c)
TARGET_LINK_LIBRARIES(client_set_bucket_acl-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_list_buckets-t client_list_buckets-t.c)
TARGET_LINK_LIBRARIES(client_list_buckets-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_create_bucket-t client_create_bucket-t.c)
TARGET_LINK_LIBRARIES(client_create_bucket-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_delete_bucket-t client_delete_bucket-t.c)
TARGET_LINK_LIBRARIES(client_delete_bucket-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_get_object-t client_get_object-t.c)
TARGET_LINK_LIBRARIES(client_get_object-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_list_object-t client_list_object-t.c)
TARGET_LINK_LIBRARIES(client_list_object-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_delete_object-t client_delete_object-t.c)
TARGET_LINK_LIBRARIES(client_delete_object-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_copy_object-t client_copy_object-t.c)
TARGET_LINK_LIBRARIES(client_copy_object-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_put_object-t client_put_object-t.c)
TARGET_LINK_LIBRARIES(client_put_object-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_initiate_multipart_upload-t client_initiate_multipart_upload-t.c)
TARGET_LINK_LIBRARIES(client_initiate_multipart_upload-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_upload_part-t client_upload_part-t.c)
TARGET_LINK_LIBRARIES(client_upload_part-t ossmodules ossutil ossauth ossc curl)


ADD_EXECUTABLE(client_complete_multipart_upload-t client_complete_multipart_upload-t.c)
TARGET_LINK_LIBRARIES(client_complete_multipart_upload-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_delete_multiple_objectx-t client_delete_multiple_objectx-t.c)
TARGET_LINK_LIBRARIES(client_delete_multiple_objectx-t ossmodules ossutil ossauth ossc curl)

