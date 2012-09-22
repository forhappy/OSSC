/*
 * =============================================================================
 *
 *       Filename:  delete_object_group.c
 *
 *    Description:  delete_object_group example.
 *
 *        Created:  09/21/2012 03:55:34 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* Delete Object Group 删除用户指定的 Object Group 信息
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

	client_delete_object_group(client, bucket_name, key, &retcode);

	printf("delete_obejct_group result:\n\n");
	if(retcode == NO_CONTENT) {
		printf("delete_object_group successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* *释放空间 */
	client_finalize(client);
	return 0;
}


