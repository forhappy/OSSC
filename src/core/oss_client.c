/*
 * =============================================================================
 *
 *       Filename:  oss_client.c
 *
 *    Description:  oss client
 *
 *        Created:  09/03/2012 09:18:04 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <curl/curl.h>

#include <ossc/client.h>
/**
 * 初始化 oss_client_t，内部使用
 */
oss_client_t *
_client_initialize(
		const char *access_id, unsigned int access_id_len,
		const char *access_key, unsigned int access_key_len,
		const char *endpoint, unsigned int endpoint_len)
{
	assert(access_id != NULL);
	assert(access_key != NULL);
	assert(endpoint != NULL);

	unsigned int byte_of_char = sizeof(char);
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

/**
 * 初始化 oss_client_t
 */
oss_client_t *
client_initialize(const char *access_id,
		const char *access_key)
{
	assert(access_id != NULL);
	assert(access_key != NULL);

	unsigned int access_id_len = strlen(access_id);
	unsigned int access_key_len = strlen(access_key);
	unsigned int endpoint_len = strlen(DEFAULT_OSS_HOST);

	return _client_initialize(access_id, access_id_len,
			access_key, access_key_len,
			DEFAULT_OSS_HOST, endpoint_len);
}

/**
 * 初始化 oss_client_t
 */
oss_client_t *
client_initialize_with_endpoint(const char *access_id,
		const char *access_key,
		const char *endpoint)
{

	assert(access_id != NULL);
	assert(access_key != NULL);
	assert(endpoint != NULL);

	unsigned int access_id_len = strlen(access_id);
	unsigned int access_key_len = strlen(access_key);
	unsigned int endpoint_len = strlen(endpoint); 
	return _client_initialize(access_id, access_id_len,
			access_key, access_key_len,
			endpoint, endpoint_len);
}

void client_finalize(oss_client_t *client) 
{
	assert(client != NULL);

	if (client->access_id != NULL) {
		free(client->access_id);
		client->access_id = NULL;
	}

	if (client->access_key != NULL) {
		free(client->access_key);
		client->access_key = NULL;
	}

	if (client->endpoint != NULL) {
		free(client->endpoint);
		client->endpoint = NULL;
	}

	free(client);
}
