/*
 * =============================================================================
 *
 *       Filename:  oss_access_control_list.c
 *
 *    Description:  oss_access_control_list.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_ACCESS_CONTROL_LIST_H
#include <ossc/modules/oss_access_control_list.h>
#undef _OSS_ACCESS_CONTROL_LIST_H

static oss_owner_t * 
_access_control_list_get_owner(oss_access_control_list_t *acl)
{
	return acl->owner;
}

static void
_access_control_list_set_owner(
		oss_access_control_list_t *acl, 
		oss_owner_t *owner)
{
	acl->owner = owner;
}

static void 
_access_control_list_grant_permission(oss_access_control_list_t *acl, 
		const char *identifier, 
		const char *permission)
{
	oss_grant_t * grant_tmp;
	grant_tmp = grant_initialize(identifier, permission);
	grant_tmp->next = acl->grants;
	acl->grants = grant_tmp;
}


static void 
_access_control_list_revoke_all_permissions(oss_access_control_list_t* acl, 
		const char *identifier)
{
	assert(identifier != NULL);
	assert(acl->grants != NULL);

	while(!(strcmp(acl->grants->identifier, identifier))) {
		oss_grant_t *tmp = acl->grants;
		acl->grants = acl->grants->next; 
		grant_finalize(tmp);
	}

	oss_grant_t *grant_tmp = acl->grants;
	oss_grant_t *grant_tmp_next = grant_tmp->next;

	while(grant_tmp_next) {
		if(!(strcmp(grant_tmp_next->identifier, identifier))) {
			grant_tmp->next = grant_tmp_next->next;
			grant_finalize(grant_tmp_next);
			grant_tmp_next = grant_tmp->next;
		} else {
			grant_tmp = grant_tmp_next;
			grant_tmp_next = grant_tmp_next->next;
		}
	}
}
	


oss_grant_t *
_access_control_list_get_grants(oss_access_control_list_t *acl)
{
	return acl->grants;
}


oss_access_control_list_t *
access_control_list_initialize(void)
{
	oss_access_control_list_t *acl;
	acl = (oss_access_control_list_t *)malloc(sizeof(oss_access_control_list_t));

	acl->owner = NULL;
	acl->grants = NULL;
	acl->grant_number_count = 0;

	acl->grant_permission = _access_control_list_grant_permission;
	acl->revoke_all_permissions = _access_control_list_revoke_all_permissions;
	acl->get_grants = _access_control_list_get_grants;
	acl->get_owner = _access_control_list_get_owner;
	acl->set_owner = _access_control_list_set_owner;

	return acl;
}

void
access_control_list_finalize(oss_access_control_list_t *acl)
{
	if (acl != NULL) {
		if (acl->owner) {
			acl->owner = NULL;
		}
		while (acl->grants) {
			oss_grant_t *tmp_grant = acl->grants->next;
			grant_finalize(acl->grants);
			acl->grants = tmp_grant;
		}
		free(acl);
	}
}
