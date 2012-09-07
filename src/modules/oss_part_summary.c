/*
 * =============================================================================
 *
 *       Filename:  oss_part_summary.c
 *
 *    Description:  oss_part_summary.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_PART_SUMMARY_H
#include <modules/oss_part_summary.h>
#undef _OSS_PART_SUMMARY_H



void 
part_summary_finalize(oss_part_summary_t *ps)
{

	if (ps) {
		if (ps->etag) {
			free(ps->etag);
			ps->etag = NULL;
		}
		if (ps->last_modified) {
			free(ps->last_modified);
			ps->last_modified = NULL;
		}
		free(ps);
		ps = NULL;
	}
}

static const char * 
_part_summary_get_etag(oss_part_summary_t *ps)
{
	return ps->etag;
}

static inline void
__part_summary_set_etag(
		oss_part_summary_t *ps, 
		const char *etag,
		size_t etag_len)
{
	if (ps->etag) {
		free(ps->etag);
		ps->etag = NULL;
	}
	ps->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(ps->etag, etag, etag_len);
	(ps->etag)[etag_len] = '\0';
}

static void
_part_summary_set_etag(
		oss_part_summary_t *ps, 
		const char *etag)
{
	assert(etag != NULL);
	size_t etag_len = strlen(etag);
	__part_summary_set_etag(ps, etag, etag_len);
}

static const char * 
_part_summary_get_last_modified(oss_part_summary_t *ps)
{
	return ps->last_modified;
}

static inline void
__part_summary_set_last_modified(
		oss_part_summary_t *ps, 
		const char *last_modified,
		size_t last_modified_len)
{
	if (ps->last_modified) {
		free(ps->last_modified);
		ps->last_modified = NULL;
	}
	ps->last_modified = (char *)malloc(sizeof(char) * last_modified_len + 1);
	strncpy(ps->last_modified, last_modified, last_modified_len);
	(ps->last_modified)[last_modified_len] = '\0';
}

static void
_part_summary_set_last_modified(
		oss_part_summary_t *ps, 
		const char *last_modified)
{
	assert(last_modified != NULL);
	size_t last_modified_len = strlen(last_modified);
	__part_summary_set_last_modified(ps, last_modified, last_modified_len);
}

static int 
_part_summary_get_part_number(oss_part_summary_t *ps)
{
	return ps->part_number;
}

static void
_part_summary_set_part_number(
		oss_part_summary_t *ps, 
		int part_number)
{
	ps->part_number = part_number;
}

static long 
_part_summary_get_size(oss_part_summary_t *ps)
{
	return ps->size;
}

static void
_part_summary_set_size(
		oss_part_summary_t *ps, 
		long size)
{
	ps->size = size;
}

oss_part_summary_t *
part_summary_initialize(void)
{
	oss_part_summary_t *ps;
	ps = (oss_part_summary_t *)malloc(sizeof(oss_part_summary_t));
	ps->etag = NULL;
	ps->last_modified = NULL;
	ps->part_number = 0;
	ps->size = 0;

	ps->get_etag = _part_summary_get_etag;
	ps->set_etag = _part_summary_set_etag;
	ps->get_last_modified = _part_summary_get_last_modified;
	ps->set_last_modified = _part_summary_set_last_modified;
	ps->get_part_number = _part_summary_get_part_number;
	ps->set_part_number = _part_summary_set_part_number;
	ps->get_size = _part_summary_get_size;
	ps->set_size = _part_summary_set_size;

	return ps;
}
