all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_bucket.o oss_owner.o

main.o: main.c

oss_bucket.o: oss_bucket.c

oss_owner.o: oss_owner.c

.PHONY:clean

clean:
	rm *.o main
