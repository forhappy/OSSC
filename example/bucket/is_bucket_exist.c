/*
 * =============================================================================
 *
 *       Filename:  is_bucket_exist.c
 *
 *    Description:  is_bucket_exist example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 判断一个bucket是否存在
*/

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "oss.aliyuncs.com";    //设置hostname

int main()
{
	bool result;
	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "bucket_example";       //设置bucket_name

	result = client_is_bucket_exist(client, bucket_name);
	printf("is_bucket_exist result:\n\n");
	if(result == true) {
		printf("bucket exist.\n");
	} else {
		printf("bucket not exist\n");
	}

	client_finalize(client);
	return 0;
}

