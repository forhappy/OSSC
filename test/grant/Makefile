all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_grant.o

main.o: main.c

oss_grant.o: oss_grant.c


.PHONY:clean

clean:
	rm *.o main
