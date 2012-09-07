all:main

CFLAGS=-g -Wall

main:main.o oss_map.o oss_copy_object_request.o  oss_object_metadata.o

main.o:main.c

oss_copy_object_request.o:oss_copy_object_request.c

oss_object_metadata.o:oss_object_metadata.c

.PHONY:clean

clean:
	rm *.o main
