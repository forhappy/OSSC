/*
 * =============================================================================
 *
 *       Filename:  oss_complete_multipart_upload_result.h
 *
 *    Description:  oss_complete_multipart_upload_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
# error Never include <ossc/oss_complete_multipart_upload_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#define OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

typedef struct oss_complete_multipart_upload_result_s oss_complete_multipart_upload_result_t;

struct oss_complete_multipart_upload_result_s {
	char *bucket_name;
	char *etag;
	char *key;
	char *location;

	const char * (*get_bucket_name)(oss_complete_multipart_upload_result_t *cmur);
	void (*set_bucket_name)(oss_complete_multipart_upload_result_t *cmur, const char *bucket_name);

	const char * (*get_etag)(oss_complete_multipart_upload_result_t *cmur);
	void (*set_etag)(oss_complete_multipart_upload_result_t *cmur, const char * etag);

	const char * (*get_key)(oss_complete_multipart_upload_result_t *cmur);
	void (*set_key)(oss_complete_multipart_upload_result_t *cmur, const char *key);

	const char * (*get_location)(oss_complete_multipart_upload_result_t *cmur);
	void (*set_location)(oss_complete_multipart_upload_result_t *cmur, const char *location);
};

extern oss_complete_multipart_upload_result_t *
complete_multipart_upload_result_initialize(void);

extern void 
complete_multipart_upload_result_finalize(oss_complete_multipart_upload_result_t *cmur);

#endif
