/*
 * =============================================================================
 *
 *       Filename:  oss_object_group_item.c
 *
 *    Description:  oss_object_group_item testcase.
 *
 *        Created:  09/07/2012 04:00:23 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_OBJECT_GROUP_ITEM_H
#include "oss_object_group_item.h"
#undef _OSS_OBJECT_GROUP_ITEM_H

int main(int argc, const char *argv[])
{
	const char *etag = "etag";
	const char *part_name = "part_name";
	size_t part_number = 1234;

	oss_object_group_item_t *item = object_group_item_initialize(etag, part_name, part_number);
	item->set_etag(item, "etag");
	item->set_part_name(item, "part_name");
	item->set_part_number(item, 1);

	printf("etag: %s\n", item->get_etag(item));
	printf("part_name: %s\n", item->get_part_name(item));
	printf("part_number: %d\n", item->get_part_number(item));

	object_group_item_finalize(item);

	return 0;
}
