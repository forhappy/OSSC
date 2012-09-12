/*
 * =============================================================================
 *
 *       Filename:  oss_delete_multiple_object.h
 *
 *    Description:  oss_delete_multiple_object.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
# error Never include <ossc/modules/oss_delete_multiple_object.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
#define OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H 
//#include "ossc-config.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct oss_delete_multiple_object_request_s oss_delete_multiple_object_request_t;

struct oss_delete_multiple_object_request_s{
	char *bucket_name;
	char **keys;
	bool mode; /**< set true if in quiet mode or false when in verbose. */
	int keynums;

	const char * (*get_bucket_name)(oss_delete_multiple_object_request_t *request);
	void (*set_bucket_name)(oss_delete_multiple_object_request_t *request, const char *bucket_name);

	const char ** (*get_keys)(oss_delete_multiple_object_request_t *request, int *keynums);
	void (*set_keys)(oss_delete_multiple_object_request_t *request, const char **keys, int keynums);

	bool  (*get_mode)(oss_delete_multiple_object_request_t *request);
	void (*set_mode)(oss_delete_multiple_object_request_t *request, bool mode);
};

extern oss_delete_multiple_object_request_t *
delete_multiple_object_request_initialize(
		const char *bucket_name,
		const char **key,
		int keynums,
		bool mode);

extern void 
delete_multiple_object_request_finalize(
		oss_delete_multiple_object_request_t *request);

#endif
