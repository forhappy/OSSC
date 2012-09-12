/*
 * =============================================================================
 *
 *       Filename:  oss_complete_multipart_upload_request.h
 *
 *    Description:  oss_complete_multipart_upload_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
# error Never include <ossc/modules/oss_complete_multipart_upload_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#define OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_PART_ETAG_H
#include <ossc/modules/oss_part_etag.h>
#undef _OSS_PART_ETAG_H

typedef struct oss_complete_multipart_upload_request_s oss_complete_multipart_upload_request_t;

struct oss_complete_multipart_upload_request_s {
	char *bucket_name;
	char *key;
	oss_part_etag_t **part_etags;
	char *upload_id;
	int part_etags_number;

	const char * (*get_bucket_name)(oss_complete_multipart_upload_request_t *request);
	void (*set_bucket_name)(oss_complete_multipart_upload_request_t *request, const char *bucket_name);

	const char * (*get_key)(oss_complete_multipart_upload_request_t *request);
	void (*set_key)(oss_complete_multipart_upload_request_t *request, const char *key);


	oss_part_etag_t ** (*get_part_etags)(oss_complete_multipart_upload_request_t *request, int *part_etags_number);
	void (*set_part_etags)(oss_complete_multipart_upload_request_t *request, oss_part_etag_t **part_etags, int part_etags_number);

	const char * (*get_upload_id)(oss_complete_multipart_upload_request_t *request);
	void (*set_upload_id)(oss_complete_multipart_upload_request_t *request, const char *upload_id);
};

extern oss_complete_multipart_upload_request_t *
complete_multipart_upload_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id,
		oss_part_etag_t **part_etagss,
		int part_etags_number);

extern void 
complete_multipart_upload_request_finalize(oss_complete_multipart_upload_request_t *request);

#endif
