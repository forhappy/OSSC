/*
 * =============================================================================
 *
 *       Filename:  oss_initiate_multipart_upload_result.h
 *
 *    Description:  initiate multipart upload result implementation.
 *
 *        Created:  09/05/2012 12:09:28 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
# error Never include <ossc/modules/oss_initiate_multipart_upload_result.h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H 
#define OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include "ossc-config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct oss_initiate_multipart_upload_result_s \
			oss_initiate_multipart_upload_result_t;

struct oss_initiate_multipart_upload_result_s {
	char *bucket_name;
	char *key;
	char *upload_id;
	const char * (*get_bucket_name)(oss_initiate_multipart_upload_result_t *result);
	const char * (*get_key)(oss_initiate_multipart_upload_result_t *result);
	const char * (*get_upload_id)(oss_initiate_multipart_upload_result_t *result);
	void (*set_bucket_name)(oss_initiate_multipart_upload_result_t *result,
			const char *bucket_name);
	void (*set_key)(oss_initiate_multipart_upload_result_t *result,
			const char *key);
	void (*set_upload_id)(oss_initiate_multipart_upload_result_t *result,
			const char *upload_id);
};

/**
 * 构造函数
 */
extern oss_initiate_multipart_upload_result_t *
initiate_multipart_upload_result_initialize(void);

/**
 * 析构函数
 */
extern void
initiate_multipart_upload_result_finalize(
		oss_initiate_multipart_upload_result_t *result);

#endif // OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
