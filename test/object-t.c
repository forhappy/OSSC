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
#include "oss_object.h"

int main(int argc, const char *argv[])
{
	oss_object_t *object = object_initialize();
	object->set_bucket_name(object, "hello world");
	object->set_key(object, "world world world");
	object->set_object_content(object, "hello hello hello", strlen("hello hello hello"));

	oss_object_metadata_t *metadata = object_metadata_initialize();

	metadata->add_user_metadata(metadata, "hello", "world");
	metadata->set_cache_control(metadata, "NoCache");

	object->set_object_metadata(object, metadata);

	oss_object_metadata_t *result = object->get_object_metadata(object);

	size_t len = 0;
	printf("bucket name: %s\n", object->get_bucket_name(object));
	printf("key: %s\n", object->get_key(object));
	printf("object content: %s\n", object->get_object_content(object, &len));


	char buf[64];

	oss_map_t *oss_map = metadata->get_user_metadata(metadata);

	oss_map_get(oss_map, "hello", buf, sizeof(buf));

	printf("%s\n", buf);
	
	object_metadata_finalize(metadata);
}
