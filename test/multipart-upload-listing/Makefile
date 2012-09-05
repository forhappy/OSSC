all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_multipart_upload_listing.o

main.o: main.c

oss_multipart_upload_listing.o: oss_multipart_upload_listing.c

.PHONY:clean

clean:
	rm *.o main
