/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_upload.h
 *
 *    Description:  oss_multipart_upload.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_MULTIPART_UPLOAD_H
#include <ossc/modules/oss_multipart_upload.h>
#undef _OSS_MULTIPART_UPLOAD_H

static const char * 
_multipart_upload_get_initiated(oss_multipart_upload_t *uploads)
{
	return uploads->initiated;
}

static inline void
__multipart_upload_set_initiated(oss_multipart_upload_t *uploads,
		const char *initiated,
		size_t initiated_len)
{
	if (uploads->initiated != NULL) {
		free(uploads->initiated);
		uploads->initiated = NULL;
	}

	uploads->initiated = (char *)malloc(sizeof(char) * initiated_len + 1);
	strncpy(uploads->initiated, initiated, initiated_len);
	(uploads->initiated)[initiated_len] = '\0';
}

static void
_multipart_upload_set_initiated(oss_multipart_upload_t *uploads,
		const char *initiated)
{
	assert(initiated != NULL);

	size_t initiated_len = strlen(initiated);
	__multipart_upload_set_initiated(uploads, initiated, initiated_len);
}

static const char * 
_multipart_upload_get_storage_class(oss_multipart_upload_t *uploads)
{
	return uploads->storage_class;
}

static inline void
__multipart_upload_set_storage_class(
		oss_multipart_upload_t *uploads,
		const char *storage_class,
		size_t storage_class_len)
{
	if (uploads->storage_class != NULL) {
		free(uploads->storage_class);
		uploads->storage_class = NULL;
	}
	uploads->storage_class = (char *)malloc(sizeof(char) * storage_class_len + 1);
	strncpy(uploads->storage_class, storage_class, storage_class_len);
	(uploads->storage_class)[storage_class_len] = '\0';
}

static void
_multipart_upload_set_storage_class(
		oss_multipart_upload_t *uploads,
		const char *storage_class)
{
	assert(storage_class != NULL);

	size_t storage_class_len = strlen(storage_class);
	__multipart_upload_set_storage_class(uploads, storage_class, storage_class_len);
}

static const char * 
_multipart_upload_get_upload_id(oss_multipart_upload_t *uploads)
{
	return uploads->upload_id;
}

static inline void
__multipart_upload_set_upload_id(oss_multipart_upload_t *uploads,
		const char *upload_id,
		size_t upload_id_len)
{
	if (uploads->upload_id != NULL) {
		free(uploads->upload_id);
		uploads->upload_id = NULL;
	}

	uploads->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(uploads->upload_id, upload_id, upload_id_len);
	(uploads->upload_id)[upload_id_len] = '\0';

}

static void
_multipart_upload_set_upload_id(oss_multipart_upload_t *uploads,
		const char *upload_id)
{
	assert(upload_id != NULL);

	size_t upload_id_len = strlen(upload_id);
	__multipart_upload_set_upload_id(uploads, upload_id, upload_id_len);
}

static const char * 
_multipart_upload_get_key(oss_multipart_upload_t *uploads)
{
	return uploads->key;
}

static inline void
__multipart_upload_set_key(oss_multipart_upload_t *uploads,
		const char *key,
		size_t key_len)
{
	if (uploads->key != NULL) {
		free(uploads->key);
		uploads->key = NULL;
	}
	uploads->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(uploads->key, key, key_len);
	(uploads->key)[key_len] = '\0';
}

static void
_multipart_upload_set_key(oss_multipart_upload_t *uploads,
		const char *key)
{
	assert(key != NULL);

	size_t key_len = strlen(key);
	__multipart_upload_set_key(uploads, key, key_len);
}

oss_multipart_upload_t *
multipart_upload_initialize(void)
{
	oss_multipart_upload_t *uploads;
	uploads = (oss_multipart_upload_t *)malloc(sizeof(oss_multipart_upload_t));

	uploads->initiated = NULL;
	uploads->key = NULL;
	uploads->storage_class = NULL;
	uploads->upload_id = NULL;

	uploads->get_initiated = _multipart_upload_get_initiated;
	uploads->set_initiated = _multipart_upload_set_initiated;
	uploads->get_storage_class = _multipart_upload_get_storage_class;
	uploads->set_storage_class = _multipart_upload_set_storage_class;
	uploads->get_key = _multipart_upload_get_key;
	uploads->set_key = _multipart_upload_set_key;
	uploads->get_upload_id = _multipart_upload_get_upload_id;
	uploads->set_upload_id = _multipart_upload_set_upload_id;

	return uploads;
}


void multipart_upload_finalize(oss_multipart_upload_t *uploads)
{
	assert(uploads != NULL);

	if (uploads->initiated != NULL) {
		free(uploads->initiated);
		uploads->initiated = NULL;
	}

	if (uploads->key != NULL) {
		free(uploads->key);
		uploads->key = NULL;
	}

	if (uploads->storage_class != NULL) {
		free(uploads->storage_class);
		uploads->storage_class = NULL;
	}

	if (uploads->upload_id != NULL) {
		free(uploads->upload_id);
		uploads->upload_id = NULL;
	}

	free(uploads);
}
