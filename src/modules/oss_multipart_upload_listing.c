/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_upload_listing.c
 *
 *    Description:  oss_multipart_upload_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_MULTIPART_UPLOAD_LISTING_H
#include <ossc/modules/oss_multipart_upload_listing.h>
#undef _OSS_MULTIPART_UPLOAD_LISTING_H

static const char * 
_multipart_upload_listing_get_bucket_name(
		oss_multipart_upload_listing_t *listing)
{
	return listing->bucket_name;
}

static inline void
__multipart_upload_listing_set_bucket_name(
		oss_multipart_upload_listing_t *listing,
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (listing->bucket_name != NULL) {
		free(listing->bucket_name);
		listing->bucket_name = NULL;
	}

	listing->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(listing->bucket_name, bucket_name, bucket_name_len);
	(listing->bucket_name)[bucket_name_len] = '\0';
}

static inline const char **
_multipart_upload_listing_get_common_prefixs(
		oss_multipart_upload_listing_t *listing, unsigned int *counts)
{
	assert(listing != NULL);

	*counts = listing->_counts_common_prefixs;
	return (const char **)(listing->common_prefixs);

}

void _multipart_upload_listing_set_common_prefixs(
		oss_multipart_upload_listing_t *listing,
		const char **common_prefixs, unsigned int counts)
{

	assert(listing != NULL);
	assert(common_prefixs != NULL);

	/* *
	 * If listing->common_prefixs != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = listing->_counts_common_prefixs;
	if (listing->common_prefixs != NULL) {

		for (j = 0; j < total; j++) {
			if (*(listing->common_prefixs + j) != NULL) {
				free(*(listing->common_prefixs + j));
				*(listing->common_prefixs + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const char **pnmec = common_prefixs;

	listing->common_prefixs = (char **)malloc(sizeof(char *) * counts);
	
	for (i = 0; i < counts; i++) {
		size_t len = strlen(*(pnmec + i));
		*(listing->common_prefixs + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(listing->common_prefixs + i), '\0', len + 1);
		strncpy(*(listing->common_prefixs + i), *(pnmec + i), len);
	}
	listing->_counts_common_prefixs = counts;
}

static void
_multipart_upload_listing_set_bucket_name(
		oss_multipart_upload_listing_t *listing,
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__multipart_upload_listing_set_bucket_name(listing, bucket_name, bucket_name_len);
}

static const char * 
_multipart_upload_listing_get_key_marker(
		oss_multipart_upload_listing_t *listing)
{
	return listing->key_marker;
}

static inline void
__multipart_upload_listing_set_key_marker(
		oss_multipart_upload_listing_t *listing,
		const char *key_marker,
		size_t key_marker_len)
{
	if (listing->key_marker != NULL) {
		free(listing->key_marker);
		listing->key_marker = NULL;
	}

	listing->key_marker = (char *)malloc(sizeof(char) * key_marker_len + 1);
	strncpy(listing->key_marker, key_marker, key_marker_len);
	(listing->key_marker)[key_marker_len] = '\0';
}

static void
_multipart_upload_listing_set_key_marker(
		oss_multipart_upload_listing_t *listing,
		const char *key_marker)
{
	assert(key_marker != NULL);

	size_t key_marker_len = strlen(key_marker);
	__multipart_upload_listing_set_key_marker(listing, key_marker, key_marker_len);
}

static const char * 
_multipart_upload_listing_get_upload_id_marker(
		oss_multipart_upload_listing_t *listing)
{
	return listing->upload_id_marker;
}

static inline void
__multipart_upload_listing_set_upload_id_marker(
		oss_multipart_upload_listing_t *listing,
		const char *upload_id_marker,
		size_t upload_id_marker_len)
{
	if (listing->upload_id_marker != NULL) {
		free(listing->upload_id_marker);
		listing->upload_id_marker = NULL;
	}

	listing->upload_id_marker = (char *)malloc(sizeof(char) * upload_id_marker_len + 1);
	strncpy(listing->upload_id_marker, upload_id_marker, upload_id_marker_len);
	(listing->upload_id_marker)[upload_id_marker_len] = '\0';
}

static void
_multipart_upload_listing_set_upload_id_marker(
		oss_multipart_upload_listing_t *listing,
		const char *upload_id_marker)
{
	assert(upload_id_marker != NULL);

	size_t upload_id_marker_len = strlen(upload_id_marker);
	__multipart_upload_listing_set_upload_id_marker(listing,
			upload_id_marker, upload_id_marker_len);
}

static const char * 
_multipart_upload_listing_get_next_key_marker(
		oss_multipart_upload_listing_t *listing)
{
	return listing->next_key_marker;
}

static inline void
__multipart_upload_listing_set_next_key_marker(
		oss_multipart_upload_listing_t *listing,
		const char *next_key_marker,
		size_t next_key_marker_len)
{
	if (listing->next_key_marker != NULL) {
		free(listing->next_key_marker);
		listing->next_key_marker = NULL;
	}
	listing->next_key_marker = (char *)malloc(sizeof(char) * next_key_marker_len + 1);
	strncpy(listing->next_key_marker, next_key_marker, next_key_marker_len);
	(listing->next_key_marker)[next_key_marker_len] = '\0';
}

static void
_multipart_upload_listing_set_next_key_marker(
		oss_multipart_upload_listing_t *listing,
		const char *next_key_marker)
{
	assert(next_key_marker != NULL);

	size_t next_key_marker_len = strlen(next_key_marker);
	__multipart_upload_listing_set_next_key_marker(listing,
			next_key_marker, next_key_marker_len);
}

static const char * 
_multipart_upload_listing_get_next_upload_id_marker(
		oss_multipart_upload_listing_t *listing)
{
	return listing->next_upload_id_marker;
}

static inline void
__multipart_upload_listing_set_next_upload_id_marker(
		oss_multipart_upload_listing_t *listing,
		const char *next_upload_id_marker,
		size_t next_upload_id_marker_len)
{
	if (listing->next_upload_id_marker != NULL) {
		free(listing->next_upload_id_marker);
		listing->next_upload_id_marker = NULL;
	}
	listing->next_upload_id_marker = (char *)malloc(sizeof(char) * next_upload_id_marker_len + 1);
	strncpy(listing->next_upload_id_marker,
			next_upload_id_marker, next_upload_id_marker_len);
	(listing->next_upload_id_marker)[next_upload_id_marker_len] = '\0';
}

static void
_multipart_upload_listing_set_next_upload_id_marker(
		oss_multipart_upload_listing_t *listing,
		const char *next_upload_id_marker)
{
	assert(next_upload_id_marker != NULL);

	size_t next_upload_id_marker_len = strlen(next_upload_id_marker);
	__multipart_upload_listing_set_next_upload_id_marker(listing,
			next_upload_id_marker, next_upload_id_marker_len);
}

static const char * 
_multipart_upload_listing_get_max_uploads(
		oss_multipart_upload_listing_t *listing)
{
	return listing->max_uploads;
}

static inline void
__multipart_upload_listing_set_max_uploads(
		oss_multipart_upload_listing_t *listing,
		const char *max_uploads,
		size_t max_uploads_len)
{
	if (listing->max_uploads != NULL) {
		free(listing->max_uploads);
		listing->max_uploads = NULL;
	}
	listing->max_uploads = (char *)malloc(sizeof(char) * max_uploads_len + 1);
	strncpy(listing->max_uploads, max_uploads, max_uploads_len);
	(listing->max_uploads)[max_uploads_len] = '\0';
}

static void
_multipart_upload_listing_set_max_uploads(
		oss_multipart_upload_listing_t *listing,
		const char *max_uploads)
{
	assert(max_uploads != NULL);

	size_t max_uploads_len = strlen(max_uploads);
	__multipart_upload_listing_set_max_uploads(listing,
			max_uploads, max_uploads_len);
}

static bool
_multipart_upload_listing_get_is_truncated(
		oss_multipart_upload_listing_t *listing)
{
	return listing->is_truncated;
}

static inline oss_multipart_upload_t ** 
_multipart_upload_listing_get_multipart_uploads(
		oss_multipart_upload_listing_t *listing,
		unsigned int *counts)
{
	assert(listing != NULL);

	*counts = listing->_counts_multipart_uploads;
	return (oss_multipart_upload_t **)(listing->multipart_uploads);
}

void _multipart_upload_listing_set_multipart_uploads(
		oss_multipart_upload_listing_t *listing,
		oss_multipart_upload_t **multipart_uploads,
		unsigned int counts)
{

	assert(listing != NULL);
	assert(multipart_uploads != NULL);

	listing->multipart_uploads = multipart_uploads;
	listing->_counts_multipart_uploads = counts;
}

#if 0
/* * This is an complex implementation of 
 * _multipart_upload_listing_set_multipart_uploads().
 * */
void _multipart_upload_listing_set_multipart_uploads(
		oss_multipart_upload_listing_t *listing,
		const oss_multipart_upload_t **multipart_uploads,
		unsigned int counts)
{

	assert(listing != NULL);
	assert(multipart_uploads != NULL);

	/* *
	 * If listing->multipart_uploads != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = listing->_counts_multipart_uploads;
	if (listing->multipart_uploads != NULL) {

		for (; j < total; j++) {
			if (*(listing->multipart_uploads + j) != NULL) {
				multipart_upload_finalize(*(listing->multipart_uploads + j));
				*(listing->multipart_uploads + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const oss_multipart_upload_t **pmu = multipart_uploads;

	listing->multipart_uploads = (char **)malloc
			(sizeof(oss_multipart_upload_t *) * counts);
	
	for (; i < counts; i++) {
		*(listing->multipart_uploads + i) = multipart_upload_initialize();
		(*(listing->multipart_uploads + i))->set_initiated(*(multipart_uploads + i),
				(*(pmu + i))->get_initiated(*(pmu + i)));
		(*(listing->multipart_uploads + i))->set_storage_class(*(multipart_uploads + i),
				(*(pmu + i))->get_storage_class(*(pmu + i)));
		(*(listing->multipart_uploads + i))->set_upload_id(*(multipart_uploads + i),
				(*(pmu + i))->get_upload_id(*(pmu + i)));
		(*(listing->multipart_uploads + i))->set_key(*(multipart_uploads + i),
				(*(pmu + i))->get_key(*(pmu + i)));
	}
	listing->_counts_multipart_uploads = counts;
}
#endif

static inline void
_multipart_upload_listing_set_is_truncated(
		oss_multipart_upload_listing_t *listing,
		bool is_truncated)
{
	listing->is_truncated = is_truncated;
}

static const char * 
_multipart_upload_listing_get_delimiter(
		oss_multipart_upload_listing_t *listing)
{
	return listing->delimiter;
}

static inline void
__multipart_upload_listing_set_delimiter(
		oss_multipart_upload_listing_t *listing,
		const char *delimiter,
		size_t delimiter_len)
{
	if (listing->delimiter != NULL) {
		free(listing->delimiter);
		listing->delimiter = NULL;
	}

	listing->delimiter = (char *)malloc(sizeof(char) * delimiter_len + 1);
	strncpy(listing->delimiter, delimiter, delimiter_len);
	(listing->delimiter)[delimiter_len] = '\0';
}

static void
_multipart_upload_listing_set_delimiter(
		oss_multipart_upload_listing_t *listing,
		const char *delimiter)
{
	assert(delimiter != NULL);

	size_t delimiter_len = strlen(delimiter);
	__multipart_upload_listing_set_delimiter(listing, delimiter, delimiter_len);
}

static const char * 
_multipart_upload_listing_get_prefix(
		oss_multipart_upload_listing_t *listing)
{
	return listing->prefix;
}

static inline void
__multipart_upload_listing_set_prefix(
		oss_multipart_upload_listing_t *listing,
		const char *prefix,
		size_t prefix_len)
{
	if (listing->prefix != NULL) {
		free(listing->prefix);
		listing->prefix = NULL;
	}

	listing->prefix = (char *)malloc(sizeof(char) * prefix_len + 1);
	strncpy(listing->prefix, prefix, prefix_len);
	(listing->prefix)[prefix_len] = '\0';
}

static void
_multipart_upload_listing_set_prefix(
		oss_multipart_upload_listing_t *listing,
		const char *prefix)
{
	assert(prefix != NULL);

	size_t prefix_len = strlen(prefix);
	__multipart_upload_listing_set_prefix(listing, prefix, prefix_len);
}

oss_multipart_upload_listing_t *
multipart_upload_listing_initialize(void)
{
	oss_multipart_upload_listing_t *listing;
	listing = (oss_multipart_upload_listing_t *)
			malloc(sizeof(oss_multipart_upload_listing_t));
	listing->bucket_name = NULL;
	listing->common_prefixs = NULL;
	listing->key_marker = NULL;
	listing->upload_id_marker = NULL;
	listing->next_key_marker = NULL;
	listing->next_upload_id_marker = NULL;
	listing->max_uploads = NULL;
	listing->delimiter = NULL;
	listing->prefix = NULL;
	listing->is_truncated = false;

	listing->get_bucket_name = _multipart_upload_listing_get_bucket_name;
	listing->set_bucket_name = _multipart_upload_listing_set_bucket_name;
	listing->get_common_prefixs = _multipart_upload_listing_get_common_prefixs;
	listing->set_common_prefixs = _multipart_upload_listing_set_common_prefixs;
	listing->get_key_marker = _multipart_upload_listing_get_key_marker;
	listing->set_key_marker = _multipart_upload_listing_set_key_marker;
	listing->get_upload_id_marker = _multipart_upload_listing_get_upload_id_marker;
	listing->set_upload_id_marker = _multipart_upload_listing_set_upload_id_marker;
	listing->get_next_key_marker = _multipart_upload_listing_get_next_key_marker;
	listing->set_next_key_marker = _multipart_upload_listing_set_next_key_marker;
	listing->get_next_upload_id_marker = _multipart_upload_listing_get_next_upload_id_marker;
	listing->set_next_upload_id_marker = _multipart_upload_listing_set_next_upload_id_marker;
	listing->get_max_uploads = _multipart_upload_listing_get_max_uploads;
	listing->set_max_uploads = _multipart_upload_listing_set_max_uploads;
	listing->get_multipart_uploads = _multipart_upload_listing_get_multipart_uploads;
	listing->set_multipart_uploads = _multipart_upload_listing_set_multipart_uploads;
	listing->get_is_truncated = _multipart_upload_listing_get_is_truncated;
	listing->set_is_truncated = _multipart_upload_listing_set_is_truncated;
	listing->get_delimiter = _multipart_upload_listing_get_delimiter;
	listing->set_delimiter = _multipart_upload_listing_set_delimiter;
	listing->get_prefix = _multipart_upload_listing_get_prefix;
	listing->set_prefix = _multipart_upload_listing_set_prefix;

	return listing;
}

void
multipart_upload_listing_finalize(
		oss_multipart_upload_listing_t *listing)
{
	assert(listing != NULL);

	if (listing->bucket_name != NULL) {
		free(listing->bucket_name);
		listing->bucket_name = NULL;
	}

	if (listing->common_prefixs != NULL) {
		size_t j = 0;
		size_t total = listing->_counts_common_prefixs;
		if (listing->common_prefixs != NULL) {
			for (j = 0; j < total; j++) {
				if (*(listing->common_prefixs + j) != NULL) {
					free(*(listing->common_prefixs + j));
					*(listing->common_prefixs + j) = NULL;
				}
			}
		}
	}

	if (listing->key_marker != NULL) {
		free(listing->key_marker);
		listing->key_marker = NULL;
	}

	if (listing->upload_id_marker != NULL) {
		free(listing->upload_id_marker);
		listing->upload_id_marker = NULL;
	}

	if (listing->next_key_marker != NULL) {
		free(listing->next_key_marker);
		listing->next_key_marker = NULL;
	}

	if (listing->next_upload_id_marker != NULL) {
		free(listing->next_upload_id_marker);
		listing->next_upload_id_marker = NULL;
	}

	if (listing->max_uploads != NULL) {
		free(listing->max_uploads);
		listing->max_uploads = NULL;
	}

	/* *
	 * Here we INTEND to leave it un-freed,
	 * it's the creator's responsibility to free it.
	 * */
	if (listing->multipart_uploads != NULL) {
		listing->multipart_uploads = NULL;
	}

	if (listing->delimiter != NULL) {
		free(listing->delimiter);
		listing->delimiter = NULL;
	}

	if (listing->prefix != NULL) {
		free(listing->prefix);
		listing->prefix = NULL;
	}
	free(listing);

}
