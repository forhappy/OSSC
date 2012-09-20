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
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "workqueue.h"

#include <ossc/client.h>
#include <ossc/oss_helper.h>
#include <ossc/oss_curl_callback.h>
#include <ossc/oss_extra.h>
#include <ossc/util/oss_auth.h>
#include <ossc/util/oss_common.h>
#include <ossc/util/oss_ttxml.h>
#include <ossc/util/oss_time.h>

#define _OSS_CLIENT_H
#include <ossc/modules/oss_client.h>
#undef _OSS_CLIENT_H

#include <curl/curl.h>

workqueue_t workqueue;

static const char *
_extra_get_file_fingerprint(FILE *fp)
{
	assert(fp != NULL);

#define PAGE_SIZE 4096
	char file_begin[PAGE_SIZE] = {0};
	char *fingerprint = NULL;
	fread(file_begin, PAGE_SIZE, 1, fp);
	fingerprint = (char *)oss_compute_md5_digest(file_begin, PAGE_SIZE);
#undef PAGE_SIZE
	return fingerprint;
}

static void
_extra_remove_upload_metadir(const char *upload_metadir)
{
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	dir = opendir(upload_metadir);
	while((entry = readdir(dir)) != NULL) {
		if ((entry->d_name)[0] != '.') {
			char fnbuf[64] = {0};
			sprintf(fnbuf, "%s/%s", upload_metadir, entry->d_name);
			char *filename = (char *)malloc(sizeof(char) * (strlen(fnbuf) + 1));
			memset(filename, 0, strlen(fnbuf) + 1);
			strncpy(filename, fnbuf, strlen(fnbuf));
			unlink(filename);
		}
	}
	closedir(dir);
	rmdir(upload_metadir);
}

static void
_extra_free_user_data(extra_curl_request_param_t *user_data) 
{
	if(user_data != NULL) {
		if(user_data->send_buffer != NULL) {
			if(user_data->send_buffer->unmovable_buffer_ptr != NULL) {
				free(user_data->send_buffer->unmovable_buffer_ptr);
				user_data->send_buffer->unmovable_buffer_ptr = NULL;
			}
			free(user_data->send_buffer);
			user_data->send_buffer = NULL;
		}
		if(user_data->recv_buffer != NULL) {
			if(user_data->recv_buffer->unmovable_buffer_ptr!= NULL) {
				free(user_data->recv_buffer->unmovable_buffer_ptr);
				user_data->recv_buffer->unmovable_buffer_ptr = NULL;
			}
			free(user_data->recv_buffer);
			user_data->recv_buffer = NULL;
		}
		if(user_data->header_buffer != NULL) {
			if(user_data->header_buffer->unmovable_buffer_ptr != NULL) {
				free(user_data->header_buffer->unmovable_buffer_ptr);
				user_data->header_buffer->unmovable_buffer_ptr = NULL;
			}
			free(user_data->header_buffer);
			user_data->header_buffer = NULL;
		}
		free(user_data);
	}
}

static oss_upload_part_result_t *
_extra_upload_part_response(extra_curl_request_param_t *user_data)
{
	const char *etag = user_data->header_buffer->ptr;
	oss_upload_part_result_t *result = upload_part_result_initialize();
	result->set_etag(result, etag);
	_extra_free_user_data(user_data);

	return result;
}

