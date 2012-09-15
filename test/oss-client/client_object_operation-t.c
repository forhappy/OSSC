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
#include <ossc/util/oss_common.h>
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

size_t object_curl_operation_header_callback_2nd(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	char etag[48] = {0};
	char type[64] = {0};
	char length[16] = {0};
	char week[5] = {0};
	char day[3] = {0};
	char mon[4] = {0};
	char year[5] = {0};
	char time[9] = {0};
	char gmt[4] = {0};
	int rcode = 0;
	int retag = 0;
	int rtype = 0;
	int rlength = 0;
	int rlastmodified = 0;
	size_t code = 0;

	rcode = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (rcode != 0) {
		header_buffer->code= code;
	}

	retag = sscanf(ptr, "ETag: %s", etag);
	if (retag != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		retag = sprintf(header_buffer->ptr + offset, "Content-Md5#%s#", etag);
		header_buffer->left -= retag;
	}
	
	rtype = sscanf(ptr, "Content-Type: %s", type);
	if (rtype != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rtype = sprintf(header_buffer->ptr + offset, "Content-Type#%s#", type);
		header_buffer->left -= rtype;
	}
	
	rlength = sscanf(ptr, "Content-Length: %s", length);
	if (rlength != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rlength = sprintf(header_buffer->ptr + offset, "Content-Length#%s#", length);
		header_buffer->left -= rlength;
	}

	rlastmodified = sscanf(ptr, "Last-Modified: %s %s %s %s %s %s", week, day, mon, year, time, gmt);
	if (rlastmodified != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rlastmodified = sprintf(header_buffer->ptr + offset, "Last-Modified#%s %s %s %s %s %s#", week,
				day, mon, year, time, gmt);
		header_buffer->left -= rlastmodified;
	}
	return size * nmemb;
}

static oss_put_object_result_t *
construct_get_object_metadata_response_on_success(const char *headers)
{
	oss_object_metadata_t *metadata = object_metadata_initialize();
	char tmpbuf[64] = {0};
	char *iter = NULL;
	iter = strtok(headers, "#");
	int flag = 0;
	while (iter != NULL) {
		if (flag % 2 == 0) {
			memset(tmpbuf, 0, 64);
			strncpy(tmpbuf, iter, 64);
		} else {
			metadata->add_default_metadata(metadata, tmpbuf, iter);
		}
		iter = strtok(NULL, "#");
		flag++;
	}
	return metadata;
}

static oss_put_object_result_t *
construct_put_object_response_on_success(const char *etag)
{
	oss_put_object_result_t *result = put_object_result_initialize();
	result->set_etag(result, etag);
	return result;
}

static oss_copy_object_result_t *
construct_copy_object_response_on_success(const char *strxml)
{
	oss_copy_object_result_t *result = copy_object_result_initialize();

	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "LastModified");
	result->set_last_modified(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "ETag");
	result->set_etag(result, *(tmp->child->attrib));

	return result;
}

static const char*
construct_copy_object_response_on_error(const char *strxml)
{

	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "Code");
	
	return *(tmp->child->attrib);
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
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback_2nd);
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

