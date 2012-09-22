/*
 * =============================================================================
 *
 *       Filename:  client_copy_object-t.c
 *
 *    Description:  client_copy_object testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
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
size_t client_copy_object_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t r = size * nmemb;
	strncpy(stream, ptr, r);
	return r;
}

/* *
 * 拷贝 Object
 * */
oss_copy_object_result_t *
client_copy_object_ext(oss_client_t *client,
		const char *source_bucket_name,
		const char *source_key,
		const char *destination_bucket_name,
		const char *destination_key,
		unsigned short *retcode)
{

	assert(client != NULL);

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};
	char user_header_copy_source[128]  = {0};
	char header_copy_source[128] = {0};

	//char headers[1024] = {0};
	//char response[4096] = {0};

	unsigned int sign_len = 0;

	CURL *curl = NULL;
	//CURLcode result;


	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s", destination_bucket_name, destination_key);
	sprintf(url, "%s/%s/%s", client->endpoint, destination_bucket_name, destination_key);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);
	sprintf(user_header_copy_source, "/%s/%s", source_bucket_name, source_key);
	sprintf(header_copy_source, "%s: %s", OSS_COPY_SOURCE, user_header_copy_source);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_COPY_SOURCE, user_header_copy_source);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, user_headers, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, client_copy_object_callback);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);


		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);
		http_headers = curl_slist_append(http_headers, header_copy_source);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}
	//printf("%s", response);

	return NULL;
}

int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	const char *source_bucket_name = "bucketname001";
	const char *destination_bucket_name = "bucketname002";
	const char *source_key = "put.png";
	const char *destination_key = "PUT.png";
	oss_client_t *client = client_initialize(access_id, access_key);
	client_copy_object_ext(client, source_bucket_name, source_key,
			destination_bucket_name, destination_key, NULL);
}
