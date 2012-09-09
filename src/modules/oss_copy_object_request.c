/*
 * =============================================================================
 *
 *       Filename:  oss_copy_object_request.c
 *
 *    Description:  oss copy object request.
 *
 *        Created:  09/07/2012 03:21:52 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_COPY_OBJECT_REQUEST_H
#include <modules/oss_copy_object_request.h>
#undef _OSS_COPY_OBJECT_REQUEST_H


/* *
 * Get 方法
 * */
static inline const char * 
_copy_object_request_get_destination_bucket_name(
		oss_copy_object_request_t *object)
{
	assert(object != NULL);
	return object->destination_bucket_name;
}

static inline const char *
_copy_object_request_get_destination_key(
		oss_copy_object_request_t *object)
{

	assert(object != NULL);
	return object->destination_key;
}

static inline const char **
_copy_object_request_get_matching_etag_constraints(
		oss_copy_object_request_t *object,
		unsigned int *counts)
{
	assert(object != NULL);
	*counts = object->_counts_matching_etag_constraints;
	return (const char **)(object->matching_etag_constraints);
}

static inline const char *
_copy_object_request_get_modified_since_constraints(
		oss_copy_object_request_t *object)
{
	assert(object != NULL);
	return object->modified_since_constraints;
}

static inline oss_object_metadata_t *
_copy_object_request_get_object_metadata(
		oss_copy_object_request_t *object)
{
	assert(object != NULL);
	return object->object_metadata;

}

static inline const char **
_copy_object_request_get_no_matching_etag_constraints(
		oss_copy_object_request_t *object,
		unsigned int *counts)
{
	assert(object != NULL);
	*counts = object->_counts_no_matching_etag_constraints;
	return (const char **)(object->no_matching_etag_constraints);
}

static inline const char *
_copy_object_request_get_source_bucket_name(
		oss_copy_object_request_t *object)
{
	assert(object != NULL);
	return object->source_bucket_name;
}

static inline const char *
_copy_object_request_get_source_key(
		oss_copy_object_request_t *object)
{
	assert(object != NULL);
	return object->source_key;
}

static inline const char *
_copy_object_request_get_unmodified_since_constraints(
		oss_copy_object_request_t *object)
{
	assert(object != NULL);
	return object->unmodified_since_constraints;
}

/* *
 * Set 方法
 * */

static inline void
__copy_object_request_set_destination_bucket_name(
		oss_copy_object_request_t *object,
		const char * destination_bucket_name,
		size_t destination_bucket_name_len)
{
	assert(object != NULL);
	assert(destination_bucket_name != NULL);
	if (object->destination_bucket_name!= NULL) {
		free(object->destination_bucket_name);
		object->destination_bucket_name = NULL;
	}

	object->destination_bucket_name = 
		(char *)malloc(sizeof(char) * destination_bucket_name_len + 1);
	memset(object->destination_bucket_name, 0, destination_bucket_name_len + 1);
	strncpy(object->destination_bucket_name,
			destination_bucket_name, destination_bucket_name_len);
}

static inline void
_copy_object_request_set_destination_bucket_name(
		oss_copy_object_request_t *object,
		const char * destination_bucket_name)
{
	assert(object != NULL);
	assert(destination_bucket_name != NULL);
	size_t destination_bucket_name_len = strlen(destination_bucket_name);

	__copy_object_request_set_destination_bucket_name(object,
		destination_bucket_name, destination_bucket_name_len);

}

static inline void
__copy_object_request_set_destination_key(
		oss_copy_object_request_t *object,
		const char *destination_key,
		size_t destination_key_len)
{
	assert(object != NULL);
	assert(destination_key != NULL);
	if (object->destination_key!= NULL) {
		free(object->destination_key);
		object->destination_key = NULL;
	}

	object->destination_key = 
		(char *)malloc(sizeof(char) * destination_key_len + 1);
	memset(object->destination_key, 0, destination_key_len + 1);
	strncpy(object->destination_key,
			destination_key, destination_key_len);
}

static inline void
_copy_object_request_set_destination_key(
		oss_copy_object_request_t *object,
		const char *destination_key)
{
	assert(object != NULL);
	assert(destination_key != NULL);
	size_t destination_key_len = strlen(destination_key);

	__copy_object_request_set_destination_key(object,
		destination_key, destination_key_len);
}

