/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_upload_part_request.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_UPLOAD_PART_REQUEST_H
#include "oss_upload_part_request.h"
#undef _OSS_UPLOAD_PART_REQUEST_H

#include <stdio.h>

int main()
{
	oss_upload_part_request_t *upr;
	upr = upload_part_request_initialize();
	upr->set_bucket_name(upr, "bucket_name");
	upr->set_key(upr, "key");
	upr->set_input_stream(upr, "input_stream", strlen("input_stream"));
	upr->set_md5_digest(upr, "md5_digest");
	upr->set_upload_id(upr, "upload_id");
	upr->set_part_number(upr, 1);
	upr->set_part_size(upr, 2);
	int input_stream_len;

	printf("bucket_name = %s\nkey = %s\ninput_stream = %s\nmd5_digest = %s\nupload_id = %s\npart_number = %d\npart_size = %ld\n", 
			upr->get_bucket_name(upr), upr->get_key(upr), upr->get_input_stream(upr, &input_stream_len), upr->get_md5_digest(upr), upr->get_upload_id(upr), upr->get_part_number(upr), upr->get_part_size(upr));
	printf("input_stream_len = %d\n", input_stream_len);
	upload_part_request_finalize(upr);
	return 0;
}
