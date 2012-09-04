/*
 * =============================================================================
 *
 *       Filename:  oss_abort_multipart_upload_request.h
 *
 *    Description:  abort multipart upload request structure.
 *
 *        Created:  09/04/2012 08:46:36 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
# error Never include <ossc/oss_abort_multipart_upload_request.h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H 
#define OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct oss_abort_multipart_upload_request_s \
			oss_abort_multipart_upload_request_t;

struct oss_abort_multipart_upload_request_s {
	char *bucket_name;
	char *key;
	char *upload_id;
	const char * (*get_bucket_name)(oss_abort_multipart_upload_request_t *request);
	const char * (*get_key)(oss_abort_multipart_upload_request_t *request);
	const char * (*get_upload_id)(oss_abort_multipart_upload_request_t *request);
	void (*set_bucket_name)(oss_abort_multipart_upload_request_t *request,
			const char *bucket_name);
	void (*set_key)(oss_abort_multipart_upload_request_t *request,
			const char *key);
	void (*set_upload_id)(oss_abort_multipart_upload_request_t *request,
			const char *key);
};

/* *
 * 构造函数
 * */
extern oss_abort_multipart_upload_request_t *
abort_multipart_upload_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id);

/* *
 * 析构函数
 * */
extern void
abort_multipart_upload_request_finalize(
		oss_abort_multipart_upload_request_t *request);


#endif // OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
