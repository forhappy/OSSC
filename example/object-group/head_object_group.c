/*
 * =============================================================================
 *
 *       Filename:  head_object_group.c
 *
 *    Description:  head_object_group example.
 *
 *        Created:  09/21/2012 03:55:34 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* Post Object Group 返回用户指定的 Object Group 信息, 不返回文件内容。
*/

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "storage.aliyun.com";    //设置hostname

int main()
{
	unsigned short retcode;			//保存服务器http返回码的解析结果;
	const char *retinfo;            //保存通过retcode获得的错误信息

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key = "a_group_file.dat";             //设置object group的名称

	oss_get_object_group_request_t *request = get_object_group_request_initialize(bucket_name, key);
	request->set_modified_since_constraint(request, "Sat, 20 Sep 2012 23:23:23 GMT");
	oss_object_metadata_t *result = client_head_object_group(client, request, &retcode);

	printf("head_obejct_group result:\n\n");
	if(retcode == OK) {
		printf("head_object_group successfully.\n");
		printf("content-length = %ld\ncontent-type = %s\nlast-modified = %s\n", result->get_content_length(result), result->get_content_type(result), result->get_last_modified(result));
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* *释放空间 */
	if(request != NULL) {
		get_object_group_request_finalize(request);
	}
	if(result) {
		object_metadata_finalize(result);
	}
	client_finalize(client);
	return 0;
}


