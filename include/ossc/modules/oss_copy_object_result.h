/*
 * =============================================================================
 *
 *       Filename:  oss_copy_object_result.h
 *
 *    Description:  oss_copy_object_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_COPY_OBJECT_RESULT_H
# error Never include <modules/oss_copy_object_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COPY_OBJECT_RESULT_H
#define OSS_COPY_OBJECT_RESULT_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_copy_object_result_t oss_copy_object_result_t
 * @{
 */

typedef struct oss_copy_object_result_s oss_copy_object_result_t;

struct oss_copy_object_result_s {
	char *etag;
	char *last_modified;

	const char * (*get_etag)(oss_copy_object_result_t *result);
	void (*set_etag)(oss_copy_object_result_t *result, const char * etag);

	const char * (*get_last_modified)(oss_copy_object_result_t *result);
	void (*set_last_modified)(oss_copy_object_result_t *result, const char *last_modified);

};

extern oss_copy_object_result_t *
copy_object_result_initialize(void);

extern void 
copy_object_result_finalize(oss_copy_object_result_t *result);
/**@}*/
#endif
