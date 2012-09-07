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
#include <modules/oss_complete_multipart_upload_result.h>
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H



void 
complete_multipart_upload_result_finalize(oss_complete_multipart_upload_result_t *cmur)
{
	assert(cmur != NULL);

	if (cmur->bucket_name) {
		free(cmur->bucket_name);
		cmur->bucket_name = NULL;
	}
	if (cmur->etag) {
		free(cmur->etag);
		cmur->etag = NULL;
	}
	if (cmur->key) {
		free(cmur->key);
		cmur->key = NULL;
	}
	if (cmur->location) {
		free(cmur->location);
		cmur->location = NULL;
	}
	if (cmur) {
		free(cmur);
		cmur = NULL;
	}
}

static const char * 
_complete_multipart_upload_result_get_bucket_name(oss_complete_multipart_upload_result_t *cmur)
{
	return cmur->bucket_name;
}

static inline void
__complete_multipart_upload_result_set_bucket_name(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (cmur->bucket_name) {
		free(cmur->bucket_name);
		cmur->bucket_name = NULL;
	}
	cmur->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(cmur->bucket_name, bucket_name, bucket_name_len);
	(cmur->bucket_name)[bucket_name_len] = '\0';
}

static void
_complete_multipart_upload_result_set_bucket_name(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__complete_multipart_upload_result_set_bucket_name(cmur, bucket_name, bucket_name_len);
}

static const char * 
_complete_multipart_upload_result_get_etag(oss_complete_multipart_upload_result_t *cmur)
{
	return cmur->etag;
}

static inline void
__complete_multipart_upload_result_set_etag(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *etag,
		size_t etag_len)
{
	if (cmur->etag) {
		free(cmur->etag);
		cmur->etag = NULL;
	}
	cmur->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(cmur->etag, etag, etag_len);
	(cmur->etag)[etag_len] = '\0';
}

static void
_complete_multipart_upload_result_set_etag(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__complete_multipart_upload_result_set_etag(cmur, etag, etag_len);
}


static const char * 
_complete_multipart_upload_result_get_key(oss_complete_multipart_upload_result_t *cmur)
{
	return cmur->key;
}

static inline void
__complete_multipart_upload_result_set_key(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *key,
		size_t key_len)
{
	if (cmur->key) {
		free(cmur->key);
		cmur->key = NULL;
	}
	cmur->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(cmur->key, key, key_len);
	(cmur->key)[key_len] = '\0';
}

static void
_complete_multipart_upload_result_set_key(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__complete_multipart_upload_result_set_key(cmur, key, key_len);
}

static const char * 
_complete_multipart_upload_result_get_location(oss_complete_multipart_upload_result_t *cmur)
{
	return cmur->location;
}

static inline void
__complete_multipart_upload_result_set_location(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *location,
		size_t location_len)
{
	if (cmur->location) {
		free(cmur->location);
		cmur->location = NULL;
	}
	cmur->location = (char *)malloc(sizeof(char) * location_len + 1);
	strncpy(cmur->location, location, location_len);
	(cmur->location)[location_len] = '\0';
}

static void
_complete_multipart_upload_result_set_location(
		oss_complete_multipart_upload_result_t *cmur, 
		const char *location)
{
	assert(location != NULL);
	size_t location_len = strlen(location);
	__complete_multipart_upload_result_set_location(cmur, location, location_len);
}

oss_complete_multipart_upload_result_t *
complete_multipart_upload_result_initialize(void)
{
	oss_complete_multipart_upload_result_t *cmur;
	cmur = (oss_complete_multipart_upload_result_t *)malloc(sizeof(oss_complete_multipart_upload_result_t));
	cmur->bucket_name = NULL;
	cmur->etag = NULL;
	cmur->key = NULL;
	cmur->location = NULL;

	cmur->get_bucket_name = _complete_multipart_upload_result_get_bucket_name;
	cmur->set_bucket_name = _complete_multipart_upload_result_set_bucket_name;
	cmur->get_key = _complete_multipart_upload_result_get_key;
	cmur->set_key = _complete_multipart_upload_result_set_key;
	cmur->get_location = _complete_multipart_upload_result_get_location;
	cmur->set_location = _complete_multipart_upload_result_set_location;
	cmur->get_etag = _complete_multipart_upload_result_get_etag;
	cmur->set_etag = _complete_multipart_upload_result_set_etag;

	return cmur;
}
