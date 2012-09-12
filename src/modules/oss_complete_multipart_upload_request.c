/*
 * =============================================================================
 *
 *       Filename:  oss_complete_multipart_upload_request.c
 *
 *    Description:  oss_complete_multipart_upload_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#include <ossc/modules/oss_complete_multipart_upload_request.h>
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H

static const char * 
_complete_multipart_upload_request_get_bucket_name(oss_complete_multipart_upload_request_t *request)
{
	return request->bucket_name;
}

static inline void
__complete_multipart_upload_request_set_bucket_name(
		oss_complete_multipart_upload_request_t *request, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (request->bucket_name) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';
}

static void
_complete_multipart_upload_request_set_bucket_name(
		oss_complete_multipart_upload_request_t *request, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__complete_multipart_upload_request_set_bucket_name(request, bucket_name, bucket_name_len);
}

static oss_part_etag_t ** 
_complete_multipart_upload_request_get_part_etags(oss_complete_multipart_upload_request_t *request, int *part_etags_number)
{
	*part_etags_number = request->part_etags_number;
	return request->part_etags;
}

static void
_complete_multipart_upload_request_set_part_etags(
		oss_complete_multipart_upload_request_t *request, 
		oss_part_etag_t **part_etags,
		int part_etags_number)
{
	assert(part_etags != NULL);
	assert(request != NULL);

	request->part_etags_number = part_etags_number;
	request->part_etags = part_etags;
}


static const char * 
_complete_multipart_upload_request_get_key(oss_complete_multipart_upload_request_t *request)
{
	return request->key;
}

static inline void
__complete_multipart_upload_request_set_key(
		oss_complete_multipart_upload_request_t *request, 
		const char *key,
		size_t key_len)
{
	if (request->key) {
		free(request->key);
		request->key = NULL;
	}

	request->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';
}

static void
_complete_multipart_upload_request_set_key(
		oss_complete_multipart_upload_request_t *request, 
		const char *key)
{
	assert(key != NULL);

	size_t key_len = strlen(key);
	__complete_multipart_upload_request_set_key(request, key, key_len);
}

static const char * 
_complete_multipart_upload_request_get_upload_id(
		oss_complete_multipart_upload_request_t *request)
{
	return request->upload_id;
}

static inline void
__complete_multipart_upload_request_set_upload_id(
		oss_complete_multipart_upload_request_t *request, 
		const char *upload_id,
		size_t upload_id_len)
{
	if (request->upload_id) {
		free(request->upload_id);
		request->upload_id = NULL;
	}

	request->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(request->upload_id, upload_id, upload_id_len);
	(request->upload_id)[upload_id_len] = '\0';
}

static void
_complete_multipart_upload_request_set_upload_id(
		oss_complete_multipart_upload_request_t *request, 
		const char *upload_id)
{
	assert(upload_id != NULL);

	size_t upload_id_len = strlen(upload_id);
	__complete_multipart_upload_request_set_upload_id(request, upload_id, upload_id_len);
}

oss_complete_multipart_upload_request_t *
complete_multipart_upload_request_initialize(
		const char *bucket_name,
		const char *key,
		const char *upload_id,
		oss_part_etag_t **part_etags,
		int part_etags_number)
{
	oss_complete_multipart_upload_request_t *request;
	request = (oss_complete_multipart_upload_request_t *)malloc(sizeof(oss_complete_multipart_upload_request_t));

	if (request->bucket_name) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	size_t bucket_name_len = strlen(bucket_name);
	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';

	if (request->key) {
		free(request->key);
		request->key = NULL;
	}

	size_t key_len = strlen(key);
	request->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';
	
	if (request->upload_id) {
		free(request->upload_id);
		request->upload_id = NULL;
	}

	size_t upload_id_len = strlen(upload_id);
	request->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(request->upload_id, upload_id, upload_id_len);
	(request->upload_id)[upload_id_len] = '\0';
	
	request->part_etags = part_etags;
	request->part_etags_number = part_etags_number;
	request->get_bucket_name = _complete_multipart_upload_request_get_bucket_name;
	request->set_bucket_name = _complete_multipart_upload_request_set_bucket_name;
	request->get_key = _complete_multipart_upload_request_get_key;
	request->set_key = _complete_multipart_upload_request_set_key;
	request->get_upload_id = _complete_multipart_upload_request_get_upload_id;
	request->set_upload_id = _complete_multipart_upload_request_set_upload_id;
	request->get_part_etags = _complete_multipart_upload_request_get_part_etags;
	request->set_part_etags = _complete_multipart_upload_request_set_part_etags;

	return request;
}

void
complete_multipart_upload_request_finalize(
		oss_complete_multipart_upload_request_t *request)
{
	if (request != NULL) {
		if (request->bucket_name) {
			free(request->bucket_name);
			request->bucket_name = NULL;
		}

		if (request->part_etags) {
			request->part_etags = NULL;
		}

		if (request->key) {
			free(request->key);
			request->key = NULL;
		}

		if (request->upload_id) {
			free(request->upload_id);
			request->upload_id = NULL;
		}

		free(request);
	}
}
