/*
 * =============================================================================
 *
 *       Filename:  generate_authentication.c
 *
 *    Description:  generate authentication
 *
 *        Created:  09/06/2012 11:24:12 AM
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

#define _OSS_CONSTANTS_H
#include "oss_constants.h"
#undef _OSS_CONSTANTS_H

#include "oss_map.h"
#include "uthash.h"

#define CANONICALIZED_HEADERS_BUFFER_SIZE 4096

/* *
 * 小心：你的代码不要轻易使用静态变量
 * */
char canonicalized_headers[CANONICALIZED_HEADERS_BUFFER_SIZE];
char *p = NULL;

typedef struct user_headers_s user_headers_t;

struct user_headers_s {
	char *key;
	char *value;
	UT_hash_handle hh;
};

user_headers_t *oss_user_headers = NULL;

int oss_sort_headers_by_key(user_headers_t *a, user_headers_t *b)
{
	return strcmp(a->key, b->key);
}

void oss_sort_headers()
{
	HASH_SORT(oss_user_headers, oss_sort_headers_by_key);
}

void oss_add_headers(const char *key, const char *value)
{
	user_headers_t *s = (user_headers_t *)malloc(sizeof(user_headers_t));
	size_t key_len = strlen(key);
	size_t value_len = strlen(value);

	s->key = (char *)malloc(sizeof(char) * key_len + 1);
	s->value = (char *)malloc(sizeof(char) * value_len + 1);

	memset(s->key, key_len + 1, '\0');
	memset(s->value, value_len + 1, '\0');
	strncpy(s->key, key, key_len);
	strncpy(s->value, value, value_len);
	HASH_ADD_INT(oss_user_headers, key, s);
}

void fill_canonicalized_headers()
{
	user_headers_t *s;
	for (s = oss_user_headers; s != NULL; s = s->hh.next) {
		size_t offset = 0;
		offset = sprintf(p, "%s:%s\n", s->key, s->value);
		p += offset;
	}
}

#if 0
static void iter_user_headers(const char *key, const char *value, const void *obj)
{
	size_t offset = 0;
	offset = sprintf(p, "%s:%s\n", key, value);
	p += offset;
}
#endif

static void iter_user_headers(const char *key, const char *value, const void *obj)
{
	oss_add_headers(key, value);
}

const char *
generate_authentication(const char *access_key, const char *method,
		oss_map_t *default_headers, oss_map_t *user_headers,
		const char *resource, unsigned int *sign_len)
{
	assert(access_key != NULL);
	assert(default_headers != NULL);
	assert(user_headers != NULL);
	assert(resource != NULL);

	char *content_md5 = NULL;
	char *content_type = NULL;
	char *date = NULL;
	char *sign = (char *)malloc(sizeof(char) * CANONICALIZED_HEADERS_BUFFER_SIZE *4);
	size_t access_key_len = strlen(access_key);

	memset(sign, CANONICALIZED_HEADERS_BUFFER_SIZE * 4, '\0');
	p = NULL;
	memset(canonicalized_headers, CANONICALIZED_HEADERS_BUFFER_SIZE, '\0');

	unsigned int result_len = oss_map_get(default_headers, OSS_CONTENT_MD5, NULL, 0);
	if (result_len != 0) {
		content_md5 = (char *)malloc(sizeof(char) * result_len);
		memset(content_md5, result_len, 0);
		oss_map_get(default_headers, OSS_CONTENT_MD5, content_md5, result_len);
	}

	result_len = oss_map_get(default_headers, OSS_CONTENT_TYPE, NULL, 0);
	if (result_len != 0) {
		content_type = (char *)malloc(sizeof(char) * result_len);
		memset(content_type, result_len, 0);
		oss_map_get(default_headers, OSS_CONTENT_TYPE, content_type, result_len);
	}

	result_len = oss_map_get(default_headers, OSS_DATE, NULL, 0);
	if (result_len != 0) {
		date = (char *)malloc(sizeof(char) * result_len);
		memset(date, result_len, 0);
		oss_map_get(default_headers, OSS_DATE, date, result_len);
	}

	p = canonicalized_headers;
	oss_map_enum(user_headers, iter_user_headers, NULL);
	oss_sort_headers();
	fill_canonicalized_headers();
	p = NULL;

	sprintf(sign, "%s\n%s\n%s\n%s\n%s%s", method,
			content_md5, content_type, date, canonicalized_headers, resource);

	printf("%s\n", sign);

	*sign_len = strlen(sign);

	char *out = (char *)malloc(sizeof(char) * 21);
	memset(out, 21, '\0');

	hmac_sha1(access_key, access_key_len, sign, *sign_len, out);

	char base64[4096];
	memset(base64, 4096, '\0');
	base64_encode(out, 20, base64, 4096);

	printf("%s\n", base64);

	return out;
}
