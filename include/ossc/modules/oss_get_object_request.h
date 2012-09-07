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
# error Never include <modules/oss_get_object_request.h> directly, use <ossc/client.h> instead.
#endif

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include <modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

#ifndef OSS_GET_OBJECT_REQUEST_H
#define OSS_GET_OBJECT_REQUEST_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

typedef struct oss_get_object_request_s oss_get_object_request_t;

struct oss_get_object_request_s {
	char *bucket_name;
	char *key;
	char *modified_since_constraint;
	long start;
	long length;
	oss_response_header_overrides_t *response_headers;
	char *unmodified_since_constraint;
	/* 
	 * MatchingETagConstraints and NonmatchingETagConstraints to be continued.
	 * */
	const char * (*get_bucket_name)(oss_get_object_request_t *gor);
	void (*set_bucket_name)(oss_get_object_request_t *gor, const char *bucket_name);

	const char * (*get_key)(oss_get_object_request_t *gor);
	void (*set_key)(oss_get_object_request_t *gor, const char *key);

	const char * (*get_modified_since_constraint)(oss_get_object_request_t *gor);
	void (*set_modified_since_constraint)(oss_get_object_request_t *gor, const char *modified_since_constraint);

	void (*get_range)(oss_get_object_request_t *gor, long *start, long *length);
	void (*set_range)(oss_get_object_request_t *gor, long start, long length);

	oss_response_header_overrides_t * (*get_response_headers)(oss_get_object_request_t *gor);
	void (*set_response_headers)(oss_get_object_request_t *gor, oss_response_header_overrides_t *response_headers);

	const char * (*get_unmodified_since_constraint)(oss_get_object_request_t *gor);
	void (*set_unmodified_since_constraint)(oss_get_object_request_t *gor, const char *unmodified_since_constraint);
	/* 
	 * get and set matchingETagConstraints to be continued.
	 * */

	/*
	 * get and set NonmatchingETagConstraints to be continued.
	 */

};

extern oss_get_object_request_t *
get_object_request_initialize(const char *bucket_name, const char *key);

extern void 
get_object_request_finalize(oss_get_object_request_t *gor);

#endif
