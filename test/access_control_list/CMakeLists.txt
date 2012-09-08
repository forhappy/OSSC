SET(SRC
	main.c)
#build_modules_tests("${test}" "")

ADD_EXECUTABLE(${SRC}.exe ${SRC})
TARGET_LINK_LIBRARIES(${SRC}.exe ossmodules)

