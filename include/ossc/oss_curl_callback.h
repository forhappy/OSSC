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

extern unsigned int
bucket_curl_operation_send_callback(
		void *ptr, unsigned int size, 
		unsigned int nmemb, void *stream);

extern unsigned int
bucket_curl_operation_recv_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
bucket_curl_operation_header_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_send_from_file_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_send_from_buffer_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_recv_to_file_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_recv_to_buffer_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_recv_to_buffer_2nd_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_header_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
object_curl_operation_header_callback_2nd(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);


extern unsigned int
multipart_upload_curl_operation_send_from_file_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int
multipart_upload_curl_operation_send_from_buffer_callback(
		void *ptr, unsigned int size, unsigned int nmemb, void *stream);

extern unsigned int 
multipart_upload_curl_operation_recv_to_file_callback(
		void *ptr, unsigned int size, 
		unsigned int nmemb, void *stream);

extern unsigned int 
multipart_upload_curl_operation_recv_to_buffer_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int 
multipart_upload_curl_operation_header_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int 
object_group_curl_operation_recv_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

extern unsigned int 
object_group_curl_operation_header_callback(
		void *ptr, unsigned int size,
		unsigned int nmemb, void *stream);

#endif // OSS_CURL_CALLBACK_H
