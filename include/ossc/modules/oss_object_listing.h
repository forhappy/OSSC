/*
 * =============================================================================
 *
 *       Filename:  oss_object_listing.h
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
#ifndef _OSS_OBJECT_LISTING_H
# error Never include <ossc/modules/oss_object_listing.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_LISTING_H
#define OSS_OBJECT_LISTING_H
#define _OSS_OBJECT_SUMMARY_H
#include <modules/oss_object_summary.h>
#undef _OSS_OBJECT_SUMMARY_H

#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif


typedef struct oss_object_listing_s oss_object_listing_t;

struct oss_object_listing_s {
	char *bucket_name;
	char *next_marker;
	char *prefix;
	char *marker;
	int max_keys;
	char * delimiter;
	bool is_truncated;
	oss_object_summary_t **summaries;
	/** 内部计数器，记录summaries数目 */
	unsigned int _counts_summaries;

	char **common_prefixes;
	/** 内部计数器，记录common_prefixes数目 */
	unsigned int _counts_common_prefixes;

	const char * (*get_bucket_name)(oss_object_listing_t *listing);
	void (*set_bucket_name)(oss_object_listing_t *listing, const char *bucket_name);
	const char * (*get_next_marker)(oss_object_listing_t *listing);
	void (*set_next_marker)(oss_object_listing_t *listing, const char *next_marker);
	const char * (*get_prefix)(oss_object_listing_t *listing);
	void (*set_prefix)(oss_object_listing_t *listing, const char *prefix);
	const char * (*get_marker)(oss_object_listing_t *listing);
	void (*set_marker)(oss_object_listing_t *listing, const char *marker);
	int (*get_max_keys)(oss_object_listing_t *listing);
	void (*set_max_keys)(oss_object_listing_t *listing, int max_keys);
	const char * (*get_delimiter)(oss_object_listing_t *listing);
	void (*set_delimiter)(oss_object_listing_t *listing, const char *delimiter);
	bool (*get_is_truncated)(oss_object_listing_t *listing);
	void (*set_is_truncated)(oss_object_listing_t *listing, bool is_truncated);
	const char ** (*get_common_prefixes)(oss_object_listing_t *listing,
			unsigned int *counts);
	void (*set_common_prefixes)(oss_object_listing_t *listing,
			const char **common_prefixes,
			unsigned int counts);
	oss_object_summary_t ** (*get_summaries)(oss_object_listing_t *listing,
			unsigned int *counts);
};

extern oss_object_listing_t *
object_listing_initialize(void);

extern void 
object_listing_finalize(oss_object_listing_t *listing);

#endif
