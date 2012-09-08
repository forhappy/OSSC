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
#include <modules/oss_generate_presigned_url_request.h>
#undef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H



void 
generate_presigned_url_request_finalize(oss_generate_presigned_url_request_t *gpur)
{
	if (gpur) {
		if (gpur->bucket_name) {
			free(gpur->bucket_name);
			gpur->bucket_name = NULL;
		}
		if (gpur->key) {
			free(gpur->key);
			gpur->key = NULL;
		}
		if (gpur->method) {
			free(gpur->method);
			gpur->method = NULL;
		}
		if (gpur->expiration) {
			free(gpur->expiration);
			gpur->expiration = NULL;
		}
		if (gpur->response_headers) {
			gpur->response_headers = NULL;
		}
		/* *
		 * Here we INTEND to leave it un-freed,
		 * it's the user's responsbility to free it.
		 * */
		gpur->user_metadata = NULL;
		free(gpur);
		gpur = NULL;
	}
}

/* *
 * 添加一个用户自定义的元数据
 * */
static inline void 
_generate_presigned_url_request_add_user_metadata(
		oss_generate_presigned_url_request_t *gpur,
		const char *key,
		const char *value)
{
	oss_map_t *oss_map = gpur->user_metadata;
	oss_map_put(oss_map, key, value);
}

static const char * 
_generate_presigned_url_request_get_bucket_name(oss_generate_presigned_url_request_t *gpur)
{
	return gpur->bucket_name;
}

static inline void
__generate_presigned_url_request_set_bucket_name(
		oss_generate_presigned_url_request_t *gpur, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (gpur->bucket_name) {
		free(gpur->bucket_name);
		gpur->bucket_name = NULL;
	}
	gpur->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(gpur->bucket_name, bucket_name, bucket_name_len);
	(gpur->bucket_name)[bucket_name_len] = '\0';
}

static void
_generate_presigned_url_request_set_bucket_name(
		oss_generate_presigned_url_request_t *gpur, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__generate_presigned_url_request_set_bucket_name(gpur, bucket_name, bucket_name_len);
}

static const char *
_generate_presigned_url_request_get_expiration(oss_generate_presigned_url_request_t *gpur)
{
	return gpur->expiration;
}

static inline void
__generate_presigned_url_request_set_expiration(
		oss_generate_presigned_url_request_t *gpur, 
		const char *expiration,
		size_t expiration_len)
{
	if (gpur->expiration) {
		free(gpur->expiration);
		gpur->expiration = NULL;
	}
	gpur->expiration = (char *)malloc(sizeof(char) * expiration_len + 1);
	strncpy(gpur->expiration, expiration, expiration_len);
	(gpur->expiration)[expiration_len] = '\0';
}

static inline void
_generate_presigned_url_request_set_expiration(
		oss_generate_presigned_url_request_t *gpur, 
		const char *expiration)
{
	assert(expiration != NULL);
	size_t expiration_len = strlen(expiration);
	__generate_presigned_url_request_set_expiration(gpur, expiration, expiration_len);

}



static const char * 
_generate_presigned_url_request_get_key(oss_generate_presigned_url_request_t *gpur)
{
	return gpur->key;
}

static inline void
__generate_presigned_url_request_set_key(
		oss_generate_presigned_url_request_t *gpur, 
		const char *key,
		size_t key_len)
{
	if (gpur->key) {
		free(gpur->key);
		gpur->key = NULL;
	}
	gpur->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(gpur->key, key, key_len);
	(gpur->key)[key_len] = '\0';
}

static void
_generate_presigned_url_request_set_key(
		oss_generate_presigned_url_request_t *gpur, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__generate_presigned_url_request_set_key(gpur, key, key_len);
}

static const char * 
_generate_presigned_url_request_get_method(oss_generate_presigned_url_request_t *gpur)
{
	return gpur->method;
}

static inline void
__generate_presigned_url_request_set_method(
		oss_generate_presigned_url_request_t *gpur, 
		const char *method,
		size_t method_len)
{
	if (gpur->method) {
		free(gpur->method);
		gpur->method = NULL;
	}
	gpur->method = (char *)malloc(sizeof(char) * method_len + 1);
	strncpy(gpur->method, method, method_len);
	(gpur->method)[method_len] = '\0';
}

static void
_generate_presigned_url_request_set_method(
		oss_generate_presigned_url_request_t *gpur, 
		const char *method)
{
	assert(method != NULL);
	size_t method_len = strlen(method);
	__generate_presigned_url_request_set_method(gpur, method, method_len);
}

/* *
 * 获取用户自定义的元数据
 * */
static inline oss_map_t *
_generate_presigned_url_request_get_user_metadata(
		oss_generate_presigned_url_request_t *gpur)
{
	oss_map_t *oss_map = gpur->user_metadata;
	return oss_map;
}

/* *
 * 设置用户自定义的元数据，表示以x-oss-meta-为前缀的请求头
 * */
static inline void
_generate_presigned_url_request_set_user_metadata(
		oss_generate_presigned_url_request_t *gpur,
		oss_map_t *user_metadata)
{
	gpur->user_metadata = user_metadata;
}

static oss_response_header_overrides_t * 
_generate_presigned_url_request_get_response_headers(oss_generate_presigned_url_request_t *gpur)
{
	return gpur->response_headers;
}

