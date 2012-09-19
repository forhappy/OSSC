/*
 * =============================================================================
 *
 *       Filename:  client_put_bucket_acl-t.c
 *
 *    Description:  client_put_bucket_acl testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include <ossc/client.h>

//#define _OSS_CLIENT_H
//#include <modules/oss_client.h>
//#undef _OSS_CLIENT_H

#include <curl/curl.h>

int main()
{
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	oss_client_t *client = client_initialize(access_id, access_key);
	unsigned short retcode;
	int i;
	/* *
	 * test get_bucket_with_prefix(list_objects)
	 */
#if 1 
	oss_object_listing_t *object_listing = client_list_objects_with_prefix(client, "bucketname2", "a", &retcode);
	
	if(retcode == 0) {
		printf("bucket_name = %s\nnext_marker = %s\nprefix = %s\nmarker = %s\nmax_keys = %d\ndelimiter = %s\nis_truncated = %d\n", object_listing->bucket_name, object_listing->next_marker, object_listing->prefix, object_listing->marker, object_listing->max_keys, object_listing->delimiter, object_listing->is_truncated);
		printf("===========================================\n");
		for(i = 0; i < object_listing->_counts_summaries; i++) {
			printf("type = %s\netag = %s\nkey = %s\nlast_modified = %s\nsize = %ld\nstorage_class = %s\nid = %s\ndisplay_name = %s\n", (object_listing->summaries)[i]->type, (object_listing->summaries)[i]->etag, (object_listing->summaries)[i]->key, (object_listing->summaries)[i]->last_modified, (object_listing->summaries)[i]->size, (object_listing->summaries)[i]->storage_class, (object_listing->summaries)[i]->owner->id, (object_listing->summaries)[i]->owner->display_name);
		}
		printf("===========================================\n");
		for(i = 0; i < object_listing->_counts_common_prefixes; i++) {
			printf("common_prefix = %s\n", (object_listing->common_prefixes)[i]);
		}
	} else {
		printf("retcode = %d\n", retcode);
		printf("error = %s\n", oss_get_error_message_from_retcode(retcode));
	}
#endif

	client_finalize(client);
	return 0;
}
