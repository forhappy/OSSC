/*
 * =============================================================================
 *
 *       Filename:  list_buckets.c
 *
 *    Description:  list_buckets example.
 *
 *        Created:  09/21/2012 09:21:49 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 返回请求者拥有的所有Bucket的列表
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

	int buckets_number;             //保存buckets的数目
	int i;
	oss_owner_t *owner = NULL;
	oss_bucket_t **buckets = client_list_buckets(client, &buckets_number, &retcode);
	printf("list buckets result:\n\n");
	if(retcode == 0) {
		printf("list buckets successfully.\n");
		if(buckets_number != 0) {
			for(i = 0; i < buckets_number; i++) {
				printf("***********************************************************************\n");
				printf("bucket [%d] : \n", i);
				printf("name = %s\tcreate_date = %s\n", buckets[i]->get_name(buckets[i]), buckets[i]->get_create_date(buckets[i]));
				owner = buckets[i]->get_owner(buckets[i]);
				printf("id = %s\tdisplay_name = %s\n", owner->get_id(owner), owner->get_display_name(owner));
			}
		} else {
			printf("empty bucket list.\n");
		}
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* 释放空间 */
	if(buckets != NULL) {
		for(i = 0; i < buckets_number; i++) {
			if(buckets[i] != NULL) {
				bucket_finalize(buckets[i]);
			}
		}
		free(buckets);
	}
	if(owner != NULL) {
		owner_finalize(owner);
	}

	client_finalize(client);
	return 0;
}

