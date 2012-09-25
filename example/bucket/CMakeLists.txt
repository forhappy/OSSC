INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/util
	${OSSC_SOURCE_DIR}/include/ossc/modules/
	${OSSC_SOURCE_DIR}/src/util/
	${OSSC_SOURCE_DIR}/src/lib/
	${OSSC_SOURCE_DIR}/src/

	)

ADD_EXECUTABLE(create_bucket create_bucket.c)
TARGET_LINK_LIBRARIES(create_bucket osscore_static curl)

ADD_EXECUTABLE(set_bucket_acl set_bucket_acl.c)
TARGET_LINK_LIBRARIES(set_bucket_acl osscore_static curl)

ADD_EXECUTABLE(get_bucket_acl get_bucket_acl.c)
TARGET_LINK_LIBRARIES(get_bucket_acl osscore_static curl)

ADD_EXECUTABLE(list_buckets list_buckets.c)
TARGET_LINK_LIBRARIES(list_buckets osscore_static curl)

ADD_EXECUTABLE(delete_bucket delete_bucket.c)
TARGET_LINK_LIBRARIES(delete_bucket osscore_static curl)

ADD_EXECUTABLE(list_objects_with_bucket_name list_objects_with_bucket_name.c)
TARGET_LINK_LIBRARIES(list_objects_with_bucket_name osscore_static curl)

ADD_EXECUTABLE(list_objects_with_prefix list_objects_with_prefix.c)
TARGET_LINK_LIBRARIES(list_objects_with_prefix osscore_static curl)

ADD_EXECUTABLE(list_objects_with_request list_objects_with_request.c)
TARGET_LINK_LIBRARIES(list_objects_with_request osscore_static curl)

ADD_EXECUTABLE(is_bucket_exist is_bucket_exist.c)
TARGET_LINK_LIBRARIES(is_bucket_exist osscore_static curl)

