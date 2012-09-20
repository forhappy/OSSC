/*
 * =============================================================================
 *
 *       Filename:  multipart_object_group-t.c
 *
 *    Description:  multipart_object_group-t testcase.
 *
 *        Created:  09/07/2012 04:00:23 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_MULTIPART_OBJECT_GROUP_H
#include "oss_multipart_object_group.h"
#undef _OSS_MULTIPART_OBJECT_GROUP_H

int main(int argc, const char *argv[])
{
	oss_multipart_object_group_t *group = multipart_object_group_initialize();
	group->set_etag(group, "etag");
	group->set_part_name(group, "part_name");
	group->set_part_number(group, 1);
	group->set_part_size(group, 2);

	printf("etag: %s\n", group->get_etag(group));
	printf("part_name: %s\n", group->get_part_name(group));
	printf("part_number: %d\n", group->get_part_number(group));
	printf("part_size: %u\n", group->get_part_size(group));

	multipart_object_group_finalize(group);

	return 0;
}
