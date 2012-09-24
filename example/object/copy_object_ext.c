/*
 * =============================================================================
 *
 *       Filename:  client_copy_object-t.c
 *
 *    Description:  client_copy_object testcase.
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

/* Object 拷贝操作 */
int main()
{
	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *source_bucket_name = "bucketname001";
	const char *destination_bucket_name = "bucketname002";
	const char *source_key = "glib-2.32.4.tar.xz";
	const char *destination_key = "copy-of-glib-2.32.4.tar.xz";

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_copy_object_result_t *result = client_copy_object_ext(client, source_bucket_name, source_key,
			destination_bucket_name, destination_key, &retcode);

	if (retcode == OK) {
		if (result != NULL) {
			printf("ETag: %s\n", result->get_etag(result));
			printf("LastModified: %s\n", result->get_last_modified(result));
		}
		printf("Copy object successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	if (result != NULL) copy_object_result_finalize(result);
	client_finalize(client);
}
