/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_group_request.c
 *
 *    Description:  oss_get_object_group_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_GET_OBJECT_REQUEST_H
#include <ossc/modules/oss_get_object_group_request.h>
#undef _OSS_GET_OBJECT_REQUEST_H

static const char * 
_get_object_group_request_get_bucket_name(oss_get_object_group_request_t *request)
{
	return request->bucket_name;
}

static inline void
__get_object_group_request_set_bucket_name(
		oss_get_object_group_request_t *request,
		const char *bucket_name,
		size_t bucket_name_len)
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
_get_object_group_request_set_bucket_name(
		oss_get_object_group_request_t *request,
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__get_object_group_request_set_bucket_name(request, bucket_name, bucket_name_len);
}

static const char * 
_get_object_group_request_get_key(oss_get_object_group_request_t *request)
{
	return request->key;
}

static inline void
__get_object_group_request_set_key(
		oss_get_object_group_request_t *request,
		const char *key,
		size_t key_len)
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
_get_object_group_request_set_key(
		oss_get_object_group_request_t *request,
		const char *key)
{
	assert(key != NULL);

	size_t key_len = strlen(key);
	__get_object_group_request_set_key(request, key, key_len);
}

static const char * 
_get_object_group_request_get_modified_since_constraint(
		oss_get_object_group_request_t *request)
{
	return request->modified_since_constraint;
}

static inline void
__get_object_group_request_set_modified_since_constraint(
		oss_get_object_group_request_t *request,
		const char *modified_since_constraint,
		size_t modified_since_constraint_len)
{
	if (request->modified_since_constraint != NULL) {
		free(request->modified_since_constraint);
		request->modified_since_constraint = NULL;
	}
	request->modified_since_constraint = (char *)
			malloc(sizeof(char) * modified_since_constraint_len + 1);
	strncpy(request->modified_since_constraint,
			modified_since_constraint, modified_since_constraint_len);
	(request->modified_since_constraint)[modified_since_constraint_len] = '\0';
}

static void
_get_object_group_request_set_modified_since_constraint(
		oss_get_object_group_request_t *request,
		const char *modified_since_constraint)
{
	assert(modified_since_constraint != NULL);

	size_t modified_since_constraint_len = strlen(modified_since_constraint);
	__get_object_group_request_set_modified_since_constraint(request, modified_since_constraint, modified_since_constraint_len);
}

static void 
_get_object_group_request_get_range(oss_get_object_group_request_t *request, long *start, long *length)
{
	*start = request->start;
	*length = request->length;
}

static void
_get_object_group_request_set_range(
		oss_get_object_group_request_t *request,
		long start,
		long length)
{
	request->start = start;
	request->length = length;
}


static oss_response_header_overrides_t * 
_get_object_group_request_get_response_headers(oss_get_object_group_request_t *request)
{
	return request->response_headers;
}

