/*
 * =============================================================================
 *
 *       Filename:  client_object_operation-t.c
 *
 *    Description:  client_object_operation testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <util/oss_tstring.h>
#include <lib/md5.h>
#include <lib/base64.h>
#include <ossc/client.h>
#define _OSS_CLIENT_H
#include <modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>
typedef struct curl_request_param_s curl_request_param_t;
typedef struct param_buffer_s param_buffer_t;

struct param_buffer_s {
	char *ptr; /**< 缓冲区首指针 */
	FILE *fp; /**< 文件指针 */
	size_t left; /** 缓冲区剩余大小 */
	size_t allocated; /** 缓冲区总大小 */
	unsigned short code; /**返回码 */
};

struct curl_request_param_s {
	param_buffer_t *send_buffer; /**< send buffer */
	param_buffer_t *recv_buffer; /**< receive buffer */
	param_buffer_t *header_buffer; /**< header buffer */
};
const char *
oss_compute_md5_digest(void *ptr, size_t len)
{
	char md5_digest[17];
	md5_state_t md5_state;

	char *base64_md5 = NULL;

	memset(md5_digest, '\0', 17);

	md5_init(&md5_state);
	md5_append(&md5_state, ptr, len);
	md5_finish(&md5_state, (md5_byte_t *)md5_digest);

	base64_md5 = (char *) malloc(sizeof(char) * 65);
	memset(base64_md5, 0, 65);
	base64_encode(md5_digest, 16, base64_md5, 65);

	return base64_md5;

}

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




size_t object_curl_operation_send_from_file_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fread(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t object_curl_operation_send_from_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t bytes_per_send = size * nmemb; 

	if(bytes_per_send < 1)
		return 0;
	if((int)(send_buffer->left) > 0) {
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

size_t object_curl_operation_recv_to_file_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fwrite(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t object_curl_operation_recv_to_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
	size_t bytes_per_recv = size * nmemb;
	if ((int)(recv_buffer->left) > 0) {
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

size_t object_curl_operation_recv_to_buffer_2nd_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{

  size_t recv_size = size * nmemb;
  param_buffer_t *recv_buffer = (param_buffer_t *)stream;
 
  recv_buffer->ptr = realloc(recv_buffer->ptr, recv_buffer->allocated + recv_size + 1);
  if (recv_buffer->ptr == NULL) {
    /* out of memory! */ 
    exit(EXIT_FAILURE);
  }
  memcpy(&(recv_buffer->ptr[recv_buffer->allocated]), ptr, recv_size);
  recv_buffer->allocated += recv_size;
  recv_buffer->ptr[recv_buffer->allocated] = 0;
  return recv_size;
}

size_t object_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	char etag[48] = {0};
	int rcode = 0;
	int retag = 0;
	size_t code = 0;

	rcode = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (rcode != 0) {
		header_buffer->code= code;
	}

	retag = sscanf(ptr, "ETag: \"%s\"", etag);
	if (retag != 0) {
		memset(header_buffer->ptr, 0, header_buffer->allocated);
		strncpy(header_buffer->ptr, etag, 48);
	}
	return size * nmemb;
}


static void
object_curl_operation(const char *method,
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


	param_buffer_t *send_buffer = params->send_buffer;
	param_buffer_t *recv_buffer = params->recv_buffer;
	param_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);

		if (strcmp(method, OSS_HTTP_PUT) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE, send_buffer->allocated);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, object_curl_operation_send_from_file_callback);
			curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} else if (strcmp(method, OSS_HTTP_GET) == 0) {
			
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_2nd_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);

		} else if (strcmp(method, OSS_HTTP_HEAD) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);	
		} else if (strcmp(method, OSS_HTTP_DELETE) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);	
		} else if (strcmp(method, OSS_HTTP_POST) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer->ptr);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		}
		
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

