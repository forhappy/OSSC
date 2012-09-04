all:main

CFLAGS=-g -Wall

main:main.o oss_abort_multipart_upload_request.o

main.o:main.c

oss_abort_multipart_upload_request.o:oss_abort_multipart_upload_request.c

.PHONY:clean

clean:
	rm *.o main
