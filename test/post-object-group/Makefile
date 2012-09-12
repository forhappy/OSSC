all: oss_object_group_item-t  oss_post_object_group_request-t

CC = gcc

CFLAGS = -g -Wall

oss_post_object_group_request-t: oss_post_object_group_request-t.o oss_post_object_group_request.o oss_object_group_item.o

oss_post_object_group_request-t.o: oss_post_object_group_request-t.c

oss_post_object_group_request.o: oss_post_object_group_request.c

oss_object_group_item-t: oss_object_group_item-t.o oss_object_group_item.o

oss_object_group_item-t.o: oss_object_group_item-t.c 

oss_object_group_item.o: oss_object_group_item.c


.PHONY:clean

clean:
	rm *.o oss_object_group_item-t  oss_post_object_group_request-t
