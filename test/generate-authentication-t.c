/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description: main testcase. 
 *
 *        Created:  09/06/2012 06:09:39 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ossc/client.h>

int main(int argc, const char *argv[])
{

	const char *access_key = "OtxrzxIsfpFjA7SwPzILwy8Bw21TLhquhboDYROV";
	const char *resource = "/hello/world";
	const char *method = "HEAD";

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);
	//oss_map_put(default_headers, OSS_CONTENT_MD5, "123456789afeq34834957aafeib23");
	//oss_map_put(default_headers, OSS_CONTENT_TYPE, "text/html");
	oss_map_put(default_headers, OSS_DATE, "2012-09-06 12:32:34 GMT");

	oss_map_put(user_headers, "x-oss-magic-id1", "hello-world");
	oss_map_put(user_headers, "x-oss-magic-name1", "forhapy");
	oss_map_put(user_headers, "x-oss-magic-id2", "hello-earth");
	oss_map_put(user_headers, "x-oss-magic-name2", "wangwei");

	const char *sign = generate_authentication(access_key, method,
			default_headers, user_headers, resource, &sign_len);
	printf("signed value length: %d\nsigned value: %s\n", sign_len, sign);
}
