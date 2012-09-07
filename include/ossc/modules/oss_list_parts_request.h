/*
 * =============================================================================
 *
 *       Filename:  oss_list_parts_request.h
 *
 *    Description:  list parts request implementation.
 *
 *        Created:  09/05/2012 10:41:00 AM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_LIST_PARTS_REQUEST_H
# error Never include <ossc/oss_list_parts_request_h_> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_LIST_PARTS_REQUEST_H
#define OSS_LIST_PARTS_REQUEST_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct oss_list_parts_request_s \
			oss_list_parts_request_t;

struct oss_list_parts_request_s {
	char *bucket_name;
	char *key;
	unsigned int max_parts;
	unsigned int part_number_marker;
	char *upload_id;

	const char * (*get_bucket_name)(oss_list_parts_request_t *request);
	const char * (*get_key)(oss_list_parts_request_t *request);
	unsigned int (*get_max_parts)(oss_list_parts_request_t *request);
	unsigned int (*get_part_number_marker)(oss_list_parts_request_t *request);
	const char * (*get_upload_id)(oss_list_parts_request_t *request);

	void (*set_bucket_name)(oss_list_parts_request_t *request,
			const char *bucket_name);
	void (*set_key)(oss_list_parts_request_t *request,
			const char *key);
	void (*set_max_parts)(oss_list_parts_request_t *request,
			unsigned int max_parts);
	void (*set_part_number_marker)(oss_list_parts_request_t *request,
			unsigned int part_number_marker);
	void (*set_upload_id)(oss_list_parts_request_t *request,
			const char *upload_id);
};

/* *
 * 构造函数
 * */
extern oss_list_parts_request_t *
list_parts_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id);
/* *
 * 析构函数
 * */
extern void
list_parts_request_finalize(
		oss_list_parts_request_t *request);


#endif // OSS_LIST_PARTS_REQUEST_H 
