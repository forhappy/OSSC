/*
 * =============================================================================
 *
 *       Filename:  oss_part_summary.h
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
#ifndef _OSS_PART_SUMMARY_H
# error Never include <ossc/modules/oss_part_summary.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PART_SUMMARY_H
#define OSS_PART_SUMMARY_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct oss_part_summary_s oss_part_summary_t;

struct oss_part_summary_s {
	char *etag;
	char *last_modified;
	int part_number;
	long size;

	const char * (*get_etag)(oss_part_summary_t *summary);
	void (*set_etag)(oss_part_summary_t *summary, const char *etag);

	const char * (*get_last_modified)(oss_part_summary_t *summary);
	void (*set_last_modified)(oss_part_summary_t *summary, const char *lastmodified);

	int (*get_part_number)(oss_part_summary_t *summary);
	void (*set_part_number)(oss_part_summary_t *summary, int part_number);

	long (*get_size)(oss_part_summary_t *summary);
	void (*set_size)(oss_part_summary_t *summary, long size);
};

extern oss_part_summary_t *
part_summary_initialize(void);

extern void 
part_summary_finalize(oss_part_summary_t *summary);

#endif
