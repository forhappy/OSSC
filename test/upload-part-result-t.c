/*
 * =============================================================================
 *
 *       Filename:  upload-part-result-t.c
 *
 *    Description:  upload-part-result.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technuprogy, CAS )
 *
 * =============================================================================
 */
#define _OSS_UPLOAD_PART_RESULT_H
#include "oss_upload_part_result.h"
#undef _OSS_UPLOAD_PART_RESULT_H


int main()
{
	oss_upload_part_result_t *upr;
	upr = upload_part_result_initialize();

	upr->set_etag(upr, "etag");
	upr->set_part_number(upr, 1);
	oss_part_etag_t * pe = upr->get_part_etag(upr);
	printf("etag = %s\npart_number = %d\n", 
			upr->get_etag(upr), upr->get_part_number(upr));
	printf("etag = %s\npart_number = %d\n", 
			pe->etag, pe->part_number);
	if (pe != NULL) {
		free(pe);
		pe = NULL;
	}

	upload_part_result_finalize(upr);
	return 0;
}
