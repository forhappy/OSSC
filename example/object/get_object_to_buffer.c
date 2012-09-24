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
#include <stdio.h>
#include <stdlib.h>
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "storage.aliyun.com";    //设置 hostname

/* 将远程服务器上的文件下载到内存中 */
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketname001";       //设置bucket_name
	const char *key         = "glib-2.32.4.tar.xz";      //设置下载 key
	const char *local_file  = "local-file.data";         //设置需要保存到本地的文件名
	size_t file_len = -1; /**< 远程文件的长度  */
	void *buffer = NULL; /* 存放文件内容的缓冲区首指针 */

	FILE *fp = fopen(local_file, "w");
	if (fp == NULL) {
		fprintf(stderr, "error in opening file %s\n", local_file);
		return -1;
	}

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_get_object_request_t *request = get_object_request_initialize(bucket_name, key);
	// request->set_range(request, 0, 2 * 1024);

	/* 将远程服务器上的文件下载到内存中 */

	/* 以下两个函数均可以成功调用 */
	/* 警告: 获取Object的函数均会返回该对象的元信息，由于元信息是动态动态分配的，
	 * 你需要获取其元信息，并在程序退出时释放它，否则会造成少量的内存泄漏(数百字节)
	 * */
	//oss_object_metadata_t *metadata =
	//	client_get_object_to_buffer(client, request, &buffer, &file_len, &retcode);
	
	oss_object_metadata_t *metadata =
		client_get_object_to_buffer_2nd(client, request, &buffer, &file_len, &retcode);

	if (retcode == OK) {
		fwrite(buffer, file_len, 1, fp);
		printf("Get object to buffer successfully.\n");
		printf("File length: %d\n", file_len);
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	if (buffer != NULL) free(buffer);
	if (request != NULL) get_object_request_finalize(request);
	if (metadata != NULL) object_metadata_finalize(metadata);
	client_finalize(client);
	fclose(fp);

	return 0;
}

