/*
 * =============================================================================
 *
 *       Filename:  client_bucket_operation.c
 *
 *    Description:  client bucket operation routines.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include <ossc/client.h>
#include <ossc/oss_helper.h>
#include <ossc/oss_curl_callback.h>
#define _OSS_CLIENT_H
#include <ossc/modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>

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

	/**
	 * send_buffer 在 Bucket 操作中没有用到
	 */
	//param_buffer_t *send_buffer = params->send_buffer;
	param_buffer_t *recv_buffer = params->recv_buffer;
	param_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		if (strcmp(method, OSS_HTTP_PUT) == 0 || strcmp(method, OSS_HTTP_DELETE) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
		}
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bucket_curl_operation_recv_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, bucket_curl_operation_header_callback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

static oss_object_listing_t *
construct_list_objects_response(
		curl_request_param_t *user_data)
{
	assert(user_data != NULL);

	const char *response = user_data->recv_buffer->ptr;
	int i;
	XmlNode *xml = NULL;
	XmlNode *name_tag, *prefix_tag, *marker_tag, *max_keys_tag;
	XmlNode *delimiter_tag, *is_truncated_tag, *next_marker_tag;
	XmlNode *contents_tag, *common_prefixes_tag, *contents_tmp;
	XmlNode *common_prefixes_tmp = NULL;

	unsigned int response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);
	oss_object_listing_t *object_listing = object_listing_initialize();

	name_tag = xml_find(xml, "Name");
	object_listing->set_bucket_name(object_listing, *name_tag->child->attrib);

	prefix_tag = xml_find(xml, "Prefix");
	if(prefix_tag->child == NULL) {
		object_listing->set_prefix(object_listing, "");
	} else {
		object_listing->set_prefix(object_listing, *prefix_tag->child->attrib);
	}

	marker_tag = xml_find(xml, "Marker");
	if(marker_tag->child == NULL) {
		object_listing->set_marker(object_listing, "");
	} else {
		object_listing->set_marker(object_listing, *marker_tag->child->attrib);
	}

	next_marker_tag = xml_find(xml, "NextMarker");
	if(next_marker_tag == NULL) {
		object_listing->set_next_marker(object_listing, "");
	} else {
		if(next_marker_tag->child == NULL) {
			object_listing->set_next_marker(object_listing, "");
		} else {
			object_listing->set_next_marker(object_listing,
					*next_marker_tag->child->attrib);
		}
	}

	max_keys_tag = xml_find(xml, "MaxKeys");
	if(max_keys_tag->child == NULL)
	{
		object_listing->set_max_keys(object_listing, 0);
	} else {
		int max_keys = atoi(*max_keys_tag->child->attrib);
		object_listing->set_max_keys(object_listing, max_keys);
	}

	delimiter_tag = xml_find(xml, "Delimiter");
	if(delimiter_tag->child == NULL) {
		object_listing->set_delimiter(object_listing, "");
	} else {
		object_listing->set_delimiter(object_listing, *delimiter_tag->child->attrib);
	}

	is_truncated_tag = xml_find(xml, "IsTruncated");
	if(is_truncated_tag->child == NULL) {
		object_listing->set_is_truncated(object_listing, false);
	} else {
		if(strcmp(*is_truncated_tag->child->attrib, "false") == 0) {
			object_listing->set_is_truncated(object_listing, false);
		} else {
			object_listing->set_is_truncated(object_listing, true);
		}
	}
	
	contents_tag = xml_find(xml, "Contents");
	if(contents_tag != NULL) {
		contents_tmp = contents_tag;
		for(; contents_tmp != NULL; contents_tmp = contents_tmp->next) {
			if(strcmp(contents_tmp->name, "Contents") == 0)
				(object_listing->_counts_summaries)++;
			else break;
		}
		oss_object_summary_t **summaries = (oss_object_summary_t **)malloc(
				sizeof(oss_object_summary_t *) * (object_listing->_counts_summaries));

		for(i = 0; i < object_listing->_counts_summaries; i++, contents_tag = contents_tag->next) {
			summaries[i] = object_summary_initialize();
			summaries[i]->set_key(summaries[i],
					*contents_tag->child->child->attrib);
			summaries[i]->set_last_modified(summaries[i],
					*contents_tag->child->next->child->attrib);
			summaries[i]->set_etag(summaries[i],
					*contents_tag->child->next->next->child->attrib);
			summaries[i]->set_type(summaries[i],
					*contents_tag->child->next->next->next->child->attrib);
			long size = atol(*contents_tag->child->next->next->next->next->child->attrib);
			summaries[i]->set_size(summaries[i], size);
			summaries[i]->set_storage_class(summaries[i],
					*contents_tag->child->next->next->next->next->next->child->attrib);
			summaries[i]->owner = owner_initialize_with_id(
					*contents_tag->child->next->next->next->next->next->next->child->child->attrib,
					*contents_tag->child->next->next->next->next->next->next->child->next->child->attrib);		
		}
		object_listing->summaries = summaries;
	}
	
	common_prefixes_tag = xml_find(xml, "CommonPrefixes");
	if(common_prefixes_tag != NULL) {
		common_prefixes_tmp = common_prefixes_tag;
		for(; common_prefixes_tmp != NULL; 
				common_prefixes_tmp = common_prefixes_tmp->next) {
			(object_listing->_counts_common_prefixes)++;
		}
		char **common_prefixes = (char **)malloc(
				sizeof(char *) * (object_listing->_counts_common_prefixes));
		for(i = 0; common_prefixes_tag != NULL; 
				i++, common_prefixes_tag = common_prefixes_tag->next) {
			const char *tmp = *common_prefixes_tag->child->child->attrib;
			unsigned int common_prefixes_len = strlen(tmp);
			common_prefixes[i] = (char *)malloc(
					sizeof(char) * (common_prefixes_len + 1));
			strncpy(common_prefixes[i], tmp, common_prefixes_len);
			common_prefixes[i][common_prefixes_len] = '\0';
		}
		object_listing->common_prefixes = common_prefixes;
	}
 
	oss_free_user_data(user_data);
 	xml_free(xml);

	return object_listing;
	
}

