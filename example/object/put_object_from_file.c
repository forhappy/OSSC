/*
 * =============================================================================
 *
 *       Filename:  client-t.c
 *
 *    Description:  client testcase.
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

/* 将本地文件上传到云服务器上 */
int main()
{
	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketexample";       //设置bucket_name
	const char *key         = "put-ojbect.data";      //设置上传key
	const char *local_file  = "proactor.pdf";         //设置需要上传的文件

	FILE *fp = fopen(local_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "error in opening file %s\n", local_file);
		return -1;
	}

	unsigned int file_len = oss_get_file_size(fp);
	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
#if 1
	/* 初始化元信息，并设置相关属性 */
	oss_object_metadata_t *metadata = object_metadata_initialize(); 
	metadata->set_content_length(metadata, file_len);
	metadata->set_content_type(metadata, "application/octet-stream");
	metadata->set_cache_control(metadata, "no-cache");
	metadata->set_content_encoding(metadata, "utf-8");
	metadata->set_content_disposition(metadata, "attachment;");
	metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");
	/* 将本地文件上传到云服务器上 */
	oss_put_object_result_t *result =
		client_put_object_from_file(client, bucket_name, key, fp, metadata, &retcode);

	if (metadata != NULL) object_metadata_finalize(metadata);
#else 
	oss_put_object_result_t *result =
		client_put_object_from_file(client, bucket_name, key, fp, NULL, &retcode);
#endif
	if (retcode == OK) {
		printf("Put object from file successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	if (result != NULL) put_object_result_finalize(result);
	client_finalize(client);
	fclose(fp);

	return 0;
}