static size_t 
_extra_curl_operation_send_from_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	extra_buffer_t *send_buffer = (extra_buffer_t*)stream;
	size_t bytes_per_send = size * nmemb; 

	if(bytes_per_send < 1)
		return 0;
	if((int)(send_buffer->left) > 0) {
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

static size_t
_extra_curl_operation_recv_to_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	extra_buffer_t *recv_buffer = (extra_buffer_t *)stream;
	size_t bytes_per_recv = size * nmemb;
	if ((int)(recv_buffer->left) > 0) {
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

static size_t
_extra_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	extra_buffer_t *header_buffer = (extra_buffer_t *)stream;
	char etag[48] = {0};
	int rcode = 0;
	int retag = 0;
	size_t code = 0;

	rcode = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (rcode != 0) {
		header_buffer->code= code;
	}

	retag = sscanf(ptr, "ETag: %s", etag);
	if (retag != 0) {
		memset(header_buffer->ptr, 0, header_buffer->allocated);
		strncpy(header_buffer->ptr, etag, 48);
	}
	return size * nmemb;
}
static void
_oss_extra_multipart_upload_curl_operation(const char *method,
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

	extra_curl_request_param_t *params = (extra_curl_request_param_t *)user_data;

	extra_buffer_t *send_buffer = params->send_buffer;
	extra_buffer_t *recv_buffer = params->recv_buffer;
	extra_buffer_t *header_buffer = params->header_buffer;

	curl = curl_easy_init();
	if (curl != NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURL_HTTP_VERSION_1_1, 1L);

		if (strcmp(method, OSS_HTTP_PUT) == 0) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE, send_buffer->allocated);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, _extra_curl_operation_send_from_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _extra_curl_operation_recv_to_buffer_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, _extra_curl_operation_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

}

static oss_upload_part_result_t *
_oss_extra_upload_part(oss_client_t *client, 
		oss_upload_part_request_t *request,
		unsigned short *retcode)
{
	assert(client != NULL);
	assert(request != NULL);
	int input_stream_len = 0;

	extra_curl_request_param_t *user_data = 
		(extra_curl_request_param_t *)malloc(sizeof(extra_curl_request_param_t));

	user_data->send_buffer = (extra_buffer_t*)malloc(sizeof(extra_buffer_t));
	user_data->send_buffer->ptr = (char *)request->get_input_stream(request, &input_stream_len);
	user_data->send_buffer->unmovable_buffer_ptr = user_data->send_buffer->ptr;
	user_data->send_buffer->left = request->get_part_size(request);
	user_data->send_buffer->allocated = request->get_part_size(request);

	user_data->recv_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
	user_data->recv_buffer->ptr = (char *)malloc(sizeof(char) * MAX_RECV_BUFFER_SIZE);
	user_data->recv_buffer->unmovable_buffer_ptr = user_data->recv_buffer->ptr;
	user_data->recv_buffer->left = MAX_RECV_BUFFER_SIZE;
	user_data->recv_buffer->allocated = MAX_RECV_BUFFER_SIZE;
	memset(user_data->recv_buffer->ptr, 0, MAX_RECV_BUFFER_SIZE);

	user_data->header_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
	user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
	user_data->header_buffer->unmovable_buffer_ptr = user_data->header_buffer->ptr;
	user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
	user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
	memset(user_data->header_buffer->ptr, 0, MAX_HEADER_BUFFER_SIZE);

	char resource[256]     = {0};
	char url[256]          = {0};
	char header_host[256]  = {0};
	char header_date[64]  = {0};
	char *now;
	char header_auth[128]  = {0};
	char header_content_type[64] = {0};
	unsigned int sign_len = 0;
	oss_map_t *default_headers = oss_map_new(16);

	sprintf(resource, "/%s/%s?partNumber=%d&uploadId=%s",
			request->get_bucket_name(request), request->get_key(request),
			request->get_part_number(request), request->get_upload_id(request));
	sprintf(url, "%s/%s/%s?partNumber=%d&uploadId=%s", client->endpoint, 
			request->get_bucket_name(request), request->get_key(request),
			request->get_part_number(request), request->get_upload_id(request));
	sprintf(header_host,"Host: %s", client->endpoint);
	now = (char *)oss_get_gmt_time();
	sprintf(header_date, "Date: %s", now);
	sprintf(header_content_type, "Content-Type: %s", "application/octet-stream");

	oss_map_put(default_headers, OSS_DATE, now);
	oss_map_put(default_headers, OSS_CONTENT_TYPE, "application/octet-stream");
	
	const char *sign = generate_authentication(client->access_key, OSS_HTTP_PUT,
			default_headers, NULL, resource, &sign_len);

	sprintf(header_auth, "Authorization: OSS %s:%s", client->access_id, sign);

	struct curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, header_content_type);
	http_headers = curl_slist_append(http_headers, header_host);
	http_headers = curl_slist_append(http_headers, header_date);
	http_headers = curl_slist_append(http_headers, header_auth);

	_oss_extra_multipart_upload_curl_operation(OSS_HTTP_PUT, resource, url, http_headers, user_data);
	
	curl_slist_free_all(http_headers);
	oss_map_delete(default_headers);

	if (user_data->header_buffer->code == 200) {
		if (retcode != NULL) *retcode = 0;
		oss_upload_part_result_t *result = 
			_extra_upload_part_response(user_data);
		result->set_part_number(result, request->get_part_number(request));
		return result;
	} else {
		if (retcode != NULL)
			*retcode = oss_get_retcode_from_response(user_data->recv_buffer->ptr);
		_extra_free_user_data(user_data);
	}
	return NULL;
}

