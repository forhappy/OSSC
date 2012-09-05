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

#define _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
#include "oss_abort_multipart_upload_request.h"
#undef _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H

int main(int argc, const char *argv[])
{
	const char *bucket_name = "test_bucket_name";
	const char *key         = "test_key";
	const char *upload_id   = "test_upload_id";

	oss_abort_multipart_upload_request_t *request =
		abort_multipart_upload_request_initialize(
				bucket_name,
				key,
				upload_id);
	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("key: %s\n", request->get_key(request));
	printf("upload_id: %s\n", request->get_upload_id(request));


	request->set_bucket_name(request, "new_test_bucket_name");
	request->set_key(request, "new_test_key");
	request->set_upload_id(request, "new_test_upload_id");

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("key: %s\n", request->get_key(request));
	printf("upload_id: %s\n", request->get_upload_id(request));
	
	abort_multipart_upload_request_finalize(request);

	return 0;
}

