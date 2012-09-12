all:main

CFLAGS=-g -Wall -std=c99

main:main.o oss_delete_multiple_object_request.o 

main.o:main.c

oss_delete_multiple_object_request.o:oss_delete_multiple_object_request.c

.PHONY:clean

clean:
	rm *.o main
