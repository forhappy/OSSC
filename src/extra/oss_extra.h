/*
 * =============================================================================
 *
 *       Filename:  oss_extra.h
 *
 *    Description:  multithreaded upload and download operation.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_EXTRA_H
#define OSS_EXTRA_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "workqueue.h"

#define NUM_THREADS 1
#define BUFFER_SIZE (8 * 1024 * 1024)

extern workqueue_t workqueue;

typedef struct extra_buffer_s extra_buffer_t;
typedef struct extra_request_param_s extra_request_param_t;

struct extra_buffer_s {
	char *ptr; /**< 缓冲区首指针 */
	size_t left; /** 缓冲区剩余大小 */
	size_t allocated; /** 缓冲区总大小 */
	unsigned short code; /**返回码 */
};

struct extra_request_param_s {
	oss_client_t *client;
	char *upload_id;
	char *bucket_name;
	char *key;
	unsigned int part_number;
	char orig_md5[48];
	char echo_md5[48];
	extra_buffer_t *send_buffer;
	extra_buffer_t *header_buffer;
};

extern void
client_extra_put_object(oss_client_t *client,
		const char *bucket_name,
		const char *key,
		const char *local_file,
		unsigned short *retcode);
#endif // OSS_EXTRA_H
