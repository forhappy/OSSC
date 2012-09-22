/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main testcase.
 *
 *        Created:  09/12/2012 02:38:37 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
#include "oss_delete_multiple_object_request.h"
#undef _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H

const char *keys[] = {
	"hello001",
	"hello123",
	"world001",
	"world999"};

int main()
{
	const char *bucket_name = "bucket_name";
	bool mode = true;
	int keynums = 0;

	oss_delete_multiple_object_request_t *request = delete_multiple_object_request_initialize(bucket_name, keys, 4, mode);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("mode: %d\n", request->get_mode(request));

	const char **p = request->get_keys(request, &keynums);

	for (int i = 0; i < keynums; i++) {
		printf("%s\n", *(p + i));
	}
	printf("keynums: %d\n", keynums);
	return 0;
}
