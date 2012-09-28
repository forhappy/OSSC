/*
 * =============================================================================
 *
 *       Filename:  oss_list_parts_request.h
 *
 *    Description:  list parts request implementation.
 *
 *        Created:  09/05/2012 10:40:45 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_LIST_PARTS_REQUEST_H
#include <ossc/modules/oss_list_parts_request.h>
#undef _OSS_LIST_PARTS_REQUEST_H

static inline void
__list_parts_request_set_bucket_name(
		oss_list_parts_request_t *request,
		const char *bucket_name, unsigned int bucket_name_len)
{
	assert(bucket_name != NULL);
	assert(bucket_name_len > 0);

	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';
}

static inline void
__list_parts_request_set_upload_id(
		oss_list_parts_request_t *request,
		const char *upload_id, unsigned int upload_id_len)
{

	assert(upload_id != NULL);
	assert(upload_id_len > 0);

	if (request->upload_id != NULL) {
		free(request->upload_id);
		request->upload_id = NULL;
	}

	request->upload_id= (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(request->upload_id, upload_id, upload_id_len);
	(request->upload_id)[upload_id_len] = '\0';
}


static inline void
__list_parts_request_set_key(
		oss_list_parts_request_t *request,
		const char *key, unsigned int key_len)
{

	assert(key!= NULL);
	assert(key_len > 0);

	if (request->key != NULL) {
		free(request->key);
		request->key = NULL;
	}

	request->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';
}

static inline const char *
_list_parts_request_get_bucket_name(
		oss_list_parts_request_t *request)
{
	return request->bucket_name;
}

static inline const char *
_list_parts_request_get_upload_id(
		oss_list_parts_request_t *request)
{
	return request->upload_id;
}

static inline unsigned int 
_list_parts_request_get_max_parts(
		oss_list_parts_request_t *request)
{
	return request->max_parts;
}

static inline const char *
_list_parts_request_get_key(
		oss_list_parts_request_t *request)
{
	return request->key;
}

static inline unsigned int 
_list_parts_request_get_part_number_marker(
		oss_list_parts_request_t *request)
{
	return request->part_number_marker;
}

static inline void
_list_parts_request_set_bucket_name(
		oss_list_parts_request_t *request,
		const char *bucket_name)
{
	unsigned int bucket_name_len = strlen(bucket_name);

	__list_parts_request_set_bucket_name(request,
			bucket_name, bucket_name_len);
}

static inline void
_list_parts_request_set_upload_id(
		oss_list_parts_request_t *request,
		const char *upload_id)
{
	unsigned int upload_id_len = strlen(upload_id);

	__list_parts_request_set_upload_id(request,
			upload_id, upload_id_len);
}

static inline void
_list_parts_request_set_max_parts(
		oss_list_parts_request_t *request,
		unsigned int max_parts)
{
	request->max_parts = max_parts;
}

static inline void
_list_parts_request_set_key(
		oss_list_parts_request_t *request,
		const char *key)
{
	unsigned int key_len = strlen(key);

	__list_parts_request_set_key(request,
			key, key_len);
}

static inline void
_list_parts_request_set_part_number_marker(
		oss_list_parts_request_t *request,
		unsigned int part_number_marker)
{
	request->part_number_marker = part_number_marker;
}

/**
 * 初始化
 */
static inline oss_list_parts_request_t *
_list_parts_request_initialize(
		const char *bucket_name, unsigned int bucket_name_len,
		const char *key, unsigned int key_len,
		const char *upload_id, unsigned int upload_id_len)
{
	unsigned int byte_of_char = sizeof(char);

	oss_list_parts_request_t * request = NULL;

	request = (oss_list_parts_request_t *)malloc(
			sizeof(oss_list_parts_request_t));

	request->bucket_name = (char *)malloc(byte_of_char * bucket_name_len + 1);
	request->key = (char *)malloc(byte_of_char * key_len + 1);
	request->upload_id = (char *)malloc(byte_of_char * upload_id_len + 1);
	request->max_parts = 0;
	request->part_number_marker = 0;

	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	strncpy(request->key, key, key_len);
	strncpy(request->upload_id, upload_id, upload_id_len);

	(request->bucket_name)[bucket_name_len] = '\0';
	(request->key)[key_len] = '\0';
	(request->upload_id)[upload_id_len] = '\0';

	request->get_bucket_name = _list_parts_request_get_bucket_name;
	request->get_upload_id = _list_parts_request_get_upload_id;
	request->get_max_parts= _list_parts_request_get_max_parts;
	request->get_key= _list_parts_request_get_key;
	request->get_part_number_marker= _list_parts_request_get_part_number_marker;

	request->set_bucket_name = _list_parts_request_set_bucket_name;
	request->set_upload_id= _list_parts_request_set_upload_id;
	request->set_max_parts= _list_parts_request_set_max_parts;
	request->set_key = _list_parts_request_set_key;
	request->set_part_number_marker= _list_parts_request_set_part_number_marker;

	return request;
}

/**
 * 初始化
 */
oss_list_parts_request_t *
list_parts_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id)
{
	assert(bucket_name != NULL);
	assert(upload_id != NULL);
	assert(key != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	unsigned int upload_id_len = strlen(upload_id);
	unsigned int key_len = strlen(key);

	return _list_parts_request_initialize(bucket_name, bucket_name_len,
			key, key_len,
			upload_id, upload_id_len);
}

/**
 * 析构函数
 */
void
list_parts_request_finalize(
		oss_list_parts_request_t *request)
{
	assert(request != NULL);

	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	if (request->key!= NULL) {
		free(request->key);
		request->key= NULL;
	}

	if (request->upload_id!= NULL) {
		free(request->upload_id);
		request->upload_id= NULL;
	}

	free(request);
}
