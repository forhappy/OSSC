/*
 * =============================================================================
 *
 *       Filename:  list_objects_with_prefix.c
 *
 *    Description:  list_objects_with_prefix example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 返回请求者指定Bucket下的所有Object的列表
*/

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "storage.aliyun.com";    //设置hostname

int main()
{
	unsigned short retcode;			//保存服务器http返回码的解析结果
	const char *retinfo;            //保存通过retcode获得的错误信息

	int i;
	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "bucket_example";
	const char *prefix = "f";       //设置前缀

	oss_object_listing_t *object_listing = client_list_objects_with_prefix(client, bucket_name, prefix, &retcode);
	printf("list_objects_with_prefix result:\n\n");
	if(retcode == 0) {
		printf("list_objects_with_prefix successfully.\n");
		printf("bucket_name = %s\nnext_marker = %s\nprefix = %s\nmarker = %s\nmax_keys = %d\ndelimiter = %s\nis_truncated = %d\n", 
				object_listing->bucket_name, object_listing->next_marker, object_listing->prefix, object_listing->marker,
				object_listing->max_keys, object_listing->delimiter, object_listing->is_truncated);
		for(i = 0; i < object_listing->_counts_summaries; i++) {
			printf("****************************************************************\n");
			printf("object [%d] : \n", i);
			printf("type = %s\netag = %s\nkey = %s\nlast_modified = %s\nsize = %ld\nstorage_class = %s\nid = %s\ndisplay_name = %s\n",
					object_listing->summaries[i]->type, object_listing->summaries[i]->etag,
					object_listing->summaries[i]->key, object_listing->summaries[i]->last_modified,
					object_listing->summaries[i]->size, object_listing->summaries[i]->storage_class,
					object_listing->summaries[i]->owner->id, object_listing->summaries[i]->owner->display_name);
		}
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	if(object_listing != NULL) {
		if(object_listing->summaries != NULL) {
			for(i = 0; i < object_listing->_counts_summaries; i++) {
				owner_finalize((object_listing->summaries)[i]->owner);
				object_summary_finalize((object_listing->summaries)[i]);
			}
			free(object_listing->summaries);
		}
		object_listing_finalize(object_listing);
	}


	client_finalize(client);
	return 0;
}

