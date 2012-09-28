/*
 * =============================================================================
 *
 *       Filename:  oss_initiate_multipart_upload_result.c
 *
 *    Description:  initiate multipart upload result implementation.
 *
 *        Created:  09/05/2012 12:21:41 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include <ossc/modules/oss_initiate_multipart_upload_result.h>
#undef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H

static inline void
__initiate_multipart_upload_result_set_bucket_name(
		oss_initiate_multipart_upload_result_t *result,
		const char *bucket_name, unsigned int bucket_name_len)
{
	assert(bucket_name != NULL);
	assert(bucket_name_len > 0);

	if (result->bucket_name != NULL) {
		free(result->bucket_name);
		result->bucket_name = NULL;
	}

	result->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(result->bucket_name, bucket_name, bucket_name_len);
	(result->bucket_name)[bucket_name_len] = '\0';
}

static inline void
__initiate_multipart_upload_result_set_key(
		oss_initiate_multipart_upload_result_t *result,
		const char *key, unsigned int key_len)
{

	assert(key != NULL);
	assert(key_len > 0);

	if (result->key != NULL) {
		free(result->key);
		result->key = NULL;
	}

	result->key= (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(result->key, key, key_len);
	(result->key)[key_len] = '\0';
}

static inline void
__initiate_multipart_upload_result_set_upload_id(
		oss_initiate_multipart_upload_result_t *result,
		const char *upload_id, unsigned int upload_id_len)
{
	assert(upload_id != NULL);
	assert(upload_id_len > 0);

	if (result->upload_id!= NULL) {
		free(result->upload_id);
		result->upload_id = NULL;
	}

	result->upload_id= (char *)malloc(sizeof(char) * upload_id_len+ 1);
	strncpy(result->upload_id, upload_id, upload_id_len);
	(result->upload_id)[upload_id_len] = '\0';
}

/**
 * 返回Bucket名称
 */
static inline const char *
_initiate_multipart_upload_result_get_bucket_name(
		oss_initiate_multipart_upload_result_t *result)
{
	return result->bucket_name;
}

/**
 * 返回OSSObject key
 */
static inline const char *
_initiate_multipart_upload_result_get_key(
		oss_initiate_multipart_upload_result_t *result)
{
	return result->key;
}

/**
 * 返回标识Multipart上传事件的Upload ID
 */
static inline const char *
_initiate_multipart_upload_result_get_upload_id(
		oss_initiate_multipart_upload_result_t *result)
{
	return result->upload_id;
}

/**
 * 设置Bucket名称
 */
static inline void
_initiate_multipart_upload_result_set_bucket_name(
		oss_initiate_multipart_upload_result_t *result,
		const char *bucket_name)
{
	unsigned int bucket_name_len = strlen(bucket_name);

	__initiate_multipart_upload_result_set_bucket_name(result,
			bucket_name, bucket_name_len);
}

/**
 * 设置OSSObject key
 */
static inline void
_initiate_multipart_upload_result_set_key(
		oss_initiate_multipart_upload_result_t *result,
		const char *key)
{
	unsigned int key_len = strlen(key);

	__initiate_multipart_upload_result_set_key(result,
			key, key_len);
}

/**
 * 设置标识Multipart上传事件的Upload ID
 */
static inline void
_initiate_multipart_upload_result_set_upload_id(
		oss_initiate_multipart_upload_result_t *result,
		const char *upload_id)
{
	unsigned int upload_id_len = strlen(upload_id);

	__initiate_multipart_upload_result_set_upload_id(result,
			upload_id, upload_id_len);
}

/**
 * 初始化
 */
static inline oss_initiate_multipart_upload_result_t *
_initiate_multipart_upload_result_initialize(
		const char *bucket_name, unsigned int bucket_name_len,
		const char *key, unsigned int key_len,
		const char *upload_id, unsigned int upload_id_len)
{
	unsigned int byte_of_char = sizeof(char);

	oss_initiate_multipart_upload_result_t * result = NULL;
	result = (oss_initiate_multipart_upload_result_t *)malloc(
			sizeof(oss_initiate_multipart_upload_result_t));

	result->bucket_name = (char *)malloc(byte_of_char * bucket_name_len + 1);
	result->key = (char *)malloc(byte_of_char * key_len + 1);
	result->upload_id = (char *)malloc(byte_of_char * upload_id_len + 1);

	strncpy(result->bucket_name, bucket_name, bucket_name_len);
	strncpy(result->key, key, key_len);
	strncpy(result->upload_id, upload_id, upload_id_len);

	(result->bucket_name)[bucket_name_len] = '\0';
	(result->key)[key_len] = '\0';
	(result->upload_id)[upload_id_len] = '\0';

	result->get_bucket_name = _initiate_multipart_upload_result_get_bucket_name;
	result->get_key = _initiate_multipart_upload_result_get_key;
	result->get_upload_id = _initiate_multipart_upload_result_get_upload_id;
	result->set_bucket_name = _initiate_multipart_upload_result_set_bucket_name;
	result->set_key = _initiate_multipart_upload_result_set_key;
	result->set_upload_id = _initiate_multipart_upload_result_set_upload_id;

	return result;
}

/**
 * 初始化
 */
oss_initiate_multipart_upload_result_t *
initiate_multipart_upload_result_initialize(void)
{
	return _initiate_multipart_upload_result_initialize(
			NULL, 0,
			NULL, 0,
			NULL, 0);

}

/**
 * 析构函数
 */
void
initiate_multipart_upload_result_finalize(
		oss_initiate_multipart_upload_result_t *result)
{
	assert(result != NULL);

	if (result->bucket_name != NULL) {
		free(result->bucket_name);
		result->bucket_name = NULL;
	}

	if (result->key!= NULL) {
		free(result->key);
		result->key = NULL;
	}

	if (result->upload_id != NULL) {
		free(result->upload_id);
		result->upload_id = NULL;
	}

	free(result);
}

