/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_upload_listing.h
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
#ifndef _OSS_MULTIPART_UPLOAD_LISTING_H
# error Never include <modules/oss_multipart_upload_listing.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_MULTIPART_UPLOAD_LISTING_H
#define OSS_MULTIPART_UPLOAD_LISTING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_MULTIPART_UPLOAD_H
#include <modules/oss_multipart_upload.h>
#undef _OSS_MULTIPART_UPLOAD_H

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

typedef struct oss_multipart_upload_listing_s oss_multipart_upload_listing_t;

struct oss_multipart_upload_listing_s {
	char *bucket_name;

	char **common_prefixs;
	/* * 内部计数器，记录common_prefixs数目 */
	unsigned int _counts_common_prefixs;
	
	char *key_marker;
	char *upload_id_marker;
	char *next_key_marker;
	char *next_upload_id_marker;
	char *max_uploads;

	oss_multipart_upload_t **multipart_uploads;
	/* * 内部计数器，记录multipart_uploads数目 */
	unsigned int _counts_multipart_uploads;
	
	bool is_truncated;
	char *delimiter;
	char *prefix;

	const char * (*get_bucket_name)(oss_multipart_upload_listing_t *mul);
	void (*set_bucket_name)(oss_multipart_upload_listing_t *mul, const char *bucket_name);

	const char * (*get_key_marker)(oss_multipart_upload_listing_t *mul);
	void (*set_key_marker)(oss_multipart_upload_listing_t *mul, const char *key_marker);

	const char * (*get_upload_id_marker)(oss_multipart_upload_listing_t *mul);
	void (*set_upload_id_marker)(oss_multipart_upload_listing_t *mul, const char *upload_id_marker);

	const char * (*get_next_key_marker)(oss_multipart_upload_listing_t *mul);
	void (*set_next_key_marker)(oss_multipart_upload_listing_t *mul, const char *next_key_marker);

	const char * (*get_next_upload_id_marker)(oss_multipart_upload_listing_t *mul);
	void (*set_next_upload_id_marker)(oss_multipart_upload_listing_t *mul, const char *next_upload_id_marker);

	const char * (*get_max_uploads)(oss_multipart_upload_listing_t *mul);
	void (*set_max_uploads)(oss_multipart_upload_listing_t *mul, const char *max_uploads);

	bool (*get_is_truncated)(oss_multipart_upload_listing_t *mul);
	void (*set_is_truncated)(oss_multipart_upload_listing_t *mul, bool is_truncated);

	const oss_multipart_upload_t ** (*get_multipart_uploads)(oss_multipart_upload_listing_t *mul,
			unsigned int *counts);
	void (*set_multipart_uploads)(oss_multipart_upload_listing_t *mul, 
			oss_multipart_upload_t **multipart_uploads,
			unsigned int counts);

	const char ** (*get_common_prefixs)(oss_multipart_upload_listing_t *mul,
			unsigned int *counts);
	void (*set_common_prefixs)(oss_multipart_upload_listing_t *mul, 
			const char **common_prefixs,
			unsigned int counts);

	const char * (*get_delimiter)(oss_multipart_upload_listing_t *mul);
	void (*set_delimiter)(oss_multipart_upload_listing_t *mul, const char *delimiter);

	const char * (*get_prefix)(oss_multipart_upload_listing_t *mul);
	void (*set_prefix)(oss_multipart_upload_listing_t *mul, const char *prefix);
};

extern oss_multipart_upload_listing_t *
multipart_upload_listing_initialize(void);

extern void 
multipart_upload_listing_finalize(oss_multipart_upload_listing_t *mul);

#endif
