all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_object_listing.o

main.o: main.c

oss_object_listing.o: oss_object_listing.c

.PHONY:clean

clean:
	rm *.o main
