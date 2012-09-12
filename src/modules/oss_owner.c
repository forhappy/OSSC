/*
 * =============================================================================
 *
 *       Filename:  oss_owner.c
 *
 *    Description:  oss owner.
 *
 *        Created:  09/05/2012 09:54:59 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_OWNER_H
#include <ossc/modules/oss_owner.h>
#undef _OSS_OWNER_H

static inline const char *
_owner_get_id(oss_owner_t *owner)
{
	return owner->id;
}

static inline const char *
_owner_get_display_name(oss_owner_t *owner)
{
	return owner->display_name;
}

static inline void
__owner_set_id(oss_owner_t *owner, const char *id, size_t id_len)
{
	assert(id != NULL);

	if (owner->id != NULL) {
		free(owner->id);
		owner->id = NULL;
	}

	owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	memset(owner->id, 0, id_len + 1);
	strncpy(owner->id, id, id_len);
	(owner->id)[id_len] = '\0';
}

static inline void
__owner_set_display_name(oss_owner_t *owner, const char *name, size_t name_len)
{
	assert(name != NULL);

	if (owner->display_name != NULL) {
		free(owner->display_name);
		owner->display_name = NULL;
	}

	owner->display_name = (char *)malloc(sizeof(char) * name_len + 1);
	memset(owner->display_name, 0, name_len + 1);
	strncpy(owner->display_name, name, name_len);
	(owner->display_name)[name_len] = '\0';
}


static inline void
_owner_set_id(oss_owner_t *owner, const char *id)
{
	assert(owner != NULL);
	assert(id != NULL);

	size_t id_len = strlen(id);

	__owner_set_id(owner, id, id_len);
}


static inline void
_owner_set_display_name(oss_owner_t *owner, const char *name)
{
	assert(owner != NULL);
	assert(name != NULL);

	size_t name_len = strlen(name);

	__owner_set_display_name(owner, name, name_len);
}

static inline oss_owner_t *
_owner_initialize_with_id_and_name(const char *id, size_t id_len,
		const char *name, size_t name_len)
{
	oss_owner_t *owner = (oss_owner_t *)malloc(
			sizeof(oss_owner_t));

	owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	owner->display_name = (char *)malloc(sizeof(char) * name_len + 1);

	memset(owner->id, 0, id_len + 1);
	memset(owner->display_name, 0, name_len + 1);

	strncpy(owner->id, id, id_len);
	strncpy(owner->display_name, name, name_len);
	
	owner->get_id = _owner_get_id;
	owner->set_id = _owner_set_id;
	owner->get_display_name = _owner_get_display_name;
	owner->set_display_name = _owner_set_display_name;
	
	return owner;

}
oss_owner_t *
owner_initialize(void)
{
	return _owner_initialize_with_id_and_name(NULL, 0,
			NULL, 0);
}

oss_owner_t *
owner_initialize_with_id(const char *id,
		const char *name)
{
	assert(id != NULL);
	assert(name != NULL);

	size_t id_len = strlen(id);
	size_t name_len = strlen(name);
	return _owner_initialize_with_id_and_name(id, id_len,
			name, name_len);
}

void
owner_finalize(oss_owner_t *owner)
{
	assert(owner != NULL);

	if (owner->id != NULL) {
		free(owner->id);
		owner->id = NULL;
	}

	if (owner->display_name != NULL) {
		free(owner->display_name);
		owner->display_name = NULL;
	}

	free(owner);
}

