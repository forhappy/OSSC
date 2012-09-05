all:main

CFLAGS=-g -Wall

main:main.o oss_list_objects_request.o

main.o:main.c

oss_list_objects_request.o:oss_list_objects_request.c

.PHONY:clean

clean:
	rm *.o main
