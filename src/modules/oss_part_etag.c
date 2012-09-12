/*
 * =============================================================================
 *
 *       Filename:  oss_part_etag.c
 *
 *    Description:  oss_part_etag.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technpeogy, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_PART_ETAG_H
#include <ossc/modules/oss_part_etag.h>
#undef _OSS_PART_ETAG_H



void part_etag_finalize(oss_part_etag_t *etag)
{
	assert(etag != NULL);
	if (etag->etag != NULL) {
		free(etag->etag);
		etag->etag = NULL;
	}
	free(etag);

}

static const char * 
_part_etag_get_etag(oss_part_etag_t *etag)
{
	return etag->etag;
}

static inline void
__part_etag_set_etag(
		oss_part_etag_t *pe, 
		const char *etag,
		size_t etag_len)
{
	if (pe->etag != NULL) {
		free(pe->etag);
		pe->etag = NULL;
	}

	pe->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(pe->etag, etag, etag_len);
	(pe->etag)[etag_len] = '\0';
}

static void
_part_etag_set_etag(
		oss_part_etag_t *pe, 
		const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__part_etag_set_etag(pe, etag, etag_len);
}

static int
_part_etag_get_part_number(oss_part_etag_t *part_etag)
{
	return part_etag->part_number;
}

static inline void
_part_etag_set_part_number(
		oss_part_etag_t *part_etag,
		int part_number)
{
	part_etag->part_number = part_number;
}

oss_part_etag_t *
_part_etag_initialize(int part_number, const char *etag, size_t etag_len)
{
	oss_part_etag_t *pe;
	pe = (oss_part_etag_t *)malloc(sizeof(oss_part_etag_t));
	pe->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(pe->etag, etag, etag_len);
	(pe->etag)[etag_len] = '\0';
	pe->part_number = part_number;

	pe->get_etag = _part_etag_get_etag;
	pe->set_etag = _part_etag_set_etag;
	pe->get_part_number = _part_etag_get_part_number;
	pe->set_part_number = _part_etag_set_part_number;

	return pe;
}

oss_part_etag_t *
part_etag_initialize(int part_number, const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	return _part_etag_initialize(part_number, etag, etag_len);
}
