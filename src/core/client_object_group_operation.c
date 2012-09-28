/*
 * =============================================================================
 *
 *       Filename:  client_object_group_operation.c
 *
 *    Description:  client object group operation routines.
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
construct_get_object_object_to_buffer_response_on_success(
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
construct_get_object_metadata_response_on_success(
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

static void
object_group_curl_operation(const char *method,
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
		if (strcmp(method, OSS_HTTP_POST) == 0) {
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params->send_buffer->ptr);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_group_curl_operation_recv_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_group_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} else if (strcmp(method, OSS_HTTP_DELETE) == 0 || strcmp(method, OSS_HTTP_HEAD) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_group_curl_operation_recv_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_group_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);

		}
		else if (strcmp(method, OSS_HTTP_GET) == 0) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_curl_operation_recv_to_buffer_2nd_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_group_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);

		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

static void
object_group_curl_operation_2nd(const char *method,
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

	//param_buffer_t *send_buffer = params->send_buffer;
	param_buffer_t *recv_buffer = params->recv_buffer;
	param_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);

		if (strcmp(method, OSS_HTTP_GET) == 0) {
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

static oss_get_object_group_index_result_t *
construct_get_object_group_index_response(curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	int i;
	XmlNode *xml, *bucket_tag, *key_tag, *etag_tag, *length_tag, *file_part_tag, *part_tag, *part_tmp;
	unsigned int response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	oss_get_object_group_index_result_t *result = get_object_group_index_result_initialize();
	bucket_tag = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *bucket_tag->child->attrib);
	key_tag = xml_find(xml, "Key");
	result->set_key(result, *key_tag->child->attrib);
	etag_tag = xml_find(xml, "ETag");
	result->set_etag(result, *etag_tag->child->attrib);
	length_tag = xml_find(xml, "FileLength");
	unsigned int file_length = (unsigned int)atoi(*length_tag->child->attrib);
	result->set_file_length(result, file_length);
	
	file_part_tag = xml_find(xml, "FilePart");
	if(file_part_tag != NULL) {
		part_tag = file_part_tag->child;
	} else {
		part_tag = NULL;
	}
	if(part_tag != NULL) {
		part_tmp = part_tag;
		for(; part_tmp != NULL; part_tmp = part_tmp->next) {
			(result->part_number)++;
		}
		oss_multipart_object_group_t **group = (oss_multipart_object_group_t **)malloc(sizeof(oss_multipart_object_group_t *) * (result->part_number));
		for(i = 0; part_tag != NULL; i++, part_tag = part_tag->next) {
			group[i] = multipart_object_group_initialize();
			group[i]->set_etag(group[i], *part_tag->child->child->attrib);
			group[i]->set_part_name(group[i], *part_tag->child->next->child->attrib);
			int part_number = atoi(*part_tag->child->next->next->child->attrib);
			group[i]->set_part_number(group[i], part_number);
			unsigned int part_size = (unsigned int)(atoi(*part_tag->child->next->next->next->child->attrib));
			group[i]->set_part_size(group[i], part_size);
		}
		result->group = group;
	}
	
	oss_free_user_data(user_data);
 	xml_free(xml);

	return result;
	
}

static oss_post_object_group_result_t *
construct_post_object_group_response(curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	XmlNode *xml, *bucket_tag, *key_tag, *etag_tag, *size_tag;
	unsigned int response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	oss_post_object_group_result_t *result = post_object_group_result_initialize();
	bucket_tag = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *bucket_tag->child->attrib);
	key_tag = xml_find(xml, "Key");
	result->set_key(result, *key_tag->child->attrib);
 	etag_tag = xml_find(xml, "ETag");
	result->set_etag(result, *etag_tag->child->attrib);
	size_tag = xml_find(xml,"Size");
	unsigned int size = (unsigned int)atoi(*size_tag->child->attrib);
	result->set_size(result, size);
 
	oss_free_user_data(user_data);
 	xml_free(xml);

	return result;
	
}

static oss_object_metadata_t *
construct_head_object_group_response(curl_request_param_t *user_data)
{

	oss_object_metadata_t *result = object_metadata_initialize();
	const char *key, *value;
	int flag = 1;
	char *ptr = user_data->header_buffer->ptr;
	while(*ptr != '\0') {
		char *tmp = strchr(ptr, '#');
		*tmp = '\0';
		if(flag % 2) {
			key = ptr;
		} else {
			value = ptr;
			//if(!strcmp(key, "Content-Length")) {
			//	long length = atol(value);
			//	result->set_content_length(result, length);
			//}
			result->add_default_metadata(result, key, value);
		}
		ptr = tmp + 1;
		flag ++;
	}
	oss_free_user_data(user_data);
	return result;
	
}

oss_post_object_group_result_t *
client_post_object_group(oss_client_t *client,
		oss_post_object_group_request_t *request,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(request != NULL);

	curl_request_param_t *user_data = (curl_request_param_t *)malloc(sizeof(curl_request_param_t));
	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	//user_data->send_buffer->ptr = (char *)malloc(sizeof(char) * 128 * 1024);
	user_data->send_buffer->fp = NULL;


	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;

	unsigned int bucket_name_len = strlen(request->bucket_name);
	unsigned int key_len = strlen(request->key);
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + key_len + strlen(client->endpoint) + 32));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	char part[256] = {0};
	unsigned int sign_len = 0;
	int parts = 0;
	unsigned int i = 0;

	oss_map_t *default_headers = oss_map_new(16);
	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s/%s?group", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s?group", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(header_host, "Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_POST,
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
	unsigned int tmp_len = strlen(tstring_data(tstr_part_item));
	char *tmp = (char *)malloc(sizeof(char) * (tmp_len + 1));
	strncpy(tmp, tstring_data(tstr_part_item), tmp_len);
	tmp[tmp_len] = '\0';

	user_data->send_buffer->ptr = tmp;
	//user_data->send_buffer->ptr = (char *)(tstring_data(tstr_part_item));
	user_data->send_buffer->left = tstring_size(tstr_part_item);
	user_data->send_buffer->allocated = tstring_size(tstr_part_item);
	/**
	 * 自定义 HTTP 请求头部
	 */
	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_group_curl_operation(OSS_HTTP_POST, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	tstring_free(tstr_part_item);
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
	if (user_data->header_buffer->code != 200) {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_post_object_group_response(user_data);
	}
}

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
	user_data->recv_buffer->ptr = NULL;
	user_data->recv_buffer->fp = file;
	user_data->recv_buffer->left = 0;
	user_data->recv_buffer->allocated = 0;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;

	unsigned int bucket_name_len = strlen(request->get_bucket_name(request));
	unsigned int key_len = strlen(request->get_key(request));
	unsigned int sign_len = 0;
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
	now = (char *)oss_get_gmt_time();

	/** 构造请求头部 */
	sprintf(header_host, "Host: %s", client->endpoint);
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
	object_group_curl_operation_2nd(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);

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
	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_metadata_response_on_success(user_data);
	} else {
		if (retcode != NULL) {
			fflush(file);
			rewind(file);
			unsigned int file_len = oss_get_file_size(file);
			char *retbuf = (char *)malloc(sizeof(char) * (file_len + 1));
			memset(retbuf, 0, file_len + 1);
			unsigned int retlen = fread(retbuf, 1, file_len, file);
			if (retlen != file_len)
				fprintf(stderr, "file mode should be set for both read and write\n");
			*retcode = oss_get_retcode_from_response(retbuf);
			free(retbuf);
		}
		oss_free_user_data(user_data);
	}

	return NULL;
}

