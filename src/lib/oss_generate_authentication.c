/*
 * =============================================================================
 *
 *       Filename:  generate_authentication.c
 *
 *    Description:  generate authentication
 *
 *        Created:  09/06/2012 11:24:12 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _OSS_CONSTANTS_H
#include <ossc/oss_constants.h>
#undef _OSS_CONSTANTS_H

#include <ossc/oss_map.h>
#include <ossc/uthash.h>

#include "base64.h"
#include "sha1.h"
#include "hmac.h"
#include "memxor.h"

#define CANONICALIZED_HEADERS_BUFFER_SIZE 4096
#define HMAC_SHA1_OUT_LEN 21
#define SIGNED_VALUE_LEN 65


/* *
 * 用户自定义头部，头部名字按字典序排序
 * */
typedef struct user_headers_s user_headers_t;

struct user_headers_s {
	char *key;
	char *value;
	UT_hash_handle hh;
};

/* *
 * 小心：你的代码不要轻易使用静态变量
 * */
static char canonicalized_headers[CANONICALIZED_HEADERS_BUFFER_SIZE];
static user_headers_t *oss_user_headers = NULL;

/* *
 * key_iter 特殊指针变量，在遍历 user_headers 时，key_iter 指向
 * canonicalized_headers 已处理的位置，下次canonicalized_headers字符串
 * 填充由该位置起
 * */
static char *key_iter = NULL;

static int oss_sort_headers_by_key(user_headers_t *a, user_headers_t *b)
{
	return strcmp(a->key, b->key);
}

/* *
 * 头部排序
 * */
static void oss_sort_headers()
{
	HASH_SORT(oss_user_headers, oss_sort_headers_by_key);
}

static void oss_add_headers(const char *key, const char *value)
{
	assert(key != NULL);
	assert(value != NULL);

	user_headers_t *s = (user_headers_t *)malloc(sizeof(user_headers_t));
	size_t key_len = strlen(key);
	size_t value_len = strlen(value);

	s->key = (char *)malloc(sizeof(char) * key_len + 1);
	s->value = (char *)malloc(sizeof(char) * value_len + 1);

	memset(s->key, '\0', key_len + 1);
	memset(s->value, '\0', value_len + 1);

	strncpy(s->key, key, key_len);
	strncpy(s->value, value, value_len);

	HASH_ADD_INT(oss_user_headers, key, s);
}
static void oss_delete_all_headers()
{
	user_headers_t *current_user_header, *tmp;
	HASH_ITER(hh, oss_user_headers, current_user_header, tmp) {
		free(current_user_header->key);
		free(current_user_header->value);
		HASH_DEL(oss_user_headers,current_user_header);  /* delete; users advances to next */
		free(current_user_header);            /* optional- if you want to free  */
	}
}
/* *
 * 将所有用户自定义头部整合在一起
 * */
void fill_canonicalized_headers()
{
	user_headers_t *s;
	for (s = oss_user_headers; s != NULL; s = s->hh.next) {
		size_t offset = 0;
		offset = sprintf(key_iter, "%s:%s\n", s->key, s->value);
		key_iter += offset;
	}
}

#if 0
static void iter_user_headers(const char *key, const char *value, const void *obj)
{
	size_t offset = 0;
	offset = sprintf(key_iter, "%s:%s\n", key, value);
	key_iter += offset;
}
#endif

/* *
 * 回调函数，遍历 user_headers 的每一个头部
 * */
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
	assert(method != NULL);
	assert(default_headers != NULL);
	assert(resource != NULL);

	char *content_md5 = NULL;
	char *content_type = NULL;
	char *date = NULL;

	/* *
	 * 将要签名的字符串长度是 canonicalized_headers 的 4 倍
	 * */
	char *string_to_sign = (char *)malloc(sizeof(char) * CANONICALIZED_HEADERS_BUFFER_SIZE *4);
	size_t access_key_len = strlen(access_key);

	/* *
	 * hmac-sha1 结果存放
	 * */
	char *hmac_sha1_out = (char *)malloc(sizeof(char) * HMAC_SHA1_OUT_LEN);

	/* *
	 * base64 处理后的签名返回值
	 * */
	char *signed_value = (char *)malloc(sizeof(char) * SIGNED_VALUE_LEN);

	memset(string_to_sign, '\0', CANONICALIZED_HEADERS_BUFFER_SIZE * 4);
	memset(canonicalized_headers, '\0', CANONICALIZED_HEADERS_BUFFER_SIZE);
	memset(hmac_sha1_out, '\0', HMAC_SHA1_OUT_LEN);
	memset(signed_value, '\0', SIGNED_VALUE_LEN);
	/* *
	 * 必须再次赋值为 NULL
	 * */
	key_iter = NULL;

	unsigned int result_len = oss_map_get(default_headers, OSS_CONTENT_MD5, NULL, 0);
	if (result_len != 0) {
		content_md5 = (char *)malloc(sizeof(char) * result_len);
		memset(content_md5, '\0', result_len);
		oss_map_get(default_headers, OSS_CONTENT_MD5, content_md5, result_len);
	}

	result_len = oss_map_get(default_headers, OSS_CONTENT_TYPE, NULL, 0);
	if (result_len != 0) {
		content_type = (char *)malloc(sizeof(char) * result_len);
		memset(content_type, '\0', result_len);
		oss_map_get(default_headers, OSS_CONTENT_TYPE, content_type, result_len);
	}

	result_len = oss_map_get(default_headers, OSS_DATE, NULL, 0);
	if (result_len != 0) {
		date = (char *)malloc(sizeof(char) * result_len);
		memset(date, '\0', result_len);
		oss_map_get(default_headers, OSS_DATE, date, result_len);
	}

	/* *
	 *  key_iter 指向canonicalized_headers起始位置
	 * */
	key_iter = canonicalized_headers;

	/* *
	 * 注册回调函数
	 * */
	if (user_headers != NULL)
		oss_map_enum(user_headers, iter_user_headers, NULL);

	/* *
	 * 用户自定义头部排序
	 * */
	oss_sort_headers();

	/* *
	 * 遍历访问 user_headers，并填充 canonicalized_headers
	 * */
	fill_canonicalized_headers();
	key_iter = NULL;

	// printf("canonicalized_headers: %s\n", canonicalized_headers);

	if (content_md5 != NULL && content_type != NULL) 
		sprintf(string_to_sign, "%s\n%s\n%s\n%s\n%s%s", method,
				content_md5, content_type, date, canonicalized_headers, resource);
	else if (content_md5 == NULL && content_type != NULL)
		sprintf(string_to_sign, "%s\n\n%s\n%s\n%s%s", method, content_type,
				date, canonicalized_headers, resource);
	else if (content_md5 == NULL && content_type == NULL)
		sprintf(string_to_sign, "%s\n\n\n%s\n%s%s", method,
				date, canonicalized_headers, resource);


	// printf("string to to signed:\n%s\n", string_to_sign);

	size_t string_to_sign_len = strlen(string_to_sign);

	hmac_sha1(access_key, access_key_len, string_to_sign, string_to_sign_len, hmac_sha1_out);

	base64_encode(hmac_sha1_out, HMAC_SHA1_OUT_LEN - 1, signed_value, SIGNED_VALUE_LEN);
	*sign_len = strlen(signed_value);

	/* *
	 * clean up.
	 * */
	free(string_to_sign);
	free(hmac_sha1_out);
	oss_delete_all_headers();
	if (content_md5 != NULL) free(content_md5);
	if (content_type != NULL) free(content_type);
	if (date != NULL) free(date);

	return signed_value;
}
