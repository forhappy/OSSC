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

size_t object_group_curl_operation_send_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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

size_t object_group_curl_operation_recv_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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

size_t object_group_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
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
		printf("INFO: %s\n", etag);
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
		} else if (strcmp(method, OSS_HTTP_DELETE) == 0 || strcmp(method, OSS_HTTP_HEAD) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
		}
		else if (strcmp(method, OSS_HTTP_GET) == 0) {
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, object_group_curl_operation_recv_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, object_group_curl_operation_header_callback);
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

static oss_get_object_group_index_result_t *
construct_get_object_group_index_response(curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	printf("response  = \n%s\n", response);
	int i;
	XmlNode *xml, *bucket_tag, *key_tag, *etag_tag, *length_tag, *file_part_tag, *part_tag, *part_tmp;
	size_t response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	oss_get_object_group_index_result_t *result = get_object_group_index_result_initialize();
	bucket_tag = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *bucket_tag->child->attrib);
	key_tag = xml_find(xml, "Key");
	result->set_key(result, *key_tag->child->attrib);
	etag_tag = xml_find(xml, "ETag");
	result->set_etag(result, *etag_tag->child->attrib);
	length_tag = xml_find(xml, "FileLength");
	size_t file_length = (size_t)atoi(*length_tag->child->attrib);
	result->set_file_length(result, file_length);
	
	file_part_tag = xml_find(xml, "FilePart");
	part_tag = file_part_tag->child;
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
			size_t part_size = (size_t)(atoi(*part_tag->child->next->next->next->child->attrib));
			group[i]->set_part_size(group[i], part_size);
		}
		result->group = group;
	}
	
	free_user_data(user_data);
 	xml_free(xml);

	return result;
	
}


#if 0

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

#endif

static oss_post_object_group_result_t *
construct_post_object_group_response(curl_request_param_t *user_data)
{
	const char *response = user_data->recv_buffer->ptr;
	assert(response != NULL);
	XmlNode *xml, *bucket_tag, *key_tag, *etag_tag, *size_tag;
	size_t response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	oss_post_object_group_result_t *result = post_object_group_result_initialize();
	bucket_tag = xml_find(xml, "Bucket");
	result->set_bucket_name(result, *bucket_tag->child->attrib);
	key_tag = xml_find(xml, "Key");
	result->set_key(result, *key_tag->child->attrib);
 	etag_tag = xml_find(xml, "ETag");
	result->set_etag(result, *etag_tag->child->attrib);
	size_tag = xml_find(xml,"Size");
	size_t size = (size_t)atoi(*size_tag->child->attrib);
	result->set_size(result, size);
 
	free_user_data(user_data);
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
	user_data->send_buffer->left = 128 * 1024;
	user_data->send_buffer->allocated = 128 * 1024;


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

	size_t bucket_name_len = strlen(request->bucket_name);
	size_t key_len = strlen(request->key);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
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
	sprintf(header_host,"Host: %s", client->endpoint);
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

	printf("tstr_part_item:\n%s\ntstr_part_item_length: %d\n",
			tstring_data(tstr_part_item), tstring_size(tstr_part_item));
	
	user_data->send_buffer->ptr = (char *)(tstring_data(tstr_part_item));

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
		return construct_post_object_group_response(user_data);
	}
}

/* *
 * 设置指定 Bucket 的 Access Control List(ACL)
 * */
#if 0

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


#endif
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
	size_t key_len = strlen(key);
	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16 );

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 64);

	char header_host[64]  = {0};
	char header_date[48]  = {0};
	//char now[32]          = {0}; /**< Fri, 24 Feb 2012 02:58:28 GMT */
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
	sprintf(header_host,"Host: %s", client->endpoint);
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

	//printf("%u\n", user_data->header_buffer->code);
	//printf("%s\n", user_data->recv_buffer->ptr);
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
		*retcode = get_retcode(user_data->recv_buffer->ptr);
		free_user_data(user_data);
		return NULL;
	} else {
		*retcode = 0;
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

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 64);

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
	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);
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
	object_group_curl_operation(OSS_HTTP_DELETE, resource, url, http_headers, user_data);

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
	if (user_data->header_buffer->code != 204) {
		*retcode = get_retcode(user_data->recv_buffer->ptr);
	} else {
		*retcode = NO_CONTENT;
	}
	free_user_data(user_data);
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

	size_t bucket_name_len = strlen(request->bucket_name);
	size_t key_len = strlen(request->key);

	/** 
	 * Resource: "/"
	 */
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);

	/**
	 * URL: "aliyun.storage.com" + resource
	 */
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 64);

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
	sprintf(resource, "/%s/%s", request->get_bucket_name(request),
			request->get_key(request));
	sprintf(url, "%s/%s/%s", client->endpoint, request->get_bucket_name(request),
			request->get_key(request));
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

	/**
	 * 发送请求
	 */
	object_group_curl_operation(OSS_HTTP_HEAD, resource, url, http_headers, user_data);

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
	printf("header = \n%s\n", user_data->header_buffer->ptr);
	if (user_data->header_buffer->code != 200) {
		*retcode = get_retcode(user_data->recv_buffer->ptr);
		free_user_data(user_data);
		return NULL;
	} else {
		*retcode = 0;
		return construct_head_object_group_response(user_data);
	}
}



