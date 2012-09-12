SET(SRC
	oss_auth.c
	oss_map.c
	oss_time.c
	oss_tstring.c
)

ADD_LIBRARY(ossutil STATIC ${SRC})

set_modules_properties(ossutil)
