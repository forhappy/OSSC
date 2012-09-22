/*
 * =============================================================================
 *
 *       Filename:  get_object_group_index_result-t.c
 *
 *    Description:  get_object_group_index_result testcase.
 *
 *        Created:  09/07/2012 04:00:23 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#include "oss_get_object_group_index_result.h"
#undef _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H

int main(int argc, const char *argv[])
{
	oss_get_object_group_index_result_t *result = get_object_group_index_result_initialize();
	result->set_bucket_name(result, "bucket_name");
	result->set_key(result, "key");
	result->set_etag(result, "etag");
	result->set_file_length(result, 1);

	int i, part_number = 3;
	oss_multipart_object_group_t **group = (oss_multipart_object_group_t **)malloc(sizeof(oss_multipart_object_group_t *) * part_number);
	for (i = 0; i < part_number; i++) {
		group[i] = multipart_object_group_initialize();
		group[i]->set_etag(group[i], "etag");
		group[i]->set_part_name(group[i], "part_name");
		group[i]->set_part_number(group[i], i);
		group[i]->set_part_size(group[i], i);
	}

	result->set_group(result, group, part_number);

	group = result->get_group(result, &part_number);

	printf("bucket name: %s\n", result->get_bucket_name(result));
	printf("key: %s\n", result->get_key(result));
	printf("etag: %s\n", result->get_etag(result));
	printf("file_length: %u\n", result->get_file_length(result));

	for(i = 0; i < part_number; i++) {
		printf("etag = %s\npart_name = %s\npart_number = %d\npart_size = %u\n", group[i]->get_etag(group[i]), group[i]->get_part_name(group[i]), group[i]->get_part_number(group[i]), group[i]->get_part_size(group[i]));
	}
	
	for(i = 0; i < part_number; i++) {
		multipart_object_group_finalize(group[i]);
	}
	get_object_group_index_result_finalize(result);

	return 0;
	
}
