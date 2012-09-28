/*
 * =============================================================================
 *
 *       Filename:  oss_response_header_overrides.c
 *
 *    Description:  oss_response_header_overrides.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technrhoogy, CAS )
 *
 *
 * =============================================================================
 */
#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include <ossc/modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

static const char * 
_response_header_overrides_get_cache_control(oss_response_header_overrides_t *overrides)
{
	return overrides->cache_control;
}

static inline void
__response_header_overrides_set_cache_control(
		oss_response_header_overrides_t *overrides,
		const char *cache_control,
		unsigned int cache_control_len)
{
	if (overrides->cache_control != NULL) {
		free(overrides->cache_control);
		overrides->cache_control = NULL;
	}
	overrides->cache_control = (char *)malloc(sizeof(char) * cache_control_len + 1);
	strncpy(overrides->cache_control, cache_control, cache_control_len);
	(overrides->cache_control)[cache_control_len] = '\0';
}

static void
_response_header_overrides_set_cache_control(
		oss_response_header_overrides_t *overrides,
		const char *cache_control)
{
	assert(cache_control != NULL);

	unsigned int cache_control_len = strlen(cache_control);
	__response_header_overrides_set_cache_control(overrides, cache_control, cache_control_len);
}

static const char * 
_response_header_overrides_get_content_disposition(oss_response_header_overrides_t *overrides)
{
	return overrides->content_disposition;
}

static inline void
__response_header_overrides_set_content_disposition(
		oss_response_header_overrides_t *overrides,
		const char *content_disposition,
		unsigned int content_disposition_len)
{
	if (overrides->content_disposition != NULL) {
		free(overrides->content_disposition);
		overrides->content_disposition = NULL;
	}

	overrides->content_disposition = (char *)malloc(sizeof(char) * content_disposition_len + 1);
	strncpy(overrides->content_disposition, content_disposition, content_disposition_len);
	(overrides->content_disposition)[content_disposition_len] = '\0';
}

static void
_response_header_overrides_set_content_disposition(
		oss_response_header_overrides_t *overrides,
		const char *content_disposition)
{
	assert(content_disposition != NULL);

	unsigned int content_disposition_len = strlen(content_disposition);
	__response_header_overrides_set_content_disposition(overrides, content_disposition, content_disposition_len);
}

static const char * 
_response_header_overrides_get_content_encoding(oss_response_header_overrides_t *overrides)
{
	return overrides->content_encoding;
}

static inline void
__response_header_overrides_set_content_encoding(
		oss_response_header_overrides_t *overrides,
		const char *content_encoding,
		unsigned int content_encoding_len)
{
	if (overrides->content_encoding != NULL) {
		free(overrides->content_encoding);
		overrides->content_encoding = NULL;
	}

	overrides->content_encoding = (char *)malloc(sizeof(char) * content_encoding_len + 1);
	strncpy(overrides->content_encoding, content_encoding, content_encoding_len);
	(overrides->content_encoding)[content_encoding_len] = '\0';
}

static void
_response_header_overrides_set_content_encoding(
		oss_response_header_overrides_t *overrides,
		const char *content_encoding)
{
	assert(content_encoding != NULL);

	unsigned int content_encoding_len = strlen(content_encoding);
	__response_header_overrides_set_content_encoding(overrides, content_encoding, content_encoding_len);
}

static const char * 
_response_header_overrides_get_content_language(oss_response_header_overrides_t *overrides)
{
	return overrides->content_language;
}

static inline void
__response_header_overrides_set_content_language(
		oss_response_header_overrides_t *overrides,
		const char *content_language,
		unsigned int content_language_len)
{
	if (overrides->content_language != NULL) {
		free(overrides->content_language);
		overrides->content_language = NULL;
	}
	overrides->content_language = (char *)malloc(sizeof(char) * content_language_len + 1);
	strncpy(overrides->content_language, content_language, content_language_len);
	(overrides->content_language)[content_language_len] = '\0';
}

static void
_response_header_overrides_set_content_language(
		oss_response_header_overrides_t *overrides,
		const char *content_language)
{
	assert(content_language != NULL);

	unsigned int content_language_len = strlen(content_language);
	__response_header_overrides_set_content_language(overrides, content_language, content_language_len);
}


static const char * 
_response_header_overrides_get_content_type(oss_response_header_overrides_t *overrides)
{
	return overrides->content_type;
}

static inline void
__response_header_overrides_set_content_type(
		oss_response_header_overrides_t *overrides,
		const char *content_type,
		unsigned int content_type_len)
{
	if (overrides->content_type != NULL) {
		free(overrides->content_type);
		overrides->content_type = NULL;
	}

	overrides->content_type = (char *)malloc(sizeof(char) * content_type_len + 1);
	strncpy(overrides->content_type, content_type, content_type_len);
	(overrides->content_type)[content_type_len] = '\0';
}

static void
_response_header_overrides_set_content_type(
		oss_response_header_overrides_t *overrides,
		const char *content_type)
{
	assert(content_type != NULL);

	unsigned int content_type_len = strlen(content_type);
	__response_header_overrides_set_content_type(overrides, content_type, content_type_len);
}