static void
object_curl_operation_2nd(const char *method,
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


	param_buffer_t *send_buffer = params->send_buffer;
	param_buffer_t *recv_buffer = params->recv_buffer;
	param_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);

		if (strcmp(method, OSS_HTTP_PUT) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE, send_buffer->allocated);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, object_curl_operation_send_from_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} if (strcmp(method, OSS_HTTP_GET) == 0) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_file_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		}
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

static void
object_curl_operation_3rd(const char *method,
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


	// param_buffer_t *send_buffer = params->send_buffer;
	param_buffer_t *recv_buffer = params->recv_buffer;
	param_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);

		if (strcmp(method, OSS_HTTP_PUT) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} if (strcmp(method, OSS_HTTP_GET) == 0) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_file_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		}
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

/* *
 * 初始化post_object_group_request Object
 * */
const char *
client_post_object_group(oss_client_t *client,
		oss_post_object_group_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 2 * 1024;
	user_data->recv_buffer->allocated = 2 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};

	char part[256] = {0};
	char response[1024] = {0};

	unsigned int sign_len = 0;
	int parts = 0;
	unsigned int i = 0;

	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s?group", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s?group", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_POST,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	oss_object_group_item_t **part_item = request->get_items(request, &parts);
	tstring_t *tstr_part_item = 
		tstring_new("<CreateFileGroup>");
	for (; i < parts; i++) {
		sprintf(part, "<Part><PartNumber>%d</PartNumber><PartName>%s</PartName><ETag>%s</ETag></Part>",
				(*(part_item + i))->get_part_number(*(part_item + i)),
				(*(part_item + i))->get_part_name(*(part_item + i)),
				(*(part_item + i))->get_etag(*(part_item + i)));
		tstring_append(tstr_part_item, part);
	}
	tstring_append(tstr_part_item, "</CreateFileGroup>\n");

	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->send_buffer->ptr = tstring_data(tstr_part_item);
	user_data->send_buffer->left = tstring_size(tstr_part_item);
	user_data->send_buffer->allocated = tstring_size(tstr_part_item);

	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_POST, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	return NULL;
}

/* **************************************************************************** */
oss_object_metadata_t *
client_get_object_group_to_file(oss_client_t *client,
		oss_get_object_group_request_t *request,
		FILE *file,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(request != NULL);
	assert(file != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	// user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 1 * 1024);
	user_data->recv_buffer->fp = file;
	user_data->recv_buffer->left = 1 * 1024;
	user_data->recv_buffer->allocated = 1 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t sign_len = 0;

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
	
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};
	char header_range[32] = {0};
	
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;
	
	if (request->get_modified_since_constraint(request) != NULL) {
		sprintf(header_if_modified_since, "If-Modified-Since: %s", request->get_modified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_modified_since);
	}
	if (request->get_unmodified_since_constraint(request) != NULL) {
		sprintf(header_if_unmodified_since, "If-Unmodified-Since: %s", request->get_unmodified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
	}
	request->get_range(request, &start, &length);
	if (start > 0 && length > 0) {
		sprintf(header_range, "Range: %ld-%ld", start, start + length);
		http_headers = curl_slist_append(http_headers, header_range);
	}

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation_2nd(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	
	return NULL;
}

/* 
 * client_get_object_group_to_buffer 不建议使用该函数。
 * */
oss_object_metadata_t *
client_get_object_group_to_buffer(oss_client_t *client,
		oss_get_object_group_request_t *request,
		void *output,
		size_t *output_len,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(request != NULL);
	assert(output != NULL);
	assert(output_len > 0);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = NULL;
	user_data->recv_buffer->left = 0;
	user_data->recv_buffer->allocated = 0;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t key_len = strlen(request->get_key(request));
	size_t sign_len = 0;
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/
	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 64));

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};
	char header_range[32] = {0};	

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;
	
	if (request->get_modified_since_constraint(request) != NULL) {
		sprintf(header_if_modified_since, "If-Modified-Since: %s", request->get_modified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_modified_since);
	}
	if (request->get_unmodified_since_constraint(request) != NULL) {
		sprintf(header_if_unmodified_since, "If-Unmodified-Since: %s", request->get_unmodified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
	}
	request->get_range(request, &start, &length);
	if (start > 0 && length > 0) {
		sprintf(header_range, "Range: %ld-%ld", start, start + length);
		http_headers = curl_slist_append(http_headers, header_range);
	}

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	
	/** 注意，output_len参数既指明了output的长度，又指明了返回文件的大小，
	 * 如果缓冲区应设置合理的大小
	 * */
	*output_len = user_data->recv_buffer->allocated - user_data->recv_buffer->left;

	return NULL;
}

