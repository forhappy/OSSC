/*
 * =============================================================================
 *
 *       Filename:  oss_list_multipart_uploads_request.h
 *
 *    Description:  list multipart uploads request implementation.
 *
 *        Created:  09/05/2012 11:31:02 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
# error Never include <modules/oss_list_multipart_uploads_request_h_> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#define OSS_LIST_MULTIPART_UPLOADS_REQUEST_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct oss_list_multipart_uploads_request_s \
			oss_list_multipart_uploads_request_t;

struct oss_list_multipart_uploads_request_s {
	char *bucket_name;
	char *delimiter;
	char *key_marker;
	unsigned int max_uploads;
	char *prefix;
	char *upload_id_marker;

	const char * (*get_bucket_name)(oss_list_multipart_uploads_request_t *request);
	const char * (*get_delimiter)(oss_list_multipart_uploads_request_t *request);
	const char * (*get_key_marker)(oss_list_multipart_uploads_request_t *request);
	unsigned int (*get_max_uploads)(oss_list_multipart_uploads_request_t *request);
	const char * (*get_prefix)(oss_list_multipart_uploads_request_t *request);
	const char * (*get_upload_id_marker)(oss_list_multipart_uploads_request_t *request);

	void (*set_bucket_name)(oss_list_multipart_uploads_request_t *request,
			const char *bucket_name);
	void (*set_delimiter)(oss_list_multipart_uploads_request_t *request,
			const char *delimiter);
	void (*set_key_marker)(oss_list_multipart_uploads_request_t *request,
			const char *key_marker);
	void (*set_max_uploads)(oss_list_multipart_uploads_request_t *request,
			unsigned int max_uploads);
	void (*set_prefix)(oss_list_multipart_uploads_request_t *request,
			const char *prefix);
	void (*set_upload_id_marker)(oss_list_multipart_uploads_request_t *request,
			const char *upload_id_marker);
};

/* *
 * 构造函数
 * */
extern oss_list_multipart_uploads_request_t *
list_multipart_uploads_request_initialize(const char *bucket_name);

/* *
 * 析构函数
 * */
extern void
list_multipart_uploads_request_finalize(
		oss_list_multipart_uploads_request_t *request);


#endif // OSS_LIST_MULTIPART_UPLOADS_REQUEST_H 


