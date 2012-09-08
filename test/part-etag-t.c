/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_part_etag.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technpeogy, CAS )
 *
 * =============================================================================
 */
#define _OSS_PART_ETAG_H
#include "oss_part_etag.h"
#undef _OSS_PART_ETAG_H


int main()
{
	oss_part_etag_t *pe;
	pe = part_etag_initialize(2, "etag");
	printf("etag = %s\npart_number = %d\n", 
			pe->get_etag(pe), pe->get_part_number(pe));

	pe->set_etag(pe, "etag_again");
	pe->set_part_number(pe, 5);
	printf("etag = %s\npart_number = %d\n", 
			pe->get_etag(pe), pe->get_part_number(pe));


	part_etag_finalize(pe);
	return 0;
}
