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
#include <modules/oss_upload_part_result.h>
#undef _OSS_UPLOAD_PART_RESULT_H



void 
upload_part_result_finalize(oss_upload_part_result_t *upr)
{
	if (upr->etag) {
		free(upr->etag);
		upr->etag = NULL;
	}
	if (upr) {
		free(upr);
		upr = NULL;
	}
}

static const char * 
_upload_part_result_get_etag(oss_upload_part_result_t *upr)
{
	return upr->etag;
}

static inline void
__upload_part_result_set_etag(
		oss_upload_part_result_t *upr, 
		const char *etag,
		size_t etag_len)
{
	if (upr->etag) {
		free(upr->etag);
		upr->etag = NULL;
	}
	upr->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(upr->etag, etag, etag_len);
	(upr->etag)[etag_len] = '\0';
}

static void
_upload_part_result_set_etag(
		oss_upload_part_result_t *upr, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__upload_part_result_set_etag(upr, etag, etag_len);
}

static int
_upload_part_result_get_part_number(oss_upload_part_result_t *upr)
{
	return upr->part_number;
}

static inline void
_upload_part_result_set_part_number(
		oss_upload_part_result_t *upr, 
		int part_number)
{
	upr->part_number = part_number;
}

static inline oss_part_etag_t * 
_upload_part_result_get_part_etag(oss_upload_part_result_t * upr)
{
	oss_part_etag_t *pe = (oss_part_etag_t *)malloc(sizeof(oss_part_etag_t));
	pe->etag = upr->etag;
	pe->part_number = upr->part_number;
	return pe;
}



oss_upload_part_result_t *
upload_part_result_initialize()
{
	oss_upload_part_result_t *upr;
	upr = (oss_upload_part_result_t *)malloc(sizeof(oss_upload_part_result_t));
	upr->etag = NULL;
	upr->part_number = 0;

	upr->get_etag = _upload_part_result_get_etag;
	upr->set_etag = _upload_part_result_set_etag;
	upr->get_part_number = _upload_part_result_get_part_number;
	upr->set_part_number = _upload_part_result_set_part_number;
	upr->get_part_etag = _upload_part_result_get_part_etag;
	return upr;
}

