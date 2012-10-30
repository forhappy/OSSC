/*
 * =============================================================================
 *
 *       Filename:  client_multipart_upload_operation.c
 *
 *    Description:  client multipart upload operation routines.
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

static oss_initiate_multipart_upload_result_t *
construct_initiate_multipart_upload_response(curl_request_param_t *user_data)
{
	const char *strxml = user_data->recv_buffer->ptr;
	oss_initiate_multipart_upload_result_t *result = initiate_multipart_upload_result_initialize();
	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "Key");
	result->set_key(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "UploadId");
	result->set_upload_id(result, *(tmp->child->attrib));

	xml_free(xml);
	oss_free_user_data(user_data);
	return result;
}

static oss_upload_part_result_t *
construct_upload_part_response(curl_request_param_t *user_data)
{
	const char *etag = user_data->header_buffer->ptr;
	oss_upload_part_result_t *result = upload_part_result_initialize();
	result->set_etag(result, etag);
	oss_free_partial_user_data_2nd(user_data);

	return result;
}

static oss_complete_multipart_upload_result_t *
construct_complete_multipart_upload_response(curl_request_param_t *user_data)
{
	const char * strxml = user_data->recv_buffer->ptr;
	oss_complete_multipart_upload_result_t *result = complete_multipart_upload_result_initialize();
	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "Location");
	result->set_location(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "Key");
	result->set_key(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "ETag");
	result->set_etag(result, *(tmp->child->attrib));

	xml_free(xml);
	oss_free_user_data(user_data);

	return result;
}

static oss_multipart_upload_listing_t *
construct_list_multipart_uploads_response(curl_request_param_t *user_data)
{
	const char *strxml = user_data->recv_buffer->ptr;
	oss_multipart_upload_listing_t *listing = multipart_upload_listing_initialize();
	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;
	XmlNode *tmp2 = NULL;
	int i = 0;

	tmp = xml_find(xml, "Bucket");
	if ((tmp->child) != NULL) {
		listing->set_bucket_name(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "KeyMarker");
	if ((tmp->child) != NULL) {
		listing->set_key_marker(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "UploadIdMarker");
	if ((tmp->child) != NULL) {
		listing->set_upload_id_marker(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "NextKeyMarker");
	if ((tmp->child) != NULL) {
		listing->set_next_key_marker(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "NextUploadIdMarker");
	if ((tmp->child) != NULL) {
		listing->set_next_upload_id_marker(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "Delimiter");
	if ((tmp->child) != NULL) {
		listing->set_delimiter(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "Prefix");
	if ((tmp->child) != NULL) {
		listing->set_prefix(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "MaxUploads");
	if ((tmp->child) != NULL) {
		listing->set_max_uploads(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "IsTruncated");
	if ((tmp->child) != NULL) {
		if (strcmp(*(tmp->child->attrib), "false") == 0) {
			listing->set_is_truncated(listing, 0);
		} else {
			listing->set_is_truncated(listing, 1);
		}
	}

	tmp = xml_find(xml, "Upload");
	unsigned int upload_counts = 0;
	tmp2 = tmp;
	if (tmp2 != NULL) upload_counts = 1;
	while (tmp2->next != NULL) {
		tmp2 = tmp2->next;
		upload_counts++;
	}
	oss_multipart_upload_t **uploads = (oss_multipart_upload_t **)
		malloc(sizeof(oss_multipart_upload_t *) * upload_counts);

	tmp2 = tmp;
	for (i = 0; i < upload_counts; i++) {
		*(uploads + i) = multipart_upload_initialize();
		(*(uploads + i))->set_key((*(uploads + i)), *(tmp2->child->child->attrib));
		(*(uploads + i))->set_upload_id((*(uploads + i)), *(tmp2->child->next->child->attrib));
		(*(uploads + i))->set_storage_class((*(uploads + i)), *(tmp2->child->next->next->child->attrib));
		(*(uploads + i))->set_initiated((*(uploads + i)), *(tmp2->child->next->next->next->child->attrib));
		tmp2 = tmp2->next;
	}

	listing->set_multipart_uploads(listing, uploads, upload_counts);

	xml_free(xml);
	oss_free_user_data(user_data);
	return listing;
}

static oss_part_listing_t *
construct_list_parts_response(curl_request_param_t *user_data)
{
	const char *strxml = user_data->recv_buffer->ptr;
	oss_part_listing_t *listing = part_listing_initialize();
	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;
	XmlNode *tmp2 = NULL;
	int i = 0;

	tmp = xml_find(xml, "Bucket");
	if ((tmp->child) != NULL) {
		listing->set_bucket_name(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "Key");
	if ((tmp->child) != NULL) {
		listing->set_key(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "UploadId");
	if ((tmp->child) != NULL) {
		listing->set_upload_id(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "StorageClass");
	if ((tmp->child) != NULL) {
		listing->set_storage_class(listing, *(tmp->child->attrib));
	}

	tmp = xml_find(xml, "PartNumberMarker");
	if ((tmp->child) != NULL) {
		listing->set_part_number_marker(listing, atoi(*(tmp->child->attrib)));
	}

	tmp = xml_find(xml, "NextPartNumberMarker");
	if ((tmp->child) != NULL) {
		listing->set_next_part_number_marker(listing, atoi(*(tmp->child->attrib)));
	}

	tmp = xml_find(xml, "MaxParts");
	if ((tmp->child) != NULL) {
		listing->set_max_parts(listing, atoi(*(tmp->child->attrib)));
	}

	tmp = xml_find(xml, "IsTruncated");
	if ((tmp->child) != NULL) {
		if (strcmp(*(tmp->child->attrib), "false") == 0) {
			listing->set_is_truncated(listing, 0);
		} else {
			listing->set_is_truncated(listing, 1);
		}
	}

	tmp = xml_find(xml, "Part");
	unsigned int part_counts = 0;
	tmp2 = tmp;
	if (tmp2 != NULL) {
		part_counts = 1;
		while (tmp2->next != NULL) {
			tmp2 = tmp2->next;
			part_counts++;
		}

		oss_part_summary_t **parts = (oss_part_summary_t **)
			malloc(sizeof(oss_part_summary_t *) * part_counts);

		tmp2 = tmp;
		for (i = 0; i < part_counts; i++) {
			*(parts + i) = part_summary_initialize();
			(*(parts + i))->set_part_number((*(parts + i)), atoi(*(tmp2->child->child->attrib)));
			(*(parts + i))->set_last_modified((*(parts + i)), *(tmp2->child->next->child->attrib));
			(*(parts + i))->set_etag((*(parts + i)), *(tmp2->child->next->next->child->attrib));
			(*(parts + i))->set_size((*(parts + i)), atoi(*(tmp2->child->next->next->next->child->attrib)));
			tmp2 = tmp2->next;
		}

		listing->set_parts(listing, parts, part_counts);
	}

	xml_free(xml);
	oss_free_user_data(user_data);
	return listing;
}

static void
multipart_upload_curl_operation(const char *method,
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
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, multipart_upload_curl_operation_send_from_file_callback);
			curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} else if (strcmp(method, OSS_HTTP_GET) == 0) {
			
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);

		} else if (strcmp(method, OSS_HTTP_HEAD) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);	
		} else if (strcmp(method, OSS_HTTP_DELETE) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);	
		} else if (strcmp(method, OSS_HTTP_POST) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer->ptr);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		}
		
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

static void
multipart_upload_curl_operation_2nd(const char *method,
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
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, multipart_upload_curl_operation_send_from_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} if (strcmp(method, OSS_HTTP_GET) == 0) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_file_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		} if (strcmp(method, OSS_HTTP_POST) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer->ptr);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		}
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

static void
multipart_upload_curl_operation_3rd(const char *method,
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
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		} else if (strcmp(method, OSS_HTTP_GET) == 0) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_file_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		} else if (strcmp(method, OSS_HTTP_POST) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, multipart_upload_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		}
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

oss_initiate_multipart_upload_result_t *
client_initiate_multipart_upload(oss_client_t *client,
		oss_initiate_multipart_upload_request_t *request,
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

	unsigned int bucket_name_len = strlen(request->get_bucket_name(request));
	unsigned int key_len = strlen(request->get_key(request));
	unsigned int sign_len = 0;
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len + 16));
	char *url = (char *)malloc(sizeof(char) *
			(bucket_name_len + key_len + strlen(client->endpoint) + 64));
	char header_host[256]  = {0};
	char header_date[48]  = {0};
	char *now; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char header_cache_control[48] = {0};
	char header_expires[64] = {0};
	char header_content_encoding[64] = {0};
	char header_content_disposition[256] = {0};
	oss_map_t *default_headers = oss_map_new(16);
	now = (char *)oss_get_gmt_time();
	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s?uploads", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s?uploads", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_object_metadata_t *metadata = request->get_object_metadata(request);
	oss_map_t *user_headers = NULL;
	oss_map_put(default_headers, OSS_DATE, now);

	if (metadata != NULL) {
		user_headers = (metadata->get_user_metadata(metadata));	
	}
	
	/**
	 * 生成签名值
	 */
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_POST,
			default_headers, user_headers, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，目前只支持标准标签，不支持 x-oss-meta开头的标签
	 */
	struct curl_slist *http_headers = NULL;
	
	if (metadata != NULL) {
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
	}

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	multipart_upload_curl_operation_3rd(OSS_HTTP_POST, resource, url, http_headers, user_data);

	/**
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);

	oss_map_delete(default_headers);
	// oss_map_delete(user_headers);

	if (now != NULL) free(now);
	if (sign != NULL) free(sign);
	if (resource != NULL) free(resource);
	if (url != NULL) free(url);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_initiate_multipart_upload_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
	return NULL;
}

oss_upload_part_result_t *
client_upload_part(oss_client_t *client, 
		oss_upload_part_request_t *request,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(request != NULL);
	int input_stream_len = 0;

	curl_request_param_t *user_data = 
		(curl_request_param_t *)malloc(sizeof(curl_request_param_t));

	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->send_buffer->ptr = (char *)request->get_input_stream(request, &input_stream_len);
	user_data->send_buffer->left = request->get_part_size(request);
	user_data->send_buffer->allocated = request->get_part_size(request);

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

	char resource[512]     = {0};
	char url[512]          = {0};
	char header_host[256]  = {0};
	char header_date[64]  = {0};
	char *now;
	char header_auth[128]  = {0};
	char header_content_type[64] = {0};
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s?partNumber=%d&uploadId=%s",
			request->get_bucket_name(request), request->get_key(request),
			request->get_part_number(request), request->get_upload_id(request));
	sprintf(url, "%s/%s/%s?partNumber=%d&uploadId=%s", client->endpoint, 
			request->get_bucket_name(request), request->get_key(request),
			request->get_part_number(request), request->get_upload_id(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);
	sprintf(header_content_type, "Content-Type: %s", "application/octet-stream");

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/octet-stream");
	
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	struct curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, header_content_type);
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	multipart_upload_curl_operation_2nd(OSS_HTTP_PUT, resource, url, http_headers, user_data);
	
	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);
	if (now != NULL) free(now);
	if (sign != NULL) free(sign);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		oss_upload_part_result_t *result = 
			construct_upload_part_response(user_data);
		result->set_part_number(result, request->get_part_number(request));
		return result;
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_partial_user_data_2nd(user_data);
	}
	return NULL;
}


oss_complete_multipart_upload_result_t*
client_complete_multipart_upload(oss_client_t *client,
		oss_complete_multipart_upload_request_t *request,
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

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;

	char resource[512]     = {0};
	char url[512]          = {0};
	char header_host[256]  = {0};
	char header_date[64]  = {0};
	char *now = NULL;
	char header_auth[128]  = {0};
	char part[512] = {0};
	unsigned int sign_len = 0;
	int parts = 0;
	unsigned int i = 0;
	oss_map_t *default_headers = oss_map_new(16);
	sprintf(resource, "/%s/%s?uploadId=%s", request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(url, "%s/%s/%s?uploadId=%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_POST,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	oss_part_etag_t **part_etag = request->get_part_etags(request, &parts);
	tstring_t *tstr_part_etag = tstring_new("<CompleteMultipartUpload>");
	for (; i < parts; i++) {
		sprintf(part, "<Part><PartNumber>%d</PartNumber><ETag>\"%s\"</ETag></Part>",
				(*(part_etag + i))->get_part_number(*(part_etag + i)),
				(*(part_etag + i))->get_etag(*(part_etag + i)));
		tstring_append(tstr_part_etag, part);
	}
	tstring_append(tstr_part_etag, "</CompleteMultipartUpload>");

	user_data->send_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->send_buffer->ptr = (char *)malloc(sizeof(char) * (tstring_size(tstr_part_etag) + 1));
	memset(user_data->send_buffer->ptr, 0, (tstring_size(tstr_part_etag) + 1));
	memcpy(user_data->send_buffer->ptr, tstring_data(tstr_part_etag), tstring_size(tstr_part_etag));
	user_data->send_buffer->left = tstring_size(tstr_part_etag);
	user_data->send_buffer->allocated = tstring_size(tstr_part_etag);

	struct curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	multipart_upload_curl_operation_2nd(OSS_HTTP_POST, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);
	if (now != NULL) free(now);
	if (sign != NULL) free(sign);
	tstring_free(tstr_part_etag);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		oss_complete_multipart_upload_result_t *result = 
			construct_complete_multipart_upload_response(user_data);
		return result;
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
	return NULL;
}

void 
client_abort_multipart_upload(oss_client_t *client,
		oss_abort_multipart_upload_request_t *request,
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

	char resource[512]     = {0};
	char url[512]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char *now = NULL;
	char header_auth[512]  = {0};
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s?uploadId=%s", request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(url, "%s/%s/%s?uploadId=%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);
	oss_map_put(default_headers, OSS_DATE, now);
	
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
	multipart_upload_curl_operation(OSS_HTTP_DELETE, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);
	if (now != NULL) free(now);
	if (sign != NULL) free(sign);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
}

oss_part_listing_t *
client_list_parts(oss_client_t *client,
		oss_list_parts_request_t *request,
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

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;

	char resource[512]     = {0};
	char url[512]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char *now = NULL;
	char header_auth[128]  = {0};
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);
	tstring_t *resource_params = tstring_new("");

	if (request->get_max_parts(request) > 0) {
		tstring_append_printf(resource_params, "&max-parts=%d", request->get_max_parts(request));
	}
	if (request->get_part_number_marker(request) > 0) {
		tstring_append_printf(resource_params, "&part-number-marker=%d",
			request->get_part_number_marker(request));
	}
	if (tstring_size(resource_params) > 0) {
		sprintf(resource, "/%s/%s?uploadId=%s%s", request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request),
			tstring_data(resource_params));
		sprintf(url, "%s/%s/%s?uploadId=%s%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request),
			tstring_data(resource_params));
	} else {
		sprintf(resource, "/%s/%s?uploadId=%s", request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
		sprintf(url, "%s/%s/%s?uploadId=%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	}
	tstring_free(resource_params);

	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	struct curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	multipart_upload_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);
	if (now != NULL) free(now);
	if (sign != NULL) free(sign);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_list_parts_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}
	return NULL;
}

oss_multipart_upload_listing_t *
client_list_multipart_uploads(oss_client_t *client,
		oss_list_multipart_uploads_request_t *request,
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

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;

	char resource[512]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char *now = NULL;
	char header_auth[128]  = {0};
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);

	tstring_t *resource_params = tstring_new("");
	if (strlen(request->get_delimiter(request)) > 0) {
		tstring_append_printf(resource_params, "&delimiter=%s", request->get_delimiter(request));
	}
	if (strlen(request->get_key_marker(request)) > 0) {
		tstring_append_printf(resource_params, "&key-marker=%s", request->get_key_marker(request));
	}
	if (request->get_max_uploads(request) > 0) {
		tstring_append_printf(resource_params, "&max-uploads=%d", request->get_max_uploads(request));
	}
	if (strlen(request->get_prefix(request)) > 0) {
		tstring_append_printf(resource_params, "&prefix=%s", request->get_prefix(request));
	}
	if (strlen(request->get_upload_id_marker(request)) > 0) {
		tstring_append_printf(resource_params, "&upload-id-marker=%s", request->get_upload_id_marker(request));
	}
	if (tstring_size(resource_params) > 0) {
		sprintf(resource, "/%s?uploads%s", request->get_bucket_name(request),
			tstring_data(resource_params));
		sprintf(url, "%s/%s?uploads%s", client->endpoint, request->get_bucket_name(request),
			tstring_data(resource_params));
	} else {
		sprintf(resource, "/%s?uploads", request->get_bucket_name(request));
		sprintf(url, "%s/%s?uploads", client->endpoint, request->get_bucket_name(request));
	}
	tstring_free(resource_params);

	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	
	char *sign = (char *)generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);


	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	struct curl_slist *http_headers = NULL;

	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	/**
	 * 发送请求
	 */
	multipart_upload_curl_operation(OSS_HTTP_GET, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);
	if (now != NULL) free(now);
	if (sign != NULL) free(sign);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		return construct_list_multipart_uploads_response(user_data);
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		oss_free_user_data(user_data);
	}

	return NULL;
}

