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

typedef struct oss_bucket_s {
	char *creation_date;
	char *name;
	char *owner;
}oss_bucket_t

#endif // OSS_BUCKET_H
