/*
 * =============================================================================
 *
 *       Filename:  oss_generate_presigned_url_request.c
 *
 *    Description:  oss_generate_presigned_url_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include <ossc/modules/oss_generate_presigned_url_request.h>
#undef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H

/**
 * 添加一个用户自定义的元数据
 */
static inline void 
_generate_presigned_url_request_add_user_metadata(
		oss_generate_presigned_url_request_t *request,
		const char *key,
		const char *value)
{
	oss_map_t *oss_map = request->user_metadata;
	oss_map_put(oss_map, key, value);
}

static const char * 
_generate_presigned_url_request_get_bucket_name(
		oss_generate_presigned_url_request_t *request)
{
	return request->bucket_name;
}

static inline void
__generate_presigned_url_request_set_bucket_name(
		oss_generate_presigned_url_request_t *request,
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
_generate_presigned_url_request_set_bucket_name(
		oss_generate_presigned_url_request_t *request,
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	__generate_presigned_url_request_set_bucket_name(request, bucket_name, bucket_name_len);
}

static const char *
_generate_presigned_url_request_get_expiration(
		oss_generate_presigned_url_request_t *request)
{
	return request->expiration;
}

static inline void
__generate_presigned_url_request_set_expiration(
		oss_generate_presigned_url_request_t *request,
		const char *expiration,
		unsigned int expiration_len)
{
	if (request->expiration != NULL) {
		free(request->expiration);
		request->expiration = NULL;
	}

	request->expiration = (char *)malloc(sizeof(char) * expiration_len + 1);
	strncpy(request->expiration, expiration, expiration_len);
	(request->expiration)[expiration_len] = '\0';
}

static inline void
_generate_presigned_url_request_set_expiration(
		oss_generate_presigned_url_request_t *request,
		const char *expiration)
{
	assert(expiration != NULL);

	unsigned int expiration_len = strlen(expiration);
	__generate_presigned_url_request_set_expiration(request, expiration, expiration_len);

}

static const char * 
_generate_presigned_url_request_get_key(
		oss_generate_presigned_url_request_t *request)
{
	return request->key;
}

static inline void
__generate_presigned_url_request_set_key(
		oss_generate_presigned_url_request_t *request,
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
_generate_presigned_url_request_set_key(
		oss_generate_presigned_url_request_t *request,
		const char *key)
{
	assert(key != NULL);

	unsigned int key_len = strlen(key);
	__generate_presigned_url_request_set_key(request, key, key_len);
}

static const char * 
_generate_presigned_url_request_get_method(
		oss_generate_presigned_url_request_t *request)
{
	return request->method;
}

static inline void
__generate_presigned_url_request_set_method(
		oss_generate_presigned_url_request_t *request,
		const char *method,
		unsigned int method_len)
{
	if (request->method != NULL) {
		free(request->method);
		request->method = NULL;
	}

	request->method = (char *)malloc(sizeof(char) * method_len + 1);
	strncpy(request->method, method, method_len);
	(request->method)[method_len] = '\0';
}

static void
_generate_presigned_url_request_set_method(
		oss_generate_presigned_url_request_t *request,
		const char *method)
{
	assert(method != NULL);

	unsigned int method_len = strlen(method);
	__generate_presigned_url_request_set_method(request, method, method_len);
}

/**
 * 获取用户自定义的元数据
 */
static inline oss_map_t *
_generate_presigned_url_request_get_user_metadata(
		oss_generate_presigned_url_request_t *request)
{
	oss_map_t *oss_map = request->user_metadata;
	return oss_map;
}

/**
 * 设置用户自定义的元数据，表示以x-oss-meta-为前缀的请求头
 */
static inline void
_generate_presigned_url_request_set_user_metadata(
		oss_generate_presigned_url_request_t *request,
		oss_map_t *user_metadata)
{
	request->user_metadata = user_metadata;
}

static oss_response_header_overrides_t * 
_generate_presigned_url_request_get_response_headers(
		oss_generate_presigned_url_request_t *request)
{
	return request->response_headers;
}

static inline void
_generate_presigned_url_request_set_response_headers(
		oss_generate_presigned_url_request_t *request,
		oss_response_header_overrides_t *response_headers)
{
	request->response_headers = response_headers;
	//if (request->response_header) {
	//	free(request->response_header);
	//	request->response_header = NULL;
	//}
	//request->response_header = (char *)malloc(sizeof(char) * response_header_len + 1);
	//strncpy(request->response_header, response_header, response_header_len);
	//(request->response_header)[response_header_len] = '\0';
}



oss_generate_presigned_url_request_t *
_generate_presigned_url_request_initialize(
		const char *bucket_name, unsigned int bucket_name_len,
		const char *key, unsigned int key_len)
{
	oss_generate_presigned_url_request_t *request;
	request = (oss_generate_presigned_url_request_t *)
			malloc(sizeof(oss_generate_presigned_url_request_t));

	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';

	request->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';

	request->method = NULL;
	request->response_headers = NULL;
	request->expiration = NULL;

	request->add_user_metadata = _generate_presigned_url_request_add_user_metadata;
	request->get_bucket_name = _generate_presigned_url_request_get_bucket_name;
	request->set_bucket_name = _generate_presigned_url_request_set_bucket_name;
	request->get_key = _generate_presigned_url_request_get_key;
	request->set_key = _generate_presigned_url_request_set_key;
	request->get_method = _generate_presigned_url_request_get_method;
	request->set_method = _generate_presigned_url_request_set_method;
	request->get_response_headers = _generate_presigned_url_request_get_response_headers;
	request->set_response_headers = _generate_presigned_url_request_set_response_headers;
	request->get_expiration = _generate_presigned_url_request_get_expiration;
	request->set_expiration = _generate_presigned_url_request_set_expiration;
	request->get_user_metadata = _generate_presigned_url_request_get_user_metadata;
	request->set_user_metadata = _generate_presigned_url_request_set_user_metadata;

	return request;
}

oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize(const char *bucket_name, const char *key)
{
	assert(bucket_name != NULL);
	assert(key != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	unsigned int key_len = strlen(key);

	return _generate_presigned_url_request_initialize(
			bucket_name, bucket_name_len, key, key_len);
}


oss_generate_presigned_url_request_t *
_generate_presigned_url_request_initialize_with_method(
		const char *bucket_name, unsigned int bucket_name_len,
		const char *key, unsigned int key_len, const char *method,
		unsigned int method_len)
{
	oss_generate_presigned_url_request_t *request;
	request = (oss_generate_presigned_url_request_t *)
			malloc(sizeof(oss_generate_presigned_url_request_t));

	if (request->bucket_name != NULL) {
		free(request->bucket_name);
		request->bucket_name = NULL;
	}
	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';

	if (request->key != NULL) {
		free(request->key);
		request->key = NULL;
	}
	request->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(request->key, key, key_len);
	(request->key)[key_len] = '\0';

	if (request->method != NULL) {
		free(request->method);
		request->method = NULL;
	}
	request->method = (char *)malloc(sizeof(char) * method_len + 1);
	strncpy(request->method, method, method_len);
	(request->method)[method_len] = '\0';


	request->response_headers = NULL;
	request->expiration = NULL;

	request->add_user_metadata = _generate_presigned_url_request_add_user_metadata;
	request->get_bucket_name = _generate_presigned_url_request_get_bucket_name;
	request->set_bucket_name = _generate_presigned_url_request_set_bucket_name;
	request->get_key = _generate_presigned_url_request_get_key;
	request->set_key = _generate_presigned_url_request_set_key;
	request->get_method = _generate_presigned_url_request_get_method;
	request->set_method = _generate_presigned_url_request_set_method;
	request->get_response_headers = _generate_presigned_url_request_get_response_headers;
	request->set_response_headers = _generate_presigned_url_request_set_response_headers;
	request->get_expiration = _generate_presigned_url_request_get_expiration;
	request->set_expiration = _generate_presigned_url_request_set_expiration;
	request->get_user_metadata = _generate_presigned_url_request_get_user_metadata;
	request->set_user_metadata = _generate_presigned_url_request_set_user_metadata;

	return request;
}

oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize_with_method(
		const char *bucket_name, const char *key, const char *method)
{
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(method != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	unsigned int key_len = strlen(key);
	unsigned int method_len = strlen(method);
	return _generate_presigned_url_request_initialize_with_method(
			bucket_name, bucket_name_len, key, key_len, method, method_len);
}

void
generate_presigned_url_request_finalize(
		oss_generate_presigned_url_request_t *request)
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

		if (request->method != NULL) {
			free(request->method);
			request->method = NULL;
		}

		if (request->expiration != NULL) {
			free(request->expiration);
			request->expiration = NULL;
		}

		if (request->response_headers != NULL) {
			request->response_headers = NULL;
		}
		/* *
		 * Here we INTEND to leave it un-freed,
		 * it's the user's responsbility to free it.
		 * */
		request->user_metadata = NULL;
		free(request);
	}
}
