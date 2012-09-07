/*
 * =============================================================================
 *
 *       Filename:  oss_object.c
 *
 *    Description:  oss object
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_OBJECT_H
#include <modules/oss_object.h>
#undef _OSS_OBJECT_H

static inline const char *
_object_get_bucket_name(oss_object_t *object)
{
	return object->bucket_name;
}

static inline const char *
_object_get_key(oss_object_t *object)
{
	return object->key;
}

static inline const char *
_object_get_object_content(oss_object_t *object)
{
	return object->object_content;
}

static inline oss_object_metadata_t *
_object_get_object_metadata(oss_object_t *object)
{
	return object->object_metadata;
}

static inline void 
__object_set_bucket_name(oss_object_t *object,
		const char *bucket_name, size_t bucket_name_len)
{
	assert(object != NULL);
	if (object->bucket_name != NULL) {
		free(object->bucket_name);
		object->bucket_name = NULL;
	}
	 object->bucket_name = (char *)malloc(sizeof(char) * bucket_name_len + 1);
	 memset(object->bucket_name, bucket_name_len + 1, '\0');
	 strncpy(object->bucket_name, bucket_name, bucket_name_len);
}

static inline void 
_object_set_bucket_name(oss_object_t *object, const char *bucket_name)
{
	assert(object != NULL);
	assert(bucket_name != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	__object_set_bucket_name(object, bucket_name, bucket_name_len);
}

static inline void 
__object_set_key(oss_object_t *object,
		const char *key, size_t key_len)
{
	assert(object != NULL);
	if (object->key!= NULL) {
		free(object->key);
		object->key= NULL;
	}
	 object->key= (char *)malloc(sizeof(char) * key_len + 1);
	 memset(object->key, key_len+ 1, '\0');
	 strncpy(object->key, key, key_len);
}

static inline void 
_object_set_key(oss_object_t *object, const char *key)
{
	assert(object != NULL);
	assert(key != NULL);

	size_t key_len = strlen(key);
	__object_set_key(object, key, key_len);
}

static inline void
__object_set_object_content(oss_object_t *object,
		const char *object_content, size_t object_content_len)
{
	assert(object != NULL);
	if (object->object_content!= NULL) {
		free(object->object_content);
		object->object_content= NULL;
	}
	 object->object_content = (char *)malloc(sizeof(char) * object_content_len+ 1);
	 memset(object->object_content, object_content_len + 1, '\0');
	 strncpy(object->object_content, object_content, object_content_len);
}

static inline void
_object_set_object_content(oss_object_t *object,
		const char *object_content)
{
	assert(object != NULL);
	assert(object_content != NULL);

	size_t object_content_len = strlen(object_content);
	__object_set_object_content(object, object_content, object_content_len);
}

static inline void
_object_set_object_metadata(oss_object_t *object,
			oss_object_metadata_t *object_metadata)
{
	assert(object != NULL);
	assert(object_metadata != NULL);
	object->object_metadata = object_metadata;
}

oss_object_t *
object_initialize(void)
{
	oss_object_t *object = (oss_object_t *)malloc(sizeof(oss_object_t));

	object->bucket_name = NULL;
	object->key = NULL;
	object->object_content = NULL;
	object->object_metadata = NULL;

	object->get_bucket_name = _object_get_bucket_name;
	object->get_key = _object_get_key;
	object->get_object_content = _object_get_object_content;
	object->get_object_metadata = _object_get_object_metadata;

	object->set_bucket_name = _object_set_bucket_name;
	object->set_key = _object_set_key;
	object->set_object_content = _object_set_object_content;
	object->set_object_metadata = _object_set_object_metadata;

	return object;
}

void
object_finalize(oss_object_t *object)
{
	assert(object != NULL);
	if (object != NULL) {
		if (object->bucket_name != NULL) {
			free(object->bucket_name);
			object->bucket_name = NULL;
		}

		if (object->key != NULL) {
			free(object->key);
			object->key = NULL;
		}

		if (object->object_content != NULL) {
			free(object->object_content);
			object->object_content = NULL;
		}
		/* *
		 * Here we intend to leave it unfreed,
		 * because it's the caller's responsibility to free
		 * the object->object_metadata, who uses it and he has to create it
		 * and free it;
		 * */
		object->object_metadata = NULL;
	}
}
