/*
 * =============================================================================
 *
 *       Filename:  oss_complete_multipart_upload_result.c
 *
 *    Description:  oss_complete_multipart_upload_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#include <ossc/modules/oss_complete_multipart_upload_result.h>
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H

static const char * 
_complete_multipart_upload_result_get_bucket_name(
		oss_complete_multipart_upload_result_t *result)
{
	return result->bucket_name;
}

static inline void
__complete_multipart_upload_result_set_bucket_name(
		oss_complete_multipart_upload_result_t *result,
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (result->bucket_name != NULL) {
		free(result->bucket_name);
		result->bucket_name = NULL;
	}

	result->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(result->bucket_name, bucket_name, bucket_name_len);
	(result->bucket_name)[bucket_name_len] = '\0';
}

static void
_complete_multipart_upload_result_set_bucket_name(
		oss_complete_multipart_upload_result_t *result,
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__complete_multipart_upload_result_set_bucket_name(
			result, bucket_name, bucket_name_len);
}

static const char * 
_complete_multipart_upload_result_get_etag(
		oss_complete_multipart_upload_result_t *result)
{
	return result->etag;
}

static inline void
__complete_multipart_upload_result_set_etag(
		oss_complete_multipart_upload_result_t *result,
		const char *etag,
		size_t etag_len)
{
	if (result->etag != NULL) {
		free(result->etag);
		result->etag = NULL;
	}

	result->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(result->etag, etag, etag_len);
	(result->etag)[etag_len] = '\0';
}

static void
_complete_multipart_upload_result_set_etag(
		oss_complete_multipart_upload_result_t *result,
		const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__complete_multipart_upload_result_set_etag(result, etag, etag_len);
}


static const char * 
_complete_multipart_upload_result_get_key(
		oss_complete_multipart_upload_result_t *result)
{
	return result->key;
}

static inline void
__complete_multipart_upload_result_set_key(
		oss_complete_multipart_upload_result_t *result,
		const char *key,
		size_t key_len)
{
	if (result->key != NULL) {
		free(result->key);
		result->key = NULL;
	}

	result->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(result->key, key, key_len);
	(result->key)[key_len] = '\0';
}

static void
_complete_multipart_upload_result_set_key(
		oss_complete_multipart_upload_result_t *result,
		const char *key)
{
	assert(key != NULL);

	size_t key_len = strlen(key);
	__complete_multipart_upload_result_set_key(result, key, key_len);
}

static const char * 
_complete_multipart_upload_result_get_location(
		oss_complete_multipart_upload_result_t *result)
{
	return result->location;
}

static inline void
__complete_multipart_upload_result_set_location(
		oss_complete_multipart_upload_result_t *result,
		const char *location,
		size_t location_len)
{
	if (result->location != NULL) {
		free(result->location);
		result->location = NULL;
	}

	result->location = (char *)malloc(sizeof(char) * location_len + 1);
	strncpy(result->location, location, location_len);
	(result->location)[location_len] = '\0';
}

static void
_complete_multipart_upload_result_set_location(
		oss_complete_multipart_upload_result_t *result,
		const char *location)
{
	assert(location != NULL);

	size_t location_len = strlen(location);
	__complete_multipart_upload_result_set_location(result, location, location_len);
}

oss_complete_multipart_upload_result_t *
complete_multipart_upload_result_initialize(void)
{
	oss_complete_multipart_upload_result_t *result;
	result = (oss_complete_multipart_upload_result_t *)malloc(sizeof(oss_complete_multipart_upload_result_t));
	result->bucket_name = NULL;
	result->etag = NULL;
	result->key = NULL;
	result->location = NULL;

	result->get_bucket_name = _complete_multipart_upload_result_get_bucket_name;
	result->set_bucket_name = _complete_multipart_upload_result_set_bucket_name;
	result->get_key = _complete_multipart_upload_result_get_key;
	result->set_key = _complete_multipart_upload_result_set_key;
	result->get_location = _complete_multipart_upload_result_get_location;
	result->set_location = _complete_multipart_upload_result_set_location;
	result->get_etag = _complete_multipart_upload_result_get_etag;
	result->set_etag = _complete_multipart_upload_result_set_etag;

	return result;
}

void
complete_multipart_upload_result_finalize(
		oss_complete_multipart_upload_result_t *result)
{
	assert(result != NULL);

	if (result->bucket_name) {
		free(result->bucket_name);
		result->bucket_name = NULL;
	}

	if (result->etag) {
		free(result->etag);
		result->etag = NULL;
	}

	if (result->key) {
		free(result->key);
		result->key = NULL;
	}

	if (result->location) {
		free(result->location);
		result->location = NULL;
	}

	free(result);
}
