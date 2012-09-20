/*
 * =============================================================================
 *
 *       Filename:  generate-presigned-url-request-t.c
 *
 *    Description:  generate-presigned-url-request.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include "oss_generate_presigned_url_request.h"
#undef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H

#include <stdio.h>

int main()
{
	oss_generate_presigned_url_request_t *gpur;
	gpur = generate_presigned_url_request_initialize("bucket_name", "key");
	gpur->set_method(gpur, "method");
	oss_response_header_overrides_t *response_headers;
	response_headers = response_header_overrides_initialize();
	response_headers->set_cache_control(response_headers, "cache_control");
	response_headers->set_content_disposition(response_headers, "content_disposition");
	response_headers->set_content_encoding(response_headers, "content_encoding");
	response_headers->set_content_language(response_headers, "content_language");
	response_headers->set_content_type(response_headers, "content_type");
	response_headers->set_expires(response_headers, "expires");
	gpur->set_response_headers(gpur, response_headers);
	gpur->set_expiration(gpur, "expiration");

	printf("bucket_name = %s\nkey = %s\nmethod = %s\nexpiration = %s\ncache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gpur->get_bucket_name(gpur), gpur->get_key(gpur), gpur->get_method(gpur), gpur->get_expiration(gpur), gpur->response_headers->cache_control, gpur->response_headers->content_disposition, gpur->response_headers->content_encoding, gpur->response_headers->content_language, gpur->response_headers->content_type, gpur->response_headers->expires);

	response_headers = gpur->get_response_headers(gpur);
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			response_headers->get_cache_control(response_headers), response_headers->get_content_disposition(response_headers), response_headers->get_content_encoding(response_headers), response_headers->get_content_language(response_headers), response_headers->get_content_type(response_headers), response_headers->get_expires(response_headers));



	gpur->set_bucket_name(gpur, "bucket_name_hehe");
	gpur->set_key(gpur, "key_hehe");

	printf("bucket_name = %s\nkey = %s\nmethod = %s\nexpiration = %s\ncache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gpur->get_bucket_name(gpur), gpur->get_key(gpur), gpur->get_method(gpur), gpur->get_expiration(gpur), gpur->response_headers->cache_control, gpur->response_headers->content_disposition, gpur->response_headers->content_encoding, gpur->response_headers->content_language, gpur->response_headers->content_type, gpur->response_headers->expires);

	response_headers = gpur->get_response_headers(gpur);
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			response_headers->get_cache_control(response_headers), response_headers->get_content_disposition(response_headers), response_headers->get_content_encoding(response_headers), response_headers->get_content_language(response_headers), response_headers->get_content_type(response_headers), response_headers->get_expires(response_headers));



	gpur = generate_presigned_url_request_initialize_with_method("bucket_name", "key", "method");
	response_headers = response_header_overrides_initialize();
	response_headers->set_cache_control(response_headers, "cache_control");
	response_headers->set_content_disposition(response_headers, "content_disposition");
	response_headers->set_content_encoding(response_headers, "content_encoding");
	response_headers->set_content_language(response_headers, "content_language");
	response_headers->set_content_type(response_headers, "content_type");
	response_headers->set_expires(response_headers, "expires");
	gpur->set_response_headers(gpur, response_headers);
	gpur->set_expiration(gpur, "expiration");

	printf("bucket_name = %s\nkey = %s\nmethod = %s\nexpiration = %s\ncache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gpur->get_bucket_name(gpur), gpur->get_key(gpur), gpur->get_method(gpur), gpur->get_expiration(gpur), gpur->response_headers->cache_control, gpur->response_headers->content_disposition, gpur->response_headers->content_encoding, gpur->response_headers->content_language, gpur->response_headers->content_type, gpur->response_headers->expires);

	response_headers = gpur->get_response_headers(gpur);
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			response_headers->get_cache_control(response_headers), response_headers->get_content_disposition(response_headers), response_headers->get_content_encoding(response_headers), response_headers->get_content_language(response_headers), response_headers->get_content_type(response_headers), response_headers->get_expires(response_headers));



	response_header_overrides_finalize(response_headers);
	generate_presigned_url_request_finalize(gpur);
	return 0;
}
