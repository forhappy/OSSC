INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	${OSSC_SOURCE_DIR}/include/ossc/modules/
	${OSSC_SOURCE_DIR}/src/util/
	)

ADD_EXECUTABLE(client_list_buckets-t client_list_buckets-t.c)
TARGET_LINK_LIBRARIES(client_list_buckets-t ossmodules ossutil ossauth ossc curl)
