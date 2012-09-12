/*
 * =============================================================================
 *
 *       Filename:  oss_put_object_result.h
 *
 *    Description:  oss_put_object_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PUT_OBJECT_RESULT_H
# error Never include <ossc/modules/oss_put_object_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PUT_OBJECT_RESULT_H
#define OSS_PUT_OBJECT_RESULT_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


typedef struct oss_put_object_result_s oss_put_object_result_t;

struct oss_put_object_result_s {
	char *etag;

	const char * (*get_etag)(oss_put_object_result_t *result);
	void (*set_etag)(oss_put_object_result_t *result, const char *etag);
};

extern oss_put_object_result_t *
put_object_result_initialize();

extern void 
put_object_result_finalize(oss_put_object_result_t *result);

#endif
