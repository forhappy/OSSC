/*
 * =============================================================================
 *
 *       Filename:  list-objects-request-t.c
 *
 *    Description:  list-objects-request testcase
 *
 *        Created:  09/04/2012 09:58:37 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_LIST_OBJECTS_REQUEST_H
#include "oss_list_objects_request.h"
#undef _OSS_LIST_OBJECTS_REQUEST_H 

int main(int argc, const char *argv[])
{
	const char *bucket_name = "test_bucket_name";
	const char *prefix         = "test_prefix";
	const char *marker   = "test_marker";
	const char *delimiter= "test_delimiter";
	unsigned int max_keys = 32;

	oss_list_objects_request_t *request =
		list_objects_request_initialize_with_args(
				bucket_name, prefix, marker, delimiter, max_keys);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("prefix: %s\n", request->get_prefix(request));
	printf("marker: %s\n", request->get_marker(request));
	printf("delimiter: %s\n", request->get_delimiter(request));
	printf("max_keys: %d\n", request->get_max_keys(request));


	request->set_bucket_name(request, "new_test_bucket_name");
	request->set_prefix(request, "new_test_prefix");
	request->set_marker(request, "new_test_marker");
	request->set_delimiter(request, "new_test_delimiter");
	request->set_max_keys(request, 45);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("prefix: %s\n", request->get_prefix(request));
	printf("marker: %s\n", request->get_marker(request));
	printf("delimiter: %s\n", request->get_delimiter(request));
	printf("max_keys: %d\n", request->get_max_keys(request));

	list_objects_request_finalize(request);

	return 0;
}

