/*
 * =============================================================================
 *
 *       Filename:  get_bucket_acl.c
 *
 *    Description:  get_bucket_acl example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 获得指定bucket的访问权限
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

	oss_access_control_list_t *acl = 
		client_get_bucket_acl(client, bucket_name, &retcode);
	oss_owner_t *owner = NULL;
	printf("get_bucket_acl result:\n\n");
	if(retcode == 0) {
		printf("get_bucket_acl successfully.\n");
		owner = acl->get_owner(acl);
		printf("grant = %s\nid = %s\ndisplay_name = %s\n", 
				acl->grant,
				owner->id,
				owner->display_name);
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	if(owner != NULL) {
		owner_finalize(owner);
	}
	if(acl != NULL) {
		access_control_list_finalize(acl);
	}

	client_finalize(client);
	return 0;
}

