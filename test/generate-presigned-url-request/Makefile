all: main

CC = gcc

CFLAGS= -g -Wall

main: main.o oss_generate_presigned_url_request.o  oss_response_header_overrides.o oss_map.o

main.o: main.c

oss_map.o:oss_map.c

oss_generate_presigned_url_request.o: oss_generate_presigned_url_request.c

oss_response_header_overrides.o: oss_response_header_overrides.c 

.PHONY:clean

clean:
	rm *.o main
