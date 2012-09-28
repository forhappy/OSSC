/*
 * =============================================================================
 *
 *       Filename:  oss_post_object_group_result.c
 *
 *    Description:  oss post_object_group_result
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_POST_OBJECT_GROUP_RESULT_H
#include <ossc/modules/oss_post_object_group_result.h>
#undef _OSS_POST_OBJECT_GROUP_RESULT_H

static inline const char *
_post_object_group_result_get_bucket_name(oss_post_object_group_result_t *result)
{
	return result->bucket_name;
}

static inline void 
__post_object_group_result_set_bucket_name(oss_post_object_group_result_t *result,
		const char *bucket_name, unsigned int bucket_name_len)
{
	assert(result != NULL);
	if (result->bucket_name != NULL) {
		free(result->bucket_name);
		result->bucket_name = NULL;
	}
	 result->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	 memset(result->bucket_name, '\0', bucket_name_len + 1);
	 strncpy(result->bucket_name, bucket_name, bucket_name_len);
}

static inline void 
_post_object_group_result_set_bucket_name(oss_post_object_group_result_t *result, const char *bucket_name)
{
	assert(result != NULL);
	assert(bucket_name != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	__post_object_group_result_set_bucket_name(result, bucket_name, bucket_name_len);
}

static inline const char *
_post_object_group_result_get_key(oss_post_object_group_result_t *result)
{
	return result->key;
}

static inline void 
__post_object_group_result_set_key(oss_post_object_group_result_t *result,
		const char *key, unsigned int key_len)
{
	assert(result != NULL);
	if (result->key!= NULL) {
		free(result->key);
		result->key= NULL;
	}
	 result->key= (char *)malloc(sizeof(char) * key_len + 1);
	 memset(result->key, '\0', key_len+ 1);
	 strncpy(result->key, key, key_len);
}

static inline void 
_post_object_group_result_set_key(oss_post_object_group_result_t *result, const char *key)
{
	assert(result != NULL);
	assert(key != NULL);

	unsigned int key_len = strlen(key);
	__post_object_group_result_set_key(result, key, key_len);
}

static inline const char *
_post_object_group_result_get_etag(oss_post_object_group_result_t *result)
{
	return result->etag;
}

static inline void 
__post_object_group_result_set_etag(oss_post_object_group_result_t *result,
		const char *etag, unsigned int etag_len)
{
	assert(result != NULL);
	if (result->etag!= NULL) {
		free(result->etag);
		result->etag= NULL;
	}
	 result->etag= (char *)malloc(sizeof(char) * etag_len + 1);
	 memset(result->etag, '\0', etag_len+ 1);
	 strncpy(result->etag, etag, etag_len);
}

static inline void 
_post_object_group_result_set_etag(oss_post_object_group_result_t *result, const char *etag)
{
	assert(result != NULL);
	assert(etag != NULL);

	unsigned int etag_len = strlen(etag);
	__post_object_group_result_set_etag(result, etag, etag_len);
}

static inline unsigned int 
_post_object_group_result_get_size(oss_post_object_group_result_t *result)
{
	return result->size;
}

static inline void 
_post_object_group_result_set_size(oss_post_object_group_result_t *result, unsigned int size)
{
	assert(result != NULL);
	result->size = size;
}

oss_post_object_group_result_t *
post_object_group_result_initialize(void)
{
	oss_post_object_group_result_t *result = (oss_post_object_group_result_t *)malloc(sizeof(oss_post_object_group_result_t));

	result->bucket_name = NULL;
	result->key = NULL;
	result->etag = NULL;
	result->size = 0;

	result->get_bucket_name = _post_object_group_result_get_bucket_name;
	result->get_key = _post_object_group_result_get_key;
	result->get_etag = _post_object_group_result_get_etag;
	result->get_size = _post_object_group_result_get_size;

	result->set_bucket_name = _post_object_group_result_set_bucket_name;
	result->set_key = _post_object_group_result_set_key;
	result->set_etag = _post_object_group_result_set_etag;
	result->set_size = _post_object_group_result_set_size;

	return result;
}

void
post_object_group_result_finalize(oss_post_object_group_result_t *result)
{
	assert(result != NULL);
	if (result != NULL) {
		if (result->bucket_name != NULL) {
			free(result->bucket_name);
			result->bucket_name = NULL;
		}

		if (result->key != NULL) {
			free(result->key);
			result->key = NULL;
		}

		if (result->etag != NULL) {
			free(result->etag);
			result->etag = NULL;
		}
		free(result);
		result = NULL;
	}
}
