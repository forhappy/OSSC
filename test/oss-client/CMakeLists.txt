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

ADD_EXECUTABLE(client_abort_multipart_upload-t client_abort_multipart_upload-t.c)
TARGET_LINK_LIBRARIES(client_abort_multipart_upload-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_list_multipart_uploads-t client_list_multipart_uploads-t.c)
TARGET_LINK_LIBRARIES(client_list_multipart_uploads-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_delete_multiple_objectx-t client_delete_multiple_objectx-t.c)
TARGET_LINK_LIBRARIES(client_delete_multiple_objectx-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_list_parts-t client_list_parts-t.c)
TARGET_LINK_LIBRARIES(client_list_parts-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_post_object_group-t client_post_object_group-t.c)
TARGET_LINK_LIBRARIES(client_post_object_group-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_get_object_group-t client_get_object_group-t.c)
TARGET_LINK_LIBRARIES(client_get_object_group-t ossmodules ossutil ossauth ossc curl)
ADD_EXECUTABLE(client_get_object_group_index-t client_get_object_group_index-t.c)
TARGET_LINK_LIBRARIES(client_get_object_group_index-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_head_object_group-t client_head_object_group-t.c)
TARGET_LINK_LIBRARIES(client_head_object_group-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_delete_object_group-t client_delete_object_group-t.c)
TARGET_LINK_LIBRARIES(client_delete_object_group-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_set_bucket_acl-x-t client_set_bucket_acl-x-t.c)
TARGET_LINK_LIBRARIES(client_set_bucket_acl-x-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_bucket_operation-t client_bucket_operation-t.c)
TARGET_LINK_LIBRARIES(client_bucket_operation-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_object_operation-t client_object_operation-t.c)
TARGET_LINK_LIBRARIES(client_object_operation-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_multipart_upload_operation-t client_multipart_upload_operation-t.c)
TARGET_LINK_LIBRARIES(client_multipart_upload_operation-t ossmodules ossutil ossauth ossc curl)

ADD_EXECUTABLE(client_object_group_operation-t client_object_group_operation-t.c)
TARGET_LINK_LIBRARIES(client_object_group_operation-t ossmodules ossutil ossauth ossc curl)

