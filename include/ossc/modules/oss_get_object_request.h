/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_request.h
 *
 *    Description:  oss_get_object_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_GET_OBJECT_REQUEST_H
# error Never include <ossc/modules/oss_get_object_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GET_OBJECT_REQUEST_H
#define OSS_GET_OBJECT_REQUEST_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include <ossc/modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

/**
 * @defgroup oss_get_object_request_t oss_get_object_request_t
 * @{
 */

typedef struct oss_get_object_request_s oss_get_object_request_t;

struct oss_get_object_request_s {
	char *bucket_name;
	char *key;
	char *modified_since_constraint;
	long start;
	long length;
	oss_response_header_overrides_t *response_headers;
	char *unmodified_since_constraint;
	char **matching_etag_constraints;
	char **no_matching_etag_constraints;

	unsigned int _counts_matching_etag_constraints;
	unsigned int _counts_no_matching_etag_constraints;

	const char * (*get_bucket_name)(oss_get_object_request_t *request);
	void (*set_bucket_name)(oss_get_object_request_t *request, const char *bucket_name);

	const char * (*get_key)(oss_get_object_request_t *request);
	void (*set_key)(oss_get_object_request_t *request, const char *key);

	const char * (*get_modified_since_constraint)(oss_get_object_request_t *request);
	void (*set_modified_since_constraint)(oss_get_object_request_t *request, const char *modified_since_constraint);

	void (*get_range)(oss_get_object_request_t *request, long *start, long *length);
	void (*set_range)(oss_get_object_request_t *request, long start, long length);

	oss_response_header_overrides_t * (*get_response_headers)(oss_get_object_request_t *request);
	void (*set_response_headers)(oss_get_object_request_t *request, oss_response_header_overrides_t *response_headers);

	const char * (*get_unmodified_since_constraint)(oss_get_object_request_t *request);
	void (*set_unmodified_since_constraint)(oss_get_object_request_t *request, const char *unmodified_since_constraint);

	const char ** (*get_matching_etag_constraints)(oss_get_object_request_t *object,
			unsigned int *counts);
	void (*set_matching_etag_constraints)(oss_get_object_request_t *object,
			const char **matching_etag_constraints, unsigned int counts);


	const char ** (*get_no_matching_etag_constraints)(oss_get_object_request_t *object,
			unsigned int *counts);

	void (*set_no_matching_etag_constraints)(oss_get_object_request_t *object,
			const char **no_matching_etag_constraints, unsigned counts);
};

extern oss_get_object_request_t *
get_object_request_initialize(const char *bucket_name, const char *key);

extern void 
get_object_request_finalize(oss_get_object_request_t *request);
/**@}*/
#endif
