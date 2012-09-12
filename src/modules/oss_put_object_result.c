/*
 * =============================================================================
 *
 *       Filename:  oss_put_object_result.c
 *
 *    Description:  oss_put_object_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technporogy, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_PUT_OBJECT_RESULT_H
#include <ossc/modules/oss_put_object_result.h>
#undef _OSS_PUT_OBJECT_RESULT_H

static const char * 
_put_object_result_get_etag(oss_put_object_result_t *result)
{
	return result->etag;
}

static inline void
__put_object_result_set_etag(
		oss_put_object_result_t *result,
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
_put_object_result_set_etag(
		oss_put_object_result_t *result,
		const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__put_object_result_set_etag(result, etag, etag_len);
}

oss_put_object_result_t *
put_object_result_initialize()
{
	oss_put_object_result_t *result;
	result = (oss_put_object_result_t *)malloc(sizeof(oss_put_object_result_t));
	result->etag = NULL;

	result->get_etag = _put_object_result_get_etag;
	result->set_etag = _put_object_result_set_etag;

	return result;
}

void
put_object_result_finalize(oss_put_object_result_t *result)
{
	if (result != NULL) {
		if (result->etag) {
			free(result->etag);
			result->etag = NULL;
		}
		free(result);
	}
}