static inline void
_generate_presigned_url_request_set_response_headers(
		oss_generate_presigned_url_request_t *gpur, 
		oss_response_header_overrides_t *response_headers)
{
	gpur->response_headers = response_headers;
	//if (gpur->response_header) {
	//	free(gpur->response_header);
	//	gpur->response_header = NULL;
	//}
	//gpur->response_header = (char *)malloc(sizeof(char) * response_header_len + 1);
	//strncpy(gpur->response_header, response_header, response_header_len);
	//(gpur->response_header)[response_header_len] = '\0';
}



oss_generate_presigned_url_request_t *
_generate_presigned_url_request_initialize(const char *bucket_name, size_t bucket_name_len, const char *key, size_t key_len)
{
	oss_generate_presigned_url_request_t *gpur;
	gpur = (oss_generate_presigned_url_request_t *)malloc(sizeof(oss_generate_presigned_url_request_t));
	if (gpur->bucket_name) {
		free(gpur->bucket_name);
		gpur->bucket_name = NULL;
	}
	gpur->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(gpur->bucket_name, bucket_name, bucket_name_len);
	(gpur->bucket_name)[bucket_name_len] = '\0';

	if (gpur->key) {
		free(gpur->key);
		gpur->key = NULL;
	}
	gpur->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(gpur->key, key, key_len);
	(gpur->key)[key_len] = '\0';

	gpur->method = NULL;
	gpur->response_headers = NULL;
	gpur->expiration = NULL;

	gpur->add_user_metadata = _generate_presigned_url_request_add_user_metadata;
	gpur->get_bucket_name = _generate_presigned_url_request_get_bucket_name;
	gpur->set_bucket_name = _generate_presigned_url_request_set_bucket_name;
	gpur->get_key = _generate_presigned_url_request_get_key;
	gpur->set_key = _generate_presigned_url_request_set_key;
	gpur->get_method = _generate_presigned_url_request_get_method;
	gpur->set_method = _generate_presigned_url_request_set_method;
	gpur->get_response_headers = _generate_presigned_url_request_get_response_headers;
	gpur->set_response_headers = _generate_presigned_url_request_set_response_headers;
	gpur->get_expiration = _generate_presigned_url_request_get_expiration;
	gpur->set_expiration = _generate_presigned_url_request_set_expiration;
	gpur->get_user_metadata = _generate_presigned_url_request_get_user_metadata;
	gpur->set_user_metadata = _generate_presigned_url_request_set_user_metadata;
	return gpur;
}

oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize(const char *bucket_name, const char *key)
{
	assert(bucket_name != NULL);
	assert(key != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	return _generate_presigned_url_request_initialize(bucket_name, bucket_name_len, key, key_len);
}


oss_generate_presigned_url_request_t *
_generate_presigned_url_request_initialize_with_method(const char *bucket_name, size_t bucket_name_len, const char *key, size_t key_len, const char *method, size_t method_len)
{
	oss_generate_presigned_url_request_t *gpur;
	gpur = (oss_generate_presigned_url_request_t *)malloc(sizeof(oss_generate_presigned_url_request_t));
	if (gpur->bucket_name) {
		free(gpur->bucket_name);
		gpur->bucket_name = NULL;
	}
	gpur->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(gpur->bucket_name, bucket_name, bucket_name_len);
	(gpur->bucket_name)[bucket_name_len] = '\0';

	if (gpur->key) {
		free(gpur->key);
		gpur->key = NULL;
	}
	gpur->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(gpur->key, key, key_len);
	(gpur->key)[key_len] = '\0';

	if (gpur->method) {
		free(gpur->method);
		gpur->method = NULL;
	}
	gpur->method = (char *)malloc(sizeof(char) * method_len + 1);
	strncpy(gpur->method, method, method_len);
	(gpur->method)[method_len] = '\0';


	gpur->response_headers = NULL;
	gpur->expiration = NULL;

	gpur->add_user_metadata = _generate_presigned_url_request_add_user_metadata;
	gpur->get_bucket_name = _generate_presigned_url_request_get_bucket_name;
	gpur->set_bucket_name = _generate_presigned_url_request_set_bucket_name;
	gpur->get_key = _generate_presigned_url_request_get_key;
	gpur->set_key = _generate_presigned_url_request_set_key;
	gpur->get_method = _generate_presigned_url_request_get_method;
	gpur->set_method = _generate_presigned_url_request_set_method;
	gpur->get_response_headers = _generate_presigned_url_request_get_response_headers;
	gpur->set_response_headers = _generate_presigned_url_request_set_response_headers;
	gpur->get_expiration = _generate_presigned_url_request_get_expiration;
	gpur->set_expiration = _generate_presigned_url_request_set_expiration;
	gpur->get_user_metadata = _generate_presigned_url_request_get_user_metadata;
	gpur->set_user_metadata = _generate_presigned_url_request_set_user_metadata;

	return gpur;
}

oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize_with_method(const char *bucket_name, const char *key, const char *method)
{
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(method != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	size_t method_len = strlen(method);
	return _generate_presigned_url_request_initialize_with_method(bucket_name, bucket_name_len, key, key_len, method, method_len);
}
