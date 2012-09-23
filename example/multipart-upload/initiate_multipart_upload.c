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

/* 初始化Multipart Upload操作*/
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key         = "multipart-upload.data";      //设置key


	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);


#if 1 /* 设置带元信息的Multipart Upload 上传请求 */

	oss_object_metadata_t *metadata = object_metadata_initialize();

	/* 设置上传对象的元信息 */
	metadata->set_cache_control(metadata, "no-cache");
	metadata->set_content_encoding(metadata, "utf-8");
	metadata->set_content_disposition(metadata, "attachment;");
	metadata->set_expiration_time(metadata, "Thu, 15 Sep 2012 21:08:42 GMT");

	oss_initiate_multipart_upload_request_t *request = 
		initiate_multipart_upload_request_initialize_with_metadata(bucket_name, key, metadata);
#else /* 不带元信息的Multipart Upload上传请求 */
	oss_initiate_multipart_upload_request_t *request = 
		initiate_multipart_upload_request_initialize(bucket_name, key, NULL);
#endif


	oss_initiate_multipart_upload_result_t *result =
		client_initiate_multipart_upload(client, request, &retcode);
	if (retcode == OK) {
		printf("%s\n", result->get_upload_id(result));
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}

	client_finalize(client);

	return 0;

}

