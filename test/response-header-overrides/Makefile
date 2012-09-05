all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_response_header_overrides.o

main.o: main.c

oss_response_header_overrides.o: oss_response_header_overrides.c

.PHONY:clean

clean:
	rm *.o main