oss_put_object_result_t *
client_put_object_from_file(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		void *input, /**< 文件指针*/
		oss_object_metadata_t *metadata,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(input != NULL);
	assert(metadata != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	// user_data->send_buffer->ptr = input; /**< 从缓冲区读取数据  */
	user_data->send_buffer->fp = (FILE *)input; /**< 从文件中读取数据  */
	user_data->send_buffer->left = metadata->get_content_length(metadata);
	user_data->send_buffer->allocated = metadata->get_content_length(metadata);


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
	char header_content_type[64] = {0};

	char header_cache_control[48] = {0};
	char header_expires[64] = {0};
	char header_content_encoding[64] = {0};
	char header_content_disposition[256] = {0};

	unsigned int sign_len = 0;

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

	/* Content-Type is a must */
	sprintf(header_content_type, "Content-Type: %s", metadata->get_content_type(metadata));

	
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, metadata->get_content_type(metadata));

	oss_map_t *user_headers = metadata->get_user_metadata(metadata);
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
	
	if (metadata->get_cache_control(metadata) != NULL) {
		sprintf(header_cache_control, "Cache-Control: %s", metadata->get_cache_control(metadata));
		http_headers = curl_slist_append(http_headers, header_cache_control);
	}
	if (metadata->get_expiration_time(metadata) != NULL) {
		sprintf(header_expires, "Expires: %s", metadata->get_expiration_time(metadata));
		http_headers = curl_slist_append(http_headers, header_expires);
	}
	if (metadata->get_content_encoding(metadata) != NULL) {
		sprintf(header_content_encoding, "Content-Encoding: %s", metadata->get_content_encoding(metadata));
		http_headers = curl_slist_append(http_headers, header_content_encoding);
	}
	if (metadata->get_content_disposition(metadata) != NULL) {
		sprintf(header_content_disposition, "Content-Disposition: %s", metadata->get_content_disposition(metadata));
		http_headers = curl_slist_append(http_headers, header_content_disposition);
	}

	http_headers = curl_slist_append(http_headers, header_content_type);
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_PUT, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	if (user_data->header_buffer->code == 200) {
		return construct_put_object_response_on_success(
				user_data->header_buffer->ptr);
	}
	return NULL;
}

oss_put_object_result_t *
client_put_object_from_buffer(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		void *input, /**< 缓冲区首地址 */
		oss_object_metadata_t *metadata,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(input != NULL);
	assert(metadata != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->send_buffer->ptr = (char *)input; /**< 从缓冲区读取数据  */
	// user_data->send_buffer->fp = (FILE *)input; /**< 从文件中读取数据  */
	user_data->send_buffer->left = metadata->get_content_length(metadata);
	user_data->send_buffer->allocated = metadata->get_content_length(metadata);


	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 1 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 1 * 1024;
	user_data->recv_buffer->allocated = 1 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

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
	char header_content_type[64] = {0};

	char header_cache_control[48] = {0};
	char header_expires[64] = {0};
	char header_content_encoding[64] = {0};
	char header_content_disposition[256] = {0};

	unsigned int sign_len = 0;

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

	/* Content-Type is a must */
	sprintf(header_content_type, "Content-Type: %s", metadata->get_content_type(metadata));

	
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, metadata->get_content_type(metadata));

	oss_map_t *user_headers = metadata->get_user_metadata(metadata);
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
	
	if (metadata->get_cache_control(metadata) != NULL) {
		sprintf(header_cache_control, "Cache-Control: %s", metadata->get_cache_control(metadata));
		http_headers = curl_slist_append(http_headers, header_cache_control);
	}
	if (metadata->get_expiration_time(metadata) != NULL) {
		sprintf(header_expires, "Expires: %s", metadata->get_expiration_time(metadata));
		http_headers = curl_slist_append(http_headers, header_expires);
	}
	if (metadata->get_content_encoding(metadata) != NULL) {
		sprintf(header_content_encoding, "Content-Encoding: %s", metadata->get_content_encoding(metadata));
		http_headers = curl_slist_append(http_headers, header_content_encoding);
	}
	if (metadata->get_content_disposition(metadata) != NULL) {
		sprintf(header_content_disposition, "Content-Disposition: %s", metadata->get_content_disposition(metadata));
		http_headers = curl_slist_append(http_headers, header_content_disposition);
	}

	http_headers = curl_slist_append(http_headers, header_content_type);
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation_2nd(OSS_HTTP_PUT, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	
	if (user_data->header_buffer->code == 200) {
		return construct_put_object_response_on_success(
				user_data->header_buffer->ptr);
	}
	return NULL;
}
/* **************************************************************************** */
oss_object_metadata_t *
client_get_object_to_file(oss_client_t *client,
		oss_get_object_request_t *request,
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
 * client_get_object_to_buffer 不建议使用该函数。
 * */
oss_object_metadata_t *
client_get_object_to_buffer(oss_client_t *client,
		oss_get_object_request_t *request,
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
 * client_get_object_to_buffer 不建议使用该函数。
 * */
oss_object_metadata_t *
client_get_object_to_buffer_2nd(oss_client_t *client,
		oss_get_object_request_t *request,
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

/* **************************************************************************** */
oss_copy_object_result_t *
client_copy_object_ext(oss_client_t *client,
		const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(source_bucket_name != NULL);
	assert(source_key != NULL);
	assert(destination_bucket_name != NULL);
	assert(destination_key != NULL);
	
	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/
	user_data->header_buffer = NULL; /** Header 缓冲区设置为NULL*/


	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->recv_buffer->left = 4 * 1024;
	user_data->recv_buffer->allocated = 4 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;


	size_t destination_bucket_name_len = strlen(destination_bucket_name);
	size_t destination_key_len = strlen(destination_key);
	size_t source_bucket_name_len = strlen(source_bucket_name);
	size_t source_key_len = strlen(source_key);

	size_t sign_len = 0;

	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) *
			(destination_bucket_name_len + destination_key_len + 16));

	/** 
	 * Copy source: "/" + source_bucket_name + "/" + source_key
	 */
	char *copy_source = (char *)malloc(sizeof(char) *
			(source_bucket_name_len + source_key_len + 16));

	memset(copy_source, 0, source_bucket_name_len + source_key_len + 16);
	sprintf(copy_source, "/%s/%s", source_bucket_name, source_key);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) *
			(destination_bucket_name_len + destination_key_len + 64));

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	size_t copy_source_len = strlen(copy_source);
	char *header_copy_source = 
		(char *)malloc(sizeof(char) * (copy_source_len + 64));
	memset(header_copy_source, 0, (copy_source_len + 64));

	//char header_if_modified_since[64] = {0};
	//char header_if_unmodified_since[64] = {0};
	
	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", destination_bucket_name, destination_key);
	sprintf(url, "%s/%s/%s", client->endpoint, destination_bucket_name, destination_key);
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);
	sprintf(header_copy_source, "%s: %s", OSS_COPY_SOURCE, copy_source);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_COPY_SOURCE, copy_source);
	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, user_headers, resource, &sign_len);

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
		sprintf(header_if_unmodified_since, "Expires: %s", request->get_unmodified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
	}
