/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.h
 *
 *    Description:  oss_bucket.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_BUCKET_H
# error Never include <ossc/modules/oss_bucket.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_BUCKET_H
#define OSS_BUCKET_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include  "ossc/modules/oss_owner.h"
#undef _OSS_OWNER_H

typedef struct oss_bucket_s oss_bucket_t;

struct oss_bucket_s {
	char *create_date;
	char *name;
	oss_owner_t *owner;

	const char * (*get_create_date)(oss_bucket_t *bucket);
	void (*set_create_date)(oss_bucket_t *bucket, const char *create_date);

	const char * (*get_name)(oss_bucket_t *bucket);
	void (*set_name)(oss_bucket_t *bucket, const char * name);

	oss_owner_t * (*get_owner)(oss_bucket_t *bucket);
	void (*set_owner)(oss_bucket_t *bucket, oss_owner_t *owner);
};

extern oss_bucket_t *
bucket_initialize(void);

extern oss_bucket_t *
bucket_initialize_with_name(const char *name);

extern void 
bucket_finalize(oss_bucket_t *bucket);

#endif
