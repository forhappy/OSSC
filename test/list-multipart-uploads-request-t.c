/*
 * =============================================================================
 *
 *       Filename:  list-multipart-upload-request-t.c
 *
 *    Description:  list-multipart-upload-request testcase
 *
 *        Created:  09/04/2012 09:58:37 PM
 *s
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#include "oss_list_multipart_uploads_request.h"
#undef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H 

int main(int argc, const char *argv[])
{
	const char *bucket_name = "test_bucket_name";
	//const char *prefix         = "test_prefix";
	//const char *key_marker   = "test_key_marker";
	//const char *delimiter= "test_delimiter";
	//const char *upload_id_marker = "test_upload_id_marker";
	//unsigned int max_uploads = 32;

	oss_list_multipart_uploads_request_t *request =
		list_multipart_uploads_request_initialize(
				bucket_name);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("prefix: %s\n", request->get_prefix(request));
	printf("key_marker: %s\n", request->get_key_marker(request));
	printf("delimiter: %s\n", request->get_delimiter(request));
	printf("upload_id_marker: %s\n", request->get_upload_id_marker(request));
	printf("max_uploads: %d\n", request->get_max_uploads(request));


	request->set_bucket_name(request, "new_test_bucket_name");
	request->set_prefix(request, "new_test_prefix");
	request->set_key_marker(request, "new_test_key_marker");
	request->set_delimiter(request, "new_test_delimiter");
	request->set_upload_id_marker(request, "new_test_upload_id_marker");
	request->set_max_uploads(request, 45);

	printf("bucket_name: %s\n", request->get_bucket_name(request));
	printf("prefix: %s\n", request->get_prefix(request));
	printf("key_marker: %s\n", request->get_key_marker(request));
	printf("delimiter: %s\n", request->get_delimiter(request));
	printf("upload_id_marker: %s\n", request->get_upload_id_marker(request));
	printf("max_uploads: %d\n", request->get_max_uploads(request));

	list_multipart_uploads_request_finalize(request);

	return 0;
}

