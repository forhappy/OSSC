all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_upload_part_request.o

main.o: main.c

oss_upload_part_request.o: oss_upload_part_request.c

.PHONY:clean

clean:
	rm *.o main
