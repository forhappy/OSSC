/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.h
 *
 *    Description:  oss bucket.
 *
 *        Created:  09/03/2012 10:34:22 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_BUCKET_H
#define OSS_BUCKET_H

typedef struct oss_bucket_s {
	char *creation_data;
	char *name;
	char *owner;
}oss_bucket_t

#endif // OSS_BUCKET_H
