/*
 * =============================================================================
 *
 *       Filename:  oss_copy_object_request.h
 *
 *    Description:  oss copy object request.
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_COPY_OBJECT_REQUEST_H
# error Never include <ossc/modules/oss_copy_object_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COPY_OBJECT_REQUEST_H
#define OSS_COPY_OBJECT_REQUEST_H
#include "ossc-config.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_OBJECT_METADATA_H
#include <ossc/modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

typedef struct oss_copy_object_request_s oss_copy_object_request_t;

struct oss_copy_object_request_s {
	char *destination_bucket_name;
	char *destination_key;
	char **matching_etag_constraints;
	char *modified_since_constraints;
	oss_object_metadata_t *object_metadata;
	char **no_matching_etag_constraints;
	char *source_bucket_name;
	char *source_key;
	char *unmodified_since_constraints;

	/**
	 * 内部计数器
	 */
	unsigned int _counts_matching_etag_constraints;
	unsigned int _counts_no_matching_etag_constraints;

	/**
	 * Get 方法
	 */
	const char * (*get_destination_bucket_name)(oss_copy_object_request_t *request);

	const char * (*get_destination_key)(oss_copy_object_request_t *request);

	const char ** (*get_matching_etag_constraints)(oss_copy_object_request_t *request,
			unsigned int *counts);

	const char * (*get_modified_since_constraints)(oss_copy_object_request_t *request);

	oss_object_metadata_t * (*get_object_metadata)(oss_copy_object_request_t *request);

	const char ** (*get_no_matching_etag_constraints)(oss_copy_object_request_t *request,
			unsigned int *counts);

	const char * (*get_source_bucket_name)(oss_copy_object_request_t *request);

	const char * (*get_source_key)(oss_copy_object_request_t *request);

	const char * (*get_unmodified_since_constraints)(oss_copy_object_request_t *request);

	/**
	 * Set 方法
	 */

	void (*set_destination_bucket_name)(oss_copy_object_request_t *request,
			const char * destination_bucket_name);

	void (*set_destination_key)(oss_copy_object_request_t *request,
			const char *destination_key);

	void (*set_matching_etag_constraints)(oss_copy_object_request_t *request,
			const char **matching_etag_constraints, unsigned int counts);

	void (*set_modified_since_constraints)(oss_copy_object_request_t *request,
			const char *modified_since_constraints);

	void (*set_new_object_metadata)(oss_copy_object_request_t *request,
			oss_object_metadata_t *object_metadata);

	void (*set_no_matching_etag_constraints)(oss_copy_object_request_t *request,
			const char **no_matching_etag_constraints, unsigned counts);

	void (*set_source_bucket_name)(oss_copy_object_request_t *request,
			const char *source_bucket_name);

	void (*set_source_key)(oss_copy_object_request_t *request,
			const char *source_key);

	void (*set_unmodified_since_constraints)(oss_copy_object_request_t *request,
			const char *unmodified_since_constraints);
};

extern oss_copy_object_request_t *
copy_object_request_initialize(const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key);

extern void copy_object_request_finalize(oss_copy_object_request_t *request);

#endif // OSS_COPY_OBJECT_REQUEST_H 

