/*
 * =============================================================================
 *
 *       Filename:  oss_list_multipart_uploads_request.c
 *
 *    Description:  list multipart uploads request implementation.
 *
 *        Created:  09/05/2012 11:37:51 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#include <ossc/modules/oss_list_multipart_uploads_request.h>
#undef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H

static inline void
__list_multipart_uploads_request_set_bucket_name(
		oss_list_multipart_uploads_request_t *request,
		const char *bucket_name, size_t bucket_name_len)
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
__list_multipart_uploads_request_set_prefix(
		oss_list_multipart_uploads_request_t *request,
		const char *prefix, size_t prefix_len)
{

	assert(prefix != NULL);
	assert(prefix_len > 0);

	if (request->prefix != NULL) {
		free(request->prefix);
		request->prefix = NULL;
	}

	request->prefix= (char *)malloc(sizeof(char) * prefix_len + 1);
	strncpy(request->prefix, prefix, prefix_len);
	(request->prefix)[prefix_len] = '\0';
}

static inline void
__list_multipart_uploads_request_set_key_marker(
		oss_list_multipart_uploads_request_t *request,
		const char *key_marker, size_t key_marker_len)
{
	assert(key_marker != NULL);
	assert(key_marker_len > 0);

	if (request->key_marker!= NULL) {
		free(request->key_marker);
		request->key_marker = NULL;
	}

	request->key_marker= (char *)malloc(sizeof(char) * key_marker_len+ 1);
	strncpy(request->key_marker, key_marker, key_marker_len);
	(request->key_marker)[key_marker_len] = '\0';
}

static inline void
__list_multipart_uploads_request_set_delimiter(
		oss_list_multipart_uploads_request_t *request,
		const char *delimiter, size_t delimiter_len)
{

	assert(delimiter!= NULL);
	assert(delimiter_len > 0);

	if (request->delimiter != NULL) {
		free(request->delimiter);
		request->delimiter = NULL;
	}

	request->delimiter = (char *)malloc(sizeof(char) * delimiter_len + 1);
	strncpy(request->delimiter, delimiter, delimiter_len);
	(request->delimiter)[delimiter_len] = '\0';
}

static inline void
__list_multipart_uploads_request_set_upload_id_marker(
		oss_list_multipart_uploads_request_t *request,
		const char *upload_id_marker, size_t upload_id_marker_len)
{

	assert(upload_id_marker!= NULL);
	assert(upload_id_marker_len > 0);

	if (request->upload_id_marker != NULL) {
		free(request->upload_id_marker);
		request->upload_id_marker= NULL;
	}

	request->upload_id_marker = (char *)malloc(sizeof(char) * upload_id_marker_len + 1);
	strncpy(request->upload_id_marker, upload_id_marker, upload_id_marker_len);
	(request->upload_id_marker)[upload_id_marker_len] = '\0';
}

static inline const char *
_list_multipart_uploads_request_get_bucket_name(
		oss_list_multipart_uploads_request_t *request)
{
	return request->bucket_name;
}

static inline const char *
_list_multipart_uploads_request_get_prefix(
		oss_list_multipart_uploads_request_t *request)
{
	return request->prefix;
}

static inline const char *
_list_multipart_uploads_request_get_key_marker(
		oss_list_multipart_uploads_request_t *request)
{
	return request->key_marker;
}

static inline const char *
_list_multipart_uploads_request_get_delimiter(
		oss_list_multipart_uploads_request_t *request)
{
	return request->delimiter;
}

static inline const char *
_list_multipart_uploads_request_get_upload_id_marker(
		oss_list_multipart_uploads_request_t *request)
{
	return request->upload_id_marker;
}

static inline unsigned int 
_list_multipart_uploads_request_get_max_uploads(
		oss_list_multipart_uploads_request_t *request)
{
	return request->max_uploads;
}

static inline void
_list_multipart_uploads_request_set_bucket_name(
		oss_list_multipart_uploads_request_t *request,
		const char *bucket_name)
{
	size_t bucket_name_len = strlen(bucket_name);

	__list_multipart_uploads_request_set_bucket_name(request,
			bucket_name, bucket_name_len);
}

static inline void
_list_multipart_uploads_request_set_prefix(
		oss_list_multipart_uploads_request_t *request,
		const char *prefix)
{
	size_t prefix_len = strlen(prefix);

	__list_multipart_uploads_request_set_prefix(request,
			prefix, prefix_len);
}

static inline void
_list_multipart_uploads_request_set_key_marker(
		oss_list_multipart_uploads_request_t *request,
		const char *key_marker)
{
	size_t key_marker_len = strlen(key_marker);

	__list_multipart_uploads_request_set_key_marker(request,
			key_marker, key_marker_len);
}

static inline void
_list_multipart_uploads_request_set_delimiter(
		oss_list_multipart_uploads_request_t *request,
		const char *delimiter)
{
	size_t delimiter_len = strlen(delimiter);

	__list_multipart_uploads_request_set_delimiter(request,
			delimiter, delimiter_len);
}

static inline void
_list_multipart_uploads_request_set_upload_id_marker(
		oss_list_multipart_uploads_request_t *request,
		const char *upload_id_marker)
{
	size_t upload_id_marker_len = strlen(upload_id_marker);

	__list_multipart_uploads_request_set_upload_id_marker(request,
			upload_id_marker, upload_id_marker_len);
}
static inline void
_list_multipart_uploads_request_set_max_uploads(
		oss_list_multipart_uploads_request_t *request,
		unsigned int max_uploads)
{
	request->max_uploads = max_uploads;
}

/**
 * 初始化
 */
