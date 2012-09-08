/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_complete_multipart_upload_request.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#include "oss_complete_multipart_upload_request.h"
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H

#include <stdio.h>

int main()
{
	oss_complete_multipart_upload_request_t *cmur;
	int i, n = 3;
	oss_part_etag_t **part_etags = (oss_part_etag_t **)malloc(sizeof(oss_part_etag_t *) * n);
	for(i = 0; i < n; i++) {
		part_etags[i] = part_etag_initialize(i, "etag");
		//part_etags[i]->set_etag(part_etags[i], "etag");
		//part_etags[i]->set_part_number(part_etags[i], i);
	}

	cmur = complete_multipart_upload_request_initialize("bucket_name", "key", "upload_id", part_etags, n);
	
	printf("bucket_name = %s\nkey = %s\nupload_id = %s\n", 
			cmur->get_bucket_name(cmur), cmur->get_key(cmur), cmur->get_upload_id(cmur));

	part_etags = cmur->get_part_etags(cmur, &n);

	for(i = 0; i < n; i++) {
		printf("part_etags[%d]->etag = %s\tpart_etags[%d]->part_number = %d\n", i, part_etags[i]->get_etag(part_etags[i]), i, part_etags[i]->get_part_number(part_etags[i]));
	}



	//cmur->set_bucket_name(cmur, "bucket_name");
	//cmur->set_key(cmur, "key");
	//cmur->set_upload_id(cmur, "upload_id");
	//cmur->set_part_etags(cmur, "part_etags");

	for(i = 0; i < n; i++) {
		part_etag_finalize(part_etags[i]);
	}

	complete_multipart_upload_request_finalize(cmur);
	return 0;
}
