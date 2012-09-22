/*
 * =============================================================================
 *
 *       Filename:  object-listing-t.c
 *
 *    Description:  object-listing.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_OBJECT_LISTING_H
#include "oss_object_listing.h"
#undef _OSS_OBJECT_LISTING_H


int main()
{
	oss_object_listing_t *ol;
	ol = object_listing_initialize();
	ol->set_bucket_name(ol, "bucket_name");
	ol->set_marker(ol, "marker");
	ol->set_next_marker(ol, "next_marker");
	ol->set_max_keys(ol, 2);
	ol->set_prefix(ol, "prefix");
	ol->set_delimiter(ol, "delimiter");
	ol->set_is_truncated(ol, 1);
	printf("bucket_name = %s\nmarker = %s\nnext_marker = %s\nmax_keys = %d\nis_truncated = %d\ndelimiter = %s\nprefix = %s\n", 
			ol->get_bucket_name(ol), ol->get_marker(ol), ol->get_next_marker(ol), ol->get_max_keys(ol), ol->get_is_truncated(ol), ol->get_delimiter(ol), ol->get_prefix(ol));


	object_listing_finalize(ol);
	return 0;
}
