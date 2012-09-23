/*
 * =============================================================================
 *
 *       Filename:  set_bucket_acl.c
 *
 *    Description:  set_bucket_acl example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 设置指定bucket的访问权限
* 暂时服务器那边的private和public-read-write是一样的
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

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "bucket_example";    //设置bucket_name

	const char *acl = "public-read";            //设置bucket的访问权限（private，public-read，public-read-write中的一个）
	client_set_bucket_acl(client, bucket_name, acl, &retcode);
	printf("set_bucket_acl result:\n\n");
	if(retcode == 0) {
		printf("set bucket acl successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	client_finalize(client);
	return 0;
}

