/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.c
 *
 *    Description:  oss_bucket.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_BUCKET_H
#include <ossc/modules/oss_bucket.h>
#undef _OSS_BUCKET_H

static const char * 
_bucket_get_create_date(oss_bucket_t *bucket)
{
	return bucket->create_date;
}

static inline void
__bucket_set_create_date(
		oss_bucket_t *bucket, 
		const char *create_date,
		size_t create_date_len)
{
	if (bucket->create_date != NULL) {
		free(bucket->create_date);
		bucket->create_date = NULL;
	}

	bucket->create_date = (char *)malloc(sizeof(char) * create_date_len + 1);
	strncpy(bucket->create_date, create_date, create_date_len);
	(bucket->create_date)[create_date_len] = '\0';
}

static void
_bucket_set_create_date(
		oss_bucket_t *bucket, 
		const char *create_date)
{
	assert(create_date != NULL);

	size_t create_date_len = strlen(create_date);
	__bucket_set_create_date(bucket, create_date, create_date_len);
}

static const char * 
_bucket_get_name(oss_bucket_t *bucket)
{
	return bucket->name;
}

static inline void
__bucket_set_name(
		oss_bucket_t *bucket, 
		const char *name,
		size_t name_len)
{
	if (bucket->name != NULL) {
		free(bucket->name);
		bucket->name = NULL;
	}

	bucket->name = (char *)malloc(sizeof(char) * name_len + 1);
	strncpy(bucket->name, name, name_len);
	(bucket->name)[name_len] = '\0';
}

static void
_bucket_set_name(
		oss_bucket_t *bucket, 
		const char *name)
{
	assert(name != NULL);

	size_t name_len = strlen(name);
	__bucket_set_name(bucket, name, name_len);
}

static oss_owner_t * 
_bucket_get_owner(oss_bucket_t *bucket)
{
	return bucket->owner;
}

static void
_bucket_set_owner(
		oss_bucket_t *bucket, 
		oss_owner_t *owner)
{
	assert(owner != NULL);

	/* *
	 * Here we only assign the owner pointer to bucket->owner.
	 * */
	bucket->owner = owner;

	// bucket->owner = (oss_owner_t *)malloc(sizeof(oss_owner_t));
	//
	/* 
	 * bucket->owner = owner_initialize();
	 * size_t display_name_len = strlen(owner->display_name);
	 * bucket->owner->display_name = (char *)malloc(sizeof(char) * display_name_len + 1);
	 * strncpy(bucket->owner->display_name, owner->display_name, display_name_len);
	 * (bucket->owner->display_name)[display_name_len] = '\0';
	 *
	 * size_t id_len = strlen(owner->id);
	 * bucket->owner->id = (char *)malloc(sizeof(char) * id_len + 1);
	 * strncpy(bucket->owner->id, owner->id, id_len);
	 * (bucket->owner->id)[id_len] = '\0';
	 */
}


oss_bucket_t *
bucket_initialize(void)
{
	oss_bucket_t *bucket;
	bucket = (oss_bucket_t *)malloc(sizeof(oss_bucket_t));
	bucket->create_date = NULL;
	bucket->name = NULL;
	bucket->owner = NULL;

	bucket->get_create_date = _bucket_get_create_date;
	bucket->set_create_date = _bucket_set_create_date;

	bucket->get_name = _bucket_get_name;
	bucket->set_name = _bucket_set_name;

	bucket->get_owner = _bucket_get_owner;
	bucket->set_owner = _bucket_set_owner;

	return bucket;
}

oss_bucket_t *
_bucket_initialize_with_name(const char *name, size_t name_len)
{
	oss_bucket_t *bucket;
	bucket = (oss_bucket_t *)malloc(sizeof(oss_bucket_t));
	bucket->create_date = NULL;

	bucket->name = (char *)malloc(sizeof(char) * name_len + 1);
	strncpy(bucket->name, name, name_len);
	(bucket->name)[name_len] = '\0';

	bucket->owner = NULL;

	bucket->get_create_date = _bucket_get_create_date;
	bucket->set_create_date = _bucket_set_create_date;

	bucket->get_name = _bucket_get_name;
	bucket->set_name = _bucket_set_name;

	bucket->get_owner = _bucket_get_owner;
	bucket->set_owner = _bucket_set_owner;

	return bucket;
}

oss_bucket_t *
bucket_initialize_with_name(const char *name)
{
	assert(name != NULL);

	size_t name_len = strlen(name);
	return _bucket_initialize_with_name(name, name_len);
}

void
bucket_finalize(oss_bucket_t *bucket)
{
	assert(bucket != NULL);

	if (bucket->create_date != NULL) {
		free(bucket->create_date);
		bucket->create_date = NULL;
	}

	if (bucket->name != NULL) {
		free(bucket->name);
		bucket->name = NULL;
	}

	if (bucket->owner != NULL) {
		/* *
		 * We did not free bucket->owner, it depends on
		 * the creator of owner who should frees it.
		 * */
		bucket->owner = NULL;
	}

	free(bucket);
}
