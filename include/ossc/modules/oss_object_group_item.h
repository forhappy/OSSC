/*
 * =============================================================================
 *
 *       Filename:  oss_object_group_item.h
 *
 *    Description:  oss object_group_item
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_OBJECT_GROUP_ITEM_H
# error Never include <ossc/modules/oss_object_group_item.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_GROUP_ITEM_H 
#define OSS_OBJECT_GROUP_ITEM_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 * @defgroup oss_object_group_item_t oss_object_group_item_t
 * @{
 */

typedef struct oss_object_group_item_s oss_object_group_item_t;

struct oss_object_group_item_s {
	char *etag;
	char *part_name;
	int part_number;

	const char * (*get_etag)(oss_object_group_item_t *item);
	void (*set_etag)(oss_object_group_item_t *item, const char *etag);

	const char * (*get_part_name)(oss_object_group_item_t *item);
	void (*set_part_name)(oss_object_group_item_t *item, const char *part_name);

	int (*get_part_number)(oss_object_group_item_t *item);
	void (*set_part_number)(oss_object_group_item_t *item, int part_name);
};


extern oss_object_group_item_t * object_group_item_initialize();

extern void object_group_item_finalize(oss_object_group_item_t *item);
/**@}*/
#endif // OSS_OBJECT_GROUP_ITEM_H 

