/*
 * =============================================================================
 *
 *       Filename:  oss_object_summary.h
 *
 *    Description:  oss_object_summary.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_OBJECT_SUMMARY_H
# error Never include <ossc/modules/oss_object_summary.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_SUMMARY_H
#define OSS_OBJECT_SUMMARY_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include  <ossc/modules/oss_owner.h>
#undef _OSS_OWNER_H

typedef struct oss_object_summary_s oss_object_summary_t;

struct oss_object_summary_s {
	char *type;
	char *etag;
	char *key;
	char *last_modified;
	oss_owner_t *owner;
	long size;
	char *storage_class;

	const char * (*get_type)(oss_object_summary_t *summary);
	void (*set_type)(oss_object_summary_t *summary, const char *type);

	const char * (*get_etag)(oss_object_summary_t *summary);
	void (*set_etag)(oss_object_summary_t *summary, const char * etag);

	const char * (*get_key)(oss_object_summary_t *summary);
	void (*set_key)(oss_object_summary_t *summary, const char *key);

	const char * (*get_last_modified)(oss_object_summary_t *summary);
	void (*set_last_modified)(oss_object_summary_t *summary, const char *last_modified);

	oss_owner_t * (*get_owner)(oss_object_summary_t *summary);
	void (*set_owner)(oss_object_summary_t *summary, oss_owner_t *owner);
	
	long  (*get_size)(oss_object_summary_t *summary);
	void (*set_size)(oss_object_summary_t *summary, long size);

	const char * (*get_storage_class)(oss_object_summary_t *summary);
	void (*set_storage_class)(oss_object_summary_t *summary, const char *storage_class);
};

extern oss_object_summary_t *
object_summary_initialize(void);

extern void 
object_summary_finalize(oss_object_summary_t *summary);

#endif