oss_object_metadata_t *
client_get_object_group_to_buffer(oss_client_t *client,
		oss_get_object_group_request_t *request,
		void **output,
		unsigned int *output_len,
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

	unsigned int bucket_name_len = strlen(request->get_bucket_name(request));
	unsigned int key_len = strlen(request->get_key(request));
	unsigned int sign_len = 0;
	long start = 0; /**< Range 起始字节位置*/
	long length = 0; /**< Range 长度*/
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 64));
	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
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
	now = (char *)oss_get_gmt_time();
	/** 构造请求头部 */
	sprintf(header_host, "Host: %s", client->endpoint);
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
	object_group_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	
	/** 注意，output_len参数既指明了output的长度，又指明了返回文件的大小，
	 * 如果缓冲区应设置合理的大小
	 * */
	*output = user_data->recv_buffer->ptr;
	*output_len = user_data->recv_buffer->allocated;

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
	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_object_to_buffer_response_on_success(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_partial_user_data(user_data);
	}
	return NULL;
}

oss_get_object_group_index_result_t *
client_get_object_group_index(
		oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);

	curl_request_param_t *user_data = (curl_request_param_t *)malloc(sizeof(curl_request_param_t));
	user_data->send_buffer = NULL;

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = NULL;
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 0;
	user_data->recv_buffer->allocated = 0;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 4 * 1024;
	user_data->header_buffer->allocated = 4 * 1024;

	unsigned int bucket_name_len = strlen(bucket_name);
	unsigned int key_len = strlen(key);
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16 );
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 64);
	char header_host[64]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	char header_group[128] = {0};
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);
	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);
	sprintf(header_host, "Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);
	sprintf(header_group, "%s:%s", OSS_OBJECT_GROUP, "I'm NULL."); 

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_OBJECT_GROUP, "I'm NULL.");
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, user_headers, resource, &sign_len);
	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部
	 */
	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	http_headers = curl_slist_append(http_headers, header_group);
	
	/**
	 * 发送请求
	 */
	object_group_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);
	oss_map_delete(user_headers);
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
	if (user_data->header_buffer->code != 200) {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_get_object_group_index_response(user_data);
	}
	
}

