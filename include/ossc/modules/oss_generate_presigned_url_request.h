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
# error Never include <ossc/modules/oss_generate_presigned_url_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#define OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include "ossc/modules/oss_response_header_overrides.h"
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

#include <ossc/oss_map.h>

typedef struct oss_generate_presigned_url_request_s oss_generate_presigned_url_request_t;

struct oss_generate_presigned_url_request_s {
	char *bucket_name;
	char *expiration;
	char *key;
	char *method;
	oss_response_header_overrides_t *response_headers;
	oss_map_t *user_metadata;

	void (*add_user_metadata)(oss_generate_presigned_url_request_t *request,
			const char *key, const char *value);

	const char * (*get_bucket_name)(oss_generate_presigned_url_request_t *request);
	void (*set_bucket_name)(oss_generate_presigned_url_request_t *request, const char *bucket_name);
	
	const char * (*get_expiration)(oss_generate_presigned_url_request_t *request);
	void (*set_expiration)(oss_generate_presigned_url_request_t *request, const char *expiration);

	const char * (*get_key)(oss_generate_presigned_url_request_t *request);
	void (*set_key)(oss_generate_presigned_url_request_t *request, const char *key);

	const char * (*get_method)(oss_generate_presigned_url_request_t *request);
	void (*set_method)(oss_generate_presigned_url_request_t *request, const char *method);

	oss_response_header_overrides_t * (*get_response_headers)(oss_generate_presigned_url_request_t *request);
	void (*set_response_headers)(oss_generate_presigned_url_request_t *request, oss_response_header_overrides_t *response_headers);

	/**
	 * 获取用户自定义的元数据
	 */
	oss_map_t * (*get_user_metadata)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置用户自定义的元数据，表示以x-oss-meta-为前缀的请求头
	 */
	void (*set_user_metadata)(oss_generate_presigned_url_request_t *request,
			oss_map_t *user_metadata);


};

extern oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize(const char *bucket_name, const char *key);

extern oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize_with_method(const char *bucket_name, const char *key, const char *method);


extern void 
generate_presigned_url_request_finalize(oss_generate_presigned_url_request_t *request);

#endif
