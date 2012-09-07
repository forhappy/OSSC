/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_upload.h
 *
 *    Description:  oss_multipart_upload.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_MULTIPART_UPLOAD_H
# error Never include <ossc/oss_multipart_upload.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_MULTIPART_UPLOAD_H
#define OSS_MULTIPART_UPLOAD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct oss_multipart_upload_s oss_multipart_upload_t;

struct oss_multipart_upload_s {
	char *initiated;
	char *storage_class;
	char *upload_id;
	char *key;
	const char * (*get_initiated)(oss_multipart_upload_t *mu);
	void (*set_initiated)(oss_multipart_upload_t *mu, const char *initiated);
	const char * (*get_storage_class)(oss_multipart_upload_t *mu);
	void (*set_storage_class)(oss_multipart_upload_t *mu, const char *storage_class);
	const char * (*get_upload_id)(oss_multipart_upload_t *mu);
	void (*set_upload_id)(oss_multipart_upload_t *mu, const char *upload_id);
	const char * (*get_key)(oss_multipart_upload_t *mu);
	void (*set_key)(oss_multipart_upload_t *mu, const char *key);
};

extern oss_multipart_upload_t *
multipart_upload_initialize(void);

extern void 
multipart_upload_finalize(oss_multipart_upload_t *mu);

#endif
