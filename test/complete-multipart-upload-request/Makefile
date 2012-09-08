all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_complete_multipart_upload_request.o oss_part_etag.o

main.o: main.c

oss_complete_multipart_upload_request.o: oss_complete_multipart_upload_request.c

oss_part_etag.o: oss_part_etag.c

.PHONY:clean

clean:
	rm *.o main
