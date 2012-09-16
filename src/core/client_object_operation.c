/*
 * =============================================================================
 *
 *       Filename:  client_object_operation.c
 *
 *    Description:  client object operation routines.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ossc/client.h>
#include <ossc/oss_helper.h>
#include <ossc/oss_curl_callback.h>
#include <ossc/util/oss_auth.h>
#include <ossc/util/oss_common.h>
#include <ossc/util/oss_ttxml.h>
#include <ossc/util/oss_time.h>

#define _OSS_CLIENT_H
#include <ossc/modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>

static oss_object_metadata_t *
construct_get_object_to_buffer_response(
		curl_request_param_t *user_data)
{
	char *headers = user_data->header_buffer->ptr;
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
	oss_free_partial_user_data(user_data);
	return metadata;
}

static oss_object_metadata_t *
construct_get_object_metadata_response(
		curl_request_param_t *user_data)
{
	char *headers = user_data->header_buffer->ptr;
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
	oss_free_user_data(user_data);
	return metadata;
}

static oss_put_object_result_t *
construct_put_object_response(
		curl_request_param_t *user_data)
{
	const char *etag = user_data->header_buffer->ptr;
	oss_put_object_result_t *result = put_object_result_initialize();
	result->set_etag(result, etag);
	oss_free_user_data(user_data);
	return result;
}

static oss_copy_object_result_t *
construct_copy_object_response(curl_request_param_t *user_data)
{
	const char *strxml = user_data->recv_buffer->ptr;
	oss_copy_object_result_t *result = copy_object_result_initialize();

	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "LastModified");
	if (tmp != NULL && tmp->child != NULL) {
		result->set_last_modified(result, *(tmp->child->attrib));
	} else {
		result->set_last_modified(result, "");
	}
	tmp = xml_find(xml, "ETag");
	if (tmp != NULL && tmp->child != NULL) {
		result->set_etag(result, *(tmp->child->attrib));
	} else {
		result->set_etag(result, "");
	}
	
	xml_free(xml);
	oss_free_user_data(user_data);

	return result;
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
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_curl_operation_header_callback_2nd);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);	
		}
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
	user_data->send_buffer->fp = (FILE *)input; /**< 从文件中读取数据  */
	user_data->send_buffer->left = metadata->get_content_length(metadata);
	user_data->send_buffer->allocated = metadata->get_content_length(metadata);

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 16);
	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + key_len + strlen(client->endpoint) + 8 ));

	char *now;  /**< Fri, 24 Feb 2012 02:58:28 GMT */

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char header_content_type[64] = {0}; /**< 64字节足够容纳MIME标准类型 */
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
	now = (char *)oss_get_gmt_time();

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	/* Content-Type is a must */
	sprintf(header_content_type, "Content-Type: %s",
			metadata->get_content_type(metadata));
	
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, metadata->get_content_type(metadata));

	oss_map_t *user_headers = metadata->get_user_metadata(metadata);
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_PUT,
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
	 * 释放资源
	 */
	oss_map_delete(default_headers);
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	curl_slist_free_all(http_headers);
	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_put_object_response(user_data);
	} else {
		if (retcode != NULL) 
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
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
	user_data->send_buffer->left = metadata->get_content_length(metadata);
	user_data->send_buffer->allocated = metadata->get_content_length(metadata);

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	char *resource = (char *)malloc(sizeof(char) * 
			(bucket_name_len + key_len + 16));
	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + key_len + strlen(client->endpoint) + 8));

	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_host[258]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char header_content_type[64] = {0};
	char header_cache_control[48] = {0};
	char header_expires[64] = {0};
	char header_content_encoding[64] = {0};
	char header_content_disposition[256] = {0};

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);
	now = (char *)oss_get_gmt_time();

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);

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
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_PUT,
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
	
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_put_object_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
	return NULL;
}

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
	user_data->recv_buffer->fp = file;
	user_data->recv_buffer->left = 1 * 1024;
	user_data->recv_buffer->allocated = 1 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);
	
	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t key_len = strlen(request->get_key(request));
	size_t sign_len = 0;

	/** 
	 * Resource: "/" + bucket_name
	 */
	char *resource = (char *)malloc(sizeof(char) *
			(bucket_name_len + key_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + key_len + strlen(client->endpoint) + 64));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};
	char header_range[32] = {0};
	
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/

	oss_map_t *default_headers = oss_map_new(16);

	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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

	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_metadata_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}

	return NULL;
}

oss_object_metadata_t *
client_get_object_to_buffer(oss_client_t *client,
		oss_get_object_request_t *request,
		void **output,
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
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 0;
	user_data->recv_buffer->allocated = 0;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);
	
	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t key_len = strlen(request->get_key(request));
	size_t sign_len = 0;
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + key_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};
	char header_range[32] = {0};	

	oss_map_t *default_headers = oss_map_new(16);
	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);
	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	/** 注意，output_len参数既指明了output的长度，又指明了返回文件的大小，
	 * 如果缓冲区应设置合理的大小
	 * */
	*output = user_data->recv_buffer->ptr;
	*output_len = user_data->recv_buffer->allocated - user_data->recv_buffer->left;

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_to_buffer_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}

	return NULL;
}

