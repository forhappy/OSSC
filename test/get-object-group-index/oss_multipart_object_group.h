/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_object_group.h
 *
 *    Description:  oss multipart_object_group
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_MULTIPART_OBJECT_GROUP_H
# error Never include <ossc/modules/oss_multipart_object_group.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_MULTIPART_OBJECT_GROUP_H
#define OSS_MULTIPART_OBJECT_GROUP_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct oss_multipart_object_group_s oss_multipart_object_group_t;

struct oss_multipart_object_group_s {
	char *etag;
	char *part_name;
	int part_number;
	size_t part_size;

	const char * (*get_etag)(oss_multipart_object_group_t *group);
	void (*set_etag)(oss_multipart_object_group_t *group, const char *etag);

	const char * (*get_part_name)(oss_multipart_object_group_t *group);
	void (*set_part_name)(oss_multipart_object_group_t *group, const char *part_name);

	int (*get_part_number)(oss_multipart_object_group_t *group);
	void (*set_part_number)(oss_multipart_object_group_t *group, int part_name);

	size_t (*get_part_size)(oss_multipart_object_group_t *group);
	void (*set_part_size)(oss_multipart_object_group_t *group, size_t part_size);

};


extern oss_multipart_object_group_t * multipart_object_group_initialize();

extern void multipart_object_group_finalize(oss_multipart_object_group_t *group);

#endif // OSS_MULTIPART_OBJECT_GROUP_H

