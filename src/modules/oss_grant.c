/*
 * =============================================================================
 *
 *       Filename:  oss_grant.c
 *
 *    Description:  oss_grant.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_GRANT_H
#include <ossc/modules/oss_grant.h>
#undef _OSS_GRANT_H

static const char * 
_grant_get_identifier(oss_grant_t *grant)
{
	return grant->identifier;
}

static inline void
__grant_set_identifier(
		oss_grant_t *grant, 
		const char *identifier,
		unsigned int identifier_len)
{
	if (grant->identifier != NULL) {
		free(grant->identifier);
		grant->identifier = NULL;
	}
	grant->identifier = (char *)malloc(sizeof(char) * identifier_len + 1);
	strncpy(grant->identifier, identifier, identifier_len);
	(grant->identifier)[identifier_len] = '\0';
}

static void
_grant_set_identifier(
		oss_grant_t *grant, 
		const char *identifier)
{
	assert(identifier != NULL);

	unsigned int identifier_len = strlen(identifier);
	__grant_set_identifier(grant, identifier, identifier_len);
}

static const char * 
_grant_get_permission(oss_grant_t *grant)
{
	return grant->permission;
}

static inline void
__grant_set_permission(
		oss_grant_t *grant, 
		const char *permission,
		unsigned int permission_len)
{
	if (grant->permission != NULL) {
		free(grant->permission);
		grant->permission = NULL;
	}
	grant->permission = (char *)malloc(sizeof(char) * permission_len + 1);
	strncpy(grant->permission, permission, permission_len);
	(grant->permission)[permission_len] = '\0';
}

static void
_grant_set_permission(
		oss_grant_t *grant, 
		const char *permission)
{
	assert(permission != NULL);

	unsigned int permission_len = strlen(permission);
	__grant_set_permission(grant, permission, permission_len);
}

oss_grant_t *
_grant_initialize(const char *identifier,
		unsigned int identifier_len,
		const char *permission,
		unsigned int permission_len)
{
	oss_grant_t *grant;
	grant = (oss_grant_t *)malloc(sizeof(oss_grant_t));

	grant->identifier = (char *)malloc(sizeof(char) * identifier_len + 1);
	strncpy(grant->identifier, identifier, identifier_len);
	(grant->identifier)[identifier_len] = '\0';

	grant->permission = (char *)malloc(sizeof(char) * permission_len + 1);
	strncpy(grant->permission, permission, permission_len);
	(grant->permission)[permission_len] = '\0';

	grant->next = NULL;

	grant->get_permission = _grant_get_permission;
	grant->set_permission = _grant_set_permission;
	grant->get_identifier = _grant_get_identifier;
	grant->set_identifier = _grant_set_identifier;

	return grant;
}

oss_grant_t *
grant_initialize(const char *identifier, const char *permission)
{
	assert(identifier != NULL);
	assert(permission != NULL);

	unsigned int identifier_len = strlen(identifier);
	unsigned int permission_len = strlen(permission);

	return _grant_initialize(identifier, identifier_len, permission, permission_len);
}

void
grant_finalize(oss_grant_t *grant)
{

	if (grant != NULL) {
		if (grant->identifier != NULL) {
			free(grant->identifier);
			grant->identifier = NULL;
		}
		if (grant->permission != NULL) {
			free(grant->permission);
			grant->permission = NULL;
		}
		free(grant);
	}
}
