/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main testcase.
 *
 *        Created:  09/18/2012 11:18:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>
#include <ossc/oss_extra.h>
#include <ossc/oss_helper.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "storage.aliyun.com";    //设置 hostname

/* 多线程断点续传，将本地大文件(> 5M)上传至云服务器中 */
int main()
{
	unsigned short retcode = -1;
	const char *retinfo = NULL;

	const char *bucket_name = "bucketexample";
	const char *key = "upload-mt.data";
	const char *local_file = "mysql-5.5.27-linux2.6-x86_64.tar.gz";
	//const char *local_file = "mysql-5.1.52.tar.gz";

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	
	/* 多线程断点续传，将本地大文件上传至云服务器中 */
	client_extra_put_object(client, bucket_name, key, local_file, &retcode);

	if (retcode == OK) {
		printf("put object with multithreaded mode successfully.\n");
	} else {
		retinfo = oss_get_error_message_from_retcode(retcode);
		printf("%s\n", retinfo);
	}
}
