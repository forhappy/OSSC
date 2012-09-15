/*
 * =============================================================================
 *
 *       Filename:  client_put_bucket_acl-t.c
 *
 *    Description:  client_put_bucket_acl testcase.
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

size_t bucket_curl_operation_send_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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

size_t bucket_curl_operation_recv_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
	size_t bytes_per_recv = size * nmemb;
	//printf("INFO bytes received this time: %d\n", bytes_per_recv);
	//printf("INFO: bytes received:\n%s\n", (char *)ptr);
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

size_t bucket_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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
		else if (strcmp(method, OSS_HTTP_GET) == 0) {
			//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bucket_curl_operation_recv_callback);
			//curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bucket_curl_operation_recv_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, bucket_curl_operation_header_callback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

void
free_user_data(curl_request_param_t *user_data) 
{
	if(user_data != NULL) {
		if(user_data->send_buffer != NULL) {
			if(user_data->send_buffer->ptr != NULL) {
				free(user_data->send_buffer->ptr);
				user_data->send_buffer->ptr = NULL;
			}
			free(user_data->send_buffer);
			user_data->send_buffer = NULL;
		}
		if(user_data->recv_buffer != NULL) {
			if(user_data->recv_buffer->ptr != NULL) {
				free(user_data->recv_buffer->ptr);
				user_data->recv_buffer->ptr = NULL;
			}
			free(user_data->recv_buffer);
			user_data->recv_buffer = NULL;
		}
		if(user_data->header_buffer != NULL) {
			if(user_data->header_buffer->ptr != NULL) {
				free(user_data->header_buffer->ptr);
				user_data->header_buffer->ptr = NULL;
			}
			free(user_data->header_buffer);
			user_data->header_buffer = NULL;
		}
		free(user_data);
		user_data = NULL;
	}
}

char *
get_retinfo_from_retcode(unsigned short retcode)
{
	if(retcode == ACCESS_DENIED) {
		return "AccessDenied";
	}
	else if(retcode == BUCKET_ALREADY_EXISTS) {
		return "BucketAlreadyExists";
	}
	else if(retcode == BUCKET_NOT_EMPTY) {
		return "BucketNotEmpty";
	}
	else if(retcode == ENTITY_TOO_LARGE) {
		return "EntityTooLarge";
	}
	else if(retcode == ENTITY_TOO_SMALL) {
		return "EntityTooSmall";
	}
	else if(retcode == FILE_GROUP_TOO_LARGE) {
		return "FileGroupTooLarge";
	}
	else if(retcode == FILE_PART_NOT_EXIST) {
		return "FilePartNotExist";
	}
	else if(retcode == FILE_PART_STALE) {
		return "FilePartStale";
	}
	else if(retcode == INTERNAL_ERROR) {
		return "InternalError";
	}
	else if(retcode == INVALID_ACCESS_KEY_ID) {
		return "InvalidAccessKeyId";
	}
	else if(retcode == INVALID_ARGUMENT) {
		return "InvalidArgument";
	}
	else if(retcode == INVALID_BUCKET_NAME) {
		return "InvalidBucketName";
	}
	else if(retcode == INVALID_DIGEST) {
		return "InvalidDigest";
	}
	else if(retcode == INVALID_OBJECT_NAME) {
		return "InvalidObjectName";
	}
	else if(retcode == INVALID_PART) {
		return "InvalidPart";
	}
	else if(retcode == INVALID_PART_ORDER) {
		return "InvalidPartOrder";
	}
	else if(retcode == MALFORMED_XML) {
		return "MalformedXML";
	}
	else if(retcode == METHOD_NOT_ALLOWED) {
		return "MethodNotAllowed";
	}
	else if(retcode == MISSING_ARGUMENT) {
		return "MissingArgument";
	}
	else if(retcode == MISSING_CONTENT_LENGTH) {
		return "MissingContentLength";
	}
	else if(retcode == NO_SUCH_BUCKET) {
		return "NoSuchBucket";
	}
	else if(retcode == NO_SUCH_KEY) {
		return "NoSuchKey";
	}
	else if(retcode == NO_SUCH_UPLOAD) {
		return "NoSuchUpload";
	}
	else if(retcode == NOT_IMPLEMENTED) {
		return "NotImplemented";
	}
	else if(retcode == PRECONDITION_FAILED) {
		return "PreconditionFailed";
	}
	else if(retcode == REQUEST_TIME_TOO_SKEWED) {
		return "RequestTimeTooSkewed";
	}
	else if(retcode == REQUEST_TIMEOUT) {
		return "RequestTimeout";
	}
	else if(retcode ==SIGNATURE_DOES_NOT_MATCH) {
		return "SignatureDoesNotMatch";
	}
	else if(retcode == TOO_MANY_BUCKETS) {
		return "TooManyBuckets";
	} else {
		return "UnknowError";
	}
}

unsigned short get_retcode_from_retinfo(const char *retinfo) 
{
	OSS_ERROR_CODE retcode;
	if(strcmp(retinfo, "AccessDenied") == 0) {
		retcode = ACCESS_DENIED;
	}
	else if(strcmp(retinfo, "BucketAlreadyExists") == 0) {
		retcode = BUCKET_ALREADY_EXISTS;
	}
	else if(strcmp(retinfo, "BucketNotEmpty") == 0) {
		retcode = BUCKET_NOT_EMPTY;
	}
	else if(strcmp(retinfo, "EntityTooLarge") == 0) {
		retcode = ENTITY_TOO_LARGE;
	}
	else if(strcmp(retinfo, "EntityTooSmall") == 0) {
		retcode = ENTITY_TOO_SMALL;
	}
	else if(strcmp(retinfo, "FileGroupTooLarge") == 0) {
		retcode = FILE_GROUP_TOO_LARGE;
	}
	else if(strcmp(retinfo, "FilePartNotExist") == 0) {
		retcode = FILE_PART_NOT_EXIST;
	}
	else if(strcmp(retinfo, "FilePartStale") == 0) {
		retcode = FILE_PART_STALE;
	}
	else if(strcmp(retinfo, "InternalError") == 0) {
		retcode = INTERNAL_ERROR;
	}
	else if(strcmp(retinfo, "InvalidAccessKeyId") == 0) {
		retcode = INVALID_ACCESS_KEY_ID;
	}
	else if(strcmp(retinfo, "InvalidArgument") == 0) {
		retcode = INVALID_ARGUMENT;
	}
	else if(strcmp(retinfo, "InvalidBucketName") == 0) {
		retcode = INVALID_BUCKET_NAME;
	}
	else if(strcmp(retinfo, "InvalidDigest") == 0) {
		retcode = INVALID_DIGEST;
	}
	else if(strcmp(retinfo, "InvalidObjectName") == 0) {
		retcode = INVALID_OBJECT_NAME;
	}
	else if(strcmp(retinfo, "InvalidPart") == 0) {
		retcode = INVALID_PART;
	}
	else if(strcmp(retinfo, "InvalidPartOrder") == 0) {
		retcode = INVALID_PART_ORDER;
	}
	else if(strcmp(retinfo, "MalformedXML") == 0) {
		retcode = MALFORMED_XML;
	}
	else if(strcmp(retinfo, "MethodNotAllowed") == 0) {
		retcode = METHOD_NOT_ALLOWED;
	}
	else if(strcmp(retinfo, "MissingArgument") == 0) {
		retcode = MISSING_ARGUMENT;
	}
	else if(strcmp(retinfo, "MissingContentLength") == 0) {
		retcode = MISSING_CONTENT_LENGTH;
	}
	else if(strcmp(retinfo, "NoSuchBucket") == 0) {
		retcode = NO_SUCH_BUCKET;
	}
	else if(strcmp(retinfo, "NoSuchKey") == 0) {
		retcode = NO_SUCH_KEY;
	}
	else if(strcmp(retinfo, "NoSuchUpload") == 0) {
		retcode = NO_SUCH_UPLOAD;
	}
	else if(strcmp(retinfo, "NotImplemented") == 0) {
		retcode = NOT_IMPLEMENTED;
	}
	else if(strcmp(retinfo, "PreconditionFailed") == 0) {
		retcode = PRECONDITION_FAILED;
	}
	else if(strcmp(retinfo, "RequestTimeTooSkewed") == 0) {
		retcode = REQUEST_TIME_TOO_SKEWED;
	}
	else if(strcmp(retinfo, "RequestTimeout") == 0) {
		retcode = REQUEST_TIMEOUT;
	}
	else if(strcmp(retinfo, "SignatureDoesNotMatch") == 0) {
		retcode = SIGNATURE_DOES_NOT_MATCH;
	}
	else if(strcmp(retinfo, "TooManyBuckets") == 0) {
		retcode = TOO_MANY_BUCKETS;
	} else {
		retcode = 1000;
	}
	return retcode;
}

unsigned short  get_retcode(const char *response)
{
	unsigned short ret;
	assert(response != NULL);
	XmlNode *xml, *code_tag;
	char *retinfo;
	size_t response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	code_tag = xml_find(xml, "Code");
	retinfo = *code_tag->child->attrib;
	ret = get_retcode_from_retinfo(retinfo);
	xml_free(xml);
	return ret;
}


static oss_object_listing_t *
construct_list_objects_response(curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	int i;
	XmlNode *xml, *name_tag, *prefix_tag, *marker_tag, *max_keys_tag, *delimiter_tag, *is_truncated_tag, *next_marker_tag, *contents_tag, *common_prefixes_tag, *contents_tmp, *common_prefixes_tmp;
	size_t response_len = strlen(response); 
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
			object_listing->set_next_marker(object_listing, *next_marker_tag->child->attrib);
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
			(object_listing->_counts_summaries)++;
		}
		oss_object_summary_t **summaries = (oss_object_summary_t **)malloc(sizeof(oss_object_summary_t *) * (object_listing->_counts_summaries));
		for(i = 0; contents_tag != NULL; i++, contents_tag = contents_tag->next) {
			summaries[i] = object_summary_initialize();
			summaries[i]->set_key(summaries[i], *contents_tag->child->child->attrib);
			summaries[i]->set_last_modified(summaries[i], *contents_tag->child->next->child->attrib);
			summaries[i]->set_etag(summaries[i], *contents_tag->child->next->next->child->attrib);
			summaries[i]->set_type(summaries[i], *contents_tag->child->next->next->next->child->attrib);
			long size = atol(*contents_tag->child->next->next->next->next->child->attrib);
			summaries[i]->set_size(summaries[i], size);
			summaries[i]->set_storage_class(summaries[i], *contents_tag->child->next->next->next->next->next->child->attrib);
			summaries[i]->owner = owner_initialize_with_id(*contents_tag->child->next->next->next->next->next->next->child->child->attrib, *contents_tag->child->next->next->next->next->next->next->child->next->child->attrib);		
		}
		object_listing->summaries = summaries;
	}
	
	common_prefixes_tag = xml_find(xml, "CommonPrefixes");
	if(common_prefixes_tag != NULL) {
		common_prefixes_tmp = common_prefixes_tag;
		for(; common_prefixes_tmp != NULL; common_prefixes_tmp = common_prefixes_tmp->next) {
			(object_listing->_counts_common_prefixes)++;
		}
		char **common_prefixes = (char **)malloc(sizeof(char *) * (object_listing->_counts_common_prefixes));
		for(i = 0; common_prefixes_tag != NULL; i++, common_prefixes_tag = common_prefixes_tag->next) {
			const char *tmp = *common_prefixes_tag->child->child->attrib;
			size_t common_prefixes_len = strlen(tmp);
			common_prefixes[i] = (char *)malloc(sizeof(char) * (common_prefixes_len + 1));
			strncpy(common_prefixes[i], tmp, common_prefixes_len);
			common_prefixes[i][common_prefixes_len] = '\0';
		}
		object_listing->common_prefixes = common_prefixes;
	}
 
	free_user_data(user_data);
 	xml_free(xml);

	return object_listing;
	
}

static oss_access_control_list_t *
construct_get_bucket_acl_response(curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	XmlNode *xml, *grant_tag, *owner_tag;
	size_t response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	owner_tag = xml_find(xml, "Owner");
	oss_owner_t *owner = owner_initialize_with_id(*owner_tag->child->child->attrib, *owner_tag->child->next->child->attrib);

 	grant_tag = xml_find(xml, "Grant");
	
	oss_access_control_list_t *acl = access_control_list_initialize();
	acl->set_grant(acl, *grant_tag->child->attrib);
	acl->set_owner(acl, owner);
	
	free_user_data(user_data);
 	xml_free(xml);

	return acl;
	
}



static oss_bucket_t **
construct_list_buckets_response(curl_request_param_t *user_data,
		int *buckets_number)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	XmlNode *xml, *buckets_tag, *bucket_tag, *owner_tag;
	int i;
	size_t response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	*buckets_number = 0;
	
	owner_tag = xml_find(xml, "Owner");
	oss_owner_t *owner = owner_initialize_with_id(*owner_tag->child->child->attrib, *owner_tag->child->next->child->attrib);

 	buckets_tag = xml_find(xml, "Buckets");
	bucket_tag = buckets_tag->child;
	for(; bucket_tag != NULL; bucket_tag = bucket_tag->next) {
		(*buckets_number)++;
	}

	oss_bucket_t **buckets = (oss_bucket_t **)malloc(sizeof(oss_bucket_t *) * (*buckets_number));
	for(i = 0, bucket_tag = buckets_tag->child; i < *buckets_number; i++, bucket_tag = bucket_tag->next) {
		buckets[i] = bucket_initialize();
		buckets[i]->set_name(buckets[i], *bucket_tag->child->child->attrib);
		buckets[i]->set_create_date(buckets[i], *bucket_tag->child->next->child->attrib);
		buckets[i]->set_owner(buckets[i], owner);
	}
 
	free_user_data(user_data);
 	xml_free(xml);

	return buckets;
	
}



oss_bucket_t **
client_list_buckets(oss_client_t *client,
		int *buckets_number,
		unsigned short *retcode)
{
	assert(client != NULL);

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

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * 64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char *now;
	//char *header_auth;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/ds");
	sprintf(url, "%s", client->endpoint);
	sprintf(header_host,"Host: %s", client->endpoint);
	//sprintf(now, "%s", oss_get_gmt_time());
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

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
	
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
		*retcode = get_retcode(user_data->recv_buffer->ptr);
		free_user_data(user_data);
		buckets_number = 0;
		return NULL;
	} else {
		*retcode = 0;
		return construct_list_buckets_response(user_data, buckets_number);
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
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 4 * 1024;
	user_data->header_buffer->allocated = 4 * 1024;

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
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char *now;
	//char *header_auth;
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
	//sprintf(now, "%s", oss_get_gmt_time());
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
	 * 释放 http_headers资源
	 */
	curl_slist_free_all(http_headers);

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s", user_data->recv_buffer->ptr);
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
	//printf("response: %s\n", user_data->recv_buffer->ptr);

	if (user_data->header_buffer->code != 200) {
		//printf("Begin to parse response... \n");
		*retcode = get_retcode(user_data->recv_buffer->ptr);
		//printf("Finishing parsing response...\n");
	} else {
		*retcode = 0;
	}
	free_user_data(user_data);
}




