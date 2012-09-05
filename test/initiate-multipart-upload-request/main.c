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

#define _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include "oss_initiate_multipart_upload_request.h"
#include "oss_object_metadata.h"
#undef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

int main(int argc, const char *argv[])
{
	const char *bucket_name = "test_bucket_name";
	const char *key         = "test_key";

	oss_object_metadata_t *metadata =
		object_metadata_initialize();

	metadata->add_user_metadata(metadata, "hello", "world");
	metadata->set_cache_control(metadata, "NoCacheAtAll");
#if 0
	oss_initiate_multipart_upload_request_t *request =
		initiate_multipart_upload_request_initialize(bucket_name, key);
#endif

	oss_initiate_multipart_upload_request_t *request =
		initiate_multipart_upload_request_initialize_with_metadata(bucket_name, key, metadata);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("key: %s\n", request->get_key(request));

	oss_object_metadata_t *om = request->get_object_metadata(request);

	printf("%s\n", om->get_cache_control(metadata));


	request->set_bucket_name(request, "new_test_bucket_name");
	request->set_key(request, "new_test_key");

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("key: %s\n", request->get_key(request));
	
	initiate_multipart_upload_request_finalize(request);

	return 0;
}

