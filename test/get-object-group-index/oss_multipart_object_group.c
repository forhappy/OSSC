/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_object_group.c
 *
 *    Description:  oss multipart_object_group
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_MULTIPART_OBJECT_GROUP_H
#include "oss_multipart_object_group.h"
#undef _OSS_MULTIPART_OBJECT_GROUP_H

static inline const char *
_multipart_object_group_get_etag(oss_multipart_object_group_t *group)
{
	return group->etag;
}

static inline const char *
_multipart_object_group_get_part_name(oss_multipart_object_group_t *group)
{
	return group->part_name;
}

static inline int
_multipart_object_group_get_part_number(oss_multipart_object_group_t *group)
{
	return group->part_number;
}

static inline size_t
_multipart_object_group_get_part_size(oss_multipart_object_group_t *group)
{
	return group->part_size;
}

static inline void 
__multipart_object_group_set_etag(oss_multipart_object_group_t *group,
		const char *etag, size_t etag_len)
{
	assert(group != NULL);
	if (group->etag != NULL) {
		free(group->etag);
		group->etag = NULL;
	}
	 group->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	 memset(group->etag, '\0', etag_len + 1);
	 strncpy(group->etag, etag, etag_len);
}

static inline void 
_multipart_object_group_set_etag(oss_multipart_object_group_t *group, const char *etag)
{
	assert(group != NULL);
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__multipart_object_group_set_etag(group, etag, etag_len);
}

static inline void 
__multipart_object_group_set_part_name(oss_multipart_object_group_t *group,
		const char *part_name, size_t part_name_len)
{
	assert(group != NULL);
	if (group->part_name!= NULL) {
		free(group->part_name);
		group->part_name= NULL;
	}
	 group->part_name= (char *)malloc(sizeof(char) * part_name_len + 1);
	 memset(group->part_name, '\0', part_name_len+ 1);
	 strncpy(group->part_name, part_name, part_name_len);
}

static inline void 
_multipart_object_group_set_part_name(oss_multipart_object_group_t *group, const char *part_name)
{
	assert(group != NULL);
	assert(part_name != NULL);

	size_t part_name_len = strlen(part_name);
	__multipart_object_group_set_part_name(group, part_name, part_name_len);
}

static inline void
_multipart_object_group_set_part_number(oss_multipart_object_group_t *group, int part_number)
{
	assert(group != NULL);
	group->part_number = part_number;
	
}

static inline void
_multipart_object_group_set_part_size(oss_multipart_object_group_t *group, size_t part_size)
{
	assert(group != NULL);
	group->part_size = part_size;
}

oss_multipart_object_group_t *
multipart_object_group_initialize(void)
{
	oss_multipart_object_group_t *group = (oss_multipart_object_group_t *)malloc(sizeof(oss_multipart_object_group_t));

	group->etag = NULL;
	group->part_name = NULL;
	group->part_number = 0;
	group->part_size = 0;

	group->get_etag = _multipart_object_group_get_etag;
	group->get_part_name = _multipart_object_group_get_part_name;
	group->get_part_number = _multipart_object_group_get_part_number;
	group->get_part_size = _multipart_object_group_get_part_size;

	group->set_etag = _multipart_object_group_set_etag;
	group->set_part_name = _multipart_object_group_set_part_name;
	group->set_part_number = _multipart_object_group_set_part_number;
	group->set_part_size = _multipart_object_group_set_part_size;

	return group;
}

void
multipart_object_group_finalize(oss_multipart_object_group_t *group)
{
	assert(group != NULL);
	if (group != NULL) {
		if (group->etag != NULL) {
			free(group->etag);
			group->etag = NULL;
		}

		if (group->part_name != NULL) {
			free(group->part_name);
			group->part_name = NULL;
		}

		free(group);
		group = NULL;
	}
}
