/*
 * =============================================================================
 *
 *       Filename:  oss_generate_presigned_url_request.h
 *
 *    Description:  oss_generate_presigned_url_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H
# error Never include <ossc/oss_generate_presigned_url_request.h> directly, use <ossc/client.h> instead.
#endif

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include "oss_response_header_overrides.h"
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

#ifndef OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#define OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

typedef struct oss_generate_presigned_url_request_s oss_generate_presigned_url_request_t;

struct oss_generate_presigned_url_request_s {
	char *bucket_name;
	char *expiration;
	char *key;
	char *method;
	oss_response_header_overrides_t *response_headers;
	/* 
	 * user_metadata to be continued.
	 * */
	const char * (*get_bucket_name)(oss_generate_presigned_url_request_t *gpur);
	void (*set_bucket_name)(oss_generate_presigned_url_request_t *gpur, const char *bucket_name);
	
	const char * (*get_expiration)(oss_generate_presigned_url_request_t *gpur);
	void (*set_expiration)(oss_generate_presigned_url_request_t *gpur, const char *expiration);

	const char * (*get_key)(oss_generate_presigned_url_request_t *gpur);
	void (*set_key)(oss_generate_presigned_url_request_t *gpur, const char *key);

	const char * (*get_method)(oss_generate_presigned_url_request_t *gpur);
	void (*set_method)(oss_generate_presigned_url_request_t *gpur, const char *method);

	oss_response_header_overrides_t * (*get_response_headers)(oss_generate_presigned_url_request_t *gpur);
	void (*set_response_headers)(oss_generate_presigned_url_request_t *gpur, oss_response_header_overrides_t *response_headers);

	/* 
	 * get and set and add user_metadata to be continued.
	 * */

};

extern oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize(const char *bucket_name, const char *key);

extern oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize_with_method(const char *bucket_name, const char *key, const char *method);


extern void 
generate_presigned_url_request_finalize(oss_generate_presigned_url_request_t *gpur);

#endif
