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

/* 将内存中的内容上传至云服务器中 */
int main()
{
	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketexample";       //设置bucket_name
	const char *key         = "put-ojbect.data";      //设置上传key
	const char *local_file  = "proactor.pdf";         //设置需要上传的文件
	size_t retsize = 0; // 读取文件时的返回值

	FILE *fp = fopen(local_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "error in opening file %s\n", local_file);
		return -1;
	}
	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	size_t file_len = oss_get_file_size(fp);

	char *buffer = (char *)malloc(sizeof(char) * file_len + 1);

	memset(buffer, '\0', file_len + 1);
	retsize = fread(buffer, 1, file_len, fp);

	if (retsize < 0) {
		fprintf(stderr, "error in reading file %s.\n", local_file);
		return -1;
	}

	/* 初始化元信息，并设置相关属性 */
	oss_object_metadata_t *metadata = object_metadata_initialize(); 
	metadata->set_content_length(metadata, file_len);
	metadata->set_content_type(metadata, "application/octet-stream");
	metadata->set_cache_control(metadata, "no-cache");
	metadata->set_content_encoding(metadata, "utf-8");
	metadata->set_content_disposition(metadata, "attachment;");
	metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");

	/* 将内存中的内容上传至云服务器中 */
	client_put_object_from_buffer(client, bucket_name, key, buffer, metadata, &retcode);
	if (retcode == OK) {
		printf("put object from file successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	client_finalize(client);
	free(buffer);
	fclose(fp);
	return 0;
}
