/*
 * =============================================================================
 *
 *       Filename:  oss_client.c
 *
 *    Description:  oss client
 *
 *        Created:  09/03/2012 09:18:04 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <curl/curl.h>

#include <ossc/client.h>

/* *
 * 初始化 oss_client_t，内部使用
 * */
oss_client_t *
_client_initialize(
		const char *access_id, size_t access_id_len,
		const char *access_key, size_t access_key_len,
		const char *endpoint, size_t endpoint_len)
{
	assert(access_id != NULL);
	assert(access_key != NULL);
	assert(endpoint != NULL);

	size_t byte_of_char = sizeof(char);
	oss_client_t *client = NULL;

	client = (oss_client_t *)malloc(sizeof(oss_client_t));

	client->access_id= (char *)malloc(byte_of_char * access_id_len+ 1);
	client->access_key = (char *)malloc(byte_of_char * access_key_len + 1);
	client->endpoint = (char *)malloc(byte_of_char * endpoint_len + 1);

	strncpy(client->access_id, access_id, access_id_len);
	strncpy(client->access_key, access_key, access_key_len);
	strncpy(client->endpoint, endpoint, endpoint_len);

	(client->access_id)[access_id_len] = '\0';
	(client->access_key)[access_key_len] = '\0';
	(client->endpoint)[endpoint_len] = '\0';

	return client;
}

/* *
 * 初始化 oss_client_t
 * */
oss_client_t *
client_initialize(const char *access_id,
		const char *access_key)
{
	assert(access_id != NULL);
	assert(access_key != NULL);

	size_t access_id_len = strlen(access_id);
	size_t access_key_len = strlen(access_key);
	size_t endpoint_len = strlen(DEFAULT_OSS_HOST);

	return _client_initialize(access_id, access_id_len,
			access_key, access_key_len,
			DEFAULT_OSS_HOST, endpoint_len);
}

/* *
 * 初始化 oss_client_t
 * */
oss_client_t *
client_initialize_with_endpoint(const char *access_id,
		const char *access_key,
		const char *endpoint)
{

	assert(access_id != NULL);
	assert(access_key != NULL);
	assert(endpoint != NULL);

	size_t access_id_len = strlen(access_id);
	size_t access_key_len = strlen(access_key);
	size_t endpoint_len = strlen(endpoint); 
	return _client_initialize(access_id, access_id_len,
			access_key, access_key_len,
			endpoint, endpoint_len);
}

/* *
 * 终止一个 Multipart 上传事件
 * */
void
client_abort_multipart_upload(oss_client_t *client, oss_abort_multipart_upload_request_t *request)
{
	return;
}

/* *
 * 完成一个 Multipart 上传事件
 * */
oss_complete_multipart_upload_result_t *
client_complete_multipart_upload(oss_client_t *client, oss_complete_multipart_upload_request_t *request)
{
	return NULL;
}

/* *
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * */
oss_copy_object_result_t *
client_copy_object(oss_client_t *client, oss_copy_object_request_t *request)
{
	return NULL;
}

/* *
 * 拷贝一个在 OSS 上已经存在的 Object 成另外一个 Object
 * */
oss_copy_object_result_t *
client_copy_object_ext(oss_client_t *client, const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key)
{
	return NULL;
}

/* *
 * 创建 Bucket
 * */
oss_bucket_t *
client_create_bucket(oss_client_t *client, const char *bucket_name)
{
	assert(client != NULL);
	assert(bucket_name != NULL);

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};

	char headers[1024] = {0};

	unsigned int sign_len = 0;

	CURL *curl = NULL;
	CURLcode result;


	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);
	oss_map_put(default_headers, OSS_DATE, now);

	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}

	return NULL;
}

/* *
 * 删除 Bucket
 * */
void
client_delete_bucket(oss_client_t *client, const char *bucket_name)
{
	return;
}

/* *
 * 删除指定的 OSSObject
 * */
void
client_delete_object(oss_client_t *client, const char *bucket_name,
		const char *key)
{
	return;
}

/* *
 * 生成一个包含签名信息并可以访问 OSSObject 的 URL
 * */
const char *
client_generate_presigned_url(oss_client_t *client, oss_generate_presigned_url_request_t *request)
{
	return NULL;
}

/* *
 * 生成一个用 HTTP GET 方法访问 OSSObject 的 URL
 * */
const char *
client_generate_presigned_url_with_expiration(oss_client_t *client, const char *bucket_name,
		const char *key,
		const char *expiration)
{
	return NULL;
}

/* *
 * 生成一个用指定 HTTP 方法访问 OSSObject 的 URL
 * */
const char *
client_generate_presigned_url_with_method(oss_client_t *client, const char *bucket_name,
		const char *key,
		const char *expiration,
		const char *method)
{
	return NULL;
}