void
client_create_bucket(oss_client_t *client,
		const char *bucket_name,
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
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 4 * 1024;
	user_data->header_buffer->allocated = 4 * 1024;

	size_t bucket_name_len = strlen(bucket_name);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) *bucket_name_len + 16 );

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * bucket_name_len +64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char *now;
	//char *header_auth;

	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	//sprintf(now, "%s", oss_get_gmt_time());
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);

	/**
	 * 请求头部构造
	 */
	oss_map_put(default_headers, OSS_DATE, now);
	
	/**
	 * 生成签名值
	 */
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

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
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
		*retcode = get_retcode(user_data->recv_buffer->ptr);
	} else {
		*retcode = 0;
	}
	free_user_data(user_data);
}


oss_object_listing_t *
client_list_objects_with_bucket_name(
		oss_client_t *client,
		const char *bucket_name,
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
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * 4 * 1024);
	user_data->header_buffer->fp = NULL;
	user_data->header_buffer->left = 4 * 1024;
	user_data->header_buffer->allocated = 4 * 1024;

	size_t bucket_name_len = strlen(bucket_name);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) *bucket_name_len + 16 );

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * bucket_name_len +64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
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
	//sprintf(now, "%s", oss_get_gmt_time());
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

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
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
		*retcode = get_retcode(user_data->recv_buffer->ptr);
		free_user_data(user_data);
		return NULL;
	} else {
		*retcode = 0;
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

	size_t bucket_name_len = strlen(bucket_name);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * bucket_name_len + 64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char *now;
	//char *header_auth;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s?acl", bucket_name);
	sprintf(url, "%s/%s?acl", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	//sprintf(now, "%s", oss_get_gmt_time());
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

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
	
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
	//printf("response = \n%s\n", user_data->recv_buffer->ptr);
	if (user_data->header_buffer->code != 200) {
		*retcode = get_retcode(user_data->recv_buffer->ptr);
		free_user_data(user_data);
		return NULL;
	} else {
		*retcode = 0;
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

	size_t bucket_name_len = strlen(bucket_name);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * bucket_name_len + 64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
	char header_auth[128] = {0};
	char *now;
	//char *header_auth;
	unsigned int sign_len = 0;

	oss_map_t *default_headers = oss_map_new(16);

	/**
	 * 构造参数，resource,url 赋值，
	 * */
	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	//sprintf(now, "%s", oss_get_gmt_time());
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

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
	
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
	//printf("response = \n%s\n", user_data->recv_buffer->ptr);
	if (user_data->header_buffer->code != 204) {
		*retcode = get_retcode(user_data->recv_buffer->ptr);
	} else {
		*retcode = NO_CONTENT;
	}
	free_user_data(user_data);
	return;
}


int main()
{
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	oss_client_t *client = client_initialize(access_id, access_key);
	unsigned short retcode;
	char *retinfo;
#if 1 
	/* *
	 * test list_buckets
	 */
	int buckets_number, i;
	oss_owner_t *owner;
	oss_bucket_t **buckets = client_list_buckets(client, &buckets_number, &retcode);
	if(buckets != NULL) {
		for(i = 0; i < buckets_number; i++) {
			printf("name = %s\tcreate_date = %s\n", buckets[i]->get_name(buckets[i]), buckets[i]->get_create_date(buckets[i]));
			owner = buckets[i]->get_owner(buckets[i]);
			printf("id = %s\tdisplay_name = %s\n", owner->get_id(owner), owner->get_display_name(owner));
		}
		printf("retcode = %d\n", retcode);
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error: %s\n", retinfo);
	}

	/* *
	 * test create_bucket
	 */
	const char *create_bucket_name = "&create_bucket_name";
	client_create_bucket(client, create_bucket_name, &retcode);
	if(retcode == 0) {
		printf("create bucket succeed.\n");
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
#endif
	/* *
	 * test set_bucket_acl
	 */
	

	client_set_bucket_acl(client, "bucketname1", "public-read-bad", &retcode);
	if(retcode == 0) {
		printf("set bucket acl succeed.\n");
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	/* *
	 * test get_bucket(list_objects)
	 */
#if 1 
	oss_object_listing_t *object_listing = client_list_objects_with_bucket_name(client, "&bucketname1", &retcode);
	
	if(retcode == 0) {
		printf("bucket_name = %s\nnext_marker = %s\nprefix = %s\nmarker = %s\nmax_keys = %d\ndelimiter = %s\nis_truncated = %d\n", object_listing->bucket_name, object_listing->next_marker, object_listing->prefix, object_listing->marker, object_listing->max_keys, object_listing->delimiter, object_listing->is_truncated);
		for(i = 0; i < object_listing->_counts_summaries; i++) {
			printf("type = %s\netag = %s\nkey = %s\nlast_modified = %s\nsize = %ld\nstorage_class = %s\nid = %s\ndisplay_name = %s\n", object_listing->summaries[i]->type, object_listing->summaries[i]->etag, object_listing->summaries[i]->key, object_listing->summaries[i]->last_modified, object_listing->summaries[i]->size, object_listing->summaries[i]->storage_class, object_listing->summaries[i]->owner->id, object_listing->summaries[i]->owner->display_name);
		}
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
#endif

	/* *
	 * test get_bucket_acl
	 */

	oss_access_control_list_t *acl = client_get_bucket_acl(client, "&bucketname1", &retcode);
	if(retcode == 0) {
		printf("grant = %s\nid = %s\ndisplay_name = %s\n", acl->grant, acl->owner->id, acl->owner->display_name);
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	/* *
	 * test delete_bucket
	 */

	client_delete_bucket(client, "bucketname2", &retcode);
	if(retcode == 1) {
		printf("delete bucket succeed.\n");
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}


	client_finalize(client);
	return 0;
}
