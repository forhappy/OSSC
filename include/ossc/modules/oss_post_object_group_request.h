/*
 * =============================================================================
 *
 *       Filename:  oss_post_object_group_request.h
 *
 *    Description:  oss post object group.
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_POST_OBJECT_GROUP_REQUEST_H
# error Never include <ossc/modules/oss_post_object_group_request.h> directly,
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_POST_OBJECT_GROUP_REQUEST_H 
#define OSS_POST_OBJECT_GROUP_REQUEST_H 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_OBJECT_GROUP_ITEM_H
#include "oss_object_group_item.h"
#undef _OSS_OBJECT_GROUP_ITEM_H 

typedef struct oss_post_object_group_request_s oss_post_object_group_request_t;

struct oss_post_object_group_request_s {
	char *bucket_name;
	char *key;
	oss_object_group_item_t **items;
	size_t itemnums;

	const char * (*get_bucket_name)(oss_post_object_group_request_t *request);
	void (*set_bucket_name)(oss_post_object_group_request_t *request, const char *bucket_name);

	const char * (*get_key)(oss_post_object_group_request_t *request);
	void (*set_key)(oss_post_object_group_request_t *request, const char *key);

	oss_object_group_item_t ** (*get_items)(oss_post_object_group_request_t *request, int *itemnums);
	void (*set_items)(oss_post_object_group_request_t *request,
			oss_object_group_item_t **item, int itemnums);
	
};


extern oss_post_object_group_request_t *
post_object_group_request_initialize(const char *bucket_name, const char *key,
		oss_object_group_item_t **items, int itemnums);

extern void post_object_group_request_finalize(oss_post_object_group_request_t *request);

#endif // OSS_POST_OBJECT_GROUP_REQUEST_H

