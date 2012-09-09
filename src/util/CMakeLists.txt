SET(SRC
	oss_map.c
	oss_time.c
)

ADD_LIBRARY(ossutil STATIC ${SRC})

set_modules_properties(ossutil)
# ADD_DEPENDENCIES(modules ossutil)
