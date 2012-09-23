/*
 * =============================================================================
 *
 *       Filename:  get_object_group_to_file.c
 *
 *    Description:  get_object_group_to_file example.
 *
 *        Created:  09/21/2012 03:55:34 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 读取用户指定的Object group到本地文件 
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

	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key = "a_group_file.dat";             //设置object group的名称
	FILE *fp = NULL;
	if((fp = fopen("get_object_group", "w+")) == NULL) {
		printf("open file failed.\n");
		return -1;
	}

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	oss_get_object_group_request_t *request = 
		get_object_group_request_initialize(bucket_name, key);
	oss_object_metadata_t *result = client_get_object_group_to_file(client, request, fp, &retcode);

	printf("get_obejct_group_to_file result:\n\n");
	if(retcode == OK) {
		printf("get_object_group_to_file successfully.\n");
		printf("etag = %s\nlast_modified = %s\ncontent_type = %s\ncontent_length = %ld\n", 
				result->get_etag(result), result->get_last_modified(result),
				result->get_content_type(result), result->get_content_length(result));
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


