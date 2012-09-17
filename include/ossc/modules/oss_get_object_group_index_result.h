/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_group_index_result.h
 *
 *    Description:  oss get_object_group_index_result
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
# error Never include <ossc/modules/oss_get_object_group_index_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#define OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_MULTIPART_OBJECT_GROUP_H
#include <ossc/modules/oss_multipart_object_group.h>
#undef _OSS_MULTIPART_OBJECT_GROUP_H

/**
 * @defgroup oss_get_object_group_index_result_t oss_get_object_group_index_result_t
 * @{
 */

typedef struct oss_get_object_group_index_result_s oss_get_object_group_index_result_t;

struct oss_get_object_group_index_result_s {
	char *bucket_name;
	char *key;
	char *etag;
	oss_multipart_object_group_t **group;
	int part_number;
	size_t file_length;

	const char * (*get_bucket_name)(oss_get_object_group_index_result_t *result);
	void (*set_bucket_name)(oss_get_object_group_index_result_t *result, const char *bucket_name);

	const char * (*get_key)(oss_get_object_group_index_result_t *result);
	void (*set_key)(oss_get_object_group_index_result_t *result, const char *key);

	const char * (*get_etag)(oss_get_object_group_index_result_t *result);
	void (*set_etag)(oss_get_object_group_index_result_t *result, const char *etag);

	oss_multipart_object_group_t ** (*get_group)(oss_get_object_group_index_result_t *result, int *part_number);
	void (*set_group)(oss_get_object_group_index_result_t *result,
			oss_multipart_object_group_t **group, int part_number);
	
	size_t (*get_file_length)(oss_get_object_group_index_result_t *result);
	void (*set_file_length)(oss_get_object_group_index_result_t *result, size_t file_length);

};


extern oss_get_object_group_index_result_t * get_object_group_index_result_initialize();

extern void get_object_group_index_result_finalize(oss_get_object_group_index_result_t *result);
/**@}*/
#endif // OSS_GET_OBJECT_GROUP_INDEX_RESULT_H

