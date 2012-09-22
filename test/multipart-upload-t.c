/*
 * =============================================================================
 *
 *       Filename:  multipart-upload-t.c
 *
 *    Description:  multipart-upload.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_MULTIPART_UPLOAD_H
#include "oss_multipart_upload.h"
#undef _OSS_MULTIPART_UPLOAD_H

#include <stdio.h>

int main()
{
	oss_multipart_upload_t *mu;
	mu = multipart_upload_initialize();
	mu->set_initiated(mu, "initiated");
	mu->set_key(mu, "key");
	mu->set_storage_class(mu, "storage_class");
	mu->set_upload_id(mu, "upload_id");

	printf("initiated = %s\tkey = %s\tstorage_class = %s\tupload_id = %s\n", 
			mu->get_initiated(mu), mu->get_key(mu), mu->get_storage_class(mu), mu->get_upload_id(mu));
	multipart_upload_finalize(mu);
	return 0;
}
