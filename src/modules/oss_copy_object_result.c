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
#include <ossc/modules/oss_copy_object_result.h>
#undef _OSS_COPY_OBJECT_RESULT_H

static const char * 
_copy_object_result_get_etag(oss_copy_object_result_t *result)
{
	return result->etag;
}

static inline void
__copy_object_result_set_etag(
		oss_copy_object_result_t *result,
		const char *etag,
		size_t etag_len)
{
	if (result->etag != NULL) {
		free(result->etag);
		result->etag = NULL;
	}

	result->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(result->etag, etag, etag_len);
	(result->etag)[etag_len] = '\0';
}

static void
_copy_object_result_set_etag(
		oss_copy_object_result_t *result,
		const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__copy_object_result_set_etag(result, etag, etag_len);
}

static const char * 
_copy_object_result_get_last_modified(oss_copy_object_result_t *result)
{
	return result->last_modified;
}

static inline void
__copy_object_result_set_last_modified(
		oss_copy_object_result_t *result,
		const char *last_modified,
		size_t last_modified_len)
{
	if (result->last_modified != NULL) {
		free(result->last_modified);
		result->last_modified = NULL;
	}

	result->last_modified = (char *)malloc(sizeof(char) * last_modified_len + 1);
	strncpy(result->last_modified, last_modified, last_modified_len);
	(result->last_modified)[last_modified_len] = '\0';
}

static void
_copy_object_result_set_last_modified(
		oss_copy_object_result_t *result,
		const char *last_modified)
{
	assert(last_modified != NULL);

	size_t last_modified_len = strlen(last_modified);
	__copy_object_result_set_last_modified(result, last_modified, last_modified_len);
}

oss_copy_object_result_t *
copy_object_result_initialize(void)
{
	oss_copy_object_result_t *result;
	result = (oss_copy_object_result_t *)malloc(sizeof(oss_copy_object_result_t));
	result->etag = NULL;
	result->last_modified = NULL;

	result->get_last_modified = _copy_object_result_get_last_modified;
	result->set_last_modified = _copy_object_result_set_last_modified;
	result->get_etag = _copy_object_result_get_etag;
	result->set_etag = _copy_object_result_set_etag;

	return result;
}

void
copy_object_result_finalize(oss_copy_object_result_t *result)
{
	assert(result != NULL);

	if (result->etag != NULL) {
		free(result->etag);
		result->etag = NULL;
	}

	if (result->last_modified != NULL) {
		free(result->last_modified);
		result->last_modified = NULL;
	}

	free(result);
}
