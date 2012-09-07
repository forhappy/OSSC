/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.c
 *
 *    Description:  oss bucket
 *
 *        Created:  09/03/2012 10:36:29 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_BUCKET_H
#include <ossc/oss_bucket.h>
#undef _OSS_BUCKET_H

#define _OSS_OWNER_H
#include <ossc/oss_owner.h>
#undef _OSS_OWNER_H

static inline const char *
_get_creation_date(oss_bucket_t *bucket)
{
	return bucket->creation_date;
}

static inline const char *
_get_name(oss_bucket_t *bucket)
{
	return bucket->name;
}

static inline oss_owner_t *
_get_owner(oss_bucket_t *bucket)
{
	return bucket->owner;
}

static inline void
set_creation_date(oss_bucket_t *bucket,
		const char *creation_date)
{}

static inline void 
_set_name(oss_bucket_t *bucket, 
		const char *name)
{}

static inline void
_set_owner(oss_bucket_t *bucket,
		oss_owner_t *owner)
{}

oss_bucket_t *
bucket_initialize(void)
{}

oss_bucket_t *
bucket_initialize_with_name(const char *name)
{}

void
bucket_finalize(oss_bucket_t *bucket)
{}
