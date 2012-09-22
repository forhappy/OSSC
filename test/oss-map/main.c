/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main testcase.
 *
 *        Created:  09/05/2012 03:06:25 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>

#include "oss_map.h"
static void iter(const char *key, const char *value, const void *obj)
{
    printf("key: %s value: %s\n", key, value);
}

int main(int argc, const char *argv[])
{
	oss_map_t *oss_map;
	char buf[256];
	int result;

	oss_map = oss_map_new(10);
	if (oss_map == NULL) {
		/* Handle allocation failure... */
	}
	/* Insert a couple of string associations */
	oss_map_put(oss_map, "application name", "Test Application");
	oss_map_put(oss_map, "application version", "1.0.0");
	oss_map_append(oss_map, "application version", ".0.0.1");
	oss_map_append(oss_map, "application version", "this is only a test.");
	
	/* Retrieve a value */
	result = oss_map_get(oss_map, "application name", buf, sizeof(buf));
	if (result == 0) {
		/* Handle value not found... */
	}
	printf("application name: %d\n",oss_map_get(oss_map, "application name", NULL, 0));
	printf("value: %s\n", buf);
	printf("%d\n", atoi("10"));
	sprintf(buf, "%d", 123456789);
	printf("%s\n", buf);
	/* When done, destroy the oss_map_t object */
	oss_map_enum(oss_map, iter, NULL);
	oss_map_delete(oss_map);
	if (oss_map == NULL) {
		printf("ooooooops, NULL\n");
	}
	return 0;
}
