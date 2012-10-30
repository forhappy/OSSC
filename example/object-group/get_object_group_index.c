/*
 * =============================================================================
 *
 *       Filename:  get_object_group_index.c
 *
 *    Description:  get_object_group_index example.
 *
 *        Created:  09/21/2012 03:55:34 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* 返回 Object Group 中的 Object List 信息
*/

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "oss.aliyuncs.com";    //设置hostname

int main()
{
	unsigned short retcode;			//保存服务器http返回码的解析结果;
	const char *retinfo;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key = "a_group_file.dat";             //设置object group的名称
	int i;

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	oss_get_object_group_index_result_t *result = client_get_object_group_index(client, bucket_name, key, &retcode);

	printf("get_obejct_group_index result:\n\n");
	if(retcode == OK) {
		printf("get_object_group_index successfully.\n");
		printf("bucket_name = %s\nkey = %s\netag = %s\nfile_length = %u\n", 
				result->get_bucket_name(result), result->get_key(result),
				result->get_etag(result), result->get_file_length(result));
		printf("============================================\n");
		for(i = 0; i < result->part_number; i++) {
			printf("part [%d] :\n", i);
			printf("etag = %s\npart_name = %s\npart_number = %d\npart_size = %u\n", 
					(result->group)[i]->etag, (result->group)[i]->part_name,
					(result->group)[i]->part_number, (result->group)[i]->part_size);
			printf("*****************************************\n");
		}
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* *释放空间 */
	if(result != NULL) {
		if(result->group != NULL) {
			for(i = 0; i < result->part_number; i++) {
				multipart_object_group_finalize((result->group)[i]);
			}
			free(result->group);
		}
		get_object_group_index_result_finalize(result);
	}
	client_finalize(client);
	 return 0;
}


