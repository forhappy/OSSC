/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.h
 *
 *    Description:  oss bucket.
 *
 *        Created:  09/03/2012 10:34:22 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_BUCKET_H
# error Never include <ossc/oss_bucket.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_BUCKET_H
#define OSS_BUCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef struct oss_bucket_s oss_bucket_t;
struct oss_bucket_s {
	char *creation_date;
	char *name;
	char *owner;

	const char * (*get_creation_date)(oss_bucket_t *bucket);
	const char * (*get_name)(oss_bucket_t *bucket);
	oss_owner_t * (*get_owner)(oss_bucket_t *bucket);

	void (*set_creation_date)(oss_bucket_t *bucket,
			const char *creation_date);
	void (*set_name)(oss_bucket_t *bucket, 
			const char *name);
	void (*set_owner)(oss_bucket_t *bucket,
			oss_owner_t *owner);
};

extern oss_bucket_t *
bucket_initialize(void);

extern oss_bucket_t *
bucket_initialize_with_name(const char *name);

extern void
bucket_finalize(oss_bucket_t *bucket);

#endif // OSS_BUCKET_H
