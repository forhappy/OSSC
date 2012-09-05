all:main

CFLAGS=-g -Wall

main:main.o oss_initiate_multipart_upload_request.o oss_map.o oss_object_metadata.o

main.o:main.c

oss_initiate_multipart_upload_request.o:oss_initiate_multipart_upload_request.c

oss_map.o:oss_map.c

oss_object_metadata.o:oss_object_metadata.c

.PHONY:clean

clean:
	rm *.o main