static const char * 
_response_header_overrides_get_expires(oss_response_header_overrides_t *overrides)
{
	return overrides->expires;
}

static inline void
__response_header_overrides_set_expires(
		oss_response_header_overrides_t *overrides,
		const char *expires,
		unsigned int expires_len)
{
	if (overrides->expires != NULL) {
		free(overrides->expires);
		overrides->expires = NULL;
	}

	overrides->expires = (char *)malloc(sizeof(char) * expires_len + 1);
	strncpy(overrides->expires, expires, expires_len);
	(overrides->expires)[expires_len] = '\0';
}

static void
_response_header_overrides_set_expires(
		oss_response_header_overrides_t *overrides,
		const char *expires)
{
	assert(expires != NULL);

	unsigned int expires_len = strlen(expires);
	__response_header_overrides_set_expires(overrides, expires, expires_len);
}

oss_response_header_overrides_t *
response_header_overrides_initialize(void)
{
	oss_response_header_overrides_t *overrides;

	overrides = (oss_response_header_overrides_t *)
			malloc(sizeof(oss_response_header_overrides_t));

	unsigned int cache_control_len = strlen(RESPONSE_HEADER_CACHE_CONTROL);
	overrides->cache_control       = (char *)malloc(sizeof(char) * cache_control_len + 1);
	strncpy(overrides->cache_control, RESPONSE_HEADER_CACHE_CONTROL, cache_control_len);
	(overrides->cache_control)[cache_control_len] = '\0';

	unsigned int content_disposition_len = strlen(RESPONSE_HEADER_CONTENT_DISPOSITION);
	overrides->content_disposition = (char *)malloc(sizeof(char) * content_disposition_len + 1);
	strncpy(overrides->content_disposition, RESPONSE_HEADER_CONTENT_DISPOSITION, content_disposition_len);
	(overrides->content_disposition)[content_disposition_len] = '\0';

	unsigned int content_encoding_len = strlen(RESPONSE_HEADER_CONTENT_ENCODING);
	overrides->content_encoding = (char *)malloc(sizeof(char) * content_encoding_len + 1);
	strncpy(overrides->content_encoding, RESPONSE_HEADER_CONTENT_ENCODING, content_encoding_len);
	(overrides->content_encoding)[content_encoding_len] = '\0';

	unsigned int content_language_len = strlen(RESPONSE_HEADER_CONTENT_LANGUAGE);
	overrides->content_language = (char *)malloc(sizeof(char) * content_language_len + 1);
	strncpy(overrides->content_language, RESPONSE_HEADER_CONTENT_LANGUAGE, content_language_len);
	(overrides->content_language)[content_language_len] = '\0';

	unsigned int content_type_len = strlen(RESPONSE_HEADER_CONTENT_TYPE);
	overrides->content_type = (char *)malloc(sizeof(char) * content_type_len + 1);
	strncpy(overrides->content_type, RESPONSE_HEADER_CONTENT_TYPE, content_type_len);
	(overrides->content_type)[content_type_len] = '\0';

	unsigned int expires_len = strlen(RESPONSE_HEADER_EXPIRES);
	overrides->expires = (char *)malloc(sizeof(char) * expires_len + 1);
	strncpy(overrides->expires, RESPONSE_HEADER_EXPIRES, expires_len);
	(overrides->expires)[expires_len] = '\0';


	overrides->get_cache_control       = _response_header_overrides_get_cache_control;
	overrides->set_cache_control       = _response_header_overrides_set_cache_control;
	overrides->get_content_disposition = _response_header_overrides_get_content_disposition;
	overrides->set_content_disposition = _response_header_overrides_set_content_disposition;
	overrides->get_content_encoding    = _response_header_overrides_get_content_encoding;
	overrides->set_content_encoding    = _response_header_overrides_set_content_encoding;
	overrides->get_content_language    = _response_header_overrides_get_content_language;
	overrides->set_content_language    = _response_header_overrides_set_content_language;
	overrides->get_content_type        = _response_header_overrides_get_content_type;
	overrides->set_content_type        = _response_header_overrides_set_content_type;
	overrides->get_expires             = _response_header_overrides_get_expires;
	overrides->set_expires             = _response_header_overrides_set_expires;

	return overrides;
}

void
response_header_overrides_finalize(oss_response_header_overrides_t *overrides)
{

	if (overrides != NULL) {
		if (overrides->cache_control != NULL) {
			free(overrides->cache_control);
			overrides->cache_control = NULL;
		}

		if (overrides->content_disposition != NULL) {
			free(overrides->content_disposition);
			overrides->content_disposition = NULL;
		}

		if (overrides->content_encoding != NULL) {
			free(overrides->content_encoding);
			overrides->content_encoding = NULL;
		}

		if (overrides->content_language != NULL) {
			free(overrides->content_language);
			overrides->content_language = NULL;
		}

		if (overrides->content_type != NULL) {
			free(overrides->content_type);
			overrides->content_type = NULL;
		}

		if (overrides->expires != NULL) {
			free(overrides->expires);
			overrides->expires = NULL;
		}

		free(overrides);
	}
}
