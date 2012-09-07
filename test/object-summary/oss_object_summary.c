/*
 * =============================================================================
 *
 *       Filename:  oss_object_summary.c
 *
 *    Description:  oss_object_summary.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_OBJECT_SUMMARY_H
#include "oss_object_summary.h"
#undef _OSS_OBJECT_SUMMARY_H



void 
object_summary_finalize(oss_object_summary_t *pl)
{
	if (pl->bucket_name) {
		free(pl->bucket_name);
		pl->bucket_name = NULL;
	}
	if (pl->etag) {
		free(pl->etag);
		pl->etag = NULL;
	}
	if (pl->key) {
		free(pl->key);
		pl->key = NULL;
	}
	if (pl->last_modified) {
		free(pl->last_modified);
		pl->last_modified = NULL;
	}
	if (pl->storage_class) {
		free(pl->storage_class);
		pl->storage_class = NULL;
	}
	if (pl->owner) {
		owner_finalize(pl->owner);
	}
	if (pl) {
		free(pl);
		pl = NULL;
	}
}

static const char * 
_object_summary_get_bucket_name(oss_object_summary_t *pl)
{
	return pl->bucket_name;
}

static inline void
__object_summary_set_bucket_name(
		oss_object_summary_t *pl, 
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
_object_summary_set_bucket_name(
		oss_object_summary_t *pl, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__object_summary_set_bucket_name(pl, bucket_name, bucket_name_len);
}

static const char * 
_object_summary_get_etag(oss_object_summary_t *pl)
{
	return pl->etag;
}

static inline void
__object_summary_set_etag(
		oss_object_summary_t *pl, 
		const char *etag,
		size_t etag_len)
{
	if (pl->etag) {
		free(pl->etag);
		pl->etag = NULL;
	}
	pl->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(pl->etag, etag, etag_len);
	(pl->etag)[etag_len] = '\0';
}

static void
_object_summary_set_etag(
		oss_object_summary_t *pl, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__object_summary_set_etag(pl, etag, etag_len);
}


static const char * 
_object_summary_get_key(oss_object_summary_t *pl)
{
	return pl->key;
}

static inline void
__object_summary_set_key(
		oss_object_summary_t *pl, 
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
_object_summary_set_key(
		oss_object_summary_t *pl, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__object_summary_set_key(pl, key, key_len);
}

static const char * 
_object_summary_get_last_modified(oss_object_summary_t *pl)
{
	return pl->last_modified;
}

static inline void
__object_summary_set_last_modified(
		oss_object_summary_t *pl, 
		const char *last_modified,
		size_t last_modified_len)
{
	if (pl->last_modified) {
		free(pl->last_modified);
		pl->last_modified = NULL;
	}
	pl->last_modified = (char *)malloc(sizeof(char) * last_modified_len + 1);
	strncpy(pl->last_modified, last_modified, last_modified_len);
	(pl->last_modified)[last_modified_len] = '\0';
}

static void
_object_summary_set_last_modified(
		oss_object_summary_t *pl, 
		const char *last_modified)
{
	assert(last_modified != NULL);
	size_t last_modified_len = strlen(last_modified);
	__object_summary_set_last_modified(pl, last_modified, last_modified_len);
}

static const char * 
_object_summary_get_storage_class(oss_object_summary_t *pl)
{
	return pl->storage_class;
}

static inline void
__object_summary_set_storage_class(
		oss_object_summary_t *pl, 
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
_object_summary_set_storage_class(
		oss_object_summary_t *pl, 
		const char *storage_class)
{
	assert(storage_class != NULL);
	size_t storage_class_len = strlen(storage_class);
	__object_summary_set_storage_class(pl, storage_class, storage_class_len);
}

static long
_object_summary_get_size(oss_object_summary_t *pl)
{
	return pl->size;
}

static void
_object_summary_set_size(
		oss_object_summary_t *pl, 
		long size)
{
	pl->size = size;
}


static oss_owner_t * 
_object_summary_get_owner(oss_object_summary_t *pl)
{
	return pl->owner;
}

static void
_object_summary_set_owner(
		oss_object_summary_t *pl, 
		oss_owner_t *owner)
{
	assert(owner != NULL);
	if(pl->owner)
		owner_finalize(pl->owner);

	pl->owner = (oss_owner_t *)malloc(sizeof(oss_owner_t));
	size_t display_name_len = strlen(owner->display_name);
	pl->owner->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	strncpy(pl->owner->display_name, owner->display_name, display_name_len);
	(pl->owner->display_name)[display_name_len] = '\0';

	size_t id_len = strlen(owner->id);
	pl->owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	strncpy(pl->owner->id, owner->id, id_len);
	(pl->owner->id)[id_len] = '\0';

}


oss_object_summary_t *
object_summary_initialize(void)
{
	oss_object_summary_t *pl;
	pl = (oss_object_summary_t *)malloc(sizeof(oss_object_summary_t));
	pl->bucket_name = NULL;
	pl->etag = NULL;
	pl->key = NULL;
	pl->last_modified = NULL;
	pl->storage_class = NULL;
	pl->size = 0;
	pl->owner = NULL;

	pl->get_bucket_name = _object_summary_get_bucket_name;
	pl->set_bucket_name = _object_summary_set_bucket_name;
	pl->get_key = _object_summary_get_key;
	pl->set_key = _object_summary_set_key;
	pl->get_last_modified = _object_summary_get_last_modified;
	pl->set_last_modified = _object_summary_set_last_modified;
	pl->get_storage_class = _object_summary_get_storage_class;
	pl->set_storage_class = _object_summary_set_storage_class;
	pl->get_size = _object_summary_get_size;
	pl->set_size = _object_summary_set_size;
	pl->get_etag = _object_summary_get_etag;
	pl->set_etag = _object_summary_set_etag;
	pl->get_owner = _object_summary_get_owner;
	pl->set_owner = _object_summary_set_owner;

	return pl;
}
