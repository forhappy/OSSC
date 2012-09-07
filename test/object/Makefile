all:main

main:main.o oss_map.o oss_object.o  oss_object_metadata.o

main.o:main.c

oss_object.o:oss_object.c

oss_object_metadata.o:oss_object_metadata.c

.PHONY:clean

clean:
	rm *.o main
