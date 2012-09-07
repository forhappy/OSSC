all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_complete_multipart_upload_result.o

main.o: main.c

oss_complete_multipart_upload_result.o: oss_complete_multipart_upload_result.c


.PHONY:clean

clean:
	rm *.o main
