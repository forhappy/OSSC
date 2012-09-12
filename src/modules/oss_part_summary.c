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
#include <ossc/modules/oss_part_summary.h>
#undef _OSS_PART_SUMMARY_H

static const char * 
_part_summary_get_etag(oss_part_summary_t *summary)
{
	return summary->etag;
}

static inline void
__part_summary_set_etag(
		oss_part_summary_t *summary,
		const char *etag,
		size_t etag_len)
{
	if (summary->etag != NULL) {
		free(summary->etag);
		summary->etag = NULL;
	}

	summary->etag = (char *)malloc(sizeof(char) * etag_len + 1);
	strncpy(summary->etag, etag, etag_len);
	(summary->etag)[etag_len] = '\0';
}

static void
_part_summary_set_etag(
		oss_part_summary_t *summary,
		const char *etag)
{
	assert(etag != NULL);

	size_t etag_len = strlen(etag);
	__part_summary_set_etag(summary, etag, etag_len);
}

static const char * 
_part_summary_get_last_modified(oss_part_summary_t *summary)
{
	return summary->last_modified;
}

static inline void
__part_summary_set_last_modified(
		oss_part_summary_t *summary,
		const char *last_modified,
		size_t last_modified_len)
{
	if (summary->last_modified != NULL) {
		free(summary->last_modified);
		summary->last_modified = NULL;
	}
	summary->last_modified = (char *)malloc(sizeof(char) * last_modified_len + 1);
	strncpy(summary->last_modified, last_modified, last_modified_len);
	(summary->last_modified)[last_modified_len] = '\0';
}

static void
_part_summary_set_last_modified(
		oss_part_summary_t *summary,
		const char *last_modified)
{
	assert(last_modified != NULL);

	size_t last_modified_len = strlen(last_modified);
	__part_summary_set_last_modified(summary, last_modified, last_modified_len);
}

static int 
_part_summary_get_part_number(oss_part_summary_t *summary)
{
	return summary->part_number;
}

static void
_part_summary_set_part_number(
		oss_part_summary_t *summary,
		int part_number)
{
	summary->part_number = part_number;
}

static long 
_part_summary_get_size(oss_part_summary_t *summary)
{
	return summary->size;
}

static void
_part_summary_set_size(
		oss_part_summary_t *summary,
		long size)
{
	summary->size = size;
}

oss_part_summary_t *
part_summary_initialize(void)
{
	oss_part_summary_t *summary;
	summary = (oss_part_summary_t *)malloc(sizeof(oss_part_summary_t));
	summary->etag = NULL;
	summary->last_modified = NULL;
	summary->part_number = 0;
	summary->size = 0;

	summary->get_etag = _part_summary_get_etag;
	summary->set_etag = _part_summary_set_etag;
	summary->get_last_modified = _part_summary_get_last_modified;
	summary->set_last_modified = _part_summary_set_last_modified;
	summary->get_part_number = _part_summary_get_part_number;
	summary->set_part_number = _part_summary_set_part_number;
	summary->get_size = _part_summary_get_size;
	summary->set_size = _part_summary_set_size;

	return summary;
}


void
part_summary_finalize(oss_part_summary_t *summary)
{
	if (summary != NULL) {
		if (summary->etag != NULL) {
			free(summary->etag);
			summary->etag = NULL;
		}
		if (summary->last_modified != NULL) {
			free(summary->last_modified);
			summary->last_modified = NULL;
		}
		free(summary);
	}
}
