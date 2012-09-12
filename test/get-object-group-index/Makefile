all: multipart_object_group-t  get_object_group_index_result-t

CC = gcc

CFLAGS = -g -Wall

get_object_group_index_result-t: get_object_group_index_result-t.o oss_get_object_group_index_result.o oss_multipart_object_group.o

get_object_group_index_result-t.o: get_object_group_index_result-t.c

oss_get_object_group_index_result.o: oss_get_object_group_index_result.c

multipart_object_group-t: multipart_object_group-t.o oss_multipart_object_group.o

multipart_object_group-t.o: multipart_object_group-t.c 

oss_multipart_object_group.o: oss_multipart_object_group.c


.PHONY:clean

clean:
	rm *.o multipart_object_group-t  get_object_group_index_result-t
