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
#include <modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H



void 
response_header_overrides_finalize(oss_response_header_overrides_t *rho)
{

	if (rho) {
		if (rho->cache_control) {
			free(rho->cache_control);
			rho->cache_control = NULL;
		}
		if (rho->content_disposition) {
			free(rho->content_disposition);
			rho->content_disposition = NULL;
		}
		if (rho->content_encoding) {
			free(rho->content_encoding);
			rho->content_encoding = NULL;
		}
		if (rho->content_language) {
			free(rho->content_language);
			rho->content_language = NULL;
		}
		if (rho->content_type) {
			free(rho->content_type);
			rho->content_type = NULL;
		}
		if (rho->expires) {
			free(rho->expires);
			rho->expires = NULL;
		}
		free(rho);
		rho = NULL;
	}
}

static const char * 
_response_header_overrides_get_cache_control(oss_response_header_overrides_t *rho)
{
	return rho->cache_control;
}

static inline void
__response_header_overrides_set_cache_control(
		oss_response_header_overrides_t *rho, 
		const char *cache_control,
		size_t cache_control_len)
{
	if (rho->cache_control) {
		free(rho->cache_control);
		rho->cache_control = NULL;
	}
	rho->cache_control = (char *)malloc(sizeof(char) * cache_control_len + 1);
	strncpy(rho->cache_control, cache_control, cache_control_len);
	(rho->cache_control)[cache_control_len] = '\0';
}

static void
_response_header_overrides_set_cache_control(
		oss_response_header_overrides_t *rho, 
		const char *cache_control)
{
	assert(cache_control != NULL);
	size_t cache_control_len = strlen(cache_control);
	__response_header_overrides_set_cache_control(rho, cache_control, cache_control_len);
}

static const char * 
_response_header_overrides_get_content_disposition(oss_response_header_overrides_t *rho)
{
	return rho->content_disposition;
}

static inline void
__response_header_overrides_set_content_disposition(
		oss_response_header_overrides_t *rho, 
		const char *content_disposition,
		size_t content_disposition_len)
{
	if (rho->content_disposition) {
		free(rho->content_disposition);
		rho->content_disposition = NULL;
	}
	rho->content_disposition = (char *)malloc(sizeof(char) * content_disposition_len + 1);
	strncpy(rho->content_disposition, content_disposition, content_disposition_len);
	(rho->content_disposition)[content_disposition_len] = '\0';
}

static void
_response_header_overrides_set_content_disposition(
		oss_response_header_overrides_t *rho, 
		const char *content_disposition)
{
	assert(content_disposition != NULL);
	size_t content_disposition_len = strlen(content_disposition);
	__response_header_overrides_set_content_disposition(rho, content_disposition, content_disposition_len);
}

static const char * 
_response_header_overrides_get_content_encoding(oss_response_header_overrides_t *rho)
{
	return rho->content_encoding;
}

static inline void
__response_header_overrides_set_content_encoding(
		oss_response_header_overrides_t *rho, 
		const char *content_encoding,
		size_t content_encoding_len)
{
	if (rho->content_encoding) {
		free(rho->content_encoding);
		rho->content_encoding = NULL;
	}
	rho->content_encoding = (char *)malloc(sizeof(char) * content_encoding_len + 1);
	strncpy(rho->content_encoding, content_encoding, content_encoding_len);
	(rho->content_encoding)[content_encoding_len] = '\0';
}

static void
_response_header_overrides_set_content_encoding(
		oss_response_header_overrides_t *rho, 
		const char *content_encoding)
{
	assert(content_encoding != NULL);
	size_t content_encoding_len = strlen(content_encoding);
	__response_header_overrides_set_content_encoding(rho, content_encoding, content_encoding_len);
}

static const char * 
_response_header_overrides_get_content_language(oss_response_header_overrides_t *rho)
{
	return rho->content_language;
}

static inline void
__response_header_overrides_set_content_language(
		oss_response_header_overrides_t *rho, 
		const char *content_language,
		size_t content_language_len)
{
	if (rho->content_language) {
		free(rho->content_language);
		rho->content_language = NULL;
	}
	rho->content_language = (char *)malloc(sizeof(char) * content_language_len + 1);
	strncpy(rho->content_language, content_language, content_language_len);
	(rho->content_language)[content_language_len] = '\0';
}

static void
_response_header_overrides_set_content_language(
		oss_response_header_overrides_t *rho, 
		const char *content_language)
{
	assert(content_language != NULL);
	size_t content_language_len = strlen(content_language);
	__response_header_overrides_set_content_language(rho, content_language, content_language_len);
}


static const char * 
_response_header_overrides_get_content_type(oss_response_header_overrides_t *rho)
{
	return rho->content_type;
}

