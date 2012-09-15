/*
 * =============================================================================
 *
 *       Filename:  oss_post_object_group_result.h
 *
 *    Description:  oss post_object_group_result
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_POST_OBJECT_GROUP_RESULT_H
# error Never include <ossc/modules/oss_post_object_group_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_POST_OBJECT_GROUP_RESULT_H
#define OSS_POST_OBJECT_GROUP_RESULT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct oss_post_object_group_result_s oss_post_object_group_result_t;

struct oss_post_object_group_result_s {
	char *bucket_name;
	char *key;
	char *etag;
	size_t size;

	const char * (*get_bucket_name)(oss_post_object_group_result_t *result);
	void (*set_bucket_name)(oss_post_object_group_result_t *result, const char *bucket_name);

	const char * (*get_key)(oss_post_object_group_result_t *result);
	void (*set_key)(oss_post_object_group_result_t *result, const char *key);

	const char * (*get_etag)(oss_post_object_group_result_t *result);
	void (*set_etag)(oss_post_object_group_result_t *result, const char *etag);

	size_t (*get_size)(oss_post_object_group_result_t *result);
	void (*set_size)(oss_post_object_group_result_t *result, size_t size);

};


extern oss_post_object_group_result_t * post_object_group_result_initialize();

extern void post_object_group_result_finalize(oss_post_object_group_result_t *result);

#endif // OSS_POST_OBJECT_GROUP_RESULT_H

