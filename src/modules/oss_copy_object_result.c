/*
 * =============================================================================
 *
 *       Filename:  oss_copy_object_result.c
 *
 *    Description:  oss_copy_object_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_COPY_OBJECT_RESULT_H
#include <modules/oss_copy_object_result.h>
#undef _OSS_COPY_OBJECT_RESULT_H



void 
copy_object_result_finalize(oss_copy_object_result_t *cor)
{
	assert(cor != NULL);

	if (cor->etag) {
		free(cor->etag);
		cor->etag = NULL;
	}
	if (cor->last_modified) {
		free(cor->last_modified);
		cor->last_modified = NULL;
	}
	if (cor) {
		free(cor);
		cor = NULL;
	}
}

static const char * 
_copy_object_result_get_etag(oss_copy_object_result_t *cor)
{
	return cor->etag;
}

static inline void
__copy_object_result_set_etag(
		oss_copy_object_result_t *cor, 
		const char *etag,
		size_t etag_len)
{
	if (cor->etag) {
		free(cor->etag);
		cor->etag = NULL;
	}
	cor->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(cor->etag, etag, etag_len);
	(cor->etag)[etag_len] = '\0';
}

static void
_copy_object_result_set_etag(
		oss_copy_object_result_t *cor, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__copy_object_result_set_etag(cor, etag, etag_len);
}

static const char * 
_copy_object_result_get_last_modified(oss_copy_object_result_t *cor)
{
	return cor->last_modified;
}

static inline void
__copy_object_result_set_last_modified(
		oss_copy_object_result_t *cor, 
		const char *last_modified,
		size_t last_modified_len)
{
	if (cor->last_modified) {
		free(cor->last_modified);
		cor->last_modified = NULL;
	}
	cor->last_modified = (char *)malloc(sizeof(char) * last_modified_len + 1);
	strncpy(cor->last_modified, last_modified, last_modified_len);
	(cor->last_modified)[last_modified_len] = '\0';
}

static void
_copy_object_result_set_last_modified(
		oss_copy_object_result_t *cor, 
		const char *last_modified)
{
	assert(last_modified != NULL);
	size_t last_modified_len = strlen(last_modified);
	__copy_object_result_set_last_modified(cor, last_modified, last_modified_len);
}

oss_copy_object_result_t *
copy_object_result_initialize(void)
{
	oss_copy_object_result_t *cor;
	cor = (oss_copy_object_result_t *)malloc(sizeof(oss_copy_object_result_t));
	cor->etag = NULL;
	cor->last_modified = NULL;

	cor->get_last_modified = _copy_object_result_get_last_modified;
	cor->set_last_modified = _copy_object_result_set_last_modified;
	cor->get_etag = _copy_object_result_get_etag;
	cor->set_etag = _copy_object_result_set_etag;

	return cor;
}