static inline void
_copy_object_request_set_matching_etag_constraints(
		oss_copy_object_request_t *object,
		const char **matching_etag_constraints, 
		unsigned int counts)
{
	assert(object != NULL);
	assert(matching_etag_constraints != NULL);

	/* *
	 * If object->no_matching_etag_constraints != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = object->_counts_matching_etag_constraints;
	if (object->matching_etag_constraints != NULL) {

		for (; j < total; j++) {
			if (*(object->matching_etag_constraints + j) != NULL) {
				free(*(object->matching_etag_constraints + j));
				*(object->matching_etag_constraints + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const char **pnmec = matching_etag_constraints;

	object->matching_etag_constraints = (char **)malloc(sizeof(char *) * counts);
	
	for (; i < counts; i++) {
		size_t len = strlen(*(pnmec + i));
		*(object->matching_etag_constraints + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(object->matching_etag_constraints), 0, len + 1);
		strncpy(*(object->matching_etag_constraints + i), *(pnmec + i), len);
	}
	object->_counts_matching_etag_constraints = counts;
}

static inline void
__copy_object_request_set_modified_since_constraints(
		oss_copy_object_request_t *object,
		const char *modified_since_constraints,
		size_t modified_since_constraints_len)
{
	assert(object != NULL);
	assert(modified_since_constraints != NULL);
	if (object->modified_since_constraints!= NULL) {
		free(object->modified_since_constraints);
		object->modified_since_constraints = NULL;
	}

	object->modified_since_constraints = 
		(char *)malloc(sizeof(char) * modified_since_constraints_len + 1);
	memset(object->modified_since_constraints, 0,
		modified_since_constraints_len + 1);
	strncpy(object->modified_since_constraints,
			modified_since_constraints, modified_since_constraints_len);
}

static inline void
_copy_object_request_set_modified_since_constraints(
		oss_copy_object_request_t *object,
		const char *modified_since_constraints)
{
	assert(object != NULL);
	assert(modified_since_constraints != NULL);
	size_t modified_since_constraints_len = strlen(modified_since_constraints);

	__copy_object_request_set_modified_since_constraints(object,
		modified_since_constraints, modified_since_constraints_len);
}

static inline void
_copy_object_request_set_new_object_metadata(
		oss_copy_object_request_t *object,
		oss_object_metadata_t *object_metadata)
{
	assert(object != NULL);
	assert(object_metadata != NULL);
	object->object_metadata = object_metadata;
}

static inline void
_copy_object_request_set_no_matching_etag_constraints(
		oss_copy_object_request_t *object,
		const char **no_matching_etag_constraints, 
		unsigned counts)
{
	assert(object != NULL);
	assert(no_matching_etag_constraints != NULL);

	/* *
	 * If object->no_matching_etag_constraints != NULL,
	 * free it one by one.
	 * */
	size_t j = 0;
	size_t total = object->_counts_no_matching_etag_constraints;
	if (object->no_matching_etag_constraints != NULL) {

		for (; j < total; j++) {
			if (*(object->no_matching_etag_constraints + j) != NULL) {
				free(*(object->no_matching_etag_constraints + j));
				*(object->no_matching_etag_constraints + j) = NULL;
			}
		}
	}

	size_t i = 0;
	const char **pnmec = no_matching_etag_constraints;

	object->no_matching_etag_constraints = (char **)malloc(sizeof(char *) * counts);
	
	for (; i < counts; i++) {
		size_t len = strlen(*(pnmec + i));
		*(object->no_matching_etag_constraints + i) = (char *)malloc(sizeof(char) * len + 1);
		memset(*(object->no_matching_etag_constraints), 0, len + 1);
		strncpy(*(object->no_matching_etag_constraints + i), *(pnmec + i), len);
	}
	object->_counts_no_matching_etag_constraints = counts;
}

static inline void
__copy_object_request_set_source_bucket_name(
		oss_copy_object_request_t *object,
		const char *source_bucket_name,
		size_t source_bucket_name_len)
{
	assert(object != NULL);
	assert(source_bucket_name != NULL);
	if (object->source_bucket_name!= NULL) {
		free(object->source_bucket_name);
		object->source_bucket_name = NULL;
	}

	object->source_bucket_name = 
		(char *)malloc(sizeof(char) * source_bucket_name_len + 1);
	memset(object->source_bucket_name, 0,
		source_bucket_name_len + 1);
	strncpy(object->source_bucket_name,
			source_bucket_name, source_bucket_name_len);
}

