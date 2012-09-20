/*
 * =============================================================================
 *
 *       Filename:  complete-multipart-upload-result-t.c
 *
 *    Description:  complete-multipart-upload-result.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#include "oss_complete_multipart_upload_result.h"
#undef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H

#include <stdio.h>

int main()
{
	oss_complete_multipart_upload_result_t *cmur;
	cmur = complete_multipart_upload_result_initialize();
	cmur->set_bucket_name(cmur, "bucket_name");
	cmur->set_key(cmur, "key");
	cmur->set_location(cmur, "location");
	cmur->set_etag(cmur, "etag");

	printf("bucket_name = %s\nkey = %s\nlocation = %s\netag = %s\n", 
			cmur->get_bucket_name(cmur), cmur->get_key(cmur), cmur->get_location(cmur), cmur->get_etag(cmur));

	complete_multipart_upload_result_finalize(cmur);
	return 0;
}
