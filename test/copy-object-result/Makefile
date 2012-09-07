all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_copy_object_result.o

main.o: main.c

oss_copy_object_result.o: oss_copy_object_result.c


.PHONY:clean

clean:
	rm *.o main
