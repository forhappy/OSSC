/*
 * =============================================================================
 *
 *       Filename:  post_object_group.c
 *
 *    Description:  post_object_group example.
 *
 *        Created:  09/21/2012 03:55:34 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/**
* Post Object Group 操作将根据用户提供的 Object 信息,在 OSS 服务器端创建出一个新的 Object Group。
* 用户需要将创建该 Object Group 所需的 Object Name, E-tag 以及标识该 Object 在整个 Group 中相对位置的 Part ID
* 按照规定的 XML 格式发给 OSS, Object 的 E-tag 值可以通过list_object功能获得。
*/

#include <ossc/client.h>
#include <stdio.h>

static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "storage.aliyun.com";    //设置hostname

/* *设置要组合的Objects的etag值 */
static const char *etags[] = {
	"6F395BF16882D154CCB448806EA8C47D",
	"321F048D6C898398F3168545F6CE8551",
	"249697174471EEE73202E9214B037E08",
	"B78E7B470FA41AA5478CFB0EDFF7B2E6",
	"E0CA28D194BC790EF3C6236231D033A7"
};

/* *设置要组合的Objects的name */
static const char *partname[] = {
	"a.txt",
	"b.txt",
	"c.txt",
	"d.txt",
	"e.txt"
};

int main()
{
	unsigned short retcode;			//保存服务器http返回码的解析结果;
	const char *retinfo;            //保存通过retcode获得的错误信息

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key = "a_group_file.dat";             //设置object group的名称
	int i;
	int n = sizeof(partname)/sizeof(char *);
	/* *构造各个Object的etag，name，ID到part_item */
	oss_object_group_item_t **part_item = 
		(oss_object_group_item_t **)malloc(sizeof(oss_object_group_item_t *) * n);
	for (i = 0; i < n; i++) {
		*(part_item + i) = object_group_item_initialize(etags[i], partname[i], i + 1);
	}
	oss_post_object_group_request_t *request = 
		post_object_group_request_initialize(bucket_name, key, part_item, n);
	oss_post_object_group_result_t *post_result = client_post_object_group(client, request, &retcode);

	printf("post_obejct_group result:\n\n");
	if(retcode == OK) {
		printf("post_object_group successfully.\n");
		printf("bucket_name = %s\nkey = %s\netag = %s\nsize = %u\n", 
				post_result->bucket_name, post_result->key,
				post_result->etag, post_result->size);
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* *释放空间 */
	if(part_item != NULL) {
		for(i = 0; i < n; i++) {
			if(part_item[i] != NULL) {
				object_group_item_finalize(part_item[i]);
			}
		}
		free(part_item);
	}
	if(request != NULL) {
		post_object_group_request_finalize(request);
	}
	if(post_result) {
		post_object_group_result_finalize(post_result);
	}
	client_finalize(client);
	return 0;
}