static oss_access_control_list_t *
construct_get_bucket_acl_response(
		curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	XmlNode *xml, *grant_tag, *owner_tag;
	unsigned int response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	owner_tag = xml_find(xml, "Owner");
	oss_owner_t *owner = owner_initialize_with_id(*owner_tag->child->child->attrib,
			*owner_tag->child->next->child->attrib);

 	grant_tag = xml_find(xml, "Grant");
	
	oss_access_control_list_t *acl = access_control_list_initialize();
	acl->set_grant(acl, *grant_tag->child->attrib);
	acl->set_owner(acl, owner);
	
	oss_free_user_data(user_data);
 	xml_free(xml);

	return acl;
	
}

static oss_bucket_t **
construct_list_buckets_response(
		curl_request_param_t *user_data,
		int *buckets_number)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	XmlNode *xml, *buckets_tag, *bucket_tag, *owner_tag;
	int i;
	unsigned int response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	oss_bucket_t **buckets = NULL;
	*buckets_number = 0;
	
	buckets_tag = xml_find(xml, "Buckets");
	if(buckets_tag != NULL) {
		bucket_tag = buckets_tag->child;
	} else {
		bucket_tag = NULL;
	}
	for(; bucket_tag != NULL; bucket_tag = bucket_tag->next) {
		(*buckets_number)++;
	}
	
	if(*buckets_number > 0) {
		owner_tag = xml_find(xml, "Owner");
		oss_owner_t *owner = owner_initialize_with_id(*owner_tag->child->child->attrib,
			*owner_tag->child->next->child->attrib);

		buckets = (oss_bucket_t **)malloc(
				sizeof(oss_bucket_t *) * (*buckets_number));
		for(i = 0, bucket_tag = buckets_tag->child;
				i < *buckets_number; i++, bucket_tag = bucket_tag->next) {
			buckets[i] = bucket_initialize();
			buckets[i]->set_name(buckets[i], *bucket_tag->child->child->attrib);
			buckets[i]->set_create_date(buckets[i],
					*bucket_tag->child->next->child->attrib);
			buckets[i]->set_owner(buckets[i], owner);
		}
	}
 
	oss_free_user_data(user_data);
 	xml_free(xml);

	return buckets;
	
}



oss_bucket_t **
client_list_buckets(
		oss_client_t *client,
		int *buckets_number,
		unsigned short *retcode)
{
	assert(client != NULL);

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

	char *resource = (char *)malloc(sizeof(char) * 16);
	char *url = (char *)malloc(sizeof(char) * strlen(client->endpoint) + 1);

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/");
	sprintf(url, "%s", client->endpoint);
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	bucket_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源，以及其他动态分配的空间。
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
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		*buckets_number = 0;
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_list_buckets_response(user_data, buckets_number);
	}
}

/**
 * 设置指定 Bucket 的 Access Control List(ACL)
 */
