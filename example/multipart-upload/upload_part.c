/*
 * =============================================================================
 *
 *       Filename:  upload_part.c
 *
 *    Description:  upload_part testcase
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>

static const char *access_id  = "ACSGmv8fkV1TDO9L"; /**设置用户 Access ID */
static const char *access_key = "BedoWbsJe2"; /** 设置用户的 Access Key */
static const char *endpoint   = "oss.aliyuncs.com";    //设置 hostname

/* 上传 Part 操作，本例演示了如果使用上传Part操作来上传一个文件，
 * 适合上传大文件，但是如果想多线程断点续传，请在Unix/Linux下使用
 * OSSC 的 extra 子程序库中的 client_extra_put_object()函数。*/
int main()
{

	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
	const char *retinfo = NULL;            //保存通过retcode获得的错误信息

	const char *bucket_name = "bucketexample";       //设置bucket_name
	const char *key         = "multipart-upload.data";      //设置key

	/* TODO: 设置upload id，此upload id由initiate multipart upload 操作返回 */
	const char *upload_id = "0004CD3FD5EC689289BF40CEED39CF60"; 

	FILE *fp= fopen("mysql-5.1.52.tar.gz", "r"); /* 需要通过multipart upload上传的文件 */
	if (fp == NULL) {
		fprintf(stderr, "error in opening file...\nplease check again before upload part !\n");
		return -1;
	}
	unsigned int file_len = oss_get_file_size(fp); /* 获取文件大小 */
	
	char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	memset(buffer, '\0', file_len + 1);
	unsigned int file_sz = fread(buffer, 1, file_len, fp);
	if (file_sz < file_len) {fprintf(stderr, "fread error.\n"); return -1;}
	const int single_request_len = 8 * 1024 * 1024; /* 每个Part的大小 */
	int requests_num = file_len / single_request_len; /* 需要多少次上传 */
	int current_part_number = 0; /* 当前上传的 Part Number */

	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);

	oss_upload_part_request_t *request = upload_part_request_initialize();
	request->set_bucket_name(request, bucket_name);
	request->set_key(request, key);
	request->set_upload_id(request, upload_id);

	for (current_part_number = 0; current_part_number < requests_num + 1; current_part_number++) {
		/* 除了最后一块的其他块，最小为 5MB，此处设置单个Part为single_request_len = 8MB */
		if (current_part_number < requests_num) { 
			request->set_part_number(request, current_part_number + 1);
			request->set_input_stream(request,
					buffer + current_part_number * single_request_len, single_request_len);
			request->set_part_size(request, single_request_len);

			/* 上传Part */
			oss_upload_part_result_t *result = client_upload_part(client, request, &retcode);

			if (retcode == OK) { /* 打印返回信息，包括 Part Number 和 ETag 值 */
				printf("PartNumber:%d, ETag:%s\n", result->get_part_number(result), result->get_etag(result));
			} else { /* 打印出错信息 */
				retinfo = oss_get_error_message_from_retcode(retcode);
				printf("%s\n", retinfo);
			}
			/* 最好此时释放 result 空间 */
			upload_part_result_finalize(result);
		} else { /* 最后一块，可以小于5MB */
			request->set_part_number(request, current_part_number + 1);
			request->set_input_stream(request,
					buffer + current_part_number *single_request_len,
					file_len - single_request_len * current_part_number);
			request->set_part_size(request, file_len - single_request_len * current_part_number);

			/* 上传Part */
			oss_upload_part_result_t *result = client_upload_part(client, request, &retcode);
			
			if (retcode == OK) { /* 打印返回信息，包括 Part Number 和 ETag 值 */
				printf("PartNumber:%d, ETag:%s\n", result->get_part_number(result), result->get_etag(result));
			} else { /* 打印出错信息 */
				retinfo = oss_get_error_message_from_retcode(retcode);
				printf("%s\n", retinfo);
			}
			/* 最好及时释放 result 空间，否则回造成内存泄漏 */
			upload_part_result_finalize(result);
		}
	}

	free(buffer);

	upload_part_request_finalize(request);
	client_finalize(client);
	fclose(fp);

	return 0;

}

