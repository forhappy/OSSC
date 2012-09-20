/*
 * =============================================================================
 *
 *       Filename:  oss_curl_callback.h
 *
 *    Description:  ossc curl callback routines.
 *
 *        Created:  09/16/2012 10:30:06 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_CURL_CALLBACK_H
#define OSS_CURL_CALLBACK_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern size_t
bucket_curl_operation_send_callback(
		void *ptr, size_t size, 
		size_t nmemb, void *stream);

extern size_t
bucket_curl_operation_recv_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
bucket_curl_operation_header_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_send_from_file_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_send_from_buffer_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_recv_to_file_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_recv_to_buffer_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_recv_to_buffer_2nd_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_header_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
object_curl_operation_header_callback_2nd(
		void *ptr, size_t size,
		size_t nmemb, void *stream);


extern size_t
multipart_upload_curl_operation_send_from_file_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t
multipart_upload_curl_operation_send_from_buffer_callback(
		void *ptr, size_t size, size_t nmemb, void *stream);

extern size_t 
multipart_upload_curl_operation_recv_to_file_callback(
		void *ptr, size_t size, 
		size_t nmemb, void *stream);

extern size_t 
multipart_upload_curl_operation_recv_to_buffer_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t 
multipart_upload_curl_operation_header_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t 
object_group_curl_operation_recv_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

extern size_t 
object_group_curl_operation_header_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream);

#endif // OSS_CURL_CALLBACK_H