static inline void
_get_object_group_request_set_response_headers(
		oss_get_object_group_request_t *request,
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


static const char *
_get_object_group_request_get_unmodified_since_constraint(oss_get_object_group_request_t *request)
{
	return request->unmodified_since_constraint;
}

static inline void
__get_object_group_request_set_unmodified_since_constraint(
		oss_get_object_group_request_t *request,
		const char *unmodified_since_constraint,
		size_t unmodified_since_constraint_len)
{
	if (request->unmodified_since_constraint != NULL) {
		free(request->unmodified_since_constraint);
		request->unmodified_since_constraint = NULL;
	}
	request->unmodified_since_constraint = (char *)
			malloc(sizeof(char) * unmodified_since_constraint_len + 1);
	strncpy(request->unmodified_since_constraint,
			unmodified_since_constraint, unmodified_since_constraint_len);
	(request->unmodified_since_constraint)[unmodified_since_constraint_len] = '\0';
}

static inline void
_get_object_group_request_set_unmodified_since_constraint(
		oss_get_object_group_request_t *request,
		const char *unmodified_since_constraint)
{
	assert(unmodified_since_constraint != NULL);

	size_t unmodified_since_constraint_len = strlen(unmodified_since_constraint);
	__get_object_group_request_set_unmodified_since_constraint(request, unmodified_since_constraint, unmodified_since_constraint_len);

}

static inline const char **
_get_object_group_request_get_matching_etag_constraints(
		oss_get_object_group_request_t *request,
		unsigned int *counts)
{
	assert(request!= NULL);

	*counts = request->_counts_matching_etag_constraints;
	return (const char **)(request->matching_etag_constraints);
}

static inline const char **
_get_object_group_request_get_no_matching_etag_constraints(
		oss_get_object_group_request_t *request,
		unsigned int *counts)
{
	assert(request!= NULL);

	*counts = request->_counts_no_matching_etag_constraints;
	return (const char **)(request->no_matching_etag_constraints);
}

static inline void
_get_object_group_request_set_matching_etag_constraints(
		oss_get_object_group_request_t *request,
		const char **matching_etag_constraints, 
		unsigned int counts)
{
	assert(request != NULL);
	assert(matching_etag_constraints != NULL);

	/* *
	 * If request->no_matching_etag_constraints != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = request->_counts_matching_etag_constraints;
	if (request->matching_etag_constraints != NULL) {

		for (j = 0; j < total; j++) {
			if (*(request->matching_etag_constraints + j) != NULL) {
				free(*(request->matching_etag_constraints + j));
				*(request->matching_etag_constraints + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const char **pnmec = matching_etag_constraints;

	request->matching_etag_constraints = (char **)malloc(sizeof(char *) * counts);
	
	for (i = 0; i < counts; i++) {
		size_t len = strlen(*(pnmec + i));
		*(request->matching_etag_constraints + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(request->matching_etag_constraints + i), 0, len + 1);
		strncpy(*(request->matching_etag_constraints + i), *(pnmec + i), len);
	}
	request->_counts_matching_etag_constraints = counts;
}

static inline void
_get_object_group_request_set_no_matching_etag_constraints(
		oss_get_object_group_request_t *request,
		const char **no_matching_etag_constraints, 
		unsigned counts)
{
	assert(request != NULL);
	assert(no_matching_etag_constraints != NULL);

	/* *
	 * If request->no_matching_etag_constraints != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = request->_counts_no_matching_etag_constraints;
	if (request->no_matching_etag_constraints != NULL) {

		for (j = 0; j < total; j++) {
			if (*(request->no_matching_etag_constraints + j) != NULL) {
				free(*(request->no_matching_etag_constraints + j));
				*(request->no_matching_etag_constraints + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const char **pnmec = no_matching_etag_constraints;

	request->no_matching_etag_constraints = (char **)malloc(sizeof(char *) * counts);
	
	for (i = 0; i < counts; i++) {
		size_t len = strlen(*(pnmec + i));
		*(request->no_matching_etag_constraints + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(request->no_matching_etag_constraints + i), 0, len + 1);
		strncpy(*(request->no_matching_etag_constraints + i), *(pnmec + i), len);
	}
	request->_counts_no_matching_etag_constraints = counts;
}

oss_get_object_group_request_t *
_get_object_group_request_initialize(const char *bucket_name, size_t bucket_name_len, const char *key, size_t key_len)
{
	oss_get_object_group_request_t *request;
	request = (oss_get_object_group_request_t *)malloc(sizeof(oss_get_object_group_request_t));

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

	request->modified_since_constraint = NULL;
	request->start = 0;
	request->length = 0;
	request->response_headers = NULL;
	request->unmodified_since_constraint = NULL;

	request->get_bucket_name = _get_object_group_request_get_bucket_name;
	request->set_bucket_name = _get_object_group_request_set_bucket_name;
	request->get_key = _get_object_group_request_get_key;
	request->set_key = _get_object_group_request_set_key;
	request->get_modified_since_constraint = _get_object_group_request_get_modified_since_constraint;
	request->set_modified_since_constraint = _get_object_group_request_set_modified_since_constraint;
	request->get_range = _get_object_group_request_get_range;
	request->set_range = _get_object_group_request_set_range;
	request->get_response_headers = _get_object_group_request_get_response_headers;
	request->set_response_headers = _get_object_group_request_set_response_headers;
	request->get_unmodified_since_constraint = _get_object_group_request_get_unmodified_since_constraint;
	request->set_unmodified_since_constraint = _get_object_group_request_set_unmodified_since_constraint;
	request->get_matching_etag_constraints = _get_object_group_request_get_matching_etag_constraints;
	request->get_no_matching_etag_constraints = _get_object_group_request_get_no_matching_etag_constraints;
	request->set_matching_etag_constraints = _get_object_group_request_set_matching_etag_constraints;
	request->set_no_matching_etag_constraints = _get_object_group_request_set_no_matching_etag_constraints;

	return request;
}

oss_get_object_group_request_t *
get_object_group_request_initialize(const char *bucket_name, const char *key)
{
	assert(bucket_name != NULL);
	assert(key != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);

	return _get_object_group_request_initialize(bucket_name, bucket_name_len, key, key_len);
}

void
get_object_group_request_finalize(oss_get_object_group_request_t *request)
{
	if (request) {
		if (request->bucket_name != NULL) {
			free(request->bucket_name);
			request->bucket_name = NULL;
		}
		if (request->key != NULL) {
			free(request->key);
			request->key = NULL;
		}
		if (request->modified_since_constraint != NULL) {
			free(request->modified_since_constraint);
			request->modified_since_constraint = NULL;
		}
		if (request->response_headers != NULL) {
			request->response_headers = NULL;
		}
		if (request->matching_etag_constraints != NULL) {
			size_t j = 0;
			size_t total = request->_counts_matching_etag_constraints;
			if (request->matching_etag_constraints != NULL) {
				for (j = 0; j < total; j++) {
					if (*(request->matching_etag_constraints + j) != NULL) {
						free(*(request->matching_etag_constraints + j));
						*(request->matching_etag_constraints + j) = NULL;
					}
				}
			}
		}

		if (request->no_matching_etag_constraints != NULL) {
			size_t j = 0;
			size_t total = request->_counts_no_matching_etag_constraints;
			if (request->no_matching_etag_constraints != NULL) {
				for (j = 0; j < total; j++) {
					if (*(request->no_matching_etag_constraints + j) != NULL) {
						free(*(request->no_matching_etag_constraints + j));
						*(request->no_matching_etag_constraints + j) = NULL;
					}
				}
			}
		}

		free(request);
	}
}

