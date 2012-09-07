/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_copy_object_result.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_COPY_OBJECT_RESULT_H
#include "oss_copy_object_result.h"
#undef _OSS_COPY_OBJECT_RESULT_H

#include <stdio.h>

int main()
{
	oss_copy_object_result_t *cor;
	cor = copy_object_result_initialize();
	cor->set_last_modified(cor, "last_modified");
	cor->set_etag(cor, "etag");

	printf("last_modified = %s\netag = %s\n", 
			cor->get_last_modified(cor), cor->get_etag(cor));

	copy_object_result_finalize(cor);
	return 0;
}