static void
_extra_complete_multipart_upload(oss_client_t *client, const char *bucket_name,
		const char *key, const char *upload_id, const char *upload_metadir, unsigned short *retcode)
{
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	int parts = 0;
	dir = opendir(upload_metadir);
	while((entry = readdir(dir)) != NULL) {
		if ((entry->d_name)[0] != '.' 
				&& strncmp(entry->d_name, "ID", 2) != 0) {
			parts++;
		}
	}
	closedir(dir);

	oss_part_etag_t **part_etag = (oss_part_etag_t **)malloc(sizeof(oss_part_etag_t *) * parts);
	dir = opendir(upload_metadir);
	char fnbuf[64] = {0};
	while((entry = readdir(dir)) != NULL) {
		if ((entry->d_name)[0] != '.'
				&& strncmp(entry->d_name, "ID", 2) != 0) {
			sprintf(fnbuf, "%s/%s", upload_metadir, entry->d_name);
			size_t fnbuf_len = strlen(fnbuf);
			char *filename = (char *)malloc(sizeof(char) * (fnbuf_len + 1));
			memset(filename, 0, fnbuf_len + 1);
			strncpy(filename, fnbuf, fnbuf_len);
			FILE *fp = fopen(filename, "r");
			char etag_orig[48] = {0};
			char etag[48] = {0};
			int j= 0, k = 0;
			fread(etag_orig, 48, 1, fp);
			for (j = 0; j < strlen(etag_orig); j++) {
				if (etag_orig[j] != '"') {
					etag[k] = etag_orig[j];
					k++;
				}
			}
			*(part_etag + atoi(entry->d_name) - 1) = part_etag_initialize(atoi(entry->d_name), etag);
			free(filename);
			fclose(fp);
		}
	}
	closedir(dir);

	oss_complete_multipart_upload_request_t *request = 
		complete_multipart_upload_request_initialize(bucket_name, key, upload_id, part_etag, parts);
	client_complete_multipart_upload(client, request, retcode);

}

static void
do_upload_part(job_t *job_data)
{
	FILE *fp = NULL;
	char fnbuf[64] = {0};

	extra_request_param_t *param = 
		(extra_request_param_t *)(((job_t*)job_data)->user_data);

	oss_upload_part_request_t *request = upload_part_request_initialize();
	request->set_bucket_name(request, param->bucket_name);
	request->set_key(request, param->key);
	request->set_upload_id(request, param->upload_id);
	request->set_part_number(request, param->part_number);
	request->set_input_stream(request, param->send_buffer->ptr, param->send_buffer->allocated);
	request->set_part_size(request, param->send_buffer->allocated);

	/* 上传文件块 */
	oss_upload_part_result_t *result = _oss_extra_upload_part(param->client, request, NULL);

	sprintf(fnbuf, "%s/%d", param->metadir, result->get_part_number(result));
	size_t fnbuf_len = strlen(fnbuf);
	char *filename = (char *)malloc(sizeof(char) * (fnbuf_len + 1));
	memset(filename, 0, fnbuf_len + 1);
	strncpy(filename, fnbuf, fnbuf_len);

	fp = fopen(filename, "w");
	if (fp == NULL) return;
	fwrite(result->get_etag(result), strlen(result->get_etag(result)), 1, fp);
	fclose(fp);

	/* clean-ups goes here. */
	free(filename);
	if (param->send_buffer != NULL) {
		if (param->send_buffer->unmovable_buffer_ptr != NULL) {
			free(param->send_buffer->unmovable_buffer_ptr);
			param->send_buffer->unmovable_buffer_ptr = NULL;
		}
	}
	if (param->header_buffer != NULL) {
		if (param->header_buffer->unmovable_buffer_ptr != NULL) {
			free(param->header_buffer->unmovable_buffer_ptr);
			param->header_buffer->unmovable_buffer_ptr = NULL;
		}
	}
	free(param);
	free(job_data);
}