/** 
 * Get object to buffer 第二版， 建议使用该函数。
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

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = NULL; /** 发送缓冲区设置为NULL*/

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = NULL;
	user_data->recv_buffer->left = 0;
	user_data->recv_buffer->allocated = 0;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t key_len = strlen(request->get_key(request));
	size_t sign_len = 0;
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/
	/** 
	 * Resource: "/" + bucket_name + "/" + key
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + key_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};
	char header_range[32] = {0};	

	oss_map_t *default_headers = oss_map_new(16);

	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	
	/** 注意，output_len参数既指明了output的长度，又指明了返回文件的大小，
	 * 如果缓冲区应设置合理的大小
	 * */
	*output = user_data->recv_buffer->ptr;
	*output_len = user_data->recv_buffer->allocated;

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_to_buffer_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
	return NULL;
}

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

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);


	size_t destination_bucket_name_len = strlen(destination_bucket_name);
	size_t destination_key_len = strlen(destination_key);
	size_t source_bucket_name_len = strlen(source_bucket_name);
	size_t source_key_len = strlen(source_key);
	size_t sign_len = 0;

	char *resource = (char *)malloc(sizeof(char) *
			(destination_bucket_name_len + destination_key_len + 16));
	char *copy_source = (char *)malloc(sizeof(char) *
			(source_bucket_name_len + source_key_len + 16));

	memset(copy_source, 0, source_bucket_name_len + source_key_len + 16);
	sprintf(copy_source, "/%s/%s", source_bucket_name, source_key);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) *
			(destination_bucket_name_len + destination_key_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	size_t copy_source_len = strlen(copy_source);
	char *header_copy_source = 
		(char *)malloc(sizeof(char) * (copy_source_len + 64));
	memset(header_copy_source, 0, (copy_source_len + 64));
	
	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", destination_bucket_name, destination_key);
	sprintf(url, "%s/%s/%s", client->endpoint, destination_bucket_name, destination_key);

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);
	sprintf(header_copy_source, "%s: %s", OSS_COPY_SOURCE, copy_source);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_COPY_SOURCE, copy_source);
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, user_headers, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	http_headers = curl_slist_append(http_headers, header_copy_source);

	/**
	 * 发送请求
	 */
	object_curl_operation_3rd(OSS_HTTP_PUT, resource, url, http_headers, user_data);
	curl_slist_free_all(http_headers);

	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	oss_map_delete(default_headers);
	oss_map_delete(user_headers);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_copy_object_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
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
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t destination_bucket_name_len = strlen(request->get_destination_bucket_name(request));
	size_t destination_key_len = strlen(request->get_destination_key(request));
	size_t source_bucket_name_len = strlen(request->get_source_bucket_name(request));
	size_t source_key_len = strlen(request->get_source_key(request));

	size_t sign_len = 0;
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
			(destination_bucket_name_len + destination_key_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	
	size_t copy_source_len = strlen(copy_source);
	char *header_copy_source = 
		(char *)malloc(sizeof(char) * (copy_source_len + 64));
	memset(header_copy_source, 0, (copy_source_len + 64));

	char header_if_modified_since[128] = {0};
	char header_if_unmodified_since[128] = {0};
	
	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", request->get_destination_bucket_name(request),
		request->get_destination_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, 
		request->get_destination_bucket_name(request),
		request->get_destination_key(request));

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
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_PUT,
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

	curl_slist_free_all(http_headers);

	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}

	oss_map_delete(default_headers);
	oss_map_delete(user_headers);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_copy_object_response(user_data);
	} else  {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
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
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	size_t sign_len = 0;
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));
	char *url = (char *)malloc(sizeof(char) *
			(key_len + bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};

	oss_map_t *default_headers = oss_map_new(16);
	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_HEAD,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，
	 * TODO：后续版本应支持If-Matching,If-None-Matching
	 */
	struct curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_curl_operation(OSS_HTTP_HEAD, resource, url, http_headers, user_data);


	/**
	 * 释放资源
	 */
	curl_slist_free_all(http_headers);
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	oss_map_delete(default_headers);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_metadata_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
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
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	size_t sign_len = 0;
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));
	char *url = (char *)malloc(sizeof(char) *
			(key_len + bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};

	oss_map_t *default_headers = oss_map_new(16);

	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);

	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_DELETE,
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

	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		oss_free_user_data(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
}

oss_delete_multiple_object_request_t*
client_delete_multiple_object(oss_client_t *client,
		oss_delete_multiple_object_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(request != NULL);

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));
	user_data->send_buffer = NULL;

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + 16));
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char header_md5[128]  = {0};
	char delete_key[1024] = {0};
	const char *content_md5 = NULL;
	unsigned int sign_len = 0;
	int keynums = 0;
	unsigned int i = 0;
	oss_map_t *default_headers = oss_map_new(16);

	now = (char *)oss_get_gmt_time();
	sprintf(resource, "/%s/?delete", request->get_bucket_name(request));
	sprintf(url, "%s/%s/?delete", client->endpoint, request->get_bucket_name(request));
	sprintf(header_host,"Host: %s", client->endpoint);
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
	user_data->send_buffer->left = tstring_size(key_list);
	user_data->send_buffer->allocated = tstring_size(key_list);

	content_md5 = oss_compute_md5_digest(tstring_data(key_list), tstring_size(key_list));
	sprintf(header_md5, "Content-MD5: %s", content_md5);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	oss_map_put(default_headers, OSS_CONTENT_MD5, content_md5);
	
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_POST,
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
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	oss_map_delete(default_headers);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	}
}
