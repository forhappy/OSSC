/*
 * =============================================================================
 *
 *       Filename:  client_get_object-t.c
 *
 *    Description:  client_get_object testcase
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "storage.aliyun.com";    //设置 hostname

/* 下载云服务器中的文件至本地文件 */
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketname001";       //设置bucket_name
	const char *key         = "glib-2.32.4.tar.xz";      //设置需要下载的key
	const char *local_file  = "local-file.data";         //设置需要下载的本地文件名，可以和key同名

	FILE *fp = fopen(local_file, "w");
	if (fp == NULL) {
		fprintf(stderr, "error in opening file %s\n", local_file);
		return -1;
	}

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_get_object_request_t *request = get_object_request_initialize(bucket_name, key);
	request->set_range(request, 0, 2 * 1024 * 1024);
	client_get_object_to_file(client, request, fp, &retcode);

	if (retcode == OK) {
		printf("Get object to file successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	client_finalize(client);
	fclose(fp);

	return 0;

}

