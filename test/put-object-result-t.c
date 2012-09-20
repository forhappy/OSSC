/*
 * =============================================================================
 *
 *       Filename:  put-object-result-t.c
 *
 *    Description:  put-object-result.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technpeogy, CAS )
 *
 * =============================================================================
 */
#define _OSS_PUT_OBJECT_RESULT_H
#include "oss_put_object_result.h"
#undef _OSS_PUT_OBJECT_RESULT_H


int main()
{
	oss_put_object_result_t *pe;
	pe = put_object_result_initialize();

	pe->set_etag(pe, "etag");
	printf("etag = %s\n", 
			pe->get_etag(pe));


	put_object_result_finalize(pe);
	return 0;
}
