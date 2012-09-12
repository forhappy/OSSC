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
#include <ossc/modules/oss_part_listing.h>
#undef _OSS_PART_LISTING_H

static const char * 
_part_listing_get_bucket_name(oss_part_listing_t *listing)
{
	return listing->bucket_name;
}

static inline void
__part_listing_set_bucket_name(
		oss_part_listing_t *listing,
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (listing->bucket_name != NULL) {
		free(listing->bucket_name);
		listing->bucket_name = NULL;
	}

	listing->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(listing->bucket_name, bucket_name, bucket_name_len);
	(listing->bucket_name)[bucket_name_len] = '\0';
}

static void
_part_listing_set_bucket_name(
		oss_part_listing_t *listing,
		const char *bucket_name)
{
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__part_listing_set_bucket_name(listing, bucket_name, bucket_name_len);
}

static const char * 
_part_listing_get_key(oss_part_listing_t *listing)
{
	return listing->key;
}

static inline void
__part_listing_set_key(
		oss_part_listing_t *listing,
		const char *key,
		size_t key_len)
{
	if (listing->key != NULL) {
		free(listing->key);
		listing->key = NULL;
	}
	listing->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(listing->key, key, key_len);
	(listing->key)[key_len] = '\0';
}

static void
_part_listing_set_key(
		oss_part_listing_t *listing,
		const char *key)
{
	assert(key != NULL);

	size_t key_len = strlen(key);
	__part_listing_set_key(listing, key, key_len);
}

static const char * 
_part_listing_get_upload_id(oss_part_listing_t *listing)
{
	return listing->upload_id;
}

static inline void
__part_listing_set_upload_id(
		oss_part_listing_t *listing,
		const char *upload_id,
		size_t upload_id_len)
{
	if (listing->upload_id != NULL) {
		free(listing->upload_id);
		listing->upload_id = NULL;
	}
	listing->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(listing->upload_id, upload_id, upload_id_len);
	(listing->upload_id)[upload_id_len] = '\0';
}

static void
_part_listing_set_upload_id(
		oss_part_listing_t *listing,
		const char *upload_id)
{
	assert(upload_id != NULL);

	size_t upload_id_len = strlen(upload_id);
	__part_listing_set_upload_id(listing, upload_id, upload_id_len);
}

static const char * 
_part_listing_get_storage_class(oss_part_listing_t *listing)
{
	return listing->storage_class;
}

static inline void
__part_listing_set_storage_class(
		oss_part_listing_t *listing,
		const char *storage_class,
		size_t storage_class_len)
{
	if (listing->storage_class != NULL) {
		free(listing->storage_class);
		listing->storage_class = NULL;
	}

	listing->storage_class = (char *)malloc(sizeof(char) * storage_class_len + 1);
	strncpy(listing->storage_class, storage_class, storage_class_len);
	(listing->storage_class)[storage_class_len] = '\0';
}

static void
_part_listing_set_storage_class(
		oss_part_listing_t *listing,
		const char *storage_class)
{
	assert(storage_class != NULL);

	size_t storage_class_len = strlen(storage_class);
	__part_listing_set_storage_class(listing, storage_class, storage_class_len);
}

static oss_owner_t * 
_part_listing_get_initiator(oss_part_listing_t *listing)
{
	return listing->initiator;
}


static void
_part_listing_set_initiator(
		oss_part_listing_t *listing,
		oss_owner_t *initiator)
{
	assert(initiator != NULL);
	//if(listing->initiator)
	//	owner_finalize(listing->initiator);

	listing->initiator = initiator;
	//size_t display_name_len = strlen(initiator->display_name);
	//listing->initiator->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	//strncpy(listing->initiator->display_name, initiator->display_name, display_name_len);
	//(listing->initiator->display_name)[display_name_len] = '\0';

	//size_t id_len = strlen(initiator->id);
	//listing->initiator->id = (char *)malloc(sizeof(char) * id_len + 1);
	//strncpy(listing->initiator->id, initiator->id, id_len);
	//(listing->initiator->id)[id_len] = '\0';
}

static int 
_part_listing_get_max_parts(oss_part_listing_t *listing)
{
	return listing->max_parts;
}

static void
_part_listing_set_max_parts(
		oss_part_listing_t *listing,
		int max_parts)
{
	listing->max_parts = max_parts;
}

static bool
_part_listing_get_is_truncated(oss_part_listing_t *listing)
{
	return listing->is_truncated;
}

static inline void
_part_listing_set_is_truncated(
		oss_part_listing_t *listing,
		bool is_truncated)
{
	listing->is_truncated = is_truncated;
}