#if 0
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

#endif

const char *etags[] = {
	"6F395BF16882D154CCB448806EA8C47D",
	"321F048D6C898398F3168545F6CE8551",
	"249697174471EEE73202E9214B037E08",
	"B78E7B470FA41AA5478CFB0EDFF7B2E6",
	"E0CA28D194BC790EF3C6236231D033A7"
};

const char *partname[] = {
	"a.txt",
	"b.txt",
	"c.txt",
	"d.txt",
	"e.txt"
};

int main()
{
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	const char *bucket_name = "bucketname2";
	const char *key = "a_group_file.dat";

	int i;
	oss_client_t *client = client_initialize(access_id, access_key);
	unsigned short retcode;
	char *retinfo;
	/* *
	 * test post_object_group
	 */
	oss_object_group_item_t **part_item = 
		(oss_object_group_item_t **)malloc(sizeof(oss_object_group_item_t *) * 5);
	for (i = 0; i < 5; i++) {
		*(part_item + i) = object_group_item_initialize(etags[i], partname[i], i + 1);
	}
	oss_post_object_group_request_t *request = 
		post_object_group_request_initialize(bucket_name, key, part_item, 5);
	oss_post_object_group_result_t *post_result = client_post_object_group(client, request, &retcode);
	if(retcode == 0) {
		printf("bucket_name = %s\nkey = %s\netag = %s\nsize = %u\n", post_result->bucket_name, post_result->key, post_result->etag, post_result->size);
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	for(i = 0; i < 5; i++) {
		object_group_item_finalize(part_item[i]);
	}
	free(part_item);


	post_object_group_request_finalize(request);
	post_object_group_result_finalize(post_result);
	/* *
	 * test get_object_group
	 */
#if 0
	/-------------- get object group left to haiping ----------------/
	oss_get_object_group_request_t *request = get_object_group_request_initialize(bucket_name, key);
	client_get_object_group(client, create_bucket_name, &retcode);
	if(retcode == 0) {
		printf("succeed.\n");
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

#endif

	/* *
	 * test get_object_group_index
	 */
	oss_get_object_group_index_result_t *index_result;
	index_result = client_get_object_group_index(client, bucket_name, key, &retcode);
	if(retcode == 0) {
		printf("bucket_name = %s\nkey = %s\netag = %s\nfile_length = %u\npart_number = %d\n", index_result->bucket_name, index_result->key, index_result->etag, index_result->file_length, index_result->part_number);
		for(i = 0; i < index_result->part_number; i++) {
			printf("etag = %s\npart_name = %s\npart_size = %u\npart_number = %d\n", (index_result->group)[i]->etag, (index_result->group)[i]->part_name, (index_result->group)[i]->part_size, (index_result->group)[i]->part_number);
		}
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}
	for(i = 0; i < index_result->part_number; i++) {
		multipart_object_group_finalize((index_result->group)[i]);
	}
	get_object_group_index_result_finalize(index_result);

	/* *
	 * test head_object_group
	 */

	oss_object_metadata_t *head_result = object_metadata_initialize();
	oss_get_object_group_request_t *head_request = get_object_group_request_initialize(bucket_name, key);
	head_result = client_head_object_group(client, head_request, &retcode);
	printf("content-length = %ld\ncontent-type = %s\nlast-modified = %s\n", head_result->get_content_length(head_result), head_result->get_content_type(head_result), head_result->get_last_modified(head_result));
	

	/* *
	 * test delete_object_group
	 */

	client_delete_object_group(client, bucket_name, key, &retcode);

	if(retcode == NO_CONTENT) {
		printf("delete object group succeed\n");
	} else {
		printf("retcode = %d\n", retcode);
		retinfo = get_retinfo_from_retcode(retcode);
		printf("error = %s\n", retinfo);
	}

	client_finalize(client);
	return 0;
}
