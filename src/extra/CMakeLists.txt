INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/
	${OSSC_SOURCE_DIR}/src
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	)

SET(SRC
	oss_extra.c
	workqueue.c
	oss_sync.c
	)

ADD_LIBRARY(ossextra STATIC ${SRC})
ADD_DEPENDENCIES(ossextra ossmodules ossutil ossauth osscore)
TARGET_LINK_LIBRARIES(ossextra ossmodules ossutil ossauth osscore)

ADD_EXECUTABLE(oss_sync oss_sync.c)
TARGET_LINK_LIBRARIES(oss_sync ossmodules ossutil ossauth osscore curl)


