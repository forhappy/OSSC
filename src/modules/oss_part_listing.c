/*
 * =============================================================================
 *
 *       Filename:  oss_part_listing.c
 *
 *    Description:  oss_part_listing.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_PART_LISTING_H
#include <modules/oss_part_listing.h>
#undef _OSS_PART_LISTING_H



void 
part_listing_finalize(oss_part_listing_t *pl)
{

	if (pl) {
		if (pl->bucket_name) {
			free(pl->bucket_name);
			pl->bucket_name = NULL;
		}
		if (pl->key) {
			free(pl->key);
			pl->key = NULL;
		}
		if (pl->upload_id) {
			free(pl->upload_id);
			pl->upload_id = NULL;
		}
		if (pl->storage_class) {
			free(pl->storage_class);
			pl->storage_class = NULL;
		}
		if (pl->initiator) {
			pl->initiator = NULL;
		}
		if (pl->owner) {
			pl->owner = NULL;
		}
		free(pl);
		pl = NULL;
	}
}

static const char * 
_part_listing_get_bucket_name(oss_part_listing_t *pl)
{
	return pl->bucket_name;
}

static inline void
__part_listing_set_bucket_name(
		oss_part_listing_t *pl, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (pl->bucket_name) {
		free(pl->bucket_name);
		pl->bucket_name = NULL;
	}
	pl->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(pl->bucket_name, bucket_name, bucket_name_len);
	(pl->bucket_name)[bucket_name_len] = '\0';
}

static void
_part_listing_set_bucket_name(
		oss_part_listing_t *pl, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__part_listing_set_bucket_name(pl, bucket_name, bucket_name_len);
}

static const char * 
_part_listing_get_key(oss_part_listing_t *pl)
{
	return pl->key;
}

static inline void
__part_listing_set_key(
		oss_part_listing_t *pl, 
		const char *key,
		size_t key_len)
{
	if (pl->key) {
		free(pl->key);
		pl->key = NULL;
	}
	pl->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(pl->key, key, key_len);
	(pl->key)[key_len] = '\0';
}

static void
_part_listing_set_key(
		oss_part_listing_t *pl, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__part_listing_set_key(pl, key, key_len);
}

static const char * 
_part_listing_get_upload_id(oss_part_listing_t *pl)
{
	return pl->upload_id;
}

static inline void
__part_listing_set_upload_id(
		oss_part_listing_t *pl, 
		const char *upload_id,
		size_t upload_id_len)
{
	if (pl->upload_id) {
		free(pl->upload_id);
		pl->upload_id = NULL;
	}
	pl->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(pl->upload_id, upload_id, upload_id_len);
	(pl->upload_id)[upload_id_len] = '\0';
}

static void
_part_listing_set_upload_id(
		oss_part_listing_t *pl, 
		const char *upload_id)
{
	assert(upload_id != NULL);
	size_t upload_id_len = strlen(upload_id);
	__part_listing_set_upload_id(pl, upload_id, upload_id_len);
}

static const char * 
_part_listing_get_storage_class(oss_part_listing_t *pl)
{
	return pl->storage_class;
}

static inline void
__part_listing_set_storage_class(
		oss_part_listing_t *pl, 
		const char *storage_class,
		size_t storage_class_len)
{
	if (pl->storage_class) {
		free(pl->storage_class);
		pl->storage_class = NULL;
	}
	pl->storage_class = (char *)malloc(sizeof(char) * storage_class_len + 1);
	strncpy(pl->storage_class, storage_class, storage_class_len);
	(pl->storage_class)[storage_class_len] = '\0';
}

static void
_part_listing_set_storage_class(
		oss_part_listing_t *pl, 
		const char *storage_class)
{
	assert(storage_class != NULL);
	size_t storage_class_len = strlen(storage_class);
	__part_listing_set_storage_class(pl, storage_class, storage_class_len);
}

static oss_owner_t * 
_part_listing_get_initiator(oss_part_listing_t *pl)
{
	return pl->initiator;
}


static void
_part_listing_set_initiator(
		oss_part_listing_t *pl, 
		oss_owner_t *initiator)
{
	assert(initiator != NULL);
	//if(pl->initiator)
	//	owner_finalize(pl->initiator);
	pl->initiator = initiator;
	//pl->initiator = owner_initialize();
	//size_t display_name_len = strlen(initiator->display_name);
	//pl->initiator->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	//strncpy(pl->initiator->display_name, initiator->display_name, display_name_len);
	//(pl->initiator->display_name)[display_name_len] = '\0';

	//size_t id_len = strlen(initiator->id);
	//pl->initiator->id = (char *)malloc(sizeof(char) * id_len + 1);
	//strncpy(pl->initiator->id, initiator->id, id_len);
	//(pl->initiator->id)[id_len] = '\0';
}

static int 
_part_listing_get_max_parts(oss_part_listing_t *pl)
{
	return pl->max_parts;
}

static void
_part_listing_set_max_parts(
		oss_part_listing_t *pl, 
		int max_parts)
{
	pl->max_parts = max_parts;
}

static bool
_part_listing_get_is_truncated(oss_part_listing_t *pl)
{
	return pl->is_truncated;
}

static inline void
_part_listing_set_is_truncated(
		oss_part_listing_t *pl, 
		bool is_truncated)
{
	pl->is_truncated = is_truncated;
}

static int 
_part_listing_get_next_part_number_marker(oss_part_listing_t *pl)
{
	return pl->next_part_number_marker;
}

static void
_part_listing_set_next_part_number_marker(
		oss_part_listing_t *pl, 
		int next_part_number_marker)
{
	pl->next_part_number_marker = next_part_number_marker;
}

static oss_owner_t * 
_part_listing_get_owner(oss_part_listing_t *pl)
{
	return pl->owner;
}

static void
_part_listing_set_owner(
		oss_part_listing_t *pl, 
		oss_owner_t *owner)
{
	assert(owner != NULL);
	//if(pl->owner)
	//	owner_finalize(pl->owner);
	pl->owner = owner;
	//pl->owner = (oss_owner_t *)malloc(sizeof(oss_owner_t));
	//size_t display_name_len = strlen(owner->display_name);
	//pl->owner->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	//strncpy(pl->owner->display_name, owner->display_name, display_name_len);
	//(pl->owner->display_name)[display_name_len] = '\0';

	//size_t id_len = strlen(owner->id);
	//pl->owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	//strncpy(pl->owner->id, owner->id, id_len);
	//(pl->owner->id)[id_len] = '\0';

}

static int 
_part_listing_get_part_number_marker(oss_part_listing_t *pl)
{
	return pl->part_number_marker;
}

static void
_part_listing_set_part_number_marker(
		oss_part_listing_t *pl, 
		int part_number_marker)
{
	pl->part_number_marker = part_number_marker;
}

oss_part_listing_t *
part_listing_initialize(void)
{
	oss_part_listing_t *pl;
	pl = (oss_part_listing_t *)malloc(sizeof(oss_part_listing_t));
	pl->bucket_name = NULL;
	pl->key = NULL;
	pl->upload_id = NULL;
	pl->storage_class = NULL;
	pl->initiator = NULL;
	pl->max_parts = 0;
	pl->next_part_number_marker = 0;
	pl->owner = NULL;
	pl->is_truncated = false;
	pl->part_number_marker = 0;

	pl->get_bucket_name = _part_listing_get_bucket_name;
	pl->set_bucket_name = _part_listing_set_bucket_name;
	pl->get_key = _part_listing_get_key;
	pl->set_key = _part_listing_set_key;
	pl->get_upload_id = _part_listing_get_upload_id;
	pl->set_upload_id = _part_listing_set_upload_id;
	pl->get_storage_class = _part_listing_get_storage_class;
	pl->set_storage_class = _part_listing_set_storage_class;
	pl->get_initiator = _part_listing_get_initiator;
	pl->set_initiator = _part_listing_set_initiator;
	pl->get_max_parts = _part_listing_get_max_parts;
	pl->set_max_parts = _part_listing_set_max_parts;
	pl->get_is_truncated = _part_listing_get_is_truncated;
	pl->set_is_truncated = _part_listing_set_is_truncated;
	pl->get_next_part_number_marker = _part_listing_get_next_part_number_marker;
	pl->set_next_part_number_marker = _part_listing_set_next_part_number_marker;
	pl->get_owner = _part_listing_get_owner;
	pl->set_owner = _part_listing_set_owner;
	pl->get_part_number_marker = _part_listing_get_part_number_marker;
	pl->set_part_number_marker = _part_listing_set_part_number_marker;

	return pl;
}
