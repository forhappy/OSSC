/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_part_summary.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_PART_SUMMARY_H
#include "oss_part_summary.h"
#undef _OSS_PART_SUMMARY_H

#include <stdio.h>

int main()
{
	oss_part_summary_t *ps;
	ps = part_summary_initialize();
	ps->set_etag(ps, "etag");
	ps->set_last_modified(ps, "last_modified");
	ps->set_part_number(ps, 1);
	ps->set_size(ps, 2);

	printf("etag = %s\nlast_modified = %s\npart_number = %d\nsize = %ld\n", 
			ps->get_etag(ps), ps->get_last_modified(ps), ps->get_part_number(ps), ps->get_size(ps));
	part_summary_finalize(ps);
	return 0;
}
