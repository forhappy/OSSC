/*
 * =============================================================================
 *
 *       Filename:  get-object-request-t.c
 *
 *    Description:  get-object-request.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_GET_OBJECT_REQUEST_H
#include "oss_get_object_request.h"
#undef _OSS_GET_OBJECT_REQUEST_H

#include <stdio.h>

int main()
{
	oss_get_object_request_t *gor;
	gor = get_object_request_initialize("bucket_name", "key");
	gor->set_range(gor, 1, 3);
	gor->set_modified_since_constraint(gor, "modified_since_constraint");
	oss_response_header_overrides_t *response_headers;
	response_headers = response_header_overrides_initialize();
	response_headers->set_cache_control(response_headers, "cache_control");
	response_headers->set_content_disposition(response_headers, "content_disposition");
	response_headers->set_content_encoding(response_headers, "content_encoding");
	response_headers->set_content_language(response_headers, "content_language");
	response_headers->set_content_type(response_headers, "content_type");
	response_headers->set_expires(response_headers, "expires");
	gor->set_response_headers(gor, response_headers);
	gor->set_unmodified_since_constraint(gor, "unmodified_since_constraint");

	long start, length;
	gor->get_range(gor, &start, &length);
	printf("bucket_name = %s\nkey = %s\nmodified_since_constraint = %s\nstart = %ld\nlength = %ld\nunmodified_since_constraint = %s\ncache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gor->get_bucket_name(gor), gor->get_key(gor), gor->get_modified_since_constraint(gor), start, length, gor->get_unmodified_since_constraint(gor), gor->response_headers->cache_control, gor->response_headers->content_disposition, gor->response_headers->content_encoding, gor->response_headers->content_language, gor->response_headers->content_type, gor->response_headers->expires);

	response_headers = gor->get_response_headers(gor);
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gor->response_headers->cache_control, gor->response_headers->content_disposition, gor->response_headers->content_encoding, gor->response_headers->content_language, gor->response_headers->content_type, gor->response_headers->expires);



	gor->set_bucket_name(gor, "bucket_name_hehe");
	gor->set_key(gor, "key_hehe");
	gor->set_range(gor, 5, 7);

	gor->get_range(gor, &start, &length);
	printf("bucket_name = %s\nkey = %s\nmodified_since_constraint = %s\nstart = %ld\nlength = %ld\nunmodified_since_constraint = %s\ncache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gor->get_bucket_name(gor), gor->get_key(gor), gor->get_modified_since_constraint(gor), start, length,  gor->get_unmodified_since_constraint(gor), gor->response_headers->cache_control, gor->response_headers->content_disposition, gor->response_headers->content_encoding, gor->response_headers->content_language, gor->response_headers->content_type, gor->response_headers->expires);

	response_headers = gor->get_response_headers(gor);
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\ncontent_type = %s\nexpires = %s\n", 
			gor->response_headers->cache_control, gor->response_headers->content_disposition, gor->response_headers->content_encoding, gor->response_headers->content_language, gor->response_headers->content_type, gor->response_headers->expires);


	response_header_overrides_finalize(response_headers);
	get_object_request_finalize(gor);
	return 0;
}