static inline void
_copy_object_request_set_source_bucket_name(
		oss_copy_object_request_t *object,
		const char *source_bucket_name)
{
	assert(object != NULL);
	assert(source_bucket_name != NULL);
	size_t source_bucket_name_len = strlen(source_bucket_name);

	__copy_object_request_set_source_bucket_name(object,
		source_bucket_name, source_bucket_name_len);
}

static inline void
__copy_object_request_set_source_key(
		oss_copy_object_request_t *object,
		const char *source_key,
		size_t source_key_len)
{
	assert(object != NULL);
	assert(source_key != NULL);
	if (object->source_key!= NULL) {
		free(object->source_key);
		object->source_key = NULL;
	}

	object->source_key = 
		(char *)malloc(sizeof(char) * source_key_len + 1);
	memset(object->source_key, 0,
		source_key_len + 1);
	strncpy(object->source_key,
			source_key, source_key_len);
}

static inline void
_copy_object_request_set_source_key(
		oss_copy_object_request_t *object,
		const char *source_key)
{
	assert(object != NULL);
	assert(source_key != NULL);
	size_t source_key_len = strlen(source_key);

	__copy_object_request_set_source_key(object,
		source_key, source_key_len);
}

static inline void
__copy_object_request_set_unmodified_since_constraints(
		oss_copy_object_request_t *object,
		const char *unmodified_since_constraints,
		size_t unmodified_since_constraints_len)
{
	assert(object != NULL);
	assert(unmodified_since_constraints != NULL);
	if (object->unmodified_since_constraints!= NULL) {
		free(object->unmodified_since_constraints);
		object->unmodified_since_constraints = NULL;
	}

	object->unmodified_since_constraints = 
		(char *)malloc(sizeof(char) * unmodified_since_constraints_len + 1);
	memset(object->unmodified_since_constraints, 0,
		unmodified_since_constraints_len + 1);
	strncpy(object->unmodified_since_constraints,
			unmodified_since_constraints, unmodified_since_constraints_len);
}

static inline void
_copy_object_request_set_unmodified_since_constraints(
		oss_copy_object_request_t *object,
		const char *unmodified_since_constraints)
{
	assert(object != NULL);
	assert(unmodified_since_constraints != NULL);
	size_t unmodified_since_constraints_len = strlen(unmodified_since_constraints);

	__copy_object_request_set_unmodified_since_constraints(object,
		unmodified_since_constraints, unmodified_since_constraints_len);
}

//TODO: implement here.
oss_copy_object_request_t *
_copy_object_request_initialize(
	const char *source_bucket_name, size_t source_bucket_name_len,
	const char *source_key, size_t source_key_len,
	const char *destination_bucket_name, size_t destination_bucket_name_len,
	const char *destination_key, size_t destination_key_len)
{
	size_t byte_of_char = sizeof(char);
	oss_copy_object_request_t *object = (
		oss_copy_object_request_t *)malloc(sizeof(oss_copy_object_request_t));

	object->source_bucket_name = (char *)malloc(byte_of_char * source_bucket_name_len + 1);
	object->source_key = (char *)malloc(byte_of_char * source_key_len + 1);
	object->destination_bucket_name = (char *)malloc(byte_of_char * destination_bucket_name_len + 1);
	object->destination_key = (char *)malloc(byte_of_char * destination_key_len + 1);
	object->matching_etag_constraints = NULL;
	object->modified_since_constraints = NULL;
	object->object_metadata = NULL;
	object->no_matching_etag_constraints = NULL;
	object->unmodified_since_constraints = NULL;

	strncpy(object->source_bucket_name, source_bucket_name, source_bucket_name_len);
	strncpy(object->source_key, source_key, source_key_len);
	strncpy(object->destination_bucket_name, destination_bucket_name, destination_bucket_name_len);
	strncpy(object->destination_key, destination_key, destination_key_len);

	(object->source_bucket_name)[source_bucket_name_len] = '\0';
	(object->source_key)[source_key_len] = '\0';
	(object->destination_bucket_name)[destination_bucket_name_len] = '\0';
	(object->destination_key)[destination_key_len] = '\0';

	object->get_destination_bucket_name = _copy_object_request_get_destination_bucket_name;
	object->get_destination_key = _copy_object_request_get_destination_key;
	object->get_matching_etag_constraints = _copy_object_request_get_matching_etag_constraints;
	object->get_modified_since_constraints = _copy_object_request_get_modified_since_constraints;
	object->get_object_metadata = _copy_object_request_get_object_metadata;
	object->get_no_matching_etag_constraints = _copy_object_request_get_no_matching_etag_constraints;
	object->get_source_bucket_name = _copy_object_request_get_source_bucket_name;
	object->get_source_key = _copy_object_request_get_source_key;
	object->get_unmodified_since_constraints = _copy_object_request_get_unmodified_since_constraints;
	object->set_destination_bucket_name = _copy_object_request_set_destination_bucket_name;
	object->set_destination_key = _copy_object_request_set_destination_key;
	object->set_matching_etag_constraints = _copy_object_request_set_matching_etag_constraints;
	object->set_modified_since_constraints = _copy_object_request_set_modified_since_constraints;
	object->set_new_object_metadata = _copy_object_request_set_new_object_metadata;
	object->set_no_matching_etag_constraints = _copy_object_request_set_no_matching_etag_constraints;
	object->set_source_bucket_name = _copy_object_request_set_source_bucket_name;
	object->set_source_key = _copy_object_request_set_source_key;
	object->set_unmodified_since_constraints = _copy_object_request_set_unmodified_since_constraints;

	return object;
}

