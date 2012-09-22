/*
 * =============================================================================
 *
 *       Filename:  client_multipart_upload_operation-t.c
 *
 *    Description:  client_multipart_upload_operation testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
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




size_t multipart_upload_curl_operation_send_from_file_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fread(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t multipart_upload_curl_operation_send_from_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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

size_t multipart_upload_curl_operation_recv_to_file_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fwrite(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t multipart_upload_curl_operation_recv_to_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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

size_t multipart_upload_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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

	retag = sscanf(ptr, "ETag: %s", etag);
	if (retag != 0) {
		memset(header_buffer->ptr, 0, header_buffer->allocated);
		strncpy(header_buffer->ptr, etag, 48);
	}
	return size * nmemb;
}

static oss_initiate_multipart_upload_result_t *
construct_initiate_multipart_upload_response_on_success(const char *strxml)
{
	oss_initiate_multipart_upload_result_t *result = initiate_multipart_upload_result_initialize();
	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "Key");
	result->set_key(result, *(tmp->child->attrib));
	tmp = xml_find(xml, "UploadId");
	result->set_upload_id(result, *(tmp->child->attrib));

	return result;
}

static oss_upload_part_result_t *
construct_upload_part_response_on_success(const char *etag)
{
	oss_upload_part_result_t *result = upload_part_result_initialize();
	result->set_etag(result, etag);

	return result;
}

static oss_complete_multipart_upload_result_t *
construct_complete_multipart_upload_response_on_success(const char *strxml)
{
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

	return result;
}

static oss_multipart_upload_listing_t *
construct_list_multipart_uploads_response_on_success(const char *strxml)
{
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
	size_t upload_counts = 0;
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
	return listing;
}

static oss_part_listing_t *
construct_list_parts_response_on_success(const char *strxml)
{
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
	size_t part_counts = 0;
	tmp2 = tmp;
	if (tmp2 != NULL) part_counts = 1;
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
	return listing;
}

static const char*
construct_complete_multipart_upload_response_on_error(const char *strxml)
{

	XmlNode *xml = xml_load_buffer(strxml, strlen(strxml));
	XmlNode *tmp = NULL;

	tmp = xml_find(xml, "Code");
	
	return *(tmp->child->attrib);
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
		
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

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
			//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, multipart_upload_curl_operation_recv_to_buffer_callback);
			//curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
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
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

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


	// param_buffer_t *send_buffer = params->send_buffer;
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
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

/* **************************************************************************** */
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
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 2 * 1024;
	user_data->recv_buffer->allocated = 2 * 1024;

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	size_t bucket_name_len = strlen(request->get_bucket_name(request));
	size_t key_len = strlen(request->get_key(request));
	size_t sign_len = 0;

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
	
	char header_cache_control[48] = {0};
	char header_expires[64] = {0};
	char header_content_encoding[64] = {0};
	char header_content_disposition[256] = {0};

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值
	 * */
	sprintf(resource, "/%s/%s?uploads", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s?uploads", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
	sprintf(now, "%s", oss_get_gmt_time());

	/** 构造请求头部 */
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(header_date, "Date: %s", now);

	oss_object_metadata_t *metadata = request->get_object_metadata(request);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_t *user_headers = (metadata->get_user_metadata(metadata));	
	
	/**
	 * 生成签名值
	 */
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_POST,
			default_headers, user_headers, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	/**
	 * 自定义 HTTP 请求头部，目前只支持标准标签，不支持 x-oss-meta开头的标签
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
	if (user_data->header_buffer->code == 200) {
		return construct_initiate_multipart_upload_response_on_success(
			user_data->recv_buffer->ptr);
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

	user_data->header_buffer = (param_buffer_t *)malloc(sizeof(param_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 2 * 1024;
	user_data->header_buffer->allocated = 2 * 1024;

	user_data->recv_buffer = NULL;

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};
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
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);
	sprintf(header_content_type, "Content-Type: %s", "application/octet-stream");

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/octet-stream");
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
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
	if (user_data->header_buffer->code == 200) {
		printf("Hello ETag: %s\n", user_data->header_buffer->ptr);
		oss_upload_part_result_t *result = 
			construct_upload_part_response_on_success(
			user_data->header_buffer->ptr);
		result->set_part_number(result, request->get_part_number(request));
		return result;
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

	unsigned int sign_len = 0;
	int parts = 0;
	unsigned int i = 0;


	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s?uploadId=%s", request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(url, "%s/%s/%s?uploadId=%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_POST,
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
	user_data->send_buffer->ptr = tstring_data(tstr_part_etag);
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
	if (user_data->header_buffer->code == 200) {
		oss_complete_multipart_upload_result_t *result = 
			construct_complete_multipart_upload_response_on_success(
			user_data->recv_buffer->ptr);
		return result;
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
	user_data->send_buffer->ptr = NULL;

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

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s?uploadId=%s", request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(url, "%s/%s/%s?uploadId=%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request), request->get_upload_id(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	
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
	multipart_upload_curl_operation(OSS_HTTP_DELETE, resource, url, http_headers, user_data);

	curl_slist_free_all(http_headers);
	if (user_data->header_buffer->code == 200) {
		*retcode = 0;
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

	
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
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
	if (user_data->header_buffer->code == 200) {
		return construct_list_parts_response_on_success(user_data->recv_buffer->ptr);
		printf("Uploads: %s\n", user_data->recv_buffer->ptr);
		*retcode = 0;
	}
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
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * 2 * 1024 * 1024);
	user_data->recv_buffer->fp = NULL;
	user_data->recv_buffer->left = 2 * 1024 * 1024;
	user_data->recv_buffer->allocated = 2 * 1024 * 1024;

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

	
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	oss_map_put(default_headers, OSS_DATE, now);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_GET,
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
	if (user_data->header_buffer->code == 200) {
		return construct_list_multipart_uploads_response_on_success(user_data->recv_buffer->ptr);
		printf("Uploads: %s\n", user_data->recv_buffer->ptr);
		*retcode = 0;
	}

	return NULL;
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

const char *etags[] = {
	"BE02E1CE4782480A173A8CF53DAE2891",
	"238AE718365615933447E232B6F2E6C9",
	"C07D578908A16300258A5A2B3C4608D7",
	"8000D1709DDA4D3B4C126726ADE4FC9C",
	"041114C556BAD24B0A8A4D076AB5F4F1"
};


int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	const char *upload_id = "0004C9B70B6F2CAA2CACE4468CBE6E70";

	unsigned short retcode = 0;
	//const char *bucket_name = "bucketname001";
	//const char *key = "put.png";
	//FILE *file = fopen("proactor.pdf", "r");
	//FILE *local_file = fopen(key, "w");
	//size_t file_len = 2 * 1024 * 1024;//oss_get_file_size(file);

	//char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	//memset(buffer, '\0', file_len + 1);
	//fread(buffer, file_len, 1, file);

	//oss_client_t *client = client_initialize(access_id, access_key);
	//
	oss_object_metadata_t *metadata = object_metadata_initialize();

	metadata->set_cache_control(metadata, "no-cachess");
	metadata->set_content_encoding(metadata, "utf-8");
	metadata->set_content_disposition(metadata, "attachment;");
	metadata->set_expiration_time(metadata, "Thu, 15 Sep 2012 21:08:42 GMT");
	//oss_put_multipart_upload_result_t *result01 = client_put_multipart_upload_from_file(client, bucket_name, key,
	//		file, request, NULL);

	///oss_put_multipart_upload_result_t *result02 = client_put_multipart_upload_from_buffer(client, bucket_name, key,
	//		buffer, request, NULL);
	//printf("ETag:\n%s\n", result02->get_etag(result02));
	//put_multipart_upload_result_finalize(result02);

	//oss_client_t *client = client_initialize(access_id, access_key);
	//oss_get_multipart_upload_request_t *request = get_multipart_upload_request_initialize(bucket_name, key);
	//client_get_multipart_upload_to_file(client, request, local_file, NULL);
	//client_get_multipart_upload_to_buffer(client, request, buffer, &file_len, NULL);

	//fwrite(buffer, file_len, 1, local_file);
	//printf("length: %d\n", file_len);
	//fclose(file);
	//fclose(local_file);

	const char *bucket_name = "bucketname001";

	const char *key = "neo4j-communityx.tar.gz";
	FILE *file = fopen("neo4j-community.tar.gz", "r");
	size_t file_len = oss_get_file_size(file);
	//int fd = fileno(file);
	
	char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	memset(buffer, '\0', file_len + 1);
	fread(buffer, file_len, 1, file);
	const int single_request_len = 8 * 1024 * 1024;
	int requests_num = file_len / single_request_len;
	int current_part_number = 0;


	oss_client_t *client = client_initialize(access_id, access_key);


	//oss_copy_multipart_upload_result_t *result = client_copy_multipart_upload_ext(client, source_bucket_name, source_key,
	//		destination_bucket_name, destination_key, NULL);
	//oss_copy_multipart_upload_request_t *request = copy_multipart_upload_request_initialize(source_bucket_name, source_key,
	//	destination_bucket_name, destination_key);
	//request->set_modified_since_constraints(request, "Wed, 12 Sep 2012 12:06:55 GMT");
	//oss_copy_multipart_upload_result_t *result = client_copy_multipart_upload(client, request, NULL);
	//if (result != NULL) {
	//	printf("ETag: %s", result->get_etag(result));
	//	printf("LastModified: %s\n", result->get_last_modified(result));
	//}
	//client_get_multipart_upload_request(client, source_bucket_name, source_key, NULL);
	//client_delete_multipart_upload(client, source_bucket_name, source_key, &retcode);
	//printf("%d\n", retcode);
#if 0
	oss_initiate_multipart_upload_request_t *request_2nd = 
		initiate_multipart_upload_request_initialize_with_metadata(bucket_name, key, metadata);


	oss_initiate_multipart_upload_result_t *result =
		client_initiate_multipart_upload(client, request_2nd, &retcode);
	printf("%s\n", result->get_upload_id(result));
#endif

#if 0
	oss_upload_part_request_t *request = upload_part_request_initialize();
	request->set_bucket_name(request, bucket_name);
	request->set_key(request, key);
	request->set_upload_id(request, upload_id);

	for (; current_part_number < requests_num + 1; current_part_number++) {
		if (current_part_number < requests_num) {
			request->set_part_number(request, current_part_number + 1);
			request->set_input_stream(request,
					buffer + current_part_number * single_request_len, single_request_len);
			request->set_part_size(request, single_request_len);
			oss_upload_part_result_t *result = client_upload_part(client, request, &retcode);
			printf("PartNumber:%d, ETag:%s\n", result->get_part_number(result), result->get_etag(result));
		} else {
			request->set_part_number(request, current_part_number + 1);
			request->set_input_stream(request,
					buffer + current_part_number *single_request_len,
					file_len - single_request_len * current_part_number);
			request->set_part_size(request, file_len - single_request_len * current_part_number);
			oss_upload_part_result_t *result = client_upload_part(client, request, &retcode);
			printf("PartNumber:%d, ETag:%s\n", result->get_part_number(result), result->get_etag(result));
			
		}
	}
#endif

#if 0
	oss_part_etag_t **part_etag = (oss_part_etag_t **)malloc(sizeof(oss_part_etag_t *) * 5);
	int i = 0;
	for (; i < 5; i++) {

		*(part_etag + i) = part_etag_initialize(i + 1, etags[i]);
	}

	//oss_client_t *client = client_initialize(access_id, access_key);

	oss_complete_multipart_upload_request_t *request = 
		complete_multipart_upload_request_initialize(bucket_name, key, upload_id, part_etag, 5);
	oss_complete_multipart_upload_result_t *result = 
		client_complete_multipart_upload(client, request, &retcode);
	printf("ETag: %s\n", result->get_etag(result));

#endif
#if 0
	oss_list_multipart_uploads_request_t *request = 
		list_multipart_uploads_request_initialize(bucket_name);
	//request->set_prefix(request, "a");
	//request->set_max_uploads(request, 2);
	oss_multipart_upload_listing_t *listing = client_list_multipart_uploads(client, request, &retcode);
	printf("Bucket name: %s\n", listing->get_bucket_name(listing));

	size_t upload_counts = 0;
	int i = 0;

	oss_multipart_upload_t **uploads = listing->get_multipart_uploads(listing, &upload_counts);
	for (i = 0; i < upload_counts; i++) {
		printf("***********************************CONTENT %d*******************************\n", i + 1);
		printf("KEY: %s\n", (*(uploads + i))->get_key(*(uploads + i)));
		printf("UPLOAD_ID: %s\n", (*(uploads + i))->get_upload_id(*(uploads + i)));
		printf("STORAGE_CLASS: %s\n", (*(uploads + i))->get_storage_class(*(uploads + i)));
		printf("INITIATED: %s\n", (*(uploads + i))->get_initiated(*(uploads + i)));
	}

#endif

#if 1
	oss_list_parts_request_t *request = 
		list_parts_request_initialize(bucket_name, key, upload_id);
	//request->set_max_parts(request, 1);

	oss_part_listing_t *listing = client_list_parts(client, request, &retcode);

	size_t part_counts = 0;
	int i = 0;

	oss_part_summary_t **parts = listing->get_parts(listing, &part_counts);
	for (i = 0; i < part_counts; i++) {
		printf("***********************************CONTENT %d*******************************\n", i + 1);
		printf("ETAG: %s\n", (*(parts + i))->get_etag(*(parts + i)));
		printf("LAST_MODIFIED: %s\n", (*(parts + i))->get_last_modified(*(parts + i)));
		printf("PARTNUMBER: %d\n", (*(parts + i))->get_part_number(*(parts + i)));
		printf("SIZE: %d\n", (*(parts + i))->get_size(*(parts + i)));
	}

#endif

}
