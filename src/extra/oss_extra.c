/*
 * =============================================================================
 *
 *       Filename:  oss_extra.c
 *
 *    Description:  multithreaded upload and download operation.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oss_extra.h"
#include "workqueue.h"

#include <ossc/client.h>
#include <ossc/oss_helper.h>
#include <ossc/oss_curl_callback.h>
#include <ossc/util/oss_auth.h>
#include <ossc/util/oss_common.h>
#include <ossc/util/oss_ttxml.h>
#include <ossc/util/oss_time.h>

#define _OSS_CLIENT_H
#include <ossc/modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>

workqueue_t workqueue;

void workqueue_wait(workqueue_t *workqueue) {
	worker_t *worker = NULL;
	for (worker = workqueue->workers; worker != NULL; worker = worker->next) {
		pthread_join(worker->thread, NULL);
	}
}

static void do_upload_part(void *job_data)
{
	FILE *fp = NULL;
	char metadir[8] = "UPLOAD";
	char filename[32] = {0};

	extra_request_param_t *param = 
		(extra_request_param_t *)(((job_t*)job_data)->user_data);

	printf("Part Number%d\n", param->part_number);

	printf("Begin to initialize upload_part_request_initialize\n");
	oss_upload_part_request_t *request = upload_part_request_initialize();
	request->set_bucket_name(request, param->bucket_name);
	request->set_key(request, param->key);
	request->set_upload_id(request, param->upload_id);
	request->set_part_number(request, param->part_number);
	request->set_input_stream(request, param->send_buffer->ptr, param->send_buffer->allocated);
	printf("End to initialize upload_part_request_initialize\n");
	request->set_part_size(request, param->send_buffer->allocated);
	printf("Beginning to upload part: %d\n", param->part_number);
	oss_upload_part_result_t *result = client_upload_part(param->client, request, NULL);
	sprintf(filename, "%s/%d", metadir, result->get_part_number(result));
	fp = fopen("filename", "w");
	if (fp == NULL) return;
	fwrite(result->get_etag(result), strlen(result->get_etag(result)), 1, fp);
	fclose(fp);
}

static void
upload_job_function(void *job_data)
{
	// assert(user_data != NULL);
	do_upload_part(job_data);
}


void 
client_extra_put_object(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *local_file,
		unsigned short *retcode)
{
	assert(client != NULL);
	if (bucket_name == NULL) return;
	if (key == NULL) return;
	if (local_file == NULL) return;

	unsigned int part_number = 0;
	job_t *job;

	FILE *fp = fopen(local_file, "r");
	if (fp == NULL) return;
	size_t file_len = oss_get_file_size(fp);

	/* 
	 * 缓冲区个数，每个缓冲区大小为8M，如果文件大小为59M，则需要
	 * 开辟 59 / 8 + 1 = 8个
	 * */
	int num_buffers = file_len / BUFFER_SIZE + 1;

	char **buffer = (char **)malloc(sizeof(char *) * num_buffers * BUFFER_SIZE);
	for (int i = 0; i < num_buffers; i++) {
		if (i != num_buffers - 1) {
			*(buffer + i) = (char *)malloc(sizeof(char) * BUFFER_SIZE);
			fread(*(buffer + i), 1, BUFFER_SIZE, fp);
		} else {
			*(buffer + i) = (char *)malloc(sizeof(char) * (file_len - BUFFER_SIZE * i));
			fread(*(buffer + i), 1, (file_len - BUFFER_SIZE * i), fp);
		}
	}

	/** 首先调用 client_initiate_multipart_upload 获取UploadID */
	oss_initiate_multipart_upload_request_t *request =
		initiate_multipart_upload_request_initialize(bucket_name, key);

	oss_initiate_multipart_upload_result_t *result = 
		client_initiate_multipart_upload(client, request, NULL);

	const char *upload_id = result->get_upload_id(result);

	if (workqueue_init(&workqueue, NUM_THREADS)) {
		fprintf(stderr, "Failed to create work queue.\n");
		workqueue_shutdown(&workqueue);
		return;
	}


	while (part_number < num_buffers) {

		extra_request_param_t *user_data =
			(extra_request_param_t *)malloc(sizeof(extra_request_param_t));
		user_data->client = client;
		user_data->upload_id = (char *)upload_id;
		size_t bucket_name_len = strlen(bucket_name);
		size_t key_len = strlen(key);

		user_data->bucket_name = (char *)malloc(sizeof(char) * (bucket_name_len + 1));
		memset(user_data->bucket_name, 0, bucket_name_len + 1);
		strncpy(user_data->bucket_name, bucket_name, bucket_name_len);
		user_data->key = (char *)malloc(sizeof(char) * (key_len + 1));
		memset(user_data->key, 0, key_len + 1);
		strncpy(user_data->key, key, key_len);
		user_data->part_number = part_number + 1;

		
		if (part_number != num_buffers - 1) {
			user_data->send_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
			memset(user_data->send_buffer, 0, sizeof(extra_buffer_t));
			user_data->send_buffer->ptr = *(buffer + part_number);
			user_data->send_buffer->left = BUFFER_SIZE;
			user_data->send_buffer->allocated = BUFFER_SIZE;
			user_data->send_buffer->code = 0;
		} else {
			user_data->send_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
			memset(user_data->send_buffer, 0, sizeof(extra_buffer_t));
			user_data->send_buffer->ptr = *(buffer + part_number);
			user_data->send_buffer->left = file_len - part_number * BUFFER_SIZE;
			user_data->send_buffer->allocated = file_len - part_number * BUFFER_SIZE;
			user_data->send_buffer->code = 0;

		}

		user_data->header_buffer= (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
		memset(user_data->header_buffer, 0, sizeof(extra_buffer_t));
		user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
		user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
		user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
		user_data->header_buffer->code = 0;

		part_number++;
		
		if ((job = malloc(sizeof(*job))) == NULL) return;

		job->job_function = upload_job_function;
		job->user_data = user_data;
		workqueue_add_job(&workqueue, job);
	}

	workqueue_wait(&workqueue);

	workqueue_shutdown(&workqueue);
	fclose(fp);

}
