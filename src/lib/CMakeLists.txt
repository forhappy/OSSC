SET(SRC
	base64.c
	oss_generate_authentication.c
	hmac-sha1.c
	md5.c
	memxor.c
	sha1.c
	)

ADD_LIBRARY(ossauth STATIC ${SRC})

set_modules_properties(ossauth)
# ADD_DEPENDENCIES(modules ossutil)
