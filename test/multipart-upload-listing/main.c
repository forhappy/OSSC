/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_multipart_upload_listing.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_MULTIPART_UPLOAD_LISTING_H
#include "oss_multipart_upload_listing.h"
#undef _OSS_MULTIPART_UPLOAD_LISTING_H

#include <stdio.h>

int main()
{
	oss_multipart_upload_listing_t *mul;
	mul = multipart_upload_listing_initialize();
	mul->set_bucket_name(mul, "bucket_name");
	mul->set_key_marker(mul, "key_marker");
	mul->set_upload_id_marker(mul, "upload_id_marker");
	mul->set_next_key_marker(mul, "next_key_marker");
	mul->set_next_upload_id_marker(mul, "next_upload_id_marker");
	mul->set_max_uploads(mul, "max_uploads");
	mul->set_is_truncated(mul, 1);
	mul->set_delimiter(mul, "delimiter");
	mul->set_prefix(mul, "prefix");

	printf("bucket_name = %s\nkey_marker = %s\nupload_id_marker = %s\nnext_key_marker = %s\nnext_upload_id_marker = %s\nmax_uploads = %s\nis_truncated = %d\ndelimiter = %s\nprefix = %s\n", 
			mul->get_bucket_name(mul), mul->get_key_marker(mul), mul->get_upload_id_marker(mul), mul->get_next_key_marker(mul), mul->get_next_upload_id_marker(mul), mul->get_max_uploads(mul), mul->get_is_truncated(mul), mul->get_delimiter(mul), mul->get_prefix(mul));
	multipart_upload_listing_finalize(mul);
	return 0;
}
