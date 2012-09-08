SET(SRC
	main.c)
#build_modules_tests("${test}" "")

ADD_EXECUTABLE(main ${SRC})
TARGET_LINK_LIBRARIES(main ossmodules)

