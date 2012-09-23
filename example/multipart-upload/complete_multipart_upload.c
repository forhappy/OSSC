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


/* TODO: 此处设置你从 Upload Part 步骤中获取的 ETag 值*/
const char *etags[] = {
	"ETag001",
	"ETag002",
	"...",
	"ETag00n"
};

/* 完成一个Multipart Upload操作*/
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucket_example";       //设置bucket_name
	const char *key         = "multipart-upload.data";      //设置key
	const char *upload_id = ""; /* TODO:此处设置你需要终止的 Upload ID */

	size_t parts = sizeof(etags) / sizeof(char *);

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_part_etag_t **part_etag = (oss_part_etag_t **)malloc(sizeof(oss_part_etag_t *) * parts);
	int i = 0;
	for (; i < 5; i++) {

		*(part_etag + i) = part_etag_initialize(i + 1, etags[i]);
	}

	oss_complete_multipart_upload_request_t *request = 
		complete_multipart_upload_request_initialize(bucket_name, key, upload_id, part_etag, parts);

	/* 完成一次Multipart Upload上传操作 */
	oss_complete_multipart_upload_result_t *result = 
		client_complete_multipart_upload(client, request, &retcode);

	if (retcode == OK) {
		printf("Complete multipart upload successfully.\n");
		printf("ETag: %s\n", result->get_etag(result));
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}
	complete_multipart_upload_request_finalize(request);
	complete_multipart_upload_result_finalize(result);
	client_finalize(client);

	return 0;

}

