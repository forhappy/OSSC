/*
 * =============================================================================
 *
 *       Filename:  oss_upload_part_request.h
 *
 *    Description:  oss_upload_part_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_UPLOAD_PART_REQUEST_H
# error Never include <ossc/oss_upload_part_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_UPLOAD_PART_REQUEST_H
#define OSS_UPLOAD_PART_REQUEST_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

typedef struct oss_upload_part_request_s oss_upload_part_request_t;

struct oss_upload_part_request_s {
	char *bucket_name;
	char *input_stream;
	int input_stream_len;
	char *key;
	char *md5_digest;
	int part_number;
	long part_size;
	char *upload_id;

	const char * (*get_bucket_name)(oss_upload_part_request_t *upr);
	void (*set_bucket_name)(oss_upload_part_request_t *upr, const char *bucket_name);

	const char * (*get_key)(oss_upload_part_request_t *upr);
	void (*set_key)(oss_upload_part_request_t *upr, const char *key);

	const char * (*get_md5_digest)(oss_upload_part_request_t *upr);
	void (*set_md5_digest)(oss_upload_part_request_t *upr, const char *md5_digest);

	const char * (*get_input_stream)(oss_upload_part_request_t *upr, int *input_stream_len);
	void (*set_input_stream)(oss_upload_part_request_t *upr, const char *input_stream, size_t input_stream_len);

	const char * (*get_upload_id)(oss_upload_part_request_t *upr);
	void (*set_upload_id)(oss_upload_part_request_t *upr, const char *upload_id);

	int (*get_part_number)(oss_upload_part_request_t *upr);
	void (*set_part_number)(oss_upload_part_request_t *upr, int part_number);

	long (*get_part_size)(oss_upload_part_request_t *upr);
	void (*set_part_size)(oss_upload_part_request_t *upr, long part_size);
};

extern oss_upload_part_request_t *
upload_part_request_initialize(void);

extern void 
upload_part_request_finalize(oss_upload_part_request_t *upr);

#endif
