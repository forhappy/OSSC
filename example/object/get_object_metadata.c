/*
 * =============================================================================
 *
 *       Filename:  client_head_object-t.c
 *
 *    Description:  client_head_object testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "storage.aliyun.com";    //设置 hostname

/* 获取Object的元信息 */
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key         = "get-ojbect.data";      //设置key

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_object_metadata_t *metadata =
		client_get_object_metadata(client, bucket_name, key, &retcode);
	if (retcode == OK) {
		printf("ETag: %s\n", metadata->get_etag(metadata));
		printf("Content-Type: %s\n", metadata->get_content_type(metadata));
		printf("Content-Length: %ld\n", metadata->get_content_length(metadata));
		printf("Last-Modified: %s\n", metadata->get_last_modified(metadata));
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	client_finalize(client);
}