#endif
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	http_headers = curl_slist_append(http_headers, header_copy_source);

	/**
	 * 发送请求
	 */
	object_curl_operation_3rd(OSS_HTTP_PUT, resource, url, http_headers, user_data);

	if (user_data->header_buffer->code == 200) {
		return construct_copy_object_response_on_success(
				user_data->recv_buffer->ptr);
	}// else return construct_copy_object_response_on_error(user_data->recv_buffer->ptr);
	return NULL;
}


oss_copy_object_result_t *
client_copy_object(oss_client_t *client,
		oss_copy_object_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(request != NULL);
	
	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/


	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->recv_buffer->left = 4 * 1024;
	user_data->recv_buffer->allocated = 4 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;


	size_t destination_bucket_name_len = strlen(request->get_destination_bucket_name(request));
	size_t destination_key_len = strlen(request->get_destination_key(request));
	size_t source_bucket_name_len = strlen(request->get_source_bucket_name(request));
	size_t source_key_len = strlen(request->get_source_key(request));

	size_t sign_len = 0;

	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) *
			(destination_bucket_name_len + destination_key_len + 16));

	/** 
	 * Copy source: "/" + source_bucket_name + "/" + source_key
	 */
	char *copy_source = (char *)malloc(sizeof(char) *
			(source_bucket_name_len + source_key_len + 16));

	memset(copy_source, 0, source_bucket_name_len + source_key_len + 16);
	sprintf(copy_source, "/%s/%s", request->get_source_bucket_name(request),
		request->get_source_key(request));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) *
			(destination_bucket_name_len + destination_key_len + 64));

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	size_t copy_source_len = strlen(copy_source);
	char *header_copy_source = 
		(char *)malloc(sizeof(char) * (copy_source_len + 64));
	memset(header_copy_source, 0, (copy_source_len + 64));

	char header_if_modified_since[128] = {0};
	char header_if_unmodified_since[128] = {0};
	
	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_destination_bucket_name(request),
		request->get_destination_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, 
		request->get_destination_bucket_name(request),
		request->get_destination_key(request));
	sprintf(now, "%s", oss_get_gmt_time());

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;

	if (request->get_modified_since_constraints(request) != NULL) {
		sprintf(header_if_modified_since, "x-oss-copy-source-if-modified-since: %s",
			request->get_modified_since_constraints(request));
		http_headers = curl_slist_append(http_headers, header_if_modified_since);
		oss_map_put(user_headers, "x-oss-copy-source-if-modified-since",
			request->get_modified_since_constraints(request));
	}
	if (request->get_unmodified_since_constraints(request) != NULL) {
		sprintf(header_if_unmodified_since, "x-oss-copy-source-if-unmodified-since: %s",
			request->get_unmodified_since_constraints(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
		oss_map_put(user_headers, "x-oss-copy-source-if-unmodified-since",
			request->get_unmodified_since_constraints(request));
	}
	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);
	sprintf(header_copy_source, "%s: %s", OSS_COPY_SOURCE, copy_source);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_COPY_SOURCE, copy_source);
	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, user_headers, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	http_headers = curl_slist_append(http_headers, header_copy_source);

	/**
	 * 发送请求
	 */
	object_curl_operation_3rd(OSS_HTTP_PUT, resource, url, http_headers, user_data);

	if (user_data->header_buffer->code == 200) {
		return construct_copy_object_response_on_success(
				user_data->recv_buffer->ptr);
	}// else return construct_copy_object_response_on_error(user_data->recv_buffer->ptr);
	return NULL;
}


