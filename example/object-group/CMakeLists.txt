INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/util
	${OSSC_SOURCE_DIR}/include/ossc/modules/
	${OSSC_SOURCE_DIR}/src/util/
	${OSSC_SOURCE_DIR}/src/lib/
	${OSSC_SOURCE_DIR}/src/

	)

ADD_EXECUTABLE(post_object_group post_object_group.c)
TARGET_LINK_LIBRARIES(post_object_group ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(get_object_group_to_file get_object_group_to_file.c)
TARGET_LINK_LIBRARIES(get_object_group_to_file ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(get_object_group_to_buffer get_object_group_to_buffer.c)
TARGET_LINK_LIBRARIES(get_object_group_to_buffer ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(get_object_group_index get_object_group_index.c)
TARGET_LINK_LIBRARIES(get_object_group_index ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(head_object_group head_object_group.c)
TARGET_LINK_LIBRARIES(head_object_group ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(delete_object_group delete_object_group.c)
TARGET_LINK_LIBRARIES(delete_object_group ossmodules ossutil ossauth osscore curl)

