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

const char *common_prefix[] = {"hello", 
	"worldworld",
	"hello1234",
	"china",
	"beijing",
	"ict"};

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
	int i, counts_mu = 8;
	oss_multipart_upload_t ** p = (oss_multipart_upload_t **) malloc(sizeof(oss_multipart_upload_t *) * counts_mu);
	for (i = 0; i < counts_mu; i++) {
		*(p + i) = multipart_upload_initialize();
		(*(p + i))->set_initiated(*(p + i), "true");
		(*(p + i))->set_storage_class(*(p + i), "storage_class");
		(*(p + i))->set_upload_id(*(p + i), "upload_id");
		(*(p + i))->set_key(*(p + i), "key");
	}

	mul->set_common_prefixs(mul, common_prefix, 6);
	mul->set_multipart_uploads(mul, p, 8);

	printf("bucket_name = %s\nkey_marker = %s\nupload_id_marker = %s\nnext_key_marker = %s\nnext_upload_id_marker = %s\nmax_uploads = %s\nis_truncated = %d\ndelimiter = %s\nprefix = %s\n", 
			mul->get_bucket_name(mul), mul->get_key_marker(mul), mul->get_upload_id_marker(mul), mul->get_next_key_marker(mul), mul->get_next_upload_id_marker(mul), mul->get_max_uploads(mul), mul->get_is_truncated(mul), mul->get_delimiter(mul), mul->get_prefix(mul));

	unsigned int counts;
	const char ** p1 = mul->get_common_prefixs(mul, &counts);

	for (i = 0; i < counts; i++) {
		printf("%s\n", *(p1 + i));
	}

	oss_multipart_upload_t ** p2 = mul->get_multipart_uploads(mul, &counts);

	for (i = 0; i < counts; i++) {
		printf("*********************%d**********************\n", i);
		printf("get_initiated: %s\n", (*(p2 + i))->get_initiated(*(p2 + i)));
		printf("get_storage_class: %s\n", (*(p2 + i))->get_storage_class(*(p2 + i)));
		printf("get_upload_id: %s\n", (*(p2 + i))->get_upload_id(*(p2 + i)));
		printf("get_key: %s\n", (*(p2 + i))->get_key(*(p2 + i)));
	}

	multipart_upload_listing_finalize(mul);
	return 0;
}
