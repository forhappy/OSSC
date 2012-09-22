/*
 * =============================================================================
 *
 *       Filename:  client_set_bucket_acl-x-t.c
 *
 *    Description:  lient_set_bucket_acl-x testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include <ossc/client.h>
#define _OSS_CLIENT_H
#include <modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>

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

size_t bucket_curl_operation_send_callback(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t bytes_per_send = size * nmemb; 

	if(bytes_per_send < 1)
		return 0;
	if(send_buffer->left > 0) {
		if (send_buffer->left > bytes_per_send) {
			memcpy(ptr, send_buffer->ptr, bytes_per_send);
			send_buffer->ptr += bytes_per_send; /* advance pointer */
			send_buffer->left -= bytes_per_send; /* less data left */
			return bytes_per_send;
		} else {
			memcpy(ptr, send_buffer->ptr, send_buffer->left);
			size_t last_sent_bytes = send_buffer->left;
			send_buffer->left -= bytes_per_send; /* less data left */
			return last_sent_bytes;
		}
	} else return 0;
}

size_t bucket_curl_operation_recv_callback(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
	size_t bytes_per_recv = size * nmemb;
	if (recv_buffer->left > 0) {
		size_t offset = recv_buffer->allocated - recv_buffer->left;
		if (recv_buffer->left > bytes_per_recv) {
			strncpy(recv_buffer->ptr + offset, ptr, size * nmemb);
			recv_buffer->left -= bytes_per_recv;
			return bytes_per_recv;
		} else {
			strncpy(recv_buffer->ptr + offset, ptr, recv_buffer->left);
			size_t last_recv_bytes = recv_buffer->left;
			recv_buffer->left -= bytes_per_recv;
			return last_recv_bytes;
		}
	} else {
		fprintf(stderr, "%s\n", "Receive buffer overflow!");
		return 0;
	}
}

size_t bucket_curl_operation_header_callback(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	int r;
	size_t code = 0;
	r = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (r != 0) {
		header_buffer->code= code;
	}
	return size * nmemb;
}
static void
bucket_curl_operation(const char *method,
		const char *resource,
		const char *url,
		struct curl_slist *http_headers,
		void *user_data)
{
	assert(method != NULL);
	assert(resource != NULL);
	assert(http_headers != NULL);
	assert(user_data != NULL);

	CURL *curl = NULL;

	curl_request_param_t *params = (curl_request_param_t *)user_data;

	param_buffer_t *recv_buffer = params->recv_buffer;
	param_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);

		if (strcmp(method, OSS_HTTP_PUT) == 0 || strcmp(method, OSS_HTTP_DELETE))
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bucket_curl_operation_recv_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, bucket_curl_operation_header_callback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

/* *
 * 设置指定 Bucket 的 Access Control List(ACL)
 * */

void
client_set_bucket_acl(oss_client_t *client,
		const char *bucket_name,
		const char *acl,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(bucket_name != NULL);

	curl_request_param_t *user_data = (curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL;

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 128 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 128 * 1024;
	user_data->recv_buffer->allocated = 128 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 16 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 16 * 1024;
	user_data->header_buffer->allocated = 16 * 1024;

	size_t bucket_name_len = strlen(bucket_name);

	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * bucket_name_len + 64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char header_acl[32]   = {0};

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_acl, "x-oss-acl: %s", acl);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_ACL, acl);
	
	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, user_headers, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部
	 */
	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	http_headers = curl_slist_append(http_headers, header_acl);

	/**
	 * 发送请求
	 */
	bucket_curl_operation(OSS_HTTP_PUT, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);

	printf("%u\n", user_data->header_buffer->code);
	printf("%s\n", user_data->recv_buffer->ptr);

}

int main()
{
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	oss_client_t *client = client_initialize(access_id, access_key);
	//client_create_bucket(client, "bucketname002");
	//client_list_buckets(client);
	client_set_bucket_acl(client, "bucketname1", "public-read", NULL);
}
