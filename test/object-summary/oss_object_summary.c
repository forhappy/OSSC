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
object_summary_finalize(oss_object_summary_t *os)
{
	if (os->bucket_name) {
		free(os->bucket_name);
		os->bucket_name = NULL;
	}
	if (os->etag) {
		free(os->etag);
		os->etag = NULL;
	}
	if (os->key) {
		free(os->key);
		os->key = NULL;
	}
	if (os->last_modified) {
		free(os->last_modified);
		os->last_modified = NULL;
	}
	if (os->storage_class) {
		free(os->storage_class);
		os->storage_class = NULL;
	}
	if (os->owner) {
		owner_finalize(os->owner);
	}
	if (os) {
		free(os);
		os = NULL;
	}
}

static const char * 
_object_summary_get_bucket_name(oss_object_summary_t *os)
{
	return os->bucket_name;
}

static inline void
__object_summary_set_bucket_name(
		oss_object_summary_t *os, 
		const char *bucket_name,
		size_t bucket_name_len)
{
	if (os->bucket_name) {
		free(os->bucket_name);
		os->bucket_name = NULL;
	}
	os->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	strncpy(os->bucket_name, bucket_name, bucket_name_len);
	(os->bucket_name)[bucket_name_len] = '\0';
}

static void
_object_summary_set_bucket_name(
		oss_object_summary_t *os, 
		const char *bucket_name)
{
	assert(bucket_name != NULL);
	size_t bucket_name_len = strlen(bucket_name);
	__object_summary_set_bucket_name(os, bucket_name, bucket_name_len);
}

static const char * 
_object_summary_get_etag(oss_object_summary_t *os)
{
	return os->etag;
}

static inline void
__object_summary_set_etag(
		oss_object_summary_t *os, 
		const char *etag,
		size_t etag_len)
{
	if (os->etag) {
		free(os->etag);
		os->etag = NULL;
	}
	os->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(os->etag, etag, etag_len);
	(os->etag)[etag_len] = '\0';
}

static void
_object_summary_set_etag(
		oss_object_summary_t *os, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__object_summary_set_etag(os, etag, etag_len);
}


static const char * 
_object_summary_get_key(oss_object_summary_t *os)
{
	return os->key;
}

static inline void
__object_summary_set_key(
		oss_object_summary_t *os, 
		const char *key,
		size_t key_len)
{
	if (os->key) {
		free(os->key);
		os->key = NULL;
	}
	os->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(os->key, key, key_len);
	(os->key)[key_len] = '\0';
}

static void
_object_summary_set_key(
		oss_object_summary_t *os, 
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__object_summary_set_key(os, key, key_len);
}

static const char * 
_object_summary_get_last_modified(oss_object_summary_t *os)
{
	return os->last_modified;
}

static inline void
__object_summary_set_last_modified(
		oss_object_summary_t *os, 
		const char *last_modified,
		size_t last_modified_len)
{
	if (os->last_modified) {
		free(os->last_modified);
		os->last_modified = NULL;
	}
	os->last_modified = (char *)malloc(sizeof(char) * last_modified_len + 1);
	strncpy(os->last_modified, last_modified, last_modified_len);
	(os->last_modified)[last_modified_len] = '\0';
}

static void
_object_summary_set_last_modified(
		oss_object_summary_t *os, 
		const char *last_modified)
{
	assert(last_modified != NULL);
	size_t last_modified_len = strlen(last_modified);
	__object_summary_set_last_modified(os, last_modified, last_modified_len);
}

static const char * 
_object_summary_get_storage_class(oss_object_summary_t *os)
{
	return os->storage_class;
}

static inline void
__object_summary_set_storage_class(
		oss_object_summary_t *os, 
		const char *storage_class,
		size_t storage_class_len)
{
	if (os->storage_class) {
		free(os->storage_class);
		os->storage_class = NULL;
	}
	os->storage_class = (char *)malloc(sizeof(char) * storage_class_len + 1);
	strncpy(os->storage_class, storage_class, storage_class_len);
	(os->storage_class)[storage_class_len] = '\0';
}

static void
_object_summary_set_storage_class(
		oss_object_summary_t *os, 
		const char *storage_class)
{
	assert(storage_class != NULL);
	size_t storage_class_len = strlen(storage_class);
	__object_summary_set_storage_class(os, storage_class, storage_class_len);
}

static long
_object_summary_get_size(oss_object_summary_t *os)
{
	return os->size;
}

static void
_object_summary_set_size(
		oss_object_summary_t *os, 
		long size)
{
	os->size = size;
}


static oss_owner_t * 
_object_summary_get_owner(oss_object_summary_t *os)
{
	return os->owner;
}

static void
_object_summary_set_owner(
		oss_object_summary_t *os, 
		oss_owner_t *owner)
{
	assert(owner != NULL);
	if(os->owner)
		owner_finalize(os->owner);

	os->owner = (oss_owner_t *)malloc(sizeof(oss_owner_t));
	size_t display_name_len = strlen(owner->display_name);
	os->owner->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	strncpy(os->owner->display_name, owner->display_name, display_name_len);
	(os->owner->display_name)[display_name_len] = '\0';

	size_t id_len = strlen(owner->id);
	os->owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	strncpy(os->owner->id, owner->id, id_len);
	(os->owner->id)[id_len] = '\0';

}


oss_object_summary_t *
object_summary_initialize(void)
{
	oss_object_summary_t *os;
	os = (oss_object_summary_t *)malloc(sizeof(oss_object_summary_t));
	os->bucket_name = NULL;
	os->etag = NULL;
	os->key = NULL;
	os->last_modified = NULL;
	os->storage_class = NULL;
	os->size = 0;
	os->owner = NULL;

	os->get_bucket_name = _object_summary_get_bucket_name;
	os->set_bucket_name = _object_summary_set_bucket_name;
	os->get_key = _object_summary_get_key;
	os->set_key = _object_summary_set_key;
	os->get_last_modified = _object_summary_get_last_modified;
	os->set_last_modified = _object_summary_set_last_modified;
	os->get_storage_class = _object_summary_get_storage_class;
	os->set_storage_class = _object_summary_set_storage_class;
	os->get_size = _object_summary_get_size;
	os->set_size = _object_summary_set_size;
	os->get_etag = _object_summary_get_etag;
	os->set_etag = _object_summary_set_etag;
	os->get_owner = _object_summary_get_owner;
	os->set_owner = _object_summary_set_owner;

	return os;
}
