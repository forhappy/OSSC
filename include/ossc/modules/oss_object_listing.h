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
# error Never include <modules/oss_object_listing.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_LISTING_H
#define OSS_OBJECT_LISTING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif


typedef struct oss_object_listing_s oss_object_listing_t;

struct oss_object_listing_s {
	char *bucket_name;
	char *next_marker;
	char *prefix;
	char *marker;
	char *max_keys;
	char * delimiter;
	bool is_truncated;

	const char * (*get_bucket_name)(oss_object_listing_t *ol);
	void (*set_bucket_name)(oss_object_listing_t *ol, const char *bucket_name);
	const char * (*get_next_marker)(oss_object_listing_t *ol);
	void (*set_next_marker)(oss_object_listing_t *ol, const char *next_marker);
	const char * (*get_prefix)(oss_object_listing_t *ol);
	void (*set_prefix)(oss_object_listing_t *ol, const char *prefix);
	const char * (*get_marker)(oss_object_listing_t *ol);
	void (*set_marker)(oss_object_listing_t *ol, const char *marker);
	const char * (*get_max_keys)(oss_object_listing_t *ol);
	void (*set_max_keys)(oss_object_listing_t *ol, const char *max_keys);
	const char * (*get_delimiter)(oss_object_listing_t *ol);
	void (*set_delimiter)(oss_object_listing_t *ol, const char *delimiter);
	bool (*get_is_truncated)(oss_object_listing_t *ol);
	void (*set_is_truncated)(oss_object_listing_t *ol, bool is_truncated);
	/* 
	 *get_object_summaries, get_common_prefixes and set_common_prefixes to be continued.
	 * */

};

extern oss_object_listing_t *
object_listing_initialize(void);

extern void 
object_listing_finalize(oss_object_listing_t *ol);

#endif
