INCLUDE_DIRECTORIES(
	${OSSC_BINARY_DIR}
	${OSSC_SOURCE_DIR}/src
	${OSSC_SOURCE_DIR}/include/
	${OSSC_SOURCE_DIR}/include/ossc/
	)
	
SET(SRC
	base64.c
	hmac-sha1.c
	md5.c
	memxor.c
	sha1.c
	)

ADD_LIBRARY(ossauth STATIC ${SRC})

set_modules_properties(ossauth)