oss_object_metadata_t *
client_get_object_metadata(oss_client_t *client,
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

	if (user_data->header_buffer->code == 200) {
		return construct_get_object_metadata_response_on_success(
				user_data->header_buffer->ptr);
	}
	return NULL;
}

void
client_delete_object(oss_client_t *client,
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


/* *********************************************************************************** */

oss_delete_multiple_object_request_t*
client_delete_multiple_object(oss_client_t *client,
		oss_delete_multiple_object_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(request != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));
	

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 16 * 1024);
	user_data->recv_buffer->left = 16 * 1024;
	user_data->recv_buffer->allocated = 16 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(request->get_bucket_name(request));

	/** 
	 * Resource: "/" + bucket_name + "?" + "delete"
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + 64));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};

	char header_md5[128]  = {0};
	char delete_key[1024] = {0};

	const char *content_md5 = NULL;

	unsigned int sign_len = 0;
	int keynums = 0;
	unsigned int i = 0;


	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/?delete", request->get_bucket_name(request));
	sprintf(url, "%s/%s/?delete", client->endpoint, request->get_bucket_name(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	const char **p = request->get_keys(request, &keynums);

	tstring_t *key_list = tstring_new("<Delete>");
	if (request->get_mode(request) == true) {
		sprintf(delete_key, "<Quiet>true</Quiet>");
	} else 
		sprintf(delete_key, "<Quiet>false</Quiet>");
	tstring_append(key_list, delete_key);

	for (i = 0; i < keynums; i++) {
		sprintf(delete_key, "<Object><Key>%s</Key></Object>", *(p + i));
		tstring_append(key_list, delete_key);
	}
	tstring_append(key_list, "</Delete>\n");

	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->send_buffer->ptr = tstring_data(key_list);
	// user_data->send_buffer->fp = (FILE *)input; /**< 从文件中读取数据  */
	user_data->send_buffer->left = tstring_size(key_list);
	user_data->send_buffer->allocated = tstring_size(key_list);


		printf("delete_key_list:\n%s\nkey_list_length: %d\n",
			tstring_data(key_list), tstring_size(key_list));



	content_md5 = oss_compute_md5_digest(tstring_data(key_list), tstring_size(key_list));
	sprintf(header_md5, "Content-MD5: %s", content_md5);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	oss_map_put(default_headers, OSS_CONTENT_MD5, content_md5);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_POST,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_md5);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_POST, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	printf("%u\n", user_data->header_buffer->code);
	printf("%s\n", user_data->recv_buffer->ptr);
}

