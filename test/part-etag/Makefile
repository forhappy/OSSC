all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_part_etag.o

main.o: main.c

oss_part_etag.o: oss_part_etag.c

.PHONY:clean

clean:
	rm *.o main
