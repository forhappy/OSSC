/*
 * =============================================================================
 *
 *       Filename:  oss_object_listing.c
 *
 *    Description:  oss_object_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_OBJECT_LISTING_H
#include <modules/oss_object_listing.h>
#undef _OSS_OBJECT_LISTING_H



void 
object_listing_finalize(oss_object_listing_t *ol)
{
	assert(ol != NULL);
	if (ol->bucket_name) {
		free(ol->bucket_name);
		ol->bucket_name = NULL;
	}
	if (ol->marker) {
		free(ol->marker);
		ol->marker = NULL;
	}
	if (ol->next_marker) {
		free(ol->next_marker);
		ol->next_marker = NULL;
	}
	if (ol->max_keys) {
		free(ol->max_keys);
		ol->max_keys = NULL;
	}
	if (ol->delimiter) {
		free(ol->delimiter);
		ol->delimiter = NULL;
	}
	if (ol->prefix) {
		free(ol->prefix);
		ol->prefix = NULL;
	}

	if (ol->common_prefixs != NULL) {
		size_t j = 0;
		size_t total = ol->_counts_common_prefixs;
		if (ol->common_prefixs != NULL) {
			for (; j < total; j++) {
				if (*(ol->common_prefixs + j) != NULL) {
					free(*(ol->common_prefixs + j));
					*(ol->common_prefixs + j) = NULL;
				}
			}
		}
	}

	if (ol) {
		free(ol);
		ol = NULL;
	}
}

static const char * 
_object_listing_get_bucket_name(oss_object_listing_t *ol)
{
	return ol->bucket_name;
}

static inline void
__object_listing_set_bucket_name(
		oss_object_listing_t *ol, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (ol->bucket_name) {
		free(ol->bucket_name);
		ol->bucket_name = NULL;
	}
	ol->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(ol->bucket_name, bucket_name, bucket_name_len);
	(ol->bucket_name)[bucket_name_len] = '\0';
}

static void
_object_listing_set_bucket_name(
		oss_object_listing_t *ol, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__object_listing_set_bucket_name(ol, bucket_name, bucket_name_len);
}

static const char * 
_object_listing_get_marker(oss_object_listing_t *ol)
{
	return ol->marker;
}

static inline void
__object_listing_set_marker(
		oss_object_listing_t *ol, 
		const char *marker,
		size_t marker_len)
{
	if (ol->marker) {
		free(ol->marker);
		ol->marker = NULL;
	}
	ol->marker = (char *)malloc(sizeof(char) * marker_len + 1);
	strncpy(ol->marker, marker, marker_len);
	(ol->marker)[marker_len] = '\0';
}

static void
_object_listing_set_marker(
		oss_object_listing_t *ol, 
		const char *marker)
{
	assert(marker != NULL);
	size_t marker_len = strlen(marker);
	__object_listing_set_marker(ol, marker, marker_len);
}

static const char * 
_object_listing_get_next_marker(oss_object_listing_t *ol)
{
	return ol->next_marker;
}

static inline void
__object_listing_set_next_marker(
		oss_object_listing_t *ol, 
		const char *next_marker,
		size_t next_marker_len)
{
	if (ol->next_marker) {
		free(ol->next_marker);
		ol->next_marker = NULL;
	}
	ol->next_marker = (char *)malloc(sizeof(char) * next_marker_len + 1);
	strncpy(ol->next_marker, next_marker, next_marker_len);
	(ol->next_marker)[next_marker_len] = '\0';
}

static void
_object_listing_set_next_marker(
		oss_object_listing_t *ol, 
		const char *next_marker)
{
	assert(next_marker != NULL);
	size_t next_marker_len = strlen(next_marker);
	__object_listing_set_next_marker(ol, next_marker, next_marker_len);
}

static const char * 
_object_listing_get_max_keys(oss_object_listing_t *ol)
{
	return ol->max_keys;
}

static inline void
__object_listing_set_max_keys(
		oss_object_listing_t *ol, 
		const char *max_keys,
		size_t max_keys_len)
{
	if (ol->max_keys) {
		free(ol->max_keys);
		ol->max_keys = NULL;
	}
	ol->max_keys = (char *)malloc(sizeof(char) * max_keys_len + 1);
	strncpy(ol->max_keys, max_keys, max_keys_len);
	(ol->max_keys)[max_keys_len] = '\0';
}

static void
_object_listing_set_max_keys(
		oss_object_listing_t *ol, 
		const char *max_keys)
{
	assert(max_keys != NULL);
	size_t max_keys_len = strlen(max_keys);
	__object_listing_set_max_keys(ol, max_keys, max_keys_len);
}

static bool
_object_listing_get_is_truncated(oss_object_listing_t *ol)
{
	return ol->is_truncated;
}

static inline void
_object_listing_set_is_truncated(
		oss_object_listing_t *ol, 
		bool is_truncated)
{
	ol->is_truncated = is_truncated;
}

static const char * 
_object_listing_get_delimiter(oss_object_listing_t *ol)
{
	return ol->delimiter;
}

static inline void
__object_listing_set_delimiter(
		oss_object_listing_t *ol, 
		const char *delimiter,
		size_t delimiter_len)
{
	if (ol->delimiter) {
		free(ol->delimiter);
		ol->delimiter = NULL;
	}
	ol->delimiter = (char *)malloc(sizeof(char) * delimiter_len + 1);
	strncpy(ol->delimiter, delimiter, delimiter_len);
	(ol->delimiter)[delimiter_len] = '\0';
}

static void
_object_listing_set_delimiter(
		oss_object_listing_t *ol, 
		const char *delimiter)
{
	assert(delimiter != NULL);
	size_t delimiter_len = strlen(delimiter);
	__object_listing_set_delimiter(ol, delimiter, delimiter_len);
}

static const char * 
_object_listing_get_prefix(oss_object_listing_t *ol)
{
	return ol->prefix;
}

static inline void
__object_listing_set_prefix(
		oss_object_listing_t *ol, 
		const char *prefix,
		size_t prefix_len)
{
	if (ol->prefix) {
		free(ol->prefix);
		ol->prefix = NULL;
	}
	ol->prefix = (char *)malloc(sizeof(char) * prefix_len + 1);
	strncpy(ol->prefix, prefix, prefix_len);
	(ol->prefix)[prefix_len] = '\0';
}

static void
_object_listing_set_prefix(
		oss_object_listing_t *ol, 
		const char *prefix)
{
	assert(prefix != NULL);
	size_t prefix_len = strlen(prefix);
	__object_listing_set_prefix(ol, prefix, prefix_len);
}

static inline const char **
_object_listing_get_common_prefixs(oss_object_listing_t *ol, unsigned int *counts)
{
	assert(ol != NULL);
	*counts = ol->_counts_common_prefixs;
	return (const char **)(ol->common_prefixs);

}

void _object_listing_set_common_prefixs(
		oss_object_listing_t *ol,
		const char **common_prefixs, unsigned int counts)
{

	assert(ol != NULL);
	assert(common_prefixs != NULL);

	/* *
	 * If ol->common_prefixs != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = ol->_counts_common_prefixs;
	if (ol->common_prefixs != NULL) {

		for (; j < total; j++) {
			if (*(ol->common_prefixs + j) != NULL) {
				free(*(ol->common_prefixs + j));
				*(ol->common_prefixs + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const char **pnmec = common_prefixs;

	ol->common_prefixs = (char **)malloc(sizeof(char *) * counts);
	
	for (; i < counts; i++) {
		size_t len = strlen(*(pnmec + i));
		*(ol->common_prefixs + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(ol->common_prefixs), len + 1, '\0');
		strncpy(*(ol->common_prefixs + i), *(pnmec + i), len);
	}
	ol->_counts_common_prefixs = counts;
}

oss_object_listing_t *
object_listing_initialize(void)
{
	oss_object_listing_t *ol;
	ol = (oss_object_listing_t *)malloc(sizeof(oss_object_listing_t));
	ol->bucket_name = NULL;
	ol->marker = NULL;
	ol->next_marker = NULL;
	ol->max_keys = NULL;
	ol->delimiter = NULL;
	ol->prefix = NULL;
	ol->is_truncated = false;
	ol->common_prefixs = NULL;
	ol->_counts_common_prefixs = 0;

	ol->get_bucket_name = _object_listing_get_bucket_name;
	ol->set_bucket_name = _object_listing_set_bucket_name;
	ol->get_marker = _object_listing_get_marker;
	ol->set_marker = _object_listing_set_marker;
	ol->get_next_marker = _object_listing_get_next_marker;
	ol->set_next_marker = _object_listing_set_next_marker;
	ol->get_max_keys = _object_listing_get_max_keys;
	ol->set_max_keys = _object_listing_set_max_keys;
	ol->get_is_truncated = _object_listing_get_is_truncated;
	ol->set_is_truncated = _object_listing_set_is_truncated;
	ol->get_delimiter = _object_listing_get_delimiter;
	ol->set_delimiter = _object_listing_set_delimiter;
	ol->get_prefix = _object_listing_get_prefix;
	ol->set_prefix = _object_listing_set_prefix;
	ol->get_common_prefixs = _object_listing_get_common_prefixs;
	ol->set_common_prefixs = _object_listing_set_common_prefixs;

	return ol;
}