oss_copy_object_request_t *
copy_object_request_initialize(
	const char *source_bucket_name, const char *source_key,
	const char *destination_bucket_name, const char *destination_key)
{
	assert(source_bucket_name != NULL);
	assert(source_key != NULL);
	assert(destination_bucket_name != NULL);
	assert(destination_key != NULL);

	size_t source_bucket_name_len = strlen(source_bucket_name);
	size_t source_key_len = strlen(source_key);
	size_t destination_bucket_name_len = strlen(destination_bucket_name);
	size_t destination_key_len = strlen(destination_key);

	return _copy_object_request_initialize(
			source_bucket_name, source_bucket_name_len,
			source_key, source_key_len,
			destination_bucket_name, destination_bucket_name_len,
			destination_key, destination_key_len);
}

void
copy_object_request_finalize(oss_copy_object_request_t *object)
{
	assert(object != NULL);
	if (object != NULL) {
		if (object->destination_bucket_name != NULL) {
			free(object->destination_bucket_name);
			object->destination_bucket_name = NULL;
		}

		if (object->destination_key != NULL) {
			free(object->destination_key);
			object->destination_key = NULL;
		}

		if (object->source_bucket_name!= NULL) {
			free(object->source_bucket_name);
			object->source_bucket_name = NULL;
		}

		if (object->source_key!= NULL) {
			free(object->source_key);
			object->source_key= NULL;
		}

		if (object->modified_since_constraints != NULL) {
			free(object->modified_since_constraints);
			object->modified_since_constraints= NULL;
		}

		if (object->matching_etag_constraints != NULL) {
			size_t j = 0;
			size_t total = object->_counts_matching_etag_constraints;
			if (object->matching_etag_constraints != NULL) {
				for (; j < total; j++) {
					if (*(object->matching_etag_constraints + j) != NULL) {
						free(*(object->matching_etag_constraints + j));
						*(object->matching_etag_constraints + j) = NULL;
					}
				}
			}
		}

		if (object->no_matching_etag_constraints != NULL) {
			size_t j = 0;
			size_t total = object->_counts_no_matching_etag_constraints;
			if (object->no_matching_etag_constraints != NULL) {
				for (; j < total; j++) {
					if (*(object->no_matching_etag_constraints + j) != NULL) {
						free(*(object->no_matching_etag_constraints + j));
						*(object->no_matching_etag_constraints + j) = NULL;
					}
				}
			}
		}

		if (object->matching_etag_constraints != NULL) {
			free(object->matching_etag_constraints);
			object->matching_etag_constraints = NULL;
		}


		/* *
		 * Here we intend to leave it unfreed,
		 * because it's the caller's responsibility to free
		 * the object->object_metadata, who uses it and he has to create it
		 * and free it;
		 * */
		object->object_metadata = NULL;
		free(object);
	}
}
