all:main

CFLAGS=-g -Wall

main:main.o oss_initiate_multipart_upload_result.o

main.o:main.c

oss_initiate_multipart_upload_result.o:oss_initiate_multipart_upload_result.c

.PHONY:clean

clean:
	rm *.o main
