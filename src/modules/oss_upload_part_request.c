/*
 * =============================================================================
 *
 *       Filename:  oss_upload_part_request.c
 *
 *    Description:  oss_upload_part_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_UPLOAD_PART_REQUEST_H
#include "oss_upload_part_request.h"
#undef _OSS_UPLOAD_PART_REQUEST_H



void 
upload_part_request_finalize(oss_upload_part_request_t *upr)
{
	if (upr->bucket_name) {
		free(upr->bucket_name);
		upr->bucket_name = NULL;
	}
	if (upr->key) {
		free(upr->key);
		upr->key = NULL;
	}
	if (upr->input_stream) {
		free(upr->input_stream);
		upr->input_stream = NULL;
	}
	if (upr->md5_digest) {
		free(upr->md5_digest);
		upr->md5_digest = NULL;
	}
	if (upr->upload_id) {
		free(upr->upload_id);
		upr->upload_id = NULL;
	}
	if (upr) {
		free(upr);
		upr = NULL;
	}
}

static const char * 
_upload_part_request_get_bucket_name(oss_upload_part_request_t *upr)
{
	return upr->bucket_name;
}

static inline void
__upload_part_request_set_bucket_name(
		oss_upload_part_request_t *upr, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (upr->bucket_name) {
		free(upr->bucket_name);
		upr->bucket_name = NULL;
	}
	upr->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(upr->bucket_name, bucket_name, bucket_name_len);
	(upr->bucket_name)[bucket_name_len] = '\0';
}

static void
_upload_part_request_set_bucket_name(
		oss_upload_part_request_t *upr, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__upload_part_request_set_bucket_name(upr, bucket_name, bucket_name_len);
}

static const char * 
_upload_part_request_get_key(oss_upload_part_request_t *upr)
{
	return upr->key;
}

static inline void
__upload_part_request_set_key(
		oss_upload_part_request_t *upr, 
		const char *key,
		size_t key_len)
{
	if (upr->key) {
		free(upr->key);
		upr->key = NULL;
	}
	upr->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(upr->key, key, key_len);
	(upr->key)[key_len] = '\0';
}

static void
_upload_part_request_set_key(
		oss_upload_part_request_t *upr, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__upload_part_request_set_key(upr, key, key_len);
}

static const char * 
_upload_part_request_get_input_stream(oss_upload_part_request_t *upr, int *input_stream_len)
{
	*input_stream_len = upr->input_stream_len;
	return upr->input_stream;
}

static inline void
__upload_part_request_set_input_stream(
		oss_upload_part_request_t *upr, 
		const char *input_stream,
		size_t input_stream_len)
{
	if (upr->input_stream) {
		free(upr->input_stream);
		upr->input_stream = NULL;
	}
	upr->input_stream_len = input_stream_len;
	upr->input_stream = (char *)malloc(sizeof(char) * input_stream_len + 1);
	memcpy(upr->input_stream, input_stream, input_stream_len);
	(upr->input_stream)[input_stream_len] = '\0';
}

static void
_upload_part_request_set_input_stream(
		oss_upload_part_request_t *upr, 
		const char *input_stream, size_t input_stream_len)
{
	assert(input_stream != NULL);
	assert(input_stream_len > 0);
	__upload_part_request_set_input_stream(upr, input_stream, input_stream_len);
}

static const char * 
_upload_part_request_get_md5_digest(oss_upload_part_request_t *upr)
{
	return upr->md5_digest;
}

static inline void
__upload_part_request_set_md5_digest(
		oss_upload_part_request_t *upr, 
		const char *md5_digest,
		size_t md5_digest_len)
{
	if (upr->md5_digest) {
		free(upr->md5_digest);
		upr->md5_digest = NULL;
	}
	upr->md5_digest = (char *)malloc(sizeof(char) * md5_digest_len + 1);
	strncpy(upr->md5_digest, md5_digest, md5_digest_len);
	(upr->md5_digest)[md5_digest_len] = '\0';
}

static void
_upload_part_request_set_md5_digest(
		oss_upload_part_request_t *upr, 
		const char *md5_digest)
{
	assert(md5_digest != NULL);
	size_t md5_digest_len = strlen(md5_digest);
	__upload_part_request_set_md5_digest(upr, md5_digest, md5_digest_len);
}

static const char * 
_upload_part_request_get_upload_id(oss_upload_part_request_t *upr)
{
	return upr->upload_id;
}

static inline void
__upload_part_request_set_upload_id(
		oss_upload_part_request_t *upr, 
		const char *upload_id,
		size_t upload_id_len)
{
	if (upr->upload_id) {
		free(upr->upload_id);
		upr->upload_id = NULL;
	}
	upr->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(upr->upload_id, upload_id, upload_id_len);
	(upr->upload_id)[upload_id_len] = '\0';
}

static void
_upload_part_request_set_upload_id(
		oss_upload_part_request_t *upr, 
		const char *upload_id)
{
	assert(upload_id != NULL);
	size_t upload_id_len = strlen(upload_id);
	__upload_part_request_set_upload_id(upr, upload_id, upload_id_len);
}

static int 
_upload_part_request_get_part_number(oss_upload_part_request_t *upr)
{
	return upr->part_number;
}

static void
_upload_part_request_set_part_number(
		oss_upload_part_request_t *upr, 
		int part_number)
{
	upr->part_number = part_number;
}

static long 
_upload_part_request_get_part_size(oss_upload_part_request_t *upr)
{
	return upr->part_size;
}

static void
_upload_part_request_set_part_size(
		oss_upload_part_request_t *upr, 
		long part_size)
{
	upr->part_size = part_size;
}

oss_upload_part_request_t *
upload_part_request_initialize(void)
{
	oss_upload_part_request_t *upr;
	upr = (oss_upload_part_request_t *)malloc(sizeof(oss_upload_part_request_t));
	upr->bucket_name = NULL;
	upr->key = NULL;
	upr->input_stream = NULL;
	upr->md5_digest = NULL;
	upr->upload_id = NULL;
	upr->part_number = 0;
	upr->part_size = 0;
	upr->input_stream_len = 0;

	upr->get_bucket_name = _upload_part_request_get_bucket_name;
	upr->set_bucket_name = _upload_part_request_set_bucket_name;
	upr->get_key = _upload_part_request_get_key;
	upr->set_key = _upload_part_request_set_key;
	upr->get_input_stream = _upload_part_request_get_input_stream;
	upr->set_input_stream = _upload_part_request_set_input_stream;
	upr->get_md5_digest = _upload_part_request_get_md5_digest;
	upr->set_md5_digest = _upload_part_request_set_md5_digest;
	upr->get_upload_id = _upload_part_request_get_upload_id;
	upr->set_upload_id = _upload_part_request_set_upload_id;
	upr->get_part_number = _upload_part_request_get_part_number;
	upr->set_part_number = _upload_part_request_set_part_number;
	upr->get_part_size = _upload_part_request_get_part_size;
	upr->set_part_size = _upload_part_request_set_part_size;

	return upr;
}