static void
upload_job_function(job_t *job_data)
{
	assert(job_data != NULL);
	do_upload_part(job_data);
}

static void *
worker_monitoring_thread(void *user_data)
{
	extra_worker_monitor_param_t *param = (extra_worker_monitor_param_t *)user_data;

	while(1) {
		int count = 0;
		DIR *dir = NULL;
		struct dirent *entry = NULL;
		dir = opendir(param->metadir);
		while((entry = readdir(dir)) != NULL) {
			if ((entry->d_name)[0] != '.') {
				count++;
			}
		}
		closedir(dir);
		if (count -1  == param->parts){
			workqueue_shutdown(&workqueue);
			//pthread_exit((void *)0);
			return 0;
		} else  sleep(5);
	}
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

	job_t *job;
	pthread_t worker_monitor;
	extra_worker_monitor_param_t *worker_monitor_param = NULL;

	FILE *fp = fopen(local_file, "r");
	if (fp == NULL) return;
	size_t file_len = oss_get_file_size(fp);

	/* 
	 * 文件块的个数，每个文件块大小为8M，如果文件大小为59M，则需要
	 *  59 / 8 + 1 = 8个块（parts）
	 * */
	int num_parts = file_len / PART_SIZE + 1;

	unsigned char *file_bitmap = (unsigned char *)malloc(
			sizeof(unsigned char) * BITNSLOTS(num_parts));
	/* 保存上传元信息的目录，目录名为文件开头的4096字节内容的MD5值 */
	char *upload_metadir = (char *)_extra_get_file_fingerprint(fp);

	worker_monitor_param = (extra_worker_monitor_param_t *)
		malloc(sizeof(extra_worker_monitor_param_t));
	worker_monitor_param->metadir = upload_metadir;
	worker_monitor_param->parts = num_parts;

	if (workqueue_init(&workqueue, NUM_THREADS)) {
		fprintf(stderr, "Failed to create work queue.\n");
		workqueue_shutdown(&workqueue);
		return;
	}

	if (access(upload_metadir, F_OK) == 0) {
		/* 该文件已经上传过，需断点续传 */
		char upload_id_fnbuf[64] = {0};
		char upload_id[48] = {0};

		/* 获取上次的Upload ID,Upload ID 存放在 upload_metadir/ID中 */
		sprintf(upload_id_fnbuf, "%s/ID", upload_metadir);

		size_t upload_id_fnbuf_len = strlen(upload_id_fnbuf);
		char *upload_id_filename = (char *)malloc(sizeof(char) * (upload_id_fnbuf_len + 1));
		memset(upload_id_filename, 0, upload_id_fnbuf_len + 1);
		strncpy(upload_id_filename, upload_id_fnbuf, upload_id_fnbuf_len);

		FILE *upload_id_fp = fopen(upload_id_filename, "r");
		if (upload_id_fp == NULL) return;
		fread(upload_id, 48, 1, upload_id_fp);
		free(upload_id_filename);
		fclose(upload_id_fp);

		/* 
		 * 用一个位数组bitmap标识哪些文件块已经上传，
		 * 这些块在本次上传过程中就不需要再次上传了
		 * */

		DIR *dir = NULL;
		struct dirent *entry = NULL;
		dir = opendir(upload_metadir);
		while((entry = readdir(dir)) != NULL) {
			if (((entry->d_name[0]) != '.')
					&&(strncmp(entry->d_name, "ID", 2) != 0)) {
				BITSET(file_bitmap, atoi(entry->d_name) - 1);
			}
		}
		/* 上传文件块，已成功上传的文件块不再重复上传 */
		int part_number = 0;
		while(part_number < num_parts) {
			if (BITTEST(file_bitmap, part_number)) {
				part_number++;
				continue; /* 已经上传的块 */
			}
			else {
				extra_request_param_t *user_data =
					(extra_request_param_t *)malloc(sizeof(extra_request_param_t));
				user_data->client = client;
				user_data->upload_id = (char *)upload_id;
				user_data->orig_md5 = NULL; /* 目前未使用 */
				user_data->echo_md5 = NULL;
				user_data->metadir = upload_metadir;

				size_t bucket_name_len = strlen(bucket_name);
				size_t key_len = strlen(key);

				user_data->bucket_name = (char *)malloc(sizeof(char) * (bucket_name_len + 1));
				memset(user_data->bucket_name, 0, bucket_name_len + 1);
				strncpy(user_data->bucket_name, bucket_name, bucket_name_len);
				user_data->key = (char *)malloc(sizeof(char) * (key_len + 1));
				memset(user_data->key, 0, key_len + 1);
				strncpy(user_data->key, key, key_len);
				user_data->part_number = part_number + 1;
				
				if (part_number != num_parts - 1) {
					user_data->send_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
					memset(user_data->send_buffer, 0, sizeof(extra_buffer_t));
					user_data->send_buffer->ptr = (char *)malloc(sizeof(char) * PART_SIZE);
					user_data->send_buffer->unmovable_buffer_ptr =  user_data->send_buffer->ptr;
					memset(user_data->send_buffer->ptr, 0, PART_SIZE);
					fseek(fp, part_number * PART_SIZE, SEEK_SET);
					fread(user_data->send_buffer->ptr, PART_SIZE, 1, fp);
					user_data->send_buffer->left = PART_SIZE;
					user_data->send_buffer->allocated = PART_SIZE;
					user_data->send_buffer->code = 0;
				} else {
					user_data->send_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
					memset(user_data->send_buffer, 0, sizeof(extra_buffer_t));
					user_data->send_buffer->ptr = (char *)malloc(sizeof(char) *
							(file_len - part_number * PART_SIZE));
					user_data->send_buffer->unmovable_buffer_ptr =  user_data->send_buffer->ptr;
					fseek(fp, part_number * PART_SIZE, SEEK_SET);
					fread(user_data->send_buffer->ptr, file_len - part_number * PART_SIZE, 1, fp);
					user_data->send_buffer->left = file_len - part_number * PART_SIZE;
					user_data->send_buffer->allocated = file_len - part_number * PART_SIZE;
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
				workqueue_add_job_ex(&workqueue, job);
			}
		}
		pthread_create(&worker_monitor, NULL, worker_monitoring_thread, worker_monitor_param);
		pthread_join(worker_monitor, NULL);
		
		_extra_complete_multipart_upload(client, bucket_name, key, upload_id, upload_metadir, retcode);
		free(worker_monitor_param);
		// workqueue_shutdown(&workqueue);
		fclose(fp);
	} else {
		/* 新上传文件 */
		/** 首先调用 client_initiate_multipart_upload 获取UploadID */
		unsigned int part_number = 0;
		oss_initiate_multipart_upload_request_t *request =
			initiate_multipart_upload_request_initialize(bucket_name, key);
		
		oss_initiate_multipart_upload_result_t *result = 
			client_initiate_multipart_upload(client, request, NULL);
		
		/* 保存Upload ID文件 */
		if (mkdir(upload_metadir, S_IRUSR | S_IWUSR | S_IXUSR) != 0) return;
		const char *upload_id = result->get_upload_id(result);
		char upload_id_fnbuf[64] = {0};

		sprintf(upload_id_fnbuf, "%s/ID", upload_metadir);
		size_t upload_id_fnbuf_len = strlen(upload_id_fnbuf);
		char *upload_id_filename = (char *)malloc(sizeof(char) * (upload_id_fnbuf_len + 1));
		memset(upload_id_filename, 0, upload_id_fnbuf_len + 1);
		strncpy(upload_id_filename, upload_id_fnbuf, upload_id_fnbuf_len);
		FILE *upload_id_fp = fopen(upload_id_filename, "w");
		if (upload_id_fp == NULL) return;
		fwrite(upload_id, strlen(upload_id), 1, upload_id_fp);
		free(upload_id_filename);
		fclose(upload_id_fp);

		while (part_number < num_parts) {

			extra_request_param_t *user_data =
				(extra_request_param_t *)malloc(sizeof(extra_request_param_t));
			user_data->client = client;
			user_data->upload_id = (char *)upload_id;
			user_data->orig_md5 = NULL;
			user_data->echo_md5 = NULL;
			user_data->metadir = upload_metadir;

			size_t bucket_name_len = strlen(bucket_name);
			size_t key_len = strlen(key);

			user_data->bucket_name = (char *)malloc(sizeof(char) * (bucket_name_len + 1));
			memset(user_data->bucket_name, 0, bucket_name_len + 1);
			strncpy(user_data->bucket_name, bucket_name, bucket_name_len);
			user_data->key = (char *)malloc(sizeof(char) * (key_len + 1));
			memset(user_data->key, 0, key_len + 1);
			strncpy(user_data->key, key, key_len);
			user_data->part_number = part_number + 1;

			
			if (part_number != num_parts - 1) {
				user_data->send_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
				memset(user_data->send_buffer, 0, sizeof(extra_buffer_t));
				user_data->send_buffer->ptr = (char *)malloc(sizeof(char) * PART_SIZE);
				user_data->send_buffer->unmovable_buffer_ptr =  user_data->send_buffer->ptr;
				memset(user_data->send_buffer->ptr, 0, PART_SIZE);
				fseek(fp, part_number * PART_SIZE, SEEK_SET);
				fread(user_data->send_buffer->ptr, PART_SIZE, 1, fp);
				user_data->send_buffer->left = PART_SIZE;
				user_data->send_buffer->allocated = PART_SIZE;
				user_data->send_buffer->code = 0;
			} else {
				user_data->send_buffer = (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
				memset(user_data->send_buffer, 0, sizeof(extra_buffer_t));
				user_data->send_buffer->ptr = (char *)malloc(sizeof(char) *
						(file_len - PART_SIZE * part_number));
				user_data->send_buffer->unmovable_buffer_ptr =  user_data->send_buffer->ptr;
				memset(user_data->send_buffer->ptr, 0, (file_len - part_number * PART_SIZE));
				fseek(fp, part_number * PART_SIZE, SEEK_SET);
				fread(user_data->send_buffer->ptr, file_len - part_number * PART_SIZE, 1, fp);
				user_data->send_buffer->left = file_len - part_number * PART_SIZE;
				user_data->send_buffer->allocated = file_len - part_number * PART_SIZE;
				user_data->send_buffer->code = 0;
			}

			user_data->header_buffer= (extra_buffer_t *)malloc(sizeof(extra_buffer_t));
			memset(user_data->header_buffer, 0, sizeof(extra_buffer_t));
			user_data->header_buffer->ptr = (char *)malloc(sizeof(char) * MAX_HEADER_BUFFER_SIZE);
			user_data->header_buffer->unmovable_buffer_ptr = user_data->header_buffer->ptr;
			user_data->header_buffer->left = MAX_HEADER_BUFFER_SIZE;
			user_data->header_buffer->allocated = MAX_HEADER_BUFFER_SIZE;
			user_data->header_buffer->code = 0;

			part_number++;
			
			if ((job = malloc(sizeof(*job))) == NULL) return;

			job->job_function = upload_job_function;
			job->user_data = user_data;
			workqueue_add_job_ex(&workqueue, job);
		}
		pthread_create(&worker_monitor, NULL, worker_monitoring_thread, worker_monitor_param);
		pthread_join(worker_monitor, NULL);
		
		_extra_complete_multipart_upload(client, bucket_name, key, upload_id, upload_metadir, retcode);
		free(worker_monitor_param);
		// workqueue_shutdown(&workqueue);
		fclose(fp);
	}

	free(file_bitmap);

	/* 退出时删除临时文件夹中的ID文件和Part-ETag文件 */
	_extra_remove_upload_metadir(upload_metadir);
	free(upload_metadir);
}