static inline oss_list_multipart_uploads_request_t *
_list_multipart_uploads_request_initialize(
		const char *bucket_name, size_t bucket_name_len,
		const char *delimiter, size_t delimiter_len,
		const char *key_marker, size_t key_marker_len,
		const char *prefix, size_t prefix_len,
		const char *upload_id_marker, size_t upload_id_marker_len,
		unsigned int max_uploads)
{
	size_t byte_of_char = sizeof(char);

	oss_list_multipart_uploads_request_t * request = NULL;
	request = (oss_list_multipart_uploads_request_t *)malloc(
			sizeof(oss_list_multipart_uploads_request_t));

	request->bucket_name = (char *)malloc(byte_of_char * bucket_name_len + 1);
	request->prefix = (char *)malloc(byte_of_char * prefix_len + 1);
	request->key_marker = (char *)malloc(byte_of_char * key_marker_len + 1);
	request->delimiter = (char *)malloc(byte_of_char * delimiter_len + 1);
	request->upload_id_marker = (char *)malloc(byte_of_char * upload_id_marker_len+ 1);
	request->max_uploads = max_uploads;

	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	strncpy(request->prefix, prefix, prefix_len);
	strncpy(request->key_marker, key_marker, key_marker_len);
	strncpy(request->delimiter, delimiter, delimiter_len);
	strncpy(request->upload_id_marker, upload_id_marker, upload_id_marker_len);

	(request->bucket_name)[bucket_name_len] = '\0';
	(request->prefix)[prefix_len] = '\0';
	(request->key_marker)[key_marker_len] = '\0';
	(request->delimiter)[delimiter_len] = '\0';
	(request->upload_id_marker)[upload_id_marker_len] = '\0';

	request->get_bucket_name = _list_multipart_uploads_request_get_bucket_name;
	request->get_prefix = _list_multipart_uploads_request_get_prefix;
	request->get_key_marker= _list_multipart_uploads_request_get_key_marker;
	request->get_delimiter= _list_multipart_uploads_request_get_delimiter;
	request->get_upload_id_marker = _list_multipart_uploads_request_get_upload_id_marker;
	request->get_max_uploads= _list_multipart_uploads_request_get_max_uploads;

	request->set_bucket_name = _list_multipart_uploads_request_set_bucket_name;
	request->set_prefix= _list_multipart_uploads_request_set_prefix;
	request->set_key_marker= _list_multipart_uploads_request_set_key_marker;
	request->set_delimiter = _list_multipart_uploads_request_set_delimiter;
	request->set_upload_id_marker = _list_multipart_uploads_request_set_upload_id_marker;
	request->set_max_uploads= _list_multipart_uploads_request_set_max_uploads;

	return request;
}

/**
 * 初始化
 */
oss_list_multipart_uploads_request_t *
list_multipart_uploads_request_initialize(const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);

	return _list_multipart_uploads_request_initialize(bucket_name, bucket_name_len,
			NULL, 0,
			NULL, 0,
			NULL, 0,
			NULL, 0,
			0);
}

/**
 * 析构函数
 */
void
list_multipart_uploads_request_finalize(
		oss_list_multipart_uploads_request_t *request)
{
	assert(request != NULL);

	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	if (request->prefix!= NULL) {
		free(request->prefix);
		request->prefix= NULL;
	}

	if (request->key_marker!= NULL) {
		free(request->key_marker);
		request->key_marker= NULL;
	}

	if (request->delimiter!= NULL) {
		free(request->delimiter);
		request->delimiter= NULL;
	}

	free(request);
}

