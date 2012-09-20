/*
 * =============================================================================
 *
 *       Filename:  list-parts-request-t.c
 *
 *    Description:  list-parts-request testcase
 *
 *        Created:  09/04/2012 09:58:37 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_LIST_PARTS_REQUEST_H
#include "oss_list_parts_request.h"
#undef _OSS_LIST_PARTS_REQUEST_H 

int main(int argc, const char *argv[])
{
	const char *bucket_name = "test_bucket_name";
	const char *upload_id         = "test_upload_id";
	//unsigned int max_part = 23;
	const char *key= "test_key";
	//unsigned int part_number_marker = 32;

	oss_list_parts_request_t *request =
		list_parts_request_initialize(
				bucket_name, key, upload_id);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("upload_id: %s\n", request->get_upload_id(request));
	printf("max_part: %d\n", request->get_max_parts(request));
	printf("key: %s\n", request->get_key(request));
	printf("part_number_marker: %d\n", request->get_part_number_marker(request));


	request->set_bucket_name(request, "new_test_bucket_name");
	request->set_upload_id(request, "new_test_upload_id");
	request->set_max_parts(request, 444);
	request->set_key(request, "new_test_key");
	request->set_part_number_marker(request, 45);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("upload_id: %s\n", request->get_upload_id(request));
	printf("max_part: %d\n", request->get_max_parts(request));
	printf("key: %s\n", request->get_key(request));
	printf("part_number_marker: %d\n", request->get_part_number_marker(request));

	list_parts_request_finalize(request);

	return 0;
}

