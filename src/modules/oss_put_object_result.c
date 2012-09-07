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
#include <modules/oss_put_object_result.h>
#undef _OSS_PUT_OBJECT_RESULT_H



void 
put_object_result_finalize(oss_put_object_result_t *por)
{
	if (por->etag) {
		free(por->etag);
		por->etag = NULL;
	}
	if (por) {
		free(por);
		por = NULL;
	}
}

static const char * 
_put_object_result_get_etag(oss_put_object_result_t *por)
{
	return por->etag;
}

static inline void
__put_object_result_set_etag(
		oss_put_object_result_t *por, 
		const char *etag,
		size_t etag_len)
{
	if (por->etag) {
		free(por->etag);
		por->etag = NULL;
	}
	por->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(por->etag, etag, etag_len);
	(por->etag)[etag_len] = '\0';
}

static void
_put_object_result_set_etag(
		oss_put_object_result_t *por, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__put_object_result_set_etag(por, etag, etag_len);
}


oss_put_object_result_t *
put_object_result_initialize()
{
	oss_put_object_result_t *por;
	por = (oss_put_object_result_t *)malloc(sizeof(oss_put_object_result_t));
	por->etag = NULL;

	por->get_etag = _put_object_result_get_etag;
	por->set_etag = _put_object_result_set_etag;

	return por;
}
