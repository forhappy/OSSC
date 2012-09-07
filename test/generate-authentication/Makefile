all:main

main:
	gcc -g -o main main.c generate_authentication.c  oss_map.c hmac-sha1.c sha1.c memxor.c  base64.c uthash.h

.PHONY:clean

clean:
	rm  main
