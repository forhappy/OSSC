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
#include "oss_copy_object_request.h"

const char *matching[] = {"hello1",
	"hello2",
	"hello3",
	"hello4"};

int main(int argc, const char *argv[])
{
	const char *source_bucket_name = "source_bucket_name";
	const char *source_key = "source_key";
	const char *destination_bucket_name = "destination_bucket_name";
	const char *destination_key = "destination_key";
	oss_copy_object_request_t *object = 
		copy_object_request_initialize(
				source_bucket_name, source_key,
				destination_bucket_name, destination_key);
#if 1 
	object->set_matching_etag_constraints(object, matching, 4);
	
	unsigned int count;
	const char **p = object->get_matching_etag_constraints(object, &count);

	int i = 0;
	for (; i < count; i++) {
		printf("%s\n", *(p + i));
	}
#endif
	oss_object_metadata_t *metadata = object_metadata_initialize();

	metadata->add_user_metadata(metadata, "hello", "world");
	metadata->set_cache_control(metadata, "NoCache");

	object->set_new_object_metadata(object, metadata);

	//oss_object_metadata_t *result = object->get_object_metadata(object);

	printf("source_bucket_name: %s\n", object->get_source_bucket_name(object));
	printf("source_key: %s\n", object->get_source_key(object));
	printf("destination_bucket_name: %s\n", object->get_destination_bucket_name(object));
	printf("destination_key: %s\n", object->get_destination_key(object));


	char buf[64];

	oss_map_t *oss_map = metadata->get_user_metadata(metadata);

	oss_map_get(oss_map, "hello", buf, sizeof(buf));

	printf("%s\n", buf);
	
	object_metadata_finalize(metadata);
	copy_object_request_finalize(object);
	return 0;
}

