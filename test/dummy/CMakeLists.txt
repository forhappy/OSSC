INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	${OSSC_SOURCE_DIR}/src/
	)

ADD_EXECUTABLE(client_put_object-t client_put_object-t.c)
TARGET_LINK_LIBRARIES(client_put_object-t ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(client_get_object-t client_get_object-t.c)
TARGET_LINK_LIBRARIES(client_get_object-t ossmodules ossutil ossauth osscore curl)

ADD_EXECUTABLE(client_extra_put_object-t client_extra_put_object-t.c)
TARGET_LINK_LIBRARIES(client_extra_put_object-t ossmodules ossutil ossauth osscore curl ossextra pthread)

ADD_EXECUTABLE(compression-t compression-t.c)
TARGET_LINK_LIBRARIES(compression-t ossutil ossauth)

ADD_EXECUTABLE(decompression-t decompression-t.c)
TARGET_LINK_LIBRARIES(decompression-t ossutil ossauth)

