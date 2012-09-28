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
#include <ossc/modules/oss_upload_part_request.h>
#undef _OSS_UPLOAD_PART_REQUEST_H

static const char * 
_upload_part_request_get_bucket_name(oss_upload_part_request_t *request)
{
	return request->bucket_name;
}

static inline void
__upload_part_request_set_bucket_name(
		oss_upload_part_request_t *request,
		const char *bucket_name,
		unsigned int bucket_name_len)
{
	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';
}

static void
_upload_part_request_set_bucket_name(
		oss_upload_part_request_t *request,
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	__upload_part_request_set_bucket_name(request, bucket_name, bucket_name_len);
}

static const char * 
_upload_part_request_get_key(oss_upload_part_request_t *request)
{
	return request->key;
}

static inline void
__upload_part_request_set_key(
		oss_upload_part_request_t *request,
		const char *key,
		unsigned int key_len)
{
	if (request->key != NULL) {
		free(request->key);
		request->key = NULL;
	}
	request->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';
}

static void
_upload_part_request_set_key(
		oss_upload_part_request_t *request,
		const char *key)
{
	assert(key != NULL);

	unsigned int key_len = strlen(key);
	__upload_part_request_set_key(request, key, key_len);
}

static const char * 
_upload_part_request_get_input_stream(oss_upload_part_request_t *request, int *input_stream_len)
{
	*input_stream_len = request->input_stream_len;
	return request->input_stream;
}

static inline void
__upload_part_request_set_input_stream(
		oss_upload_part_request_t *request,
		const char *input_stream,
		unsigned int input_stream_len)
{
	if (request->input_stream != NULL) {
		free(request->input_stream);
		request->input_stream = NULL;
	}

	request->input_stream_len = input_stream_len;
	request->input_stream = (char *)malloc(sizeof(char) * input_stream_len + 1);
	memcpy(request->input_stream, input_stream, input_stream_len);
	(request->input_stream)[input_stream_len] = '\0';
}

static void
_upload_part_request_set_input_stream(
		oss_upload_part_request_t *request,
		const char *input_stream, unsigned int input_stream_len)
{
	assert(input_stream != NULL);

	assert(input_stream_len > 0);
	__upload_part_request_set_input_stream(request, input_stream, input_stream_len);
}

static const char * 
_upload_part_request_get_md5_digest(oss_upload_part_request_t *request)
{
	return request->md5_digest;
}

static inline void
__upload_part_request_set_md5_digest(
		oss_upload_part_request_t *request,
		const char *md5_digest,
		unsigned int md5_digest_len)
{
	if (request->md5_digest != NULL) {
		free(request->md5_digest);
		request->md5_digest = NULL;
	}

	request->md5_digest = (char *)malloc(sizeof(char) * md5_digest_len + 1);
	strncpy(request->md5_digest, md5_digest, md5_digest_len);
	(request->md5_digest)[md5_digest_len] = '\0';
}

static void
_upload_part_request_set_md5_digest(
		oss_upload_part_request_t *request,
		const char *md5_digest)
{
	assert(md5_digest != NULL);

	unsigned int md5_digest_len = strlen(md5_digest);
	__upload_part_request_set_md5_digest(request, md5_digest, md5_digest_len);
}

static const char * 
_upload_part_request_get_upload_id(oss_upload_part_request_t *request)
{
	return request->upload_id;
}

static inline void
__upload_part_request_set_upload_id(
		oss_upload_part_request_t *request,
		const char *upload_id,
		unsigned int upload_id_len)
{
	if (request->upload_id != NULL) {
		free(request->upload_id);
		request->upload_id = NULL;
	}

	request->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(request->upload_id, upload_id, upload_id_len);
	(request->upload_id)[upload_id_len] = '\0';
}

static void
_upload_part_request_set_upload_id(
		oss_upload_part_request_t *request,
		const char *upload_id)
{
	assert(upload_id != NULL);

	unsigned int upload_id_len = strlen(upload_id);
	__upload_part_request_set_upload_id(request, upload_id, upload_id_len);
}

static int 
_upload_part_request_get_part_number(oss_upload_part_request_t *request)
{
	return request->part_number;
}

static void
_upload_part_request_set_part_number(
		oss_upload_part_request_t *request,
		int part_number)
{
	request->part_number = part_number;
}

static long 
_upload_part_request_get_part_size(oss_upload_part_request_t *request)
{
	return request->part_size;
}

static void
_upload_part_request_set_part_size(
		oss_upload_part_request_t *request,
		long part_size)
{
	request->part_size = part_size;
}

oss_upload_part_request_t *
upload_part_request_initialize(void)
{
	oss_upload_part_request_t *request;
	request = (oss_upload_part_request_t *)malloc(sizeof(oss_upload_part_request_t));
	request->bucket_name = NULL;
	request->key = NULL;
	request->input_stream = NULL;
	request->md5_digest = NULL;
	request->upload_id = NULL;
	request->part_number = 0;
	request->part_size = 0;
	request->input_stream_len = 0;

	request->get_bucket_name = _upload_part_request_get_bucket_name;
	request->set_bucket_name = _upload_part_request_set_bucket_name;
	request->get_key = _upload_part_request_get_key;
	request->set_key = _upload_part_request_set_key;
	request->get_input_stream = _upload_part_request_get_input_stream;
	request->set_input_stream = _upload_part_request_set_input_stream;
	request->get_md5_digest = _upload_part_request_get_md5_digest;
	request->set_md5_digest = _upload_part_request_set_md5_digest;
	request->get_upload_id = _upload_part_request_get_upload_id;
	request->set_upload_id = _upload_part_request_set_upload_id;
	request->get_part_number = _upload_part_request_get_part_number;
	request->set_part_number = _upload_part_request_set_part_number;
	request->get_part_size = _upload_part_request_get_part_size;
	request->set_part_size = _upload_part_request_set_part_size;

	return request;
}

void
upload_part_request_finalize(oss_upload_part_request_t *request)
{

	if (request != NULL) {
		if (request->bucket_name != NULL) {
			free(request->bucket_name);
			request->bucket_name = NULL;
		}
		if (request->key != NULL) {
			free(request->key);
			request->key = NULL;
		}
		if (request->input_stream != NULL) {
			free(request->input_stream);
			request->input_stream = NULL;
		}
		if (request->md5_digest != NULL) {
			free(request->md5_digest);
			request->md5_digest = NULL;
		}
		if (request->upload_id != NULL) {
			free(request->upload_id);
			request->upload_id = NULL;
		}
		free(request);
	}
}
