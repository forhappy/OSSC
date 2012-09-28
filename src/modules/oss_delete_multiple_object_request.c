/*
 * =============================================================================
 *
 *       Filename:  oss_delete_multiple_object_request.c
 *
 *    Description:  oss_delete_multiple_object_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
#include <ossc/modules/oss_delete_multiple_object_request.h>
#undef _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H


static const char * 
_delete_multiple_object_request_get_bucket_name(
		oss_delete_multiple_object_request_t *request)
{
	return request->bucket_name;
}

static inline void
__delete_multiple_object_request_set_bucket_name(
		oss_delete_multiple_object_request_t *request, 
		const char *bucket_name,
		unsigned int bucket_name_len)
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
_delete_multiple_object_request_set_bucket_name(
		oss_delete_multiple_object_request_t *request, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	unsigned int bucket_name_len = strlen(bucket_name);
	__delete_multiple_object_request_set_bucket_name(request, bucket_name, bucket_name_len);
}

static const char **
_delete_multiple_object_request_get_keys(
		oss_delete_multiple_object_request_t *request,
		int *keynums)
{
	assert(request != NULL);

	*keynums = request->keynums;
	return (const char **)(request->keys);
}

static inline void
__delete_multiple_object_request_set_key(
		oss_delete_multiple_object_request_t *request, 
		const char *key,
		unsigned int key_len)
{
}

static void
_delete_multiple_object_request_set_keys(
		oss_delete_multiple_object_request_t *request, 
		const char **keys, int keynums)
{
	assert(keys != NULL);

	/* *
	 * If request->keys != NULL,
	 * free it one by one.
	 * */
	unsigned int j = 0;
	unsigned int total = request->keynums;
	if (request->keys != NULL) {
		for (j = 0; j < total; j++) {
			if (*(request->keys + j) != NULL) {
				free(*(request->keys + j));
				*(request->keys + j) = NULL;
			}
		}
	}

	unsigned int i = 0;
	const char **pk = keys;

	request->keys = (char **)malloc(sizeof(char *) * keynums);
	
	for (i = 0; i < keynums; i++) {
		unsigned int len = strlen(*(pk + i));
		*(request->keys + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(request->keys + i), '\0', len + 1);
		strncpy(*(request->keys + i), *(pk + i), len);
	}
	request->keynums = keynums;
}

static inline bool
_delete_multiple_object_request_get_mode(
		oss_delete_multiple_object_request_t *request)
{
	assert(request != NULL);

	return request->mode;
}

static inline void
_delete_multiple_object_request_set_mode(
		oss_delete_multiple_object_request_t *request, bool mode)
{
	assert(request != NULL);

	request->mode = mode;
}
oss_delete_multiple_object_request_t *
delete_multiple_object_request_initialize(
		const char *bucket_name,
		const char **keys,
		int keynums,
		bool mode)
{
	int i = 0;
	oss_delete_multiple_object_request_t *request;
	request = (oss_delete_multiple_object_request_t *)malloc(sizeof(oss_delete_multiple_object_request_t));

	unsigned int bucket_name_len = strlen(bucket_name);
	request->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	(request->bucket_name)[bucket_name_len] = '\0';

	request->keys = (char **)malloc(sizeof(char *) * keynums);

	const char **pk = keys;
	for (i = 0; i < keynums; i++) {
		unsigned int len = strlen(*(pk + i));
		*(request->keys + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(request->keys + i), '\0', len + 1);
		strncpy(*(request->keys + i), *(pk + i), len);
	}

	request->keynums = keynums;
	request->mode = mode;

	request->get_bucket_name = _delete_multiple_object_request_get_bucket_name;
	request->set_bucket_name = _delete_multiple_object_request_set_bucket_name;
	request->get_keys = _delete_multiple_object_request_get_keys;
	request->set_keys = _delete_multiple_object_request_set_keys;
	request->get_mode = _delete_multiple_object_request_get_mode;
	request->set_mode = _delete_multiple_object_request_set_mode;

	return request;
}

void
delete_multiple_object_request_finalize(
		oss_delete_multiple_object_request_t *request)
{
	if (request != NULL) {
		if (request->bucket_name != NULL) {
			free(request->bucket_name);
			request->bucket_name = NULL;
		}

		if (request->keys != NULL) {
			unsigned int j = 0;
			unsigned int total = request->keynums;
			if (request->keys != NULL) {
				for (j = 0; j < total; j++) {
					if (*(request->keys + j) != NULL) {
						free(*(request->keys + j));
						*(request->keys + j) = NULL;
					}
				}
			}
			free(request->keys);
		}

		free(request);
	}
}
