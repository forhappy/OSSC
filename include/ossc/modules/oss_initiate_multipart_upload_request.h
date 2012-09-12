/*
 * =============================================================================
 *
 *       Filename:  oss_initiate_multipart_upload_request.h
 *
 *    Description:  initiate multipart upload request.
 *
 *        Created:  09/05/2012 02:32:01 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
# error Never include <ossc/modules/oss_initiate_multipart_upload_request.h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H 
#define OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include "ossc-config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _OSS_OBJECT_METADATA_H
#include <ossc/modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

typedef struct oss_initiate_multipart_upload_request_s \
			oss_initiate_multipart_upload_request_t;

struct oss_initiate_multipart_upload_request_s {
	char *bucket_name;
	char *key;
	oss_object_metadata_t *object_metadata;

	const char * (*get_bucket_name)(oss_initiate_multipart_upload_request_t *request);
	const char * (*get_key)(oss_initiate_multipart_upload_request_t *request);
	oss_object_metadata_t * (*get_object_metadata)(oss_initiate_multipart_upload_request_t *request);
	void (*set_bucket_name)(oss_initiate_multipart_upload_request_t *request,
			const char *bucket_name);
	void (*set_key)(oss_initiate_multipart_upload_request_t *request,
			const char *key);
	void (*set_object_metadata)(oss_initiate_multipart_upload_request_t *request,
			oss_object_metadata_t *object_metadata);
};

/* *
 * 构造函数
 * */
extern oss_initiate_multipart_upload_request_t *
initiate_multipart_upload_request_initialize(const char *bucket_name,
		const char *key);

/* *
 * 构造函数
 * */
extern oss_initiate_multipart_upload_request_t *
initiate_multipart_upload_request_initialize_with_metadata(const char *bucket_name,
		const char *key,
		oss_object_metadata_t *object_metadata);

/* *
 * 析构函数
 * */
extern void
initiate_multipart_upload_request_finalize(
		oss_initiate_multipart_upload_request_t *request);

#endif // OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

