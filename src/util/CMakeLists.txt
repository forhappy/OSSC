SET(SRC
	oss_auth.c
	oss_common.c
	oss_compression.c
	oss_decompression.c
	oss_map.c
	oss_time.c
	oss_tstring.c
	oss_ttxml.c
)

ADD_LIBRARY(ossutil STATIC ${SRC})

set_modules_properties(ossutil)
