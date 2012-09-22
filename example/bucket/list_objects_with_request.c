/*
 * =============================================================================
 *
 *       Filename:  list_objects_with_request.c
 *
 *    Description:  list_objects_with_request example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 返回请求者制定Bucket下的所有Object的列表
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
	/* 构造 request 参数 */
	oss_list_objects_request_t *request = list_objects_request_initialize();
	request->set_bucket_name(request, "bucket_example");
	request->set_prefix(request,"fun/");          //设置prefix
	request->set_delimiter(request, "/");      //设置delimiter
	request->set_max_keys(request, 100);      //设置max_keys
	request->set_marker(request, "t");         //设置marker


	oss_object_listing_t *object_listing = client_list_objects(client, request, &retcode);
	printf("list_objects_with_request result:\n\n");
	if(retcode == 0) {
		printf("list_objects_with_request successfully.\n");
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
		printf("=============================================================\n");
		for(i = 0; i < object_listing->_counts_common_prefixes; i++) {
			printf("common_prefix = %s\n", (object_listing->common_prefixes)[i]);
		}
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* *释放空间 */
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

