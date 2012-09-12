/*
 * =============================================================================
 *
 *       Filename:  oss_upload_part_result.c
 *
 *    Description:  oss_upload_part_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technuprogy, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_UPLOAD_PART_RESULT_H
#include <ossc/modules/oss_upload_part_result.h>
#undef _OSS_UPLOAD_PART_RESULT_H

static const char * 
_upload_part_result_get_etag(oss_upload_part_result_t *result)
{
	return result->etag;
}

static inline void
__upload_part_result_set_etag(
		oss_upload_part_result_t *result,
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
_upload_part_result_set_etag(
		oss_upload_part_result_t *result,
		const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__upload_part_result_set_etag(result, etag, etag_len);
}

static int
_upload_part_result_get_part_number(oss_upload_part_result_t *result)
{
	return result->part_number;
}

static inline void
_upload_part_result_set_part_number(
		oss_upload_part_result_t *result,
		int part_number)
{
	result->part_number = part_number;
}

static inline oss_part_etag_t * 
_upload_part_result_get_part_etag(oss_upload_part_result_t * result)
{
	oss_part_etag_t *pe = (oss_part_etag_t *)malloc(sizeof(oss_part_etag_t));
	pe->etag = result->etag;
	pe->part_number = result->part_number;
	return pe;
}



oss_upload_part_result_t *
upload_part_result_initialize()
{
	oss_upload_part_result_t *result;
	result = (oss_upload_part_result_t *)malloc(sizeof(oss_upload_part_result_t));
	result->etag = NULL;
	result->part_number = 0;

	result->get_etag = _upload_part_result_get_etag;
	result->set_etag = _upload_part_result_set_etag;
	result->get_part_number = _upload_part_result_get_part_number;
	result->set_part_number = _upload_part_result_set_part_number;
	result->get_part_etag = _upload_part_result_get_part_etag;
	return result;
}

void
upload_part_result_finalize(oss_upload_part_result_t *result)
{

	if (result != NULL) {
		if (result->etag != NULL) {
			free(result->etag);
			result->etag = NULL;
		}
		free(result);
	}
}