const char *keys[] = {
	"putxxx-test.pdf",
	"ccan.tar.bz2",
	"ncurseass-5.9.tar.gz",
	"nmon_linsdfux_14g.tar.gz"
};



int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	unsigned short retcode = 0;
	const char *bucket_name = "bucketname001";
	const char *key = "glib-2.32.4.tar.xz";
	//FILE *file = fopen("proactor.pdf", "r");
	FILE *local_file = fopen(key, "w");
	size_t file_len = 0;//2 * 1024 * 1024;//oss_get_file_size(file);
	void *buffer = NULL;

	//char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	//memset(buffer, '\0', file_len + 1);
	//fread(buffer, file_len, 1, file);

	oss_client_t *client = client_initialize(access_id, access_key);

	//oss_object_metadata_t *metadata = object_metadata_initialize();

	//metadata->set_content_length(metadata, file_len);
	//metadata->set_content_type(metadata, "application/octet-stream");
	//metadata->set_cache_control(metadata, "no-cache");
	//metadata->set_content_encoding(metadata, "utf-8");
	//metadata->set_content_disposition(metadata, "attachment;");
	//metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");

	//oss_put_object_result_t *result01 = client_put_object_from_file(client, bucket_name, key,
	//		file, metadata, NULL);

	///oss_put_object_result_t *result02 = client_put_object_from_buffer(client, bucket_name, key,
	//		buffer, metadata, NULL);
	//printf("ETag:\n%s\n", result02->get_etag(result02));
	//put_object_result_finalize(result02);

	//oss_client_t *client = client_initialize(access_id, access_key);
	//oss_get_object_request_t *request = get_object_request_initialize(bucket_name, key);
	//client_get_object_to_file(client, request, local_file, NULL);
	//client_get_object_to_buffer_2nd(client, request, &buffer, &file_len, NULL);

	//fwrite(buffer, file_len, 1, local_file);
	//printf("length: %d\n", file_len);
	//fclose(file);
	//fclose(local_file);

	//const char *source_bucket_name = "bucketname001";
	//const char *destination_bucket_name = "bucketname002";
	//const char *source_key = "put.png";
	//const char *destination_key = "PUT.png";
	//oss_client_t *client = client_initialize(access_id, access_key);

	//oss_copy_object_result_t *result = client_copy_object_ext(client, source_bucket_name, source_key,
	//		destination_bucket_name, destination_key, NULL);
	//oss_copy_object_request_t *request = copy_object_request_initialize(source_bucket_name, source_key,
	//	destination_bucket_name, destination_key);
	//request->set_modified_since_constraints(request, "Wed, 12 Sep 2012 12:06:55 GMT");
	//oss_copy_object_result_t *result = client_copy_object(client, request, NULL);
	//if (result != NULL) {
	//	printf("ETag: %s", result->get_etag(result));
	//	printf("LastModified: %s\n", result->get_last_modified(result));
	//}
	oss_object_metadata_t *metadata = client_get_object_metadata(client, bucket_name, key, NULL);
	printf("ETag: %s\n", metadata->get_etag(metadata));
	printf("Content-Type: %s\n", metadata->get_content_type(metadata));
	printf("Content-Length: %d\n", metadata->get_content_length(metadata));

	//client_delete_object(client, source_bucket_name, source_key, &retcode);
	//printf("%d\n", retcode);
	//	oss_delete_multiple_object_request_t *request = 
	//	delete_multiple_object_request_initialize(source_bucket_name, keys, 4, false);

	//client_delete_multiple_object(client, request, &retcode);
}