void 
client_delete_object_group(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);

	curl_request_param_t *user_data = (curl_request_param_t *)malloc(sizeof(curl_request_param_t));
	user_data->send_buffer = NULL;


	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 128 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 128 * 1024;
	user_data->recv_buffer->allocated = 128 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 4 * 1024;
	user_data->header_buffer->allocated = 4 * 1024;

	unsigned int bucket_name_len = strlen(bucket_name);
	unsigned int key_len = strlen(key);
	char *resource = (char *)malloc(
			sizeof(char) * (bucket_name_len + key_len) + 16);
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + key_len + strlen(client->endpoint) + 64));
	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);
	sprintf(header_host, "Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_DELETE,
			default_headers, NULL, resource, &sign_len);
	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);
	/**
	 * 自定义 HTTP 请求头部
	 */
	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	object_group_curl_operation(OSS_HTTP_DELETE, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);

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
	if (user_data->header_buffer->code != 204) {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
	} else {
		if (retcode != NULL) *retcode = NO_CONTENT;
	}
	oss_free_user_data(user_data);
}

oss_object_metadata_t *
client_head_object_group(oss_client_t *client,
		oss_get_object_group_request_t *request,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(request != NULL);

	curl_request_param_t *user_data = (curl_request_param_t *)malloc(sizeof(curl_request_param_t));
	user_data->send_buffer = NULL;

	user_data->recv_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 128 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 128 * 1024;
	user_data->recv_buffer->allocated = 128 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 4 * 1024;
	user_data->header_buffer->allocated = 4 * 1024;

	unsigned int bucket_name_len = strlen(request->bucket_name);
	unsigned int key_len = strlen(request->key);

	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);

	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + key_len + strlen(client->endpoint) + 8));
	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	char header_if_modified_since[64] = {0};
	char header_if_unmodified_since[64] = {0};

	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(header_host, "Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_HEAD,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部
	 */
	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	if (request->get_modified_since_constraint(request) != NULL) {
		sprintf(header_if_modified_since, "If-Modified-Since: %s", request->get_modified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_modified_since);
	}
	if (request->get_unmodified_since_constraint(request) != NULL) {
		sprintf(header_if_unmodified_since, "If-Unmodified-Since: %s", request->get_unmodified_since_constraint(request));
		http_headers = curl_slist_append(http_headers, header_if_unmodified_since);
	}

	/**
	 * 发送请求
	 */
	object_group_curl_operation(OSS_HTTP_HEAD, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);

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
	if (user_data->header_buffer->code != 200) {
		unsigned short tmp = user_data->header_buffer->code;
		if (retcode != NULL) {
			if(tmp == 404) {
				*retcode = FILE_NOT_FOUND;
			} else if(tmp == 304) {
				*retcode = NOT_MODIFIED;
			} else if(tmp == 412) {
				*retcode = PRECONDITION_FAILED;
			} else {
				*retcode = 1000;
			}
		}
		//*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_head_object_group_response(user_data);
	}
}

