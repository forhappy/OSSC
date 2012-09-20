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

//#define _OSS_CLIENT_H
//#include <modules/oss_client.h>
//#undef _OSS_CLIENT_H
#include <time.h>

#include <curl/curl.h>

int main()
{
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";
	oss_client_t *client = client_initialize(access_id, access_key);
	//unsigned short retcode;
	time_t t = time(NULL); 
	t += 60;
	char  expiration[20];
	sprintf(expiration, "%ld", t);
	printf("expiration = %s\n", expiration);
	
	char * url = client_generate_presigned_url_with_expiration(client, "bucketname2", "a.txt", expiration);
	printf("url = %s\n", url);
	if(url != NULL) {
		free(url);
		url = NULL;
	}

	url = client_generate_presigned_url_with_method(client, "bucketname2", "a.txt", expiration, "GET");
	printf("url = %s\n", url);
	if(url != NULL) {
		free(url);
		url = NULL;
	}

	
#if 0
	oss_generate_presigned_url_request_t *request = generate_presigned_url_request_initialize();
	request->set_bucket_name(request, "bucketname2");
	request->set_key(request, "a.txt");
	request->set_method(request, "GET");
	request->set_expiration(request, expiration);
	url = client_generate_presigned(client, request);
	printf("url = %s\n", url);
	if(url != NULL) {
		free(url);
		url = NULL;
	}


	char *now = (char *)oss_get_gmt_time();
	char date[20];
	sprintf(date, "Date: %s", now);

	CURL *curl = NULL;

	struct curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, "Host: aliyun.storage.com");
	http_headers = curl_slist_append(http_headers, date);

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "HEAD");
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	curl_slist_free_all(http_headers);
	if(now != NULL) {
		free(now);
		now = NULL;
	}
	if(url != NULL) {
		free(url);
		url = NULL;
	}
	if(now != NULL) {
		free(now);
		now = NULL;
	}
#endif

	client_finalize(client);
	return 0;
}
