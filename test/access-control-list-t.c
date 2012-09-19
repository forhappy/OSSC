/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_access_control_list.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
//#define _OSS_ACCESS_CONTROL_LIST_H
//#include "oss_access_control_list.h"
//#undef _OSS_ACCESS_CONTROL_LIST_H

#include <stdio.h>

#include <ossc/client.h>

int main()
{
	oss_access_control_list_t *access_control_list;
	access_control_list = access_control_list_initialize();

	oss_owner_t *owner = owner_initialize();
	owner->set_id(owner, "id");
	owner->set_display_name(owner, "display_name");

	//access_control_list->grant_permission(access_control_list, "identifier1", "permission1");
	//access_control_list->grant_permission(access_control_list, "identifier2", "permission2");
	//access_control_list->grant_permission(access_control_list, "identifier3", "permission3");
	//access_control_list->grant_permission(access_control_list, "identifier4", "permission4");
	//access_control_list->grant_permission(access_control_list, "identifier4", "permission5");
	access_control_list->set_grant(access_control_list, "grant");
	access_control_list->set_owner(access_control_list, owner);

	printf("owner->display_name = %s\nowner->id = %s\n", 
			access_control_list->owner->display_name, access_control_list->owner->id);
	
	owner = access_control_list -> get_owner(access_control_list);
	printf("owner->display_name = %s\nowner->id = %s\n", 
			access_control_list->owner->get_display_name(access_control_list->owner), access_control_list->owner->get_id(access_control_list->owner));

	printf("grant = %s\n", access_control_list->grant);
	//oss_grant_t * grant = access_control_list->get_grants(access_control_list);
	//while(grant) {
	//	printf("grant->identifier = %s, %s\ngrant->permission = %s, %s\n", grant->identifier, grant->get_identifier(grant), grant->permission, grant->get_permission(grant));
	//	grant = grant->next;
	//}


	//access_control_list->revoke_all_permissions(access_control_list, "identifier4");
	//grant = access_control_list->get_grants(access_control_list);
	//while(grant) {
	//	printf("grant->identifier = %s, %s\ngrant->permission = %s, %s\n", grant->identifier, grant->get_identifier(grant), grant->permission, grant->get_permission(grant));
	//	grant = grant->next;
	//}

	owner_finalize(owner);
	access_control_list_finalize(access_control_list);
	return 0;
}