/* 
 * client_get_object_group_to_buffer 不建议使用该函数。
 * */
oss_object_metadata_t *
client_get_object_group_to_buffer_2nd(oss_client_t *client,
		oss_get_object_group_request_t *request,
		void **output,
		size_t *output_len,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(request != NULL);
	//assert(output != NULL);
	//assert(output_len > 0);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = NULL;
	user_data->recv_buffer->left = 0;
	user_data->recv_buffer->allocated = 0;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t key_len = strlen(request->get_key(request));
	size_t sign_len = 0;
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/
	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 64));

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};
	char header_range[32] = {0};	

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;
	
	if (request->get_modified_since_constraint(request) != NULL) {
		sprintf(header_if_modified_since, "If-Modified-Since: %s", request->get_modified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_modified_since);
	}
	if (request->get_unmodified_since_constraint(request) != NULL) {
		sprintf(header_if_unmodified_since, "If-Unmodified-Since: %s", request->get_unmodified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
	}
	request->get_range(request, &start, &length);
	if (start > 0 && length > 0) {
		sprintf(header_range, "Range: %ld-%ld", start, start + length);
		http_headers = curl_slist_append(http_headers, header_range);
	}

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	
	/** 注意，output_len参数既指明了output的长度，又指明了返回文件的大小，
	 * 如果缓冲区应设置合理的大小
	 * */
	*output = user_data->recv_buffer->ptr;
	*output_len = user_data->recv_buffer->allocated;
	//*output_len = user_data->recv_buffer->allocated - user_data->recv_buffer->left;

	return NULL;
}





oss_object_metadata_t *
client_get_object_group_metadata(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 16 * 1024);
	user_data->recv_buffer->left = 16 * 1024;
	user_data->recv_buffer->allocated = 16 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	size_t sign_len = 0;

	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (key_len + bucket_name_len + 64));

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	//char header_if_modified_since[64] = {0};
	//char header_if_unmodified_since[64] = {0};
	
	


	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_HEAD,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;
#if 0
	if (request->get_modified_since_constraint(request) != NULL) {
		sprintf(header_if_modified_since, "If-Modified-Since: %s", request->get_modified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_modified_since);
	}
	if (request->get_unmodified_since_constraint(request) != NULL) {
		sprintf(header_if_unmodified_since, "If-Unmodified-Since: %s", request->get_unmodified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
	}
#endif
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_HEAD, resource, url, http_headers, user_data);


	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	printf("%u\n", user_data->header_buffer->code);
	printf("%s\n", user_data->recv_buffer->ptr);
	return NULL;
}

void
client_delete_group_object(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 16 * 1024);
	user_data->recv_buffer->left = 16 * 1024;
	user_data->recv_buffer->allocated = 16 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	size_t sign_len = 0;

	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (key_len + bucket_name_len + 64));

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_DELETE,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_DELETE, resource, url, http_headers, user_data);


	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	*retcode = user_data->header_buffer->code;
	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
}

long oss_get_file_size( FILE *fp )
{
    long int save_pos;
    long size_of_file;
    /* Save the current position. */
    save_pos = ftell(fp);
    /* Jump to the end of the file. */
    fseek(fp, 0L, SEEK_END);
    /* Get the end position. */
    size_of_file = ftell(fp);
    /* Jump back to the original position. */
    fseek(fp, save_pos, SEEK_SET);
    return size_of_file;
}



int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	unsigned short retcode = 0;
	const char *bucket_name = "bucketname001";
	const char *key = "glib-2.32.4.tar.xz";

}
