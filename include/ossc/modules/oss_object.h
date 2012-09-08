/*
 * =============================================================================
 *
 *       Filename:  oss_object.h
 *
 *    Description:  oss object
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_OBJECT_H
#define OSS_OBJECT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_OBJECT_METADATA_H
#include <modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

typedef struct oss_object_s oss_object_t;

struct oss_object_s {
	char *bucket_name;
	char *key;
	char *object_content;
	oss_object_metadata_t *object_metadata;
	size_t object_content_len;

	const char * (*get_bucket_name)(oss_object_t *object);
	const char * (*get_key)(oss_object_t *object);
	const char * (*get_object_content)(oss_object_t *object, size_t *object_content_len);
	oss_object_metadata_t * (*get_object_metadata)(oss_object_t *object);

	void (*set_bucket_name)(oss_object_t *object, const char *bucket_name);
	void (*set_key)(oss_object_t *object, const char *key);
	void (*set_object_content)(oss_object_t *object,
			const char *object_content,
			size_t object_content_len);
	void (*set_object_metadata)(oss_object_t *object,
			oss_object_metadata_t *object_metadata);
};
extern oss_object_t * object_initialize();

extern void object_finalize(oss_object_t *object);

#endif // OSS_OBJECT_H

