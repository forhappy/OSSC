/*
 * =============================================================================
 *
 *       Filename:  example.c
 *
 *    Description:  test functions we have completed.
 *
 *        Created:  09/20/2012 09:17:45 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "";   //设置用户帐号
static const char *access_key = "";  //设置用户密码
static const char *endpoint = "";    //设置hostname

int main()
{
	unsigned short retcode;   //保存服务器http返回码的解析结果;
	char *retinfo;            //保存通过retcode获得的错误信息

	oss_client_t *client = client_initialize(access_id, access_key, endpoint);
	/**
	 * test create_bucket.
	 */
	const char *bucket_name = "";
	client_create_bucket(client, bucket_name, &retcode);
	printf("create_bucket:\n");
	if(retcode == 0) {
		printf("create bucket successfully.\n");
	} else {
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	printf("=============================================\n");

	/**
	 * test set_bucket_acl.
	 */
	const char *acl = "";
	client_set_bucket_acl(client, bucket_name, acl, &retcode);
	printf("set_bucket_acl:\n");
	if(retcode == 0) {
		printf("set bucket acl successfully.\n");
	} else {
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	printf("=============================================\n");

	/**
	 * test get_bucket_acl.
	 */
	oss_access_control_list_t *access_control_list = 
		client_get_bucket_acl(client, bucket_name, &retcode);
	printf("get_bucket_acl:\n");
	if(retcode == 0) {
		printf("grant = %s\nid = %s\ndisplay_name = %s\n", 
				access_control_list->grant,
				access_control_list->owner->id,
				access_control_list->owner->display_name);
	} else {
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	printf("=============================================\n");

	/**
	 * test list_buckets.
	 */
	int buckets_number;       //保存buckets的数目
	int i;
	oss_owner_t *owner;
	oss_bucket_t **buckets = client_list_buckets(client, &buckets_number, &retcode);
	printf("list buckets:\n");
	if(retcode == 0) {
		if(buckets_number != 0) {
			for(i = 0; i < buckets_number; i++) {
				printf("bucket [%d] :\n", i + 1);
				printf("name = %s\tcreate_date = %s\n", 
						buckets[i]->get_name(buckets[i]),
						buckets[i]->get_create_date(buckets[i]));
				owner = buckets[i]->get_owner(buckets[i]);
				printf("id = %s\tdisplay_name = %s\n", 
						owner->get_id(owner),
						owner->get_display_name(owner));
				printf("**********************************");
			}
		} else {
			printf("")
		}
	} else {
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error: %s\n", retinfo);
	}
	

}
