/*
 * =============================================================================
 *
 *       Filename:  client_upload_part-t.c
 *
 *    Description:  client_upload_part testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ossc/client.h>

#define _OSS_CLIENT_H
#include <modules/oss_client.h>
#undef _OSS_CLIENT_H
#include <lib/md5.h>

#include <curl/curl.h>

typedef struct _oss_upload_part_send_buffer_s _oss_upload_part_send_buffer_t;

struct _oss_upload_part_send_buffer_s {
	const char *ptr;
	size_t left;
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

void compute_md5_digest(void *ptr, size_t len)
{
	unsigned char md5_digest[17];
	memset(md5_digest, '\0', 17);
	md5_state_t md5_state;
	md5_init(&md5_state);
	md5_append(&md5_state, ptr, len);
	md5_finish(&md5_state, md5_digest);
	int i = 0;
	printf("---------MD5_Digest----------\n");
	for (; i < 16; i++) {
		printf("%0x", md5_digest[i]);
	}
	printf("\n");
}

#if 0
size_t client_upload_part_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t total = size * nmemb;
	printf("size * nmemb: %d\n", total);
	char * ret = mempcpy(ptr, stream, total);
	compute_md5_digest(ptr, r);
	return total;
}
#endif

size_t client_upload_part_callback(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	_oss_upload_part_send_buffer_t *send_buffer = (_oss_upload_part_send_buffer_t *)stream;
 
	size_t bytes_per_send = size * nmemb; 

	printf("^^^^^^^^^^^^^^^^^^^^^^^size * nmemb: %d^^^^^^^^^^^^^^^^^^^^^^^^^^^\n", bytes_per_send);
	if(bytes_per_send < 1)
		return 0;
	if(send_buffer->left > 0) {
		if (send_buffer->left > bytes_per_send) {
			memcpy(ptr, send_buffer->ptr, bytes_per_send);
			compute_md5_digest(ptr, bytes_per_send);
			send_buffer->ptr += bytes_per_send; /* advance pointer */
			send_buffer->left -= bytes_per_send; /* less data left */
			printf("buffer left: %d\n", send_buffer->left);
			printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
			return bytes_per_send;
		} else {
			memcpy(ptr, send_buffer->ptr, send_buffer->left);
			size_t last_sent_bytes = send_buffer->left;
			send_buffer->left -= bytes_per_send; /* less data left */
			compute_md5_digest(ptr, last_sent_bytes);
			printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
			return last_sent_bytes;
		}
	} else 
		return 0;
	// return 0; /* *never execute here, just to depress compiler wanings */
}

/* *
 * 拷贝 Object
 * */
oss_upload_part_result_t *
client_upload_part(oss_client_t *client, 
		oss_upload_part_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};
	char header_content_type[64] = {0};
	//char header_content_length[64] = {0};

	unsigned int sign_len = 0;
	int input_stream_len = 0;

	CURL *curl = NULL;

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

	_oss_upload_part_send_buffer_t *send_buffer = 
		(_oss_upload_part_send_buffer_t *)malloc(sizeof(_oss_upload_part_send_buffer_t));

	send_buffer->ptr = request->get_input_stream(request, &input_stream_len);
	send_buffer->left = request->get_part_size(request);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, request->get_part_size(request));
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, client_upload_part_callback);
		//curl_easy_setopt(curl, CURLOPT_READDATA, request->get_input_stream(request, &input_stream_len));
		curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);

		http_headers = curl_slist_append(http_headers, header_content_type);
		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);


		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}

	return NULL;
}

long file_size( FILE *fp )
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
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	const char *bucket_name = "bucketname1";
	const char *upload_id = "0004C96978130296AD8E152D1AAAB358";
	const char *key = "YellowSubmarine.tar.gz";
	FILE *file = fopen("yellow_submarine.tar.gz", "r");
	size_t file_len = file_size(file);
	//int fd = fileno(file);
	
	char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	memset(buffer, '\0', file_len + 1);
	fread(buffer, file_len, 1, file);
	const int single_request_len = 8 * 1024 * 1024;
	int requests_num = file_len / single_request_len;
	int current_part_number = 0;

	oss_client_t *client = client_initialize(access_id, access_key);
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
			client_upload_part(client, request, NULL);
			compute_md5_digest(buffer + current_part_number * single_request_len,
					single_request_len);
		} else {
			request->set_part_number(request, current_part_number + 1);
			request->set_input_stream(request,
					buffer + current_part_number *single_request_len,
					file_len - single_request_len * current_part_number);
			request->set_part_size(request, file_len - single_request_len * current_part_number);
			client_upload_part(client, request, NULL);
			compute_md5_digest(buffer + current_part_number * single_request_len,
					file_len- current_part_number * single_request_len);
		}
	}

}
