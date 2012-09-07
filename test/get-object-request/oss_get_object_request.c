/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_request.c
 *
 *    Description:  oss_get_object_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_GET_OBJECT_REQUEST_H
#include "oss_get_object_request.h"
#undef _OSS_GET_OBJECT_REQUEST_H



void 
get_object_request_finalize(oss_get_object_request_t *gor)
{
	if (gor) {
		if (gor->bucket_name) {
			free(gor->bucket_name);
			gor->bucket_name = NULL;
		}
		if (gor->key) {
			free(gor->key);
			gor->key = NULL;
		}
		if (gor->modified_since_constraint) {
			free(gor->modified_since_constraint);
			gor->modified_since_constraint = NULL;
		}
		if (gor->response_headers) {
			gor->response_headers = NULL;
		}
		free(gor);
		gor = NULL;
	}
}

static const char * 
_get_object_request_get_bucket_name(oss_get_object_request_t *gor)
{
	return gor->bucket_name;
}

static inline void
__get_object_request_set_bucket_name(
		oss_get_object_request_t *gor, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (gor->bucket_name) {
		free(gor->bucket_name);
		gor->bucket_name = NULL;
	}
	gor->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(gor->bucket_name, bucket_name, bucket_name_len);
	(gor->bucket_name)[bucket_name_len] = '\0';
}

static void
_get_object_request_set_bucket_name(
		oss_get_object_request_t *gor, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__get_object_request_set_bucket_name(gor, bucket_name, bucket_name_len);
}

static const char * 
_get_object_request_get_key(oss_get_object_request_t *gor)
{
	return gor->key;
}

static inline void
__get_object_request_set_key(
		oss_get_object_request_t *gor, 
		const char *key,
		size_t key_len)
{
	if (gor->key) {
		free(gor->key);
		gor->key = NULL;
	}
	gor->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(gor->key, key, key_len);
	(gor->key)[key_len] = '\0';
}

static void
_get_object_request_set_key(
		oss_get_object_request_t *gor, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__get_object_request_set_key(gor, key, key_len);
}

static const char * 
_get_object_request_get_modified_since_constraint(oss_get_object_request_t *gor)
{
	return gor->modified_since_constraint;
}

static inline void
__get_object_request_set_modified_since_constraint(
		oss_get_object_request_t *gor, 
		const char *modified_since_constraint,
		size_t modified_since_constraint_len)
{
	if (gor->modified_since_constraint) {
		free(gor->modified_since_constraint);
		gor->modified_since_constraint = NULL;
	}
	gor->modified_since_constraint = (char *)malloc(sizeof(char) * modified_since_constraint_len + 1);
	strncpy(gor->modified_since_constraint, modified_since_constraint, modified_since_constraint_len);
	(gor->modified_since_constraint)[modified_since_constraint_len] = '\0';
}

static void
_get_object_request_set_modified_since_constraint(
		oss_get_object_request_t *gor, 
		const char *modified_since_constraint)
{
	assert(modified_since_constraint != NULL);
	size_t modified_since_constraint_len = strlen(modified_since_constraint);
	__get_object_request_set_modified_since_constraint(gor, modified_since_constraint, modified_since_constraint_len);
}

static void 
_get_object_request_get_range(oss_get_object_request_t *gor, long *start, long *length)
{
	*start = gor->start;
	*length = gor->length;
}

static void
_get_object_request_set_range(
		oss_get_object_request_t *gor, 
		long start,
		long length)
{
	gor->start = start;
	gor->length = length;
}


static oss_response_header_overrides_t * 
_get_object_request_get_response_headers(oss_get_object_request_t *gor)
{
	return gor->response_headers;
}

static inline void
_get_object_request_set_response_headers(
		oss_get_object_request_t *gor, 
		oss_response_header_overrides_t *response_headers)
{
	gor->response_headers = response_headers;
	//if (gor->response_header) {
	//	free(gor->response_header);
	//	gor->response_header = NULL;
	//}
	//gor->response_header = (char *)malloc(sizeof(char) * response_header_len + 1);
	//strncpy(gor->response_header, response_header, response_header_len);
	//(gor->response_header)[response_header_len] = '\0';
}


static const char *
_get_object_request_get_unmodified_since_constraint(oss_get_object_request_t *gor)
{
	return gor->unmodified_since_constraint;
}

static inline void
__get_object_request_set_unmodified_since_constraint(
		oss_get_object_request_t *gor, 
		const char *unmodified_since_constraint,
		size_t unmodified_since_constraint_len)
{
	if (gor->unmodified_since_constraint) {
		free(gor->unmodified_since_constraint);
		gor->unmodified_since_constraint = NULL;
	}
	gor->unmodified_since_constraint = (char *)malloc(sizeof(char) * unmodified_since_constraint_len + 1);
	strncpy(gor->unmodified_since_constraint, unmodified_since_constraint, unmodified_since_constraint_len);
	(gor->unmodified_since_constraint)[unmodified_since_constraint_len] = '\0';
}

static inline void
_get_object_request_set_unmodified_since_constraint(
		oss_get_object_request_t *gor, 
		const char *unmodified_since_constraint)
{
	assert(unmodified_since_constraint != NULL);
	size_t unmodified_since_constraint_len = strlen(unmodified_since_constraint);
	__get_object_request_set_unmodified_since_constraint(gor, unmodified_since_constraint, unmodified_since_constraint_len);

}


oss_get_object_request_t *
_get_object_request_initialize(const char *bucket_name, size_t bucket_name_len, const char *key, size_t key_len)
{
	oss_get_object_request_t *gor;
	gor = (oss_get_object_request_t *)malloc(sizeof(oss_get_object_request_t));
	if (gor->bucket_name) {
		free(gor->bucket_name);
		gor->bucket_name = NULL;
	}
	gor->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(gor->bucket_name, bucket_name, bucket_name_len);
	(gor->bucket_name)[bucket_name_len] = '\0';

	if (gor->key) {
		free(gor->key);
		gor->key = NULL;
	}
	gor->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(gor->key, key, key_len);
	(gor->key)[key_len] = '\0';

	gor->modified_since_constraint = NULL;
	gor->start = 0;
	gor->length = 0;
	gor->response_headers = NULL;
	gor->unmodified_since_constraint = NULL;

	gor->get_bucket_name = _get_object_request_get_bucket_name;
	gor->set_bucket_name = _get_object_request_set_bucket_name;
	gor->get_key = _get_object_request_get_key;
	gor->set_key = _get_object_request_set_key;
	gor->get_modified_since_constraint = _get_object_request_get_modified_since_constraint;
	gor->set_modified_since_constraint = _get_object_request_set_modified_since_constraint;
	gor->get_range = _get_object_request_get_range;
	gor->set_range = _get_object_request_set_range;
	gor->get_response_headers = _get_object_request_get_response_headers;
	gor->set_response_headers = _get_object_request_set_response_headers;
	gor->get_unmodified_since_constraint = _get_object_request_get_unmodified_since_constraint;
	gor->set_unmodified_since_constraint = _get_object_request_set_unmodified_since_constraint;

	return gor;
}

oss_get_object_request_t *
get_object_request_initialize(const char *bucket_name, const char *key)
{
	assert(bucket_name != NULL);
	assert(key != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	return _get_object_request_initialize(bucket_name, bucket_name_len, key, key_len);
}

