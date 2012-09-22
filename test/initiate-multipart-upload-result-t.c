/*
 * =============================================================================
 *
 *       Filename:  initiate-multipart-upload-result-t.c
 *
 *    Description:  initiate-multipart-upload-result testcase
 *
 *        Created:  09/04/2012 09:58:37 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include "oss_initiate_multipart_upload_result.h"
#undef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H

int main(int argc, const char *argv[])
{
	//const char *bucket_name = "test_bucket_name";
	//const char *key         = "test_key";
	//const char *upload_id   = "test_upload_id";

	oss_initiate_multipart_upload_result_t *result =
		initiate_multipart_upload_result_initialize();
	printf("bucket_name: %s\n", result->get_bucket_name(result));
	printf("key: %s\n", result->get_key(result));
	printf("upload_id: %s\n", result->get_upload_id(result));


	result->set_bucket_name(result, "new_test_bucket_name");
	result->set_key(result, "new_test_key");
	result->set_upload_id(result, "new_test_upload_id");

	printf("bucket_name: %s\n", result->get_bucket_name(result));
	printf("key: %s\n", result->get_key(result));
	printf("upload_id: %s\n", result->get_upload_id(result));
	
	initiate_multipart_upload_result_finalize(result);

	return 0;
}

