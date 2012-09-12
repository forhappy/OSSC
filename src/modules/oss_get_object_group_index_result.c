/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_group_index_result.c
 *
 *    Description:  oss get_object_group_index_result
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#include <ossc/modules/oss_get_object_group_index_result.h>
#undef _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H

static inline const char *
_get_object_group_index_result_get_bucket_name(oss_get_object_group_index_result_t *result)
{
	return result->bucket_name;
}

static inline void 
__get_object_group_index_result_set_bucket_name(oss_get_object_group_index_result_t *result,
		const char *bucket_name, size_t bucket_name_len)
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
_get_object_group_index_result_set_bucket_name(oss_get_object_group_index_result_t *result, const char *bucket_name)
{
	assert(result != NULL);
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__get_object_group_index_result_set_bucket_name(result, bucket_name, bucket_name_len);
}

static inline const char *
_get_object_group_index_result_get_key(oss_get_object_group_index_result_t *result)
{
	return result->key;
}

static inline void 
__get_object_group_index_result_set_key(oss_get_object_group_index_result_t *result,
		const char *key, size_t key_len)
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
_get_object_group_index_result_set_key(oss_get_object_group_index_result_t *result, const char *key)
{
	assert(result != NULL);
	assert(key != NULL);

	size_t key_len = strlen(key);
	__get_object_group_index_result_set_key(result, key, key_len);
}

static inline const char *
_get_object_group_index_result_get_etag(oss_get_object_group_index_result_t *result)
{
	return result->etag;
}

static inline void 
__get_object_group_index_result_set_etag(oss_get_object_group_index_result_t *result,
		const char *etag, size_t etag_len)
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
_get_object_group_index_result_set_etag(oss_get_object_group_index_result_t *result, const char *etag)
{
	assert(result != NULL);
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__get_object_group_index_result_set_etag(result, etag, etag_len);
}

static inline size_t 
_get_object_group_index_result_get_file_length(oss_get_object_group_index_result_t *result)
{
	return result->file_length;
}

static inline void 
_get_object_group_index_result_set_file_length(oss_get_object_group_index_result_t *result, size_t file_length)
{
	assert(result != NULL);
	result->file_length = file_length;
}


static inline oss_multipart_object_group_t **
_get_object_group_index_result_get_group(oss_get_object_group_index_result_t *result, int *part_number)
{
	*part_number = result->part_number;
	return result->group;
}

static inline void
_get_object_group_index_result_set_group(oss_get_object_group_index_result_t *result,
			oss_multipart_object_group_t **group, int part_number)
{
	assert(result != NULL);
	assert(group != NULL);
	result->group = group;
	result->part_number = part_number;
}

oss_get_object_group_index_result_t *
get_object_group_index_result_initialize(void)
{
	oss_get_object_group_index_result_t *result = (oss_get_object_group_index_result_t *)malloc(sizeof(oss_get_object_group_index_result_t));

	result->bucket_name = NULL;
	result->key = NULL;
	result->etag = NULL;
	result->group = NULL;
	result->file_length = 0;
	result->part_number = 0;

	result->get_bucket_name = _get_object_group_index_result_get_bucket_name;
	result->get_key = _get_object_group_index_result_get_key;
	result->get_etag = _get_object_group_index_result_get_etag;
	result->get_group = _get_object_group_index_result_get_group;
	result->get_file_length = _get_object_group_index_result_get_file_length;

	result->set_bucket_name = _get_object_group_index_result_set_bucket_name;
	result->set_key = _get_object_group_index_result_set_key;
	result->set_etag = _get_object_group_index_result_set_etag;
	result->set_group = _get_object_group_index_result_set_group;
	result->set_file_length = _get_object_group_index_result_set_file_length;

	return result;
}

void
get_object_group_index_result_finalize(oss_get_object_group_index_result_t *result)
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
		/* *
		 * Here we intend to leave it unfreed,
		 * because it's the caller's responsibility to free
		 * the index_result->get_object_group_index_result_metadata, who uses it and he has to create it
		 * and free it;
		 * */
		if (result->group) {
			result->group = NULL;
		}

		free(result);
		result = NULL;
	}
}
