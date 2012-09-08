all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_multipart_upload_listing.o oss_multipart_upload.o

main.o: main.c

oss_multipart_upload_listing.o: oss_multipart_upload_listing.c

oss_multipart_upload.o:oss_multipart_upload.c

.PHONY:clean

clean:
	rm *.o main
