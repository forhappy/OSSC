/*
 * =============================================================================
 *
 *       Filename:  oss_part_listing.h
 *
 *    Description:  oss_part_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PART_LISTING_H
# error Never include <ossc/oss_part_listing.h> directly, use <ossc/client.h> instead.
#endif

#define _OSS_OWNER_H
#include  "../../include/ossc/oss_owner.h"
#undef _OSS_OWNER_H

#ifndef OSS_PART_LISTING_H
#define OSS_PART_LISTING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

typedef struct oss_part_listing_s oss_part_listing_t;

struct oss_part_listing_s {
	char *bucket_name;
	oss_owner_t *initiator;
	bool is_truncated;
	char *key;
	int max_parts;
	int next_part_number_marker;
	oss_owner_t *owner;
	int part_number_marker;
	//list parts;
	char *storage_class;
	char *upload_id;

	const char * (*get_bucket_name)(oss_part_listing_t *pl);
	void (*set_bucket_name)(oss_part_listing_t *pl, const char *bucket_name);

	const char * (*get_key)(oss_part_listing_t *pl);
	void (*set_key)(oss_part_listing_t *pl, const char *key);

	const char * (*get_upload_id)(oss_part_listing_t *pl);
	void (*set_upload_id)(oss_part_listing_t *pl, const char *upload_id);

	const char * (*get_storage_class)(oss_part_listing_t *pl);
	void (*set_storage_class)(oss_part_listing_t *pl, const char *storage_class);

	oss_owner_t * (*get_initiator)(oss_part_listing_t *pl);
	void (*set_initiator)(oss_part_listing_t *pl, oss_owner_t *initiator);

	int (*get_max_parts)(oss_part_listing_t *pl);
	void (*set_max_parts)(oss_part_listing_t *pl, int max_parts);

	bool (*get_is_truncated)(oss_part_listing_t *pl);
	void (*set_is_truncated)(oss_part_listing_t *pl, bool is_truncated);
	/* 
	 * get and set parts to be continued.
	 * */


	int (*get_next_part_number_marker)(oss_part_listing_t *pl);
	void (*set_next_part_number_marker)(oss_part_listing_t *pl, int next_part_number_marker);

	oss_owner_t * (*get_owner)(oss_part_listing_t *pl);
	void (*set_owner)(oss_part_listing_t *pl, oss_owner_t *owner);
	
	int (*get_part_number_marker)(oss_part_listing_t *pl);
	void (*set_part_number_marker)(oss_part_listing_t *pl, int part_number_marker);



};

extern oss_part_listing_t *
part_listing_initialize(void);

extern void 
part_listing_finalize(oss_part_listing_t *pl);

#endif
