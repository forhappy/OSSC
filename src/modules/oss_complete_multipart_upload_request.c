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
#include <modules/oss_complete_multipart_upload_request.h>
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H



void 
complete_multipart_upload_request_finalize(oss_complete_multipart_upload_request_t *cmur)
{
	if (cmur) {
		if (cmur->bucket_name) {
			free(cmur->bucket_name);
			cmur->bucket_name = NULL;
		}
		if (cmur->part_etags) {
			cmur->part_etags = NULL;
		}
		if (cmur->key) {
			free(cmur->key);
			cmur->key = NULL;
		}
		if (cmur->upload_id) {
			free(cmur->upload_id);
			cmur->upload_id = NULL;
		}
		free(cmur);
		cmur = NULL;
	}
}

static const char * 
_complete_multipart_upload_request_get_bucket_name(oss_complete_multipart_upload_request_t *cmur)
{
	return cmur->bucket_name;
}

static inline void
__complete_multipart_upload_request_set_bucket_name(
		oss_complete_multipart_upload_request_t *cmur, 
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
_complete_multipart_upload_request_set_bucket_name(
		oss_complete_multipart_upload_request_t *cmur, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__complete_multipart_upload_request_set_bucket_name(cmur, bucket_name, bucket_name_len);
}

static oss_part_etag_t ** 
_complete_multipart_upload_request_get_part_etags(oss_complete_multipart_upload_request_t *cmur, int *part_etags_number)
{
	*part_etags_number = cmur->part_etags_number;
	return cmur->part_etags;
}

static void
_complete_multipart_upload_request_set_part_etags(
		oss_complete_multipart_upload_request_t *cmur, 
		oss_part_etag_t **part_etags,
		int part_etags_number)
{
	assert(part_etags != NULL);
	assert(cmur != NULL);
	cmur->part_etags_number = part_etags_number;
	cmur->part_etags = part_etags;
}


static const char * 
_complete_multipart_upload_request_get_key(oss_complete_multipart_upload_request_t *cmur)
{
	return cmur->key;
}

static inline void
__complete_multipart_upload_request_set_key(
		oss_complete_multipart_upload_request_t *cmur, 
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
_complete_multipart_upload_request_set_key(
		oss_complete_multipart_upload_request_t *cmur, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__complete_multipart_upload_request_set_key(cmur, key, key_len);
}

static const char * 
_complete_multipart_upload_request_get_upload_id(oss_complete_multipart_upload_request_t *cmur)
{
	return cmur->upload_id;
}

static inline void
__complete_multipart_upload_request_set_upload_id(
		oss_complete_multipart_upload_request_t *cmur, 
		const char *upload_id,
		size_t upload_id_len)
{
	if (cmur->upload_id) {
		free(cmur->upload_id);
		cmur->upload_id = NULL;
	}
	cmur->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(cmur->upload_id, upload_id, upload_id_len);
	(cmur->upload_id)[upload_id_len] = '\0';
}

static void
_complete_multipart_upload_request_set_upload_id(
		oss_complete_multipart_upload_request_t *cmur, 
		const char *upload_id)
{
	assert(upload_id != NULL);
	size_t upload_id_len = strlen(upload_id);
	__complete_multipart_upload_request_set_upload_id(cmur, upload_id, upload_id_len);
}

oss_complete_multipart_upload_request_t *
complete_multipart_upload_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id,
		oss_part_etag_t **part_etags,
		int part_etags_number)
{
	oss_complete_multipart_upload_request_t *cmur;
	cmur = (oss_complete_multipart_upload_request_t *)malloc(sizeof(oss_complete_multipart_upload_request_t));

	if (cmur->bucket_name) {
		free(cmur->bucket_name);
		cmur->bucket_name = NULL;
	}

	size_t bucket_name_len = strlen(bucket_name);
	cmur->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(cmur->bucket_name, bucket_name, bucket_name_len);
	(cmur->bucket_name)[bucket_name_len] = '\0';

	if (cmur->key) {
		free(cmur->key);
		cmur->key = NULL;
	}
	size_t key_len = strlen(key);
	cmur->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(cmur->key, key, key_len);
	(cmur->key)[key_len] = '\0';
	
	if (cmur->upload_id) {
		free(cmur->upload_id);
		cmur->upload_id = NULL;
	}
	size_t upload_id_len = strlen(upload_id);
	cmur->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(cmur->upload_id, upload_id, upload_id_len);
	(cmur->upload_id)[upload_id_len] = '\0';
	
	cmur->part_etags = part_etags;
	cmur->part_etags_number = part_etags_number;

	cmur->get_bucket_name = _complete_multipart_upload_request_get_bucket_name;
	cmur->set_bucket_name = _complete_multipart_upload_request_set_bucket_name;
	cmur->get_key = _complete_multipart_upload_request_get_key;
	cmur->set_key = _complete_multipart_upload_request_set_key;
	cmur->get_upload_id = _complete_multipart_upload_request_get_upload_id;
	cmur->set_upload_id = _complete_multipart_upload_request_set_upload_id;
	cmur->get_part_etags = _complete_multipart_upload_request_get_part_etags;
	cmur->set_part_etags = _complete_multipart_upload_request_set_part_etags;

	return cmur;
}
