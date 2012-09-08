all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_access_control_list.o oss_owner.o oss_grant.o

main.o: main.c

oss_access_control_list.o: oss_access_control_list.c

oss_owner.o: oss_owner.c

oss_grant.o: oss_grant.c



.PHONY:clean

clean:
	rm *.o main
