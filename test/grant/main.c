/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_grant.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_GRANT_H
#include "oss_grant.h"
#undef _OSS_GRANT_H

#include <stdio.h>

int main()
{
	oss_grant_t *grant;
	grant = grant_initialize("identifier", "permission");
	printf("permission = %s\nidentifier = %s\n", 
			grant->get_permission(grant), grant->get_identifier(grant));

	grant->set_permission(grant, "permission_hehe");
	grant->set_identifier(grant, "identifier_hehe");

	printf("permission = %s\nidentifier = %s\n", 
			grant->get_permission(grant), grant->get_identifier(grant));

	grant_finalize(grant);
	return 0;
}