/* *
 * 返回给定 Bucket 的 Access Control List(ACL)
 * */
oss_access_control_list_t *
client_get_bucket_acl(oss_client_t *client, const char *bucket_name)
{
	return NULL;
}

/* *
 * 返回访问的OSS Endpoint
 * */
const char *
client_get_endpoint(oss_client_t *client)
{
	return NULL;
}

/* *
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * */
oss_object_t *
client_get_object(oss_client_t *client, oss_get_object_request_t *request)
{

	assert(client != NULL);

	char resource[256]     = {0};
	//char request_line[256] = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};
	const char *bucket_name="";

	char headers[1024] = {0};

	unsigned int sign_len = 0;

	CURL *curl = NULL;
	CURLcode result;


	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);
	printf("sign: %s\n", sign);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}

	return NULL;
	return NULL;
}

/* *
 * 从 OSS 指定的 Bucket 中导出指定的 OSSObject 到目标文件
 * */
oss_object_metadata_t *
client_get_object_to_file(oss_client_t *client, oss_get_object_request_t *request,
		FILE *file)
{
	return NULL;
}

/* *
 * 从 OSS 指定的 Bucket 中导出 OSSObject
 * */
oss_object_t *
client_get_object_with_bucket_name(oss_client_t *client, const char *bucket_name,
		const char *key)
{
	return NULL;
}

/* *
 * 返回 OSSObject 的元数据
 * */
oss_object_metadata_t *
client_get_object_metadata(oss_client_t *client, const char *bucket_name,
		const char *key)
{
	return NULL;
}

/* *
 * 初始化一个 Multipart 上传事件
 * */
oss_initiate_multipart_upload_result_t *
client_initiate_multipart_upload(oss_client_t *client, oss_initiate_multipart_upload_request_t *request)
{
	return NULL;
}

/* *
 * 判断给定 Bucket 是否存在
 * */
bool
client_is_bucket_exist(oss_client_t *client, const char *bucket_name)
{
	return true;
}

/* *
 * 返回请求者拥有的所有 Bucket 的列表
 * */
oss_bucket_t *
client_list_buckets(oss_client_t *client)
{

	assert(client != NULL);

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};
	const char *bucket_name="";

	char headers[1024] = {0};

	unsigned int sign_len = 0;

	CURL *curl = NULL;
	CURLcode result;


	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);
	printf("sign: %s\n", sign);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}

	return NULL;
}

/* *
 * 列出所有执行中的 Multipart 上传事件
 * */
oss_multipart_upload_listing_t *
client_list_multipart_uploads(oss_client_t *client, oss_list_multipart_uploads_request_t *request)
{
	return NULL;
}

/* *
 * 列出指定 Bucket 下的 OSSObject
 * */
oss_object_listing_t *
client_list_objects(oss_client_t *client, oss_list_objects_request_t *request)
{
	return NULL;
}

/* *
 * 列出指定 Bucket 下的 OSSObject
 * */
oss_object_listing_t *
client_list_objects_with_bucket_name(oss_client_t *client, const char *bucket_name)
{
	return NULL;
}

/* *
 * 列出指定 Bucket 下 key 以给定 prefix 开头的 OSSObject
 * */
oss_object_listing_t *
client_list_objects_with_prefix(oss_client_t *client, const char *bucket_name,
		const char *prefix)
{
	return NULL;
}

/* *
 * 列出 multipart 中上传的所有 part 信息
 * */
oss_part_listing_t *
client_list_parts(oss_client_t *client, oss_list_parts_request_t *request)
{
	return NULL;
}

/* *
 * 上传指定的 OSSObject 到 OSS 中指定的 Bucket
 * */
oss_put_object_result_t *
client_put_object(oss_client_t *client, const char *bucket_name,
		const char *key,
		const char *input,
		oss_object_metadata_t *metadata)
{
	return NULL;
}

/* *
 * 设置指定 Bucket 的 Access Control List(ACL)
 * */
void
client_set_bucket_acl(oss_client_t *client, const char *bucket_name,
		const char *acl)
{
	return;
}

/* *
 * 上传一个分块（Part）到指定的的 Multipart 上传事件中
 * */
oss_upload_part_result_t *
client_upload_part(oss_client_t *client, oss_upload_part_request_t *request)
{
	return NULL;
}

void client_finalize(oss_client_t *client) 
{
	assert(client != NULL);

	if (client->access_id != NULL) {
		free(client->access_id);
		client->access_id = NULL;
	}

	if (client->access_key != NULL) {
		free(client->access_key);
		client->access_key = NULL;
	}

	if (client->endpoint != NULL) {
		free(client->endpoint);
		client->endpoint = NULL;
	}

	free(client);
	client = NULL;

}
