/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main testcase.
 *
 *        Created:  09/07/2012 04:00:23 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_POST_OBJECT_GROUP_REQUEST_H
#include "oss_post_object_group_request.h"
#undef _OSS_POST_OBJECT_GROUP_REQUEST_H

int main(int argc, const char *argv[])
{
	int i, item_number = 3;
	oss_object_group_item_t **items = (oss_object_group_item_t **)malloc(sizeof(oss_object_group_item_t *) * item_number);
	for (i = 0; i < item_number; i++) {
		items[i] = object_group_item_initialize("eeeeeetag", "part_name", 4321);
	}

	oss_post_object_group_request_t *result = post_object_group_request_initialize("bucket_name", "key", items, 3);

	oss_object_group_item_t **pitems = result->get_items(result, &item_number);

	printf("bucket name: %s\n", result->get_bucket_name(result));
	printf("key: %s\n", result->get_key(result));

	for(i = 0; i < item_number; i++) {
		printf("etag = %s\npart_name = %s\npart_number = %d\n", (*(pitems + i))->get_etag(*(pitems + i)), (*(pitems + i))->get_part_name(*(pitems + i)), (*(pitems + i))->get_part_number(*(pitems + i)));
	}
	
	for(i = 0; i < item_number; i++) {
		object_group_item_finalize(items[i]);
	}
	post_object_group_request_finalize(result);

	return 0;
	
}
