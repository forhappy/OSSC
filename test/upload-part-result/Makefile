all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_upload_part_result.o

main.o: main.c

oss_upload_part_result.o: oss_upload_part_result.c

.PHONY:clean

clean:
	rm *.o main