static int 
_part_listing_get_next_part_number_marker(oss_part_listing_t *listing)
{
	return listing->next_part_number_marker;
}

static void
_part_listing_set_next_part_number_marker(
		oss_part_listing_t *listing,
		int next_part_number_marker)
{
	listing->next_part_number_marker = next_part_number_marker;
}

static oss_owner_t * 
_part_listing_get_owner(oss_part_listing_t *listing)
{
	return listing->owner;
}

static void
_part_listing_set_owner(
		oss_part_listing_t *listing,
		oss_owner_t *owner)
{
	assert(owner != NULL);
	//if(listing->owner)
	//	owner_finalize(listing->owner);

	listing->owner = owner;
	//size_t display_name_len = strlen(owner->display_name);
	//listing->owner->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	//strncpy(listing->owner->display_name, owner->display_name, display_name_len);
	//(listing->owner->display_name)[display_name_len] = '\0';

	//size_t id_len = strlen(owner->id);
	//listing->owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	//strncpy(listing->owner->id, owner->id, id_len);
	//(listing->owner->id)[id_len] = '\0';

}

static int 
_part_listing_get_part_number_marker(oss_part_listing_t *listing)
{
	return listing->part_number_marker;
}

static void
_part_listing_set_part_number_marker(
		oss_part_listing_t *listing,
		int part_number_marker)
{
	listing->part_number_marker = part_number_marker;
}


static oss_part_summary_t ** 
_part_listing_get_parts(oss_part_listing_t *listing, int *parts_number)
{
	*parts_number = listing->parts_number;
	return listing->parts;
}

static void
_part_listing_set_parts(
		oss_part_listing_t *listing,
		oss_part_summary_t **parts,
		int parts_number)
{
	assert(parts != NULL);
	assert(listing != NULL);

	listing->parts_number = parts_number;
	listing->parts = parts;
}



oss_part_listing_t *
part_listing_initialize(void)
{
	oss_part_listing_t *listing;
	listing = (oss_part_listing_t *)malloc(sizeof(oss_part_listing_t));

	listing->bucket_name = NULL;
	listing->key = NULL;
	listing->upload_id = NULL;
	listing->storage_class = NULL;
	listing->initiator = NULL;
	listing->max_parts = 0;
	listing->next_part_number_marker = 0;
	listing->owner = NULL;
	listing->is_truncated = false;
	listing->part_number_marker = 0;
	listing->parts = NULL;
	listing->parts_number = 0;

	listing->get_bucket_name = _part_listing_get_bucket_name;
	listing->set_bucket_name = _part_listing_set_bucket_name;
	listing->get_key = _part_listing_get_key;
	listing->set_key = _part_listing_set_key;
	listing->get_upload_id = _part_listing_get_upload_id;
	listing->set_upload_id = _part_listing_set_upload_id;
	listing->get_storage_class = _part_listing_get_storage_class;
	listing->set_storage_class = _part_listing_set_storage_class;
	listing->get_initiator = _part_listing_get_initiator;
	listing->set_initiator = _part_listing_set_initiator;
	listing->get_max_parts = _part_listing_get_max_parts;
	listing->set_max_parts = _part_listing_set_max_parts;
	listing->get_is_truncated = _part_listing_get_is_truncated;
	listing->set_is_truncated = _part_listing_set_is_truncated;
	listing->get_next_part_number_marker = _part_listing_get_next_part_number_marker;
	listing->set_next_part_number_marker = _part_listing_set_next_part_number_marker;
	listing->get_owner = _part_listing_get_owner;
	listing->set_owner = _part_listing_set_owner;
	listing->get_part_number_marker = _part_listing_get_part_number_marker;
	listing->set_part_number_marker = _part_listing_set_part_number_marker;
	listing->get_parts = _part_listing_get_parts;
	listing->set_parts = _part_listing_set_parts;

	return listing;
}

void
part_listing_finalize(oss_part_listing_t *listing)
{
	if (listing != NULL) {
		if (listing->bucket_name != NULL) {
			free(listing->bucket_name);
			listing->bucket_name = NULL;
		}
		if (listing->key != NULL) {
			free(listing->key);
			listing->key = NULL;
		}
		if (listing->upload_id != NULL) {
			free(listing->upload_id);
			listing->upload_id = NULL;
		}
		if (listing->storage_class != NULL) {
			free(listing->storage_class);
			listing->storage_class = NULL;
		}
		if (listing->initiator != NULL) {
			listing->initiator = NULL;
		}
		if (listing->owner != NULL) {
			listing->owner = NULL;
		}
		if (listing->parts != NULL) {
			listing->parts = NULL;
		}
		free(listing);
	}
}
