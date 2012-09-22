/*
 * =============================================================================
 *
 *       Filename:  client_put_object-t.c
 *
 *    Description:  client_put_object testcase.
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

size_t client_put_object_callback_write(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	size_t r = size * nmemb;
	memcpy(stream, ptr, r);
	return r;
}

size_t client_put_object_callback_read(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	size_t r = size * nmemb;
	read(*(int *)stream, ptr, r);
	return r;
}

size_t client_put_object_callback_header(void *ptr,
		size_t size,
		size_t nmemb,
		void *stream)
{
	sscanf(ptr, "ETag: %s\n", (char *)stream);
	return size * nmemb;
}
/* *
 * 拷贝 Object
 * */
oss_put_object_result_t *
client_put_object(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		void *input,
		oss_object_metadata_t *metadata,
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
	char response[4096] = {0};

	unsigned int sign_len = 0;

	CURL *curl = NULL;

	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s", bucket_name, key);
	sprintf(url, "%s/%s/%s", client->endpoint, bucket_name, key);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);
	//sprintf(header_content_length, "Content-Length: %ld", metadata->get_content_length(metadata));
	sprintf(header_content_type, "Content-Type: %s", metadata->get_content_type(metadata));

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, metadata->get_content_type(metadata));
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, metadata->get_content_length(metadata));
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, client_put_object_callback_read);
		curl_easy_setopt(curl, CURLOPT_READDATA, (int *)input);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, client_put_object_callback_header);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, response);

		//http_headers = curl_slist_append(http_headers, header_content_length);
		http_headers = curl_slist_append(http_headers, header_content_type);
		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);


		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}
	printf("RESPONSE ETag:\n%s\n", response);

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
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	const char *bucket_name = "bucketname001";
	const char *key = "putxxx.pdf";
	FILE *file = fopen("proactor.pdf", "r");
	size_t file_len = file_size(file);
	int fd = fileno(file);
	//char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	//memset(buffer, '\0', file_len + 1);
	//fread(buffer, file_len, 1, file);

	oss_client_t *client = client_initialize(access_id, access_key);
	oss_object_metadata_t *metadata = object_metadata_initialize();
	metadata->set_content_length(metadata, file_len);
	metadata->set_content_type(metadata, "application/octet-stream");

	client_put_object(client, bucket_name, key,
			&fd, metadata, NULL);
	fclose(file);
}
