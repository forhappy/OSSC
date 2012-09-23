/*
 * =============================================================================
 *
 *       Filename:  generate_presigned_url_with_expiration.c
 *
 *    Description:  generate_presigned_url_with_expiration example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 生成一个用HTTP GET方法访问OSSObject的URL
*/

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "storage.aliyun.com";    //设置hostname

int main()
{
	//unsigned short retcode;			//保存服务器http返回码的解析结果;
	//const char *retinfo;            //保存通过retcode获得的错误信息

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key = "a.txt";

	time_t t = time(NULL); 
	t += 60;
	char  expiration[20];
	sprintf(expiration, "%ld", t);
	printf("expiration = %s\n", expiration);
	
	char * url = client_generate_presigned_url_with_expiration(client, bucket_name, key, expiration);
	printf("url = %s\n", url);
	if(url != NULL) {
		free(url);
		url = NULL;
	}

	client_finalize(client);
	return 0;
}

