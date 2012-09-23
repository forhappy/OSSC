/*
 * =============================================================================
 *
 *       Filename:  abort_multipart_upload.c
 *
 *    Description:  abort_multipart_upload testcase
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "storage.aliyun.com";    //设置 hostname

/* 终止一个Multipart Upload操作*/
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketexample";       //设置bucket_name
	const char *key         = "multipart-upload.data";      //设置key
	const char *upload_id = "0004CA5FAC0DD11BFCEE7E76679BD09D"; /* TODO:此处设置你需要终止的 Upload ID */


	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_abort_multipart_upload_request_t *request = 
		abort_multipart_upload_request_initialize(bucket_name, key, upload_id);
	
	/* 终止一个Multipart Upload操作*/
	client_abort_multipart_upload(client, request, &retcode);

	if (retcode == OK) {
		printf("Abort multipart upload successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	client_finalize(client);
	abort_multipart_upload_request_finalize(request);

	return 0;

}

