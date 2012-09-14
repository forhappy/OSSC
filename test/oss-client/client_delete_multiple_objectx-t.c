/*
 * =============================================================================
 *
 *       Filename:  client-t.c
 *
 *    Description:  client testcase.
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

#include <util/oss_tstring.h>
#include <lib/md5.h>
#include <lib/base64.h>

#include <ossc/client.h>

#define _OSS_CLIENT_H
#include <modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>

const char *
compute_md5_digest(void *ptr, size_t len)
{
	char md5_digest[17];
	md5_state_t md5_state;

	char *base64_md5 = NULL;

	memset(md5_digest, '\0', 17);

	md5_init(&md5_state);
	md5_append(&md5_state, ptr, len);
	md5_finish(&md5_state, (md5_byte_t *)md5_digest);

	base64_md5 = (char *) malloc(sizeof(char) * 65);
	memset(base64_md5, 0, 65);
	base64_encode(md5_digest, 16, base64_md5, 65);

	return base64_md5;

}

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

size_t client_delete_multiple_object_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t r = size * nmemb;
	strncpy(stream, ptr, r);
	return r;
}

/* *
 * 初始化 oss_delete_multiple_object_request_t Object
 * */
oss_delete_multiple_object_request_t*
client_delete_multiple_object(oss_client_t *client,
		oss_delete_multiple_object_request_t *request,
		unsigned short *retcode)
{

	assert(client != NULL);


	char resource[256]     = {0};
	//char request_line[256] = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[128]  = {0};
	char now[128]          = {0};
	char header_auth[512]  = {0};
	char header_md5[512]  = {0};

	//char headers[1024] = {0};
	char delete_key[256] = {0};
	char response[1024] = {0};
	const char *content_md5 = NULL;

	unsigned int sign_len = 0;
	int keynums = 0;
	unsigned int i = 0;

	CURL *curl = NULL;
	//CURLcode result;


	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/?delete", request->get_bucket_name(request));
	sprintf(url, "%s/%s/?delete", client->endpoint, request->get_bucket_name(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	sprintf(now, "%s", oss_get_gmt_time());
	sprintf(header_date, "Date: %s", now);

	const char **p = request->get_keys(request, &keynums);

	tstring_t *key_list = 
		tstring_new("<Delete>");
	if (request->get_mode(request) == true) {
		sprintf(delete_key, "<Quiet>true</Quiet>");
	} else 
		sprintf(delete_key, "<Quiet>false</Quiet>");
	tstring_append(key_list, delete_key);

	for (i = 0; i < keynums; i++) {
		sprintf(delete_key, "<Object><Key>%s</Key></Object>", *(p + i));
		tstring_append(key_list, delete_key);
	}
	tstring_append(key_list, "</Delete>\n");

	printf("delete_key_list:\n%s\nkey_list_length: %d\n",
			tstring_data(key_list), tstring_size(key_list));


	content_md5 = compute_md5_digest(tstring_data(key_list), tstring_size(key_list));
	sprintf(header_md5, "Content-MD5: %s", content_md5);

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/x-www-form-urlencoded");
	oss_map_put(default_headers, OSS_CONTENT_MD5, content_md5);
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_POST,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);


	curl = curl_easy_init();
	if (curl != NULL) {
		struct curl_slist *http_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		//curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_INFILESIZE, tstring_size(key_list));
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, tstring_data(key_list));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, client_delete_multiple_object_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);


		http_headers = curl_slist_append(http_headers, header_host);
		http_headers = curl_slist_append(http_headers, header_md5);
		http_headers = curl_slist_append(http_headers, header_date);
		http_headers = curl_slist_append(http_headers, header_auth);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);

		curl_slist_free_all(http_headers);
		curl_easy_cleanup(curl);
	}
	printf("%s", response);

	return NULL;
}

const char *keys[] = {
	"putxxx.pdf",
	"put.png",
	"a_very_large_file.dat",
	"a_very_large_file.tar.bz2"
};

int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	const char *bucket_name = "bucketname001";


	oss_client_t *client = client_initialize(access_id, access_key);

	oss_delete_multiple_object_request_t *request = 
		delete_multiple_object_request_initialize(bucket_name, keys, 4, false);

	client_delete_multiple_object(client, request, NULL);
}
