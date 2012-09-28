/*
 * =============================================================================
 *
 *       Filename:  oss_post_object_group_request.c
 *
 *    Description:  oss post_object_group_request
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_POST_OBJECT_GROUP_REQUEST_H
#include <ossc/modules/oss_post_object_group_request.h>
#undef _OSS_POST_OBJECT_GROUP_REQUEST_H

static inline const char *
_post_object_group_request_get_bucket_name(
		oss_post_object_group_request_t *request)
{
	return request->bucket_name;
}

static inline void 
__post_object_group_request_set_bucket_name(
		oss_post_object_group_request_t *request,
		const char *bucket_name, unsigned int bucket_name_len)
{
	assert(request != NULL);
	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}
	 request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	 memset(request->bucket_name, '\0', bucket_name_len + 1);
	 strncpy(request->bucket_name, bucket_name, bucket_name_len);
}

static inline void 
_post_object_group_request_set_bucket_name(
		oss_post_object_group_request_t *request, const char *bucket_name)
{
	assert(request != NULL);
	assert(bucket_name != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	__post_object_group_request_set_bucket_name(request, bucket_name, bucket_name_len);
}

static inline const char *
_post_object_group_request_get_key(oss_post_object_group_request_t *request)
{
	return request->key;
}

static inline void 
__post_object_group_request_set_key(oss_post_object_group_request_t *request,
		const char *key, unsigned int key_len)
{
	assert(request != NULL);
	if (request->key!= NULL) {
		free(request->key);
		request->key= NULL;
	}
	 request->key= (char *)malloc(sizeof(char) * key_len + 1);
	 memset(request->key, '\0', key_len+ 1);
	 strncpy(request->key, key, key_len);
}

static inline void 
_post_object_group_request_set_key(oss_post_object_group_request_t *request, const char *key)
{
	assert(request != NULL);
	assert(key != NULL);

	unsigned int key_len = strlen(key);
	__post_object_group_request_set_key(request, key, key_len);
}


static inline oss_object_group_item_t **
_post_object_group_request_get_items(oss_post_object_group_request_t *request, int *itemnums)
{
	*itemnums = request->itemnums;
	return request->items;
}

static inline void
_post_object_group_request_set_items(oss_post_object_group_request_t *request,
			oss_object_group_item_t **items, int itemnums)
{
	assert(request != NULL);
	assert(items != NULL);
	request->items = items;
	request->itemnums = itemnums;
}

static inline oss_post_object_group_request_t *
_post_object_group_request_initialize(
		const char *bucket_name, unsigned int bucket_name_len,
		const char *key, unsigned int key_len,
		oss_object_group_item_t **items,
		int itemnums)
{
	oss_post_object_group_request_t *request = 
		(oss_post_object_group_request_t *)malloc(sizeof(oss_post_object_group_request_t));

	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	memset(request->bucket_name, '\0', bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);

	request->key= (char *)malloc(sizeof(char) * key_len + 1);
	memset(request->key, '\0', key_len+ 1);
	strncpy(request->key, key, key_len);

	request->items = items;
	request->itemnums = itemnums;

	request->get_bucket_name = _post_object_group_request_get_bucket_name;
	request->get_key = _post_object_group_request_get_key;
	request->get_items = _post_object_group_request_get_items;

	request->set_bucket_name = _post_object_group_request_set_bucket_name;
	request->set_key = _post_object_group_request_set_key;
	request->set_items = _post_object_group_request_set_items;

	return request;
}

extern oss_post_object_group_request_t *
post_object_group_request_initialize(
		const char *bucket_name, const char *key,
		oss_object_group_item_t **items, int itemnums)
{
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(items != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	unsigned int key_len = strlen(key);

	return _post_object_group_request_initialize(bucket_name, bucket_name_len,
			key, key_len, items, itemnums);
}

void
post_object_group_request_finalize(oss_post_object_group_request_t *request)
{
	assert(request != NULL);
	if (request != NULL) {
		if (request->bucket_name != NULL) {
			free(request->bucket_name);
			request->bucket_name = NULL;
		}

		if (request->key != NULL) {
			free(request->key);
			request->key = NULL;
		}

		/* *
		 * Here we intend to leave it unfreed,
		 * because it's the caller's responsibility to free
		 * the index_request->post_object_group_request_metadata, who uses it and he has to create it
		 * and free it;
		 * */
		if (request->items != NULL) {
			request->items = NULL;
		}

		free(request);
	}
}
