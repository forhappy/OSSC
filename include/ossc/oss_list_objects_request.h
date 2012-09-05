/*
 * =============================================================================
 *
 *       Filename:  oss_list_objects_request.h
 *
 *    Description:  list object request implementation. 
 *
 *        Created:  09/04/2012 10:54:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_LIST_OBJECTS_REQUEST_H
# error Never include <ossc/oss_list_objects_request_h_> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_LIST_OBJECTS_REQUEST_H
#define OSS_LIST_OBJECTS_REQUEST_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct oss_list_objects_request_s \
			oss_list_objects_request_t;

struct oss_list_objects_request_s {
	char *bucket_name;
	char *delimiter;
	char *maker;
	unsigned int max_keys;
	char *prefix;

	const char * (*get_bucket_name)(oss_list_objects_request_t *request);
	const char * (*get_delimiter)(oss_list_objects_request_t *request);
	const char * (*get_maker)(oss_list_objects_request_t *request);
	unsigned int (*get_max_keys)(oss_list_objects_request_t *request);
	const char * (*get_prefix)(oss_list_objects_request_t *request);

	void (*set_bucket_name)(oss_list_objects_request_t *request,
			const char *bucket_name);
	void (*set_delimiter)(oss_list_objects_request_t *request,
			const char *delimiter);
	void (*set_maker)(oss_list_objects_request_t *request,
			const char *maker);
	void (*set_max_keys)(oss_list_objects_request_t *request,
			unsigned int max_keys);
	void (*set_prefix)(oss_list_objects_request_t *request,
			const char *prefix);
};

/* *
 * 构造函数
 * */
extern oss_list_objects_request_t *
list_objects_request_initialize(void);

/* *
 * 构造函数
 * */
extern oss_list_objects_request_t *
list_objects_request_initialize_with_bucket_name(const char *bucket_name);

/* *
 * 构造函数
 * */
extern oss_list_objects_request_t *
list_objects_request_initialize_with_args(const char *bucket_name,
		const char *prefix,
		const char *maker,
		const char *delimiter,
		unsigned int max_keys);
/* *
 * 析构函数
 * */
extern void
list_objects_request_finalize(
		oss_list_objects_request_t *request);


#endif // OSS_LIST_OBJECTS_REQUEST_H 
