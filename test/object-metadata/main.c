/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main testcase
 *
 *        Created:  09/04/2012 09:58:37 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>

#include "oss_object_metadata.h"

int main(int argc, const char *argv[])
{

	oss_object_metadata_t *metadata =
		object_metadata_initialize();
	metadata->add_user_metadata(metadata, "hello", "world");
	metadata->set_cache_control(metadata, "ssssssssssssss");
	printf("%s\n", metadata->get_cache_control(metadata));
	oss_map_t *oss_map = metadata->get_user_metadata(metadata);

	char buf[64];

	oss_map_get(oss_map, "hello", buf, sizeof(buf));

	printf("%s\n", buf);
	
	object_metadata_finalize(metadata);

	return 0;
}