static inline void
__response_header_overrides_set_content_type(
		oss_response_header_overrides_t *rho, 
		const char *content_type,
		size_t content_type_len)
{
	if (rho->content_type) {
		free(rho->content_type);
		rho->content_type = NULL;
	}
	rho->content_type = (char *)malloc(sizeof(char) * content_type_len + 1);
	strncpy(rho->content_type, content_type, content_type_len);
	(rho->content_type)[content_type_len] = '\0';
}

static void
_response_header_overrides_set_content_type(
		oss_response_header_overrides_t *rho, 
		const char *content_type)
{
	assert(content_type != NULL);
	size_t content_type_len = strlen(content_type);
	__response_header_overrides_set_content_type(rho, content_type, content_type_len);
}

static const char * 
_response_header_overrides_get_expires(oss_response_header_overrides_t *rho)
{
	return rho->expires;
}

static inline void
__response_header_overrides_set_expires(
		oss_response_header_overrides_t *rho, 
		const char *expires,
		size_t expires_len)
{
	if (rho->expires) {
		free(rho->expires);
		rho->expires = NULL;
	}
	rho->expires = (char *)malloc(sizeof(char) * expires_len + 1);
	strncpy(rho->expires, expires, expires_len);
	(rho->expires)[expires_len] = '\0';
}

static void
_response_header_overrides_set_expires(
		oss_response_header_overrides_t *rho, 
		const char *expires)
{
	assert(expires != NULL);
	size_t expires_len = strlen(expires);
	__response_header_overrides_set_expires(rho, expires, expires_len);
}

oss_response_header_overrides_t *
response_header_overrides_initialize(void)
{
	oss_response_header_overrides_t *rho;

	rho                      = (oss_response_header_overrides_t *)malloc(sizeof(oss_response_header_overrides_t));

	size_t cache_control_len = strlen(RESPONSE_HEADER_CACHE_CONTROL);
	rho->cache_control       = (char *)malloc(sizeof(char) * cache_control_len + 1);
	strncpy(rho->cache_control, RESPONSE_HEADER_CACHE_CONTROL, cache_control_len);
	(rho->cache_control)[cache_control_len] = '\0';

	size_t content_disposition_len = strlen(RESPONSE_HEADER_CONTENT_DISPOSITION);
	rho->content_disposition = (char *)malloc(sizeof(char) * content_disposition_len + 1);
	strncpy(rho->content_disposition, RESPONSE_HEADER_CONTENT_DISPOSITION, content_disposition_len);
	(rho->content_disposition)[content_disposition_len] = '\0';

	size_t content_encoding_len = strlen(RESPONSE_HEADER_CONTENT_ENCODING);
	rho->content_encoding = (char *)malloc(sizeof(char) * content_encoding_len + 1);
	strncpy(rho->content_encoding, RESPONSE_HEADER_CONTENT_ENCODING, content_encoding_len);
	(rho->content_encoding)[content_encoding_len] = '\0';

	size_t content_language_len = strlen(RESPONSE_HEADER_CONTENT_LANGUAGE);
	rho->content_language = (char *)malloc(sizeof(char) * content_language_len + 1);
	strncpy(rho->content_language, RESPONSE_HEADER_CONTENT_LANGUAGE, content_language_len);
	(rho->content_language)[content_language_len] = '\0';

	size_t content_type_len = strlen(RESPONSE_HEADER_CONTENT_TYPE);
	rho->content_type = (char *)malloc(sizeof(char) * content_type_len + 1);
	strncpy(rho->content_type, RESPONSE_HEADER_CONTENT_TYPE, content_type_len);
	(rho->content_type)[content_type_len] = '\0';

	size_t expires_len = strlen(RESPONSE_HEADER_EXPIRES);
	rho->expires = (char *)malloc(sizeof(char) * expires_len + 1);
	strncpy(rho->expires, RESPONSE_HEADER_EXPIRES, expires_len);
	(rho->expires)[expires_len] = '\0';


	rho->get_cache_control       = _response_header_overrides_get_cache_control;
	rho->set_cache_control       = _response_header_overrides_set_cache_control;
	rho->get_content_disposition = _response_header_overrides_get_content_disposition;
	rho->set_content_disposition = _response_header_overrides_set_content_disposition;
	rho->get_content_encoding    = _response_header_overrides_get_content_encoding;
	rho->set_content_encoding    = _response_header_overrides_set_content_encoding;
	rho->get_content_language    = _response_header_overrides_get_content_language;
	rho->set_content_language    = _response_header_overrides_set_content_language;
	rho->get_content_type        = _response_header_overrides_get_content_type;
	rho->set_content_type        = _response_header_overrides_set_content_type;
	rho->get_expires             = _response_header_overrides_get_expires;
	rho->set_expires             = _response_header_overrides_set_expires;

	return rho;
}
