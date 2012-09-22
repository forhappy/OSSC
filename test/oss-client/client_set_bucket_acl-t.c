/*
 * =============================================================================
 *
 *       Filename:  client_set_bucket_acl-t.c
 *
 *    Description:  client_set_bucket_acl testcase.
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

	size_t bucket_name_len = strlen(bucket_name);
	char *resource = (char *)malloc(sizeof(char) * bucket_name_len + 20);
	char *url = (char *)malloc(sizeof(char) * bucket_name_len + 40);
	char header_host[25] = {0};
	char header_date[50]  = {0};
	char now[30]          = {0};
	char header_auth[512]  = {0};
	char header_acl[30] = {0};
	//const char *bucket_name="";

	//char headers[2048] = {0};

	unsigned int sign_len = 0;

	CURL *curl = NULL;
	//CURLcode result;


	oss_map_t *default_headers = oss_map_new(16);
	oss_map_t *user_headers = oss_map_new(16);

	sprintf(resource, "/%s", bucket_name);
	sprintf(url, "%s/%s", client->endpoint, bucket_name);
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);
	sprintf(header_acl, "x-oss-acl: %s", acl);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(user_headers, OSS_ACL, acl);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, user_headers, resource, &sign_len);
	printf("sign: %s\n", sign);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);
		http_headers = curl_slist_append(http_headers, header_acl);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}

}

int main()
{
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	oss_client_t *client = client_initialize(access_id, access_key);
	//client_create_bucket(client, "bucketname002");
	//client_list_buckets(client);
	client_set_bucket_acl(client, "$bucketname1", "public-read", NULL);
}
