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
#include <modules/oss_multipart_upload.h>
#undef _OSS_MULTIPART_UPLOAD_H

void 
multipart_upload_finalize(oss_multipart_upload_t *mu)
{
	if(mu) {
		if (mu->initiated) {
			free(mu->initiated);
			mu->initiated = NULL;
		}
		if (mu->key) {
			free(mu->key);
			mu->key = NULL;
		}
		if (mu->storage_class) {
			free(mu->storage_class);
			mu->storage_class = NULL;
		}
		if (mu->upload_id) {
			free(mu->upload_id);
			mu->upload_id = NULL;
		}
		free(mu);
		mu = NULL;
	}
}

static const char * 
_multipart_upload_get_initiated(oss_multipart_upload_t *mu)
{
	return mu->initiated;
}

static inline void
__multipart_upload_set_initiated(oss_multipart_upload_t *mu, 
		const char *initiated,
		size_t initiated_len)
{
	if (mu->initiated) {
		free(mu->initiated);
		mu->initiated = NULL;
	}
	mu->initiated = (char *)malloc(sizeof(char) * initiated_len + 1);
	strncpy(mu->initiated, initiated, initiated_len);
	(mu->initiated)[initiated_len] = '\0';
}

static void
_multipart_upload_set_initiated(oss_multipart_upload_t *mu, 
		const char *initiated)
{
	assert(initiated != NULL);
	size_t initiated_len = strlen(initiated);
	__multipart_upload_set_initiated(mu, initiated, initiated_len);
}

static const char * 
_multipart_upload_get_storage_class(oss_multipart_upload_t *mu)
{
	return mu->storage_class;
}

static inline void
__multipart_upload_set_storage_class(oss_multipart_upload_t *mu, 
		const char *storage_class,
		size_t storage_class_len)
{
	if (mu->storage_class) {
		free(mu->storage_class);
		mu->storage_class = NULL;
	}
	mu->storage_class = (char *)malloc(sizeof(char) * storage_class_len + 1);
	strncpy(mu->storage_class, storage_class, storage_class_len);
	(mu->storage_class)[storage_class_len] = '\0';
}

static void
_multipart_upload_set_storage_class(oss_multipart_upload_t *mu, 
		const char *storage_class)
{
	assert(storage_class != NULL);
	size_t storage_class_len = strlen(storage_class);
	__multipart_upload_set_storage_class(mu, storage_class, storage_class_len);
}

static const char * 
_multipart_upload_get_upload_id(oss_multipart_upload_t *mu)
{
	return mu->upload_id;
}

static inline void
__multipart_upload_set_upload_id(oss_multipart_upload_t *mu, 
		const char *upload_id,
		size_t upload_id_len)
{
	if (mu->upload_id) {
		free(mu->upload_id);
		mu->upload_id = NULL;
	}
	mu->upload_id = (char *)malloc(sizeof(char) * upload_id_len + 1);
	strncpy(mu->upload_id, upload_id, upload_id_len);
	(mu->upload_id)[upload_id_len] = '\0';

}

static void
_multipart_upload_set_upload_id(oss_multipart_upload_t *mu, 
		const char *upload_id)
{
	assert(upload_id != NULL);
	size_t upload_id_len = strlen(upload_id);
	__multipart_upload_set_upload_id(mu, upload_id, upload_id_len);
}

static const char * 
_multipart_upload_get_key(oss_multipart_upload_t *mu)
{
	return mu->key;
}

static inline void
__multipart_upload_set_key(oss_multipart_upload_t *mu,
		const char *key,
		size_t key_len)
{
	if (mu->key) {
		free(mu->key);
		mu->key = NULL;
	}
	mu->key = (char *)malloc(sizeof(char) * key_len + 1);
	strncpy(mu->key, key, key_len);
	(mu->key)[key_len] = '\0';
}

static void
_multipart_upload_set_key(oss_multipart_upload_t *mu,
		const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	__multipart_upload_set_key(mu, key, key_len);
}

oss_multipart_upload_t *
multipart_upload_initialize(void)
{
	oss_multipart_upload_t *mu;
	mu = (oss_multipart_upload_t *)malloc(sizeof(oss_multipart_upload_t));
	mu->initiated = NULL;
	mu->key = NULL;
	mu->storage_class = NULL;
	mu->upload_id = NULL;
	mu->get_initiated = _multipart_upload_get_initiated;
	mu->set_initiated = _multipart_upload_set_initiated;
	mu->get_storage_class = _multipart_upload_get_storage_class;
	mu->set_storage_class = _multipart_upload_set_storage_class;
	mu->get_key = _multipart_upload_get_key;
	mu->set_key = _multipart_upload_set_key;
	mu->get_upload_id = _multipart_upload_get_upload_id;
	mu->set_upload_id = _multipart_upload_set_upload_id;
	return mu;
}
