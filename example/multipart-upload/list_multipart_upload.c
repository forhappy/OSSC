/*
 * =============================================================================
 *
 *       Filename:  list_multipart_upload.c
 *
 *    Description:  list_multipart_upload testcase
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "storage.aliyun.com";    //设置 hostname

/* 列出所有的Multipart Upload操作*/
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucket_example";       //设置bucket_name


	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);


	oss_list_multipart_uploads_request_t *request = 
		list_multipart_uploads_request_initialize(bucket_name);

	oss_multipart_upload_listing_t *listing = client_list_multipart_uploads(client, request, &retcode);

	if (retcode == OK) {

		printf("BUCKET NAME: %s\n", listing->get_bucket_name(listing));

		size_t upload_counts = 0;
		int i = 0;

		oss_multipart_upload_t **uploads = listing->get_multipart_uploads(listing, &upload_counts);
		for (i = 0; i < upload_counts; i++) {
			printf("***********************************CONTENT %d*******************************\n", i + 1);
			printf("KEY: %s\n", (*(uploads + i))->get_key(*(uploads + i)));
			printf("UPLOAD ID: %s\n", (*(uploads + i))->get_upload_id(*(uploads + i)));
			printf("STORAGE CLASS: %s\n", (*(uploads + i))->get_storage_class(*(uploads + i)));
			printf("INITIATED: %s\n", (*(uploads + i))->get_initiated(*(uploads + i)));
		}
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}



	list_multipart_uploads_request_finalize(request);
	multipart_upload_listing_finalize(listing);
	client_finalize(client);

	return 0;

}

