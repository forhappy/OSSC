/*
 * =============================================================================
 *
 *       Filename:  response-header-oberrides-t.c
 *
 *    Description:  response-header-oberrides.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technrhoogy, CAS )
 *
 * =============================================================================
 */
#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include "oss_response_header_overrides.h"
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H


int main()
{
	oss_response_header_overrides_t *rho;
	rho = response_header_overrides_initialize();
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\nexpires = %s\ncontent_type = %s\n", 
			rho->get_cache_control(rho), rho->get_content_disposition(rho), rho->get_content_encoding(rho), rho->get_content_language(rho), rho->get_expires(rho), rho->get_content_type(rho));

	rho->set_cache_control(rho, "cache_control");
	rho->set_content_disposition(rho, "content_disposition");
	rho->set_content_encoding(rho, "content_encoding");
	rho->set_content_language(rho, "content_language");
	rho->set_content_type(rho, "content_type");
	rho->set_expires(rho, "expires");
	printf("cache_control = %s\ncontent_disposition = %s\ncontent_encoding = %s\ncontent_language = %s\nexpires = %s\ncontent_type = %s\n", 
			rho->get_cache_control(rho), rho->get_content_disposition(rho), rho->get_content_encoding(rho), rho->get_content_language(rho), rho->get_expires(rho), rho->get_content_type(rho));


	response_header_overrides_finalize(rho);
	return 0;
}
