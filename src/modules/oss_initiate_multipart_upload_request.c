/*
 * =============================================================================
 *
 *       Filename:  oss_initiate_multipart_upload_request.c
 *
 *    Description:  initiate multipart upload request.
 *
 *        Created:  09/05/2012 02:31:41 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include <ossc/modules/oss_initiate_multipart_upload_request.h>
#undef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

static inline void
__initiate_multipart_upload_request_set_bucket_name(
		oss_initiate_multipart_upload_request_t *request,
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
__initiate_multipart_upload_request_set_key(
		oss_initiate_multipart_upload_request_t *request,
		const char *key, size_t key_len)
{

	assert(key != NULL);
	assert(key_len > 0);

	if (request->key != NULL) {
		free(request->key);
		request->key = NULL;
	}

	request->key= (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';
}


/**
 * 返回Bucket名称
 */
static inline const char *
_initiate_multipart_upload_request_get_bucket_name(
		oss_initiate_multipart_upload_request_t *request)
{
	return request->bucket_name;
}

/**
 * 返回OSSObject key
 */
static inline const char *
_initiate_multipart_upload_request_get_key(
		oss_initiate_multipart_upload_request_t *request)
{
	return request->key;
}

/**
 * 返回标识Multipart上传事件的Upload ID
 */
static inline oss_object_metadata_t *
_initiate_multipart_upload_request_get_object_metadata(
		oss_initiate_multipart_upload_request_t *request)
{
	return request->object_metadata;
}

/**
 * 设置Bucket名称
 */
static inline void
_initiate_multipart_upload_request_set_bucket_name(
		oss_initiate_multipart_upload_request_t *request,
		const char *bucket_name)
{
	size_t bucket_name_len = strlen(bucket_name);

	__initiate_multipart_upload_request_set_bucket_name(request,
			bucket_name, bucket_name_len);
}

/**
 * 设置OSSObject key
 */
static inline void
_initiate_multipart_upload_request_set_key(
		oss_initiate_multipart_upload_request_t *request,
		const char *key)
{
	size_t key_len = strlen(key);

	__initiate_multipart_upload_request_set_key(request,
			key, key_len);
}

static inline void
_initiate_multipart_upload_request_set_object_metadata(
		oss_initiate_multipart_upload_request_t *request,
		oss_object_metadata_t *object_metadata )
{
	assert(object_metadata != NULL);

	if (request->object_metadata!= NULL) {
		free(request->object_metadata);
		request->object_metadata = NULL;
	}
	request->object_metadata = object_metadata;
}

/**
 * 初始化
 */
static inline oss_initiate_multipart_upload_request_t *
_initiate_multipart_upload_request_initialize(
		const char *bucket_name, size_t bucket_name_len,
		const char *key, size_t key_len,
		oss_object_metadata_t *object_metadata )
{
	size_t byte_of_char = sizeof(char);

	oss_initiate_multipart_upload_request_t * request = NULL;
	request = (oss_initiate_multipart_upload_request_t *)malloc(
			sizeof(oss_initiate_multipart_upload_request_t));

	request->bucket_name = (char *)malloc(byte_of_char * bucket_name_len + 1);
	request->key = (char *)malloc(byte_of_char * key_len + 1);
	request->object_metadata = object_metadata;

	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	strncpy(request->key, key, key_len);

	(request->bucket_name)[bucket_name_len] = '\0';
	(request->key)[key_len] = '\0';

	request->get_bucket_name = _initiate_multipart_upload_request_get_bucket_name;
	request->get_key = _initiate_multipart_upload_request_get_key;
	request->get_object_metadata = _initiate_multipart_upload_request_get_object_metadata;
	request->set_bucket_name = _initiate_multipart_upload_request_set_bucket_name;
	request->set_key = _initiate_multipart_upload_request_set_key;
	request->set_object_metadata = _initiate_multipart_upload_request_set_object_metadata;

	return request;
}

/**
 * 初始化
 */
oss_initiate_multipart_upload_request_t *
initiate_multipart_upload_request_initialize(const char *bucket_name,
		const char *key)
{
	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);

	return _initiate_multipart_upload_request_initialize(
			bucket_name, bucket_name_len,
			key, key_len,
			NULL);

}

/**
 * 初始化
 */
oss_initiate_multipart_upload_request_t *
initiate_multipart_upload_request_initialize_with_metadata(const char *bucket_name,
		const char *key,
		oss_object_metadata_t *object_metadata)
{
	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);

	return _initiate_multipart_upload_request_initialize(
			bucket_name, bucket_name_len,
			key, key_len,
			object_metadata);

}

/**
 * 析构函数
 */
void
initiate_multipart_upload_request_finalize(
		oss_initiate_multipart_upload_request_t *request)
{
	assert(request != NULL);

	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}

	if (request->key!= NULL) {
		free(request->key);
		request->key = NULL;
	}

	if (request->object_metadata != NULL) {
		free(request->object_metadata);
		request->object_metadata = NULL;
	}

	free(request);
}
