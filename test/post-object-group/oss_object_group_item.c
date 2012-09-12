/*
 * =============================================================================
 *
 *       Filename:  oss_object_group_item.c
 *
 *    Description:  oss object_group_item
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_OBJECT_GROUP_ITEM_H
#include "oss_object_group_item.h"
#undef _oss_object_group_item_h

static inline const char *
_object_group_item_get_etag(oss_object_group_item_t *item)
{
	return item->etag;
}

static inline const char *
_object_group_item_get_part_name(oss_object_group_item_t *item)
{
	return item->part_name;
}

static inline int
_object_group_item_get_part_number(oss_object_group_item_t *item)
{
	return item->part_number;
}


static inline void 
__object_group_item_set_etag(oss_object_group_item_t *item,
		const char *etag, size_t etag_len)
{
	assert(item != NULL);
	if (item->etag != NULL) {
		free(item->etag);
		item->etag = NULL;
	}
	
	item->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	memset(item->etag, '\0', etag_len + 1);
	strncpy(item->etag, etag, etag_len);
}

static inline void 
_object_group_item_set_etag(oss_object_group_item_t *item, const char *etag)
{
	assert(item != NULL);
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__object_group_item_set_etag(item, etag, etag_len);
}

static inline void 
__object_group_item_set_part_name(oss_object_group_item_t *item,
		const char *part_name, size_t part_name_len)
{
	assert(item != NULL);
	if (item->part_name!= NULL) {
		free(item->part_name);
		item->part_name= NULL;
	}

	 item->part_name= (char *)malloc(sizeof(char) * part_name_len + 1);
	 memset(item->part_name, '\0', part_name_len+ 1);
	 strncpy(item->part_name, part_name, part_name_len);
}

static inline void 
_object_group_item_set_part_name(oss_object_group_item_t *item, const char *part_name)
{
	assert(item != NULL);
	assert(part_name != NULL);

	size_t part_name_len = strlen(part_name);
	__object_group_item_set_part_name(item, part_name, part_name_len);
}

static inline void
_object_group_item_set_part_number(oss_object_group_item_t *item, int part_number)
{
	assert(item != NULL);
	item->part_number = part_number;
	
}


oss_object_group_item_t *
_object_group_item_initialize(const char *etag, size_t etag_len,
		const char *part_name, size_t part_name_len,
		size_t part_number)
{
	oss_object_group_item_t *item = 
		(oss_object_group_item_t *)malloc(sizeof(oss_object_group_item_t));

	item->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	memset(item->etag, '\0', etag_len + 1);
	strncpy(item->etag, etag, etag_len);

	item->part_name= (char *)malloc(sizeof(char) * part_name_len + 1);
	memset(item->part_name, '\0', part_name_len+ 1);
	strncpy(item->part_name, part_name, part_name_len);

	item->part_number = part_number;

	item->get_etag = _object_group_item_get_etag;
	item->get_part_name = _object_group_item_get_part_name;
	item->get_part_number = _object_group_item_get_part_number;

	item->set_etag = _object_group_item_set_etag;
	item->set_part_name = _object_group_item_set_part_name;
	item->set_part_number = _object_group_item_set_part_number;

	return item;
}

oss_object_group_item_t *
object_group_item_initialize(const char *etag,
		const char *part_name,
		size_t part_number)
{
	assert(etag != NULL);
	assert(part_name != NULL);
	assert(part_number > 0);

	size_t etag_len = strlen(etag);
	size_t part_name_len = strlen(part_name);

	return _object_group_item_initialize(etag,
			etag_len, part_name, part_name_len, part_number);
}

void
object_group_item_finalize(oss_object_group_item_t *item)
{
	assert(item != NULL);
	if (item != NULL) {
		if (item->etag != NULL) {
			free(item->etag);
			item->etag = NULL;
		}

		if (item->part_name != NULL) {
			free(item->part_name);
			item->part_name = NULL;
		}

		free(item);
	}
}