void
client_set_bucket_acl(oss_client_t *client,
		const char *bucket_name,
		const char *acl,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(bucket_name != NULL);

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

	unsigned int bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(
			sizeof(char) * (bucket_name_len + 16));
	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
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
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_ACL, acl);
	
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

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);
	http_headers = curl_slist_append(http_headers, header_acl);

	/**
	 * 发送请求
	 */
	bucket_curl_operation(OSS_HTTP_PUT, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源和其他动态分配的空间
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
	} else {
		if (retcode != NULL) *retcode = 0;
	}
	oss_free_user_data(user_data);
}

void
client_create_bucket(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);

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

	unsigned int bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(sizeof(char) *bucket_name_len + 16 );
	char *url = (char *)malloc(sizeof(char) * 
			(bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);
	oss_map_put(default_headers, OSS_DATE, now);

	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_PUT,
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
	bucket_curl_operation(OSS_HTTP_PUT, resource, url, http_headers, user_data);

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
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
	} else {
		if (retcode != NULL) *retcode = 0;
	}
	oss_free_user_data(user_data);
}

oss_object_listing_t *
client_list_objects(oss_client_t *client,
		oss_list_objects_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert((request->bucket_name) != NULL);

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

	unsigned int bucket_name_len = strlen(request->bucket_name);
	char *resource = (char *)malloc(sizeof(char) *bucket_name_len + 16 );
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + strlen(client->endpoint) + 1024));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	int is_first_param = 0;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", request->bucket_name);
	sprintf(url, "%s/%s", client->endpoint, request->bucket_name);
	if(strcmp(request->prefix, "")) {
			sprintf(url, "%s?prefix=%s", url, request->prefix);
			is_first_param = 1;
	}	
	if(strcmp(request->delimiter, "")) {
		if(is_first_param == 0) {
			sprintf(url, "%s?delimiter=%s", url, request->delimiter);
			is_first_param = 1;
		} else 
			sprintf(url, "%s&delimiter=%s", url, request->delimiter);
	}
	if(strcmp(request->marker, "")) {
		if(is_first_param == 0) {
			sprintf(url, "%s?marker=%s", url, request->marker);
			is_first_param = 1;
		} else 
			sprintf(url, "%s&marker=%s", url, request->marker);
	}
	if(request->max_keys != 0) {
		if(is_first_param == 0) {
			sprintf(url, "%s?=max-keys%u", url, request->max_keys);
			is_first_param = 1;
		} else 
			sprintf(url, "%s&max-keys=%u", url, request->max_keys);
	}
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	bucket_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

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
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_list_objects_response(user_data);
	}
	
}



oss_object_listing_t *
client_list_objects_with_prefix(
		oss_client_t *client,
		const char *bucket_name,
		const char *prefix,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);
	if(prefix == NULL || (!strcmp(prefix, "")))  {
		return client_list_objects_with_bucket_name(client, bucket_name, retcode);
	}

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

	unsigned int bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(sizeof(char) *bucket_name_len + 32 );
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + strlen(client->endpoint) + strlen(prefix) + 32));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s?prefix=%s", client->endpoint, bucket_name, prefix);
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	bucket_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

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
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_list_objects_response(user_data);
	}
	
}



oss_object_listing_t *
client_list_objects_with_bucket_name(
		oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode)
{

	assert(client != NULL);
	assert(bucket_name != NULL);

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

	unsigned int bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(sizeof(char) *bucket_name_len + 16 );
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	bucket_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

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
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_list_objects_response(user_data);
	}
	
}


oss_access_control_list_t *
client_get_bucket_acl(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(bucket_name != NULL);

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

	unsigned int bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 16);
	char *url = (char *)malloc(
			sizeof(char) * (bucket_name_len + strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s?acl", bucket_name);
	sprintf(url, "%s/%s?acl", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
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
	bucket_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

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
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
		return NULL;
	} else {
		if (retcode != NULL) *retcode = 0;
		return construct_get_bucket_acl_response(user_data);
	}
}


void
client_delete_bucket(oss_client_t *client,
		const char *bucket_name,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(bucket_name != NULL);

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

	unsigned int bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 16);
	char *url = (char *)malloc(
			sizeof(char) * (bucket_name_len +strlen(client->endpoint) + 8));

	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char header_auth[128] = {0};
	char *now;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
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
	bucket_curl_operation(OSS_HTTP_DELETE, resource, url, http_headers, user_data);

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
	return;
}

bool
client_is_bucket_exist(oss_client_t *client,
		const char *bucket_name)
{
	unsigned short retcode;
	oss_access_control_list_t *acl = NULL;
	oss_owner_t *owner = NULL;
	acl = client_get_bucket_acl(client, bucket_name, &retcode);
	if(retcode == OK) {
		owner = acl->get_owner(acl);
		owner_finalize(owner);
		access_control_list_finalize(acl);
		return true;
	} else {
		return false;
	}
}
