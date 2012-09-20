/*
 * =============================================================================
 *
 *       Filename:  client_generate_url.c
 *
 *    Description:  client generate url.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include <ossc/client.h>
//#define _OSS_CLIENT_H
//#include <ossc/modules/oss_client.h>
//#undef _OSS_CLIENT_H

char *
client_generate_presigned_url_with_expiration(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *expiration)
{
	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(expiration != NULL);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 256);
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);
	unsigned int sign_len = 0;
	oss_map_t * default_headers = oss_map_new(1);

	/* *
	 * 构造各参数
	 */
	sprintf(resource, "/%s/%s", bucket_name, key);
	oss_map_put(default_headers, OSS_DATE, expiration);

	/**
	 * 生成签名值
	 */
	char * sign = generate_authentication(client->access_key, OSS_HTTP_GET,
			default_headers, NULL, resource, &sign_len);

	sprintf(url, "http://%s%s?OSSAccessKeyId=%s&Expires=%s&Signature=%s", client->endpoint, resource, client->access_id, expiration, sign);
	
	oss_map_delete(default_headers);
	if(sign != NULL) {
		free((char *)sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	
	return url;
}

char *
client_generate_presigned_url_with_method(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *expiration,
		const char *method)
{
	assert(client != NULL);
	assert(bucket_name != NULL);
	assert(key != NULL);
	assert(expiration != NULL);
	if((method == NULL) || !strcmp(method, ""))
		return client_generate_presigned_url_with_expiration(client,
				bucket_name, key, expiration);

	size_t bucket_name_len = strlen(bucket_name);
	size_t key_len = strlen(key);
	char *url = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 256);
	char *resource = (char *)malloc(sizeof(char) * (bucket_name_len + key_len) + 16);
	unsigned int sign_len = 0;
	oss_map_t * default_headers = oss_map_new(1);

	/* *
	 * 构造各参数
	 */
	sprintf(resource, "/%s/%s", bucket_name, key);
	oss_map_put(default_headers, OSS_DATE, expiration);

	/**
	 * 生成签名值
	 */
	char *sign = generate_authentication(client->access_key, method,
			default_headers, NULL, resource, &sign_len);

	sprintf(url, "http://%s%s?OSSAccessKeyId=%s&Expires=%s&Signature=%s", client->endpoint, resource, client->access_id, expiration, sign);
	
	oss_map_delete(default_headers);
	if(sign != NULL) {
		free(sign);
		sign = NULL;
	}
	if(resource != NULL) {
		free(resource);
		resource = NULL;
	}
	
	return url;

}
