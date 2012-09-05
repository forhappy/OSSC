all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_part_summary.o

main.o: main.c

oss_part_summary.o: oss_part_summary.c

.PHONY:clean

clean:
	rm *.o main
