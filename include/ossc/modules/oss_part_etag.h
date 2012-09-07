/*
 * =============================================================================
 *
 *       Filename:  oss_part_etag.h
 *
 *    Description:  oss_part_etag.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PART_ETAG_H
# error Never include <modules/oss_part_etag.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PART_ETAG_H
#define OSS_PART_ETAG_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif


typedef struct oss_part_etag_s oss_part_etag_t;

struct oss_part_etag_s {
	char *etag;
	int part_number;

	const char * (*get_etag)(oss_part_etag_t *ol);
	void (*set_etag)(oss_part_etag_t *ol, const char *etag);
	int(*get_part_number)(oss_part_etag_t *ol);
	void (*set_part_number)(oss_part_etag_t *ol, int part_number);
};

extern oss_part_etag_t *
part_etag_initialize(int part_number, const char *etag);

extern void 
part_etag_finalize(oss_part_etag_t *ol);

#endif