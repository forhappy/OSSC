/*
 * =============================================================================
 *
 *       Filename:  oss_list_objects_request.c
 *
 *    Description:  list objects request implementation
 *
 *        Created:  09/04/2012 11:10:19 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_LIST_OBJECTS_REQUEST_H
#include "oss_list_objects_request.h"
#undef _OSS_LIST_OBJECTS_REQUEST_H


//#define _OSS_LIST_OBJECTS_REQUEST_H
//#include <ossc/oss_list_objects_request.h>
//#undef _OSS_LIST_OBJECTS_REQUEST_H

inline void
__list_objects_request_set_bucket_name(
		oss_list_objects_request_t *request,
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

inline void
__list_objects_request_set_prefix(
		oss_list_objects_request_t *request,
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

inline void
__list_objects_request_set_marker(
		oss_list_objects_request_t *request,
		const char *marker, size_t marker_len)
{
	assert(marker != NULL);
	assert(marker_len > 0);
	if (request->marker!= NULL) {
		free(request->marker);
		request->marker = NULL;
	}

	request->marker= (char *)malloc(sizeof(char) * marker_len+ 1);
	strncpy(request->marker, marker, marker_len);
	(request->marker)[marker_len] = '\0';
}

inline void
__list_objects_request_set_delimiter(
		oss_list_objects_request_t *request,
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

inline const char *
_list_objects_request_get_bucket_name(
		oss_list_objects_request_t *request)
{
	return request->bucket_name;
}

inline const char *
_list_objects_request_get_prefix(
		oss_list_objects_request_t *request)
{
	return request->prefix;
}

inline const char *
_list_objects_request_get_marker(
		oss_list_objects_request_t *request)
{
	return request->marker;
}

inline const char *
_list_objects_request_get_delimiter(
		oss_list_objects_request_t *request)
{
	return request->delimiter;
}

inline unsigned int 
_list_objects_request_get_max_keys(
		oss_list_objects_request_t *request)
{
	return request->max_keys;
}

inline void
_list_objects_request_set_bucket_name(
		oss_list_objects_request_t *request,
		const char *bucket_name)
{
	size_t bucket_name_len = strlen(bucket_name);

	__list_objects_request_set_bucket_name(request,
			bucket_name, bucket_name_len);
}

inline void
_list_objects_request_set_prefix(
		oss_list_objects_request_t *request,
		const char *prefix)
{
	size_t prefix_len = strlen(prefix);

	__list_objects_request_set_prefix(request,
			prefix, prefix_len);
}

inline void
_list_objects_request_set_marker(
		oss_list_objects_request_t *request,
		const char *marker)
{
	size_t marker_len = strlen(marker);

	__list_objects_request_set_marker(request,
			marker, marker_len);
}

inline void
_list_objects_request_set_delimiter(
		oss_list_objects_request_t *request,
		const char *delimiter)
{
	size_t delimiter_len = strlen(delimiter);

	__list_objects_request_set_delimiter(request,
			delimiter, delimiter_len);
}

inline void
_list_objects_request_set_max_keys(
		oss_list_objects_request_t *request,
		unsigned int max_keys)
{
	request->max_keys = max_keys;
}

/* *
 * 初始化
 * */
inline oss_list_objects_request_t *
_list_objects_request_initialize(
		const char *bucket_name, size_t bucket_name_len,
		const char *prefix, size_t prefix_len,
		const char *marker, size_t marker_len,
		const char *delimiter, size_t delimiter_len,
		unsigned int max_keys)
{
	size_t byte_of_char = sizeof(char);

	oss_list_objects_request_t * request = NULL;
	request = (oss_list_objects_request_t *)malloc(
			sizeof(oss_list_objects_request_t));
	request->bucket_name = (char *)malloc(byte_of_char * bucket_name_len + 1);
	request->prefix = (char *)malloc(byte_of_char * prefix_len + 1);
	request->marker = (char *)malloc(byte_of_char * marker_len + 1);
	request->delimiter = (char *)malloc(byte_of_char * delimiter_len + 1);
	request->max_keys = max_keys;

	strncpy(request->bucket_name, bucket_name, bucket_name_len);
	strncpy(request->prefix, prefix, prefix_len);
	strncpy(request->marker, marker, marker_len);
	strncpy(request->delimiter, delimiter, delimiter_len);

	(request->bucket_name)[bucket_name_len] = '\0';
	(request->prefix)[prefix_len] = '\0';
	(request->marker)[marker_len] = '\0';
	(request->delimiter)[delimiter_len] = '\0';

	request->get_bucket_name = _list_objects_request_get_bucket_name;
	request->get_prefix = _list_objects_request_get_prefix;
	request->get_marker= _list_objects_request_get_marker;
	request->get_delimiter= _list_objects_request_get_delimiter;
	request->get_max_keys= _list_objects_request_get_max_keys;

	request->set_bucket_name = _list_objects_request_set_bucket_name;
	request->set_prefix= _list_objects_request_set_prefix;
	request->set_marker= _list_objects_request_set_marker;
	request->set_delimiter = _list_objects_request_set_delimiter;
	request->set_max_keys= _list_objects_request_set_max_keys;

	return request;
}

/* *
 * 初始化
 * */
oss_list_objects_request_t *
list_objects_request_initialize(void)
{
	return _list_objects_request_initialize(NULL, 0,
			NULL, 0,
			NULL, 0,
			NULL, 0,
			0);
}

/* *
 * 初始化
 * */
oss_list_objects_request_t *
list_objects_request_initialize_with_bucket_name(const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);

	return _list_objects_request_initialize(bucket_name, bucket_name_len,
			NULL, 0,
			NULL, 0,
			NULL, 0,
			0);
}

/* *
 * 初始化
 * */
oss_list_objects_request_t *
list_objects_request_initialize_with_args(const char *bucket_name,
		const char *prefix,
		const char *marker,
		const char *delimiter,
		unsigned int max_keys)
{
	assert(bucket_name != NULL);
	assert(prefix != NULL);
	assert(marker != NULL);
	assert(delimiter != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	size_t prefix_len = strlen(prefix);
	size_t marker_len = strlen(marker);
	size_t delimiter_len = strlen(delimiter);

	return _list_objects_request_initialize(bucket_name, bucket_name_len,
			prefix, prefix_len,
			marker, marker_len,
			delimiter, delimiter_len,
			max_keys);
}

/* *
 * 析构函数
 * */
void
list_objects_request_finalize(
		oss_list_objects_request_t *request)
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

	if (request->marker!= NULL) {
		free(request->marker);
		request->marker= NULL;
	}

	if (request->delimiter!= NULL) {
		free(request->delimiter);
		request->delimiter= NULL;
	}

	free(request);
	request = NULL;
}
