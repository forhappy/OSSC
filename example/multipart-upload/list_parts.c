/*
 * =============================================================================
 *
 *       Filename:  list_parts.c
 *
 *    Description:  list_parts testcase
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "oss.aliyuncs.com";    //设置 hostname

/* 列出一次Multipart Upload操作已经上传的所有Parts */
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketexample";       //设置bucket_name
	const char *key         = "multipart-upload.data";      //设置key

	 /* TODO: 此处设置Upload ID */
	const char *upload_id = "0004CD3FD5EC689289BF40CEED39CF60"; 

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_list_parts_request_t *request = 
		list_parts_request_initialize(bucket_name, key, upload_id);

	oss_part_listing_t *listing = client_list_parts(client, request, &retcode);

	if (retcode == OK) {
		int part_counts = 0;
		int i = 0;
		oss_part_summary_t **parts = listing->get_parts(listing, &part_counts);

		for (i = 0; i < part_counts; i++) {
			printf("********************************** CONTENT %d*******************************\n", i + 1);
			printf("ETAG: %s\n", (*(parts + i))->get_etag(*(parts + i)));
			printf("LAST_MODIFIED: %s\n", (*(parts + i))->get_last_modified(*(parts + i)));
			printf("PARTNUMBER: %d\n", (int)(*(parts + i))->get_part_number(*(parts + i)));
			printf("SIZE: %d\n", (int)(*(parts + i))->get_size(*(parts + i)));
		}
	} else {
			retinfo = oss_get_error_message_from_retcode(retcode);
			printf("%s\n", retinfo);
	}

	client_finalize(client);
	list_parts_request_finalize(request);
	part_listing_finalize(listing);

	return 0;

}

