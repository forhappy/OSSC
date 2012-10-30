/*
 * =============================================================================
 *
 *       Filename:  sync_download.c
 *
 *    Description:  sync_download testcase.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <ossc/client.h>
#include <ossc/oss_extra.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "oss.aliyuncs.com";    //设置hostname

int main()
{

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "qqaazz";       //设置bucket_name
	if(oss_sync_download(client, "/home/wangwei/Documents/download_test/",bucket_name) == 0) {
		printf("sync_download success.\n");
	} else {
		printf("sync_download failed.\n");
	}
}

