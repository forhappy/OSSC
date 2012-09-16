/*
 * =============================================================================
 *
 *       Filename:  oss_curl_callback.c
 *
 *    Description:  oss curl callback routines.
 *
 *        Created:  09/16/2012 10:22:17 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

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

size_t
bucket_curl_operation_send_callback(
		void *ptr, size_t size, 
		size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t bytes_per_send = size * nmemb; 

	if(bytes_per_send < 1)
		return 0;
	if(send_buffer->left > 0) {
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

size_t
bucket_curl_operation_recv_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
	size_t bytes_per_recv = size * nmemb;
	if (recv_buffer->left > 0) {
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

size_t
bucket_curl_operation_header_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	int r;
	size_t code = 0;
	r = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (r != 0) {
		header_buffer->code= code;
	}
	return size * nmemb;
}

size_t
object_curl_operation_send_from_file_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fread(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t
object_curl_operation_send_from_buffer_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
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

size_t
object_curl_operation_recv_to_file_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fwrite(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t
object_curl_operation_recv_to_buffer_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
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

size_t
object_curl_operation_recv_to_buffer_2nd_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{

  size_t recv_size = size * nmemb;
  param_buffer_t *recv_buffer = (param_buffer_t *)stream;
 
  recv_buffer->ptr = realloc(recv_buffer->ptr, recv_buffer->allocated + recv_size + 1);
  if (recv_buffer->ptr == NULL) {
    /* out of memory! */ 
    exit(EXIT_FAILURE);
  }
  memcpy(&(recv_buffer->ptr[recv_buffer->allocated]), ptr, recv_size);
  recv_buffer->allocated += recv_size;
  recv_buffer->ptr[recv_buffer->allocated] = 0;
  return recv_size;
}

size_t
object_curl_operation_header_callback(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	char etag[48] = {0};
	int rcode = 0;
	int retag = 0;
	size_t code = 0;

	rcode = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (rcode != 0) {
		header_buffer->code= code;
	}

	retag = sscanf(ptr, "ETag: \"%s\"", etag);
	if (retag != 0) {
		memset(header_buffer->ptr, 0, header_buffer->allocated);
		strncpy(header_buffer->ptr, etag, 48);
	}
	return size * nmemb;
}

size_t
object_curl_operation_header_callback_2nd(
		void *ptr, size_t size,
		size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	char etag[48] = {0};
	char type[64] = {0};
	char length[16] = {0};
	char week[5] = {0};
	char day[3] = {0};
	char mon[4] = {0};
	char year[5] = {0};
	char time[9] = {0};
	char gmt[4] = {0};
	int rcode = 0;
	int retag = 0;
	int rtype = 0;
	int rlength = 0;
	int rlastmodified = 0;
	size_t code = 0;

	rcode = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (rcode != 0) {
		header_buffer->code= code;
	}

	retag = sscanf(ptr, "ETag: %s", etag);
	if (retag != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		retag = sprintf(header_buffer->ptr + offset, "Content-Md5#%s#", etag);
		header_buffer->left -= retag;
	}
	
	rtype = sscanf(ptr, "Content-Type: %s", type);
	if (rtype != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rtype = sprintf(header_buffer->ptr + offset, "Content-Type#%s#", type);
		header_buffer->left -= rtype;
	}
	
	rlength = sscanf(ptr, "Content-Length: %s", length);
	if (rlength != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rlength = sprintf(header_buffer->ptr + offset, "Content-Length#%s#", length);
		header_buffer->left -= rlength;
	}

	rlastmodified = sscanf(ptr, "Last-Modified: %s %s %s %s %s %s", week, day, mon, year, time, gmt);
	if (rlastmodified != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rlastmodified = sprintf(header_buffer->ptr + offset, "Last-Modified#%s %s %s %s %s %s#", week,
				day, mon, year, time, gmt);
		header_buffer->left -= rlastmodified;
	}
	return size * nmemb;
}


size_t multipart_upload_curl_operation_send_from_file_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fread(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t multipart_upload_curl_operation_send_from_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
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

size_t multipart_upload_curl_operation_recv_to_file_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *send_buffer = (param_buffer_t *)stream;
	size_t r = fwrite(ptr, size, nmemb, send_buffer->fp);
	return r;
}

size_t multipart_upload_curl_operation_recv_to_buffer_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
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

size_t multipart_upload_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
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

size_t object_group_curl_operation_recv_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *recv_buffer = (param_buffer_t *)stream;
	size_t bytes_per_recv = size * nmemb;
	//printf("INFO bytes received this time: %d\n", bytes_per_recv);
	//printf("INFO: bytes received:\n%s\n", (char *)ptr);
	if (recv_buffer->left > 0) {
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

size_t object_group_curl_operation_header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	param_buffer_t *header_buffer = (param_buffer_t *)stream;
	char etag[48] = {0};
	char type[64] = {0};
	char length[16] = {0};
	char week[5] = {0};
	char day[3] = {0};
	char mon[4] = {0};
	char year[5] = {0};
	char time[9] = {0};
	char gmt[4] = {0};
	int rcode = 0;
	int retag = 0;
	int rtype = 0;
	int rlength = 0;
	int rlastmodified = 0;
	size_t code = 0;

	rcode = sscanf(ptr, "HTTP/1.1 %u\n", &code);
	if (rcode != 0) {
		header_buffer->code= code;
	}

	retag = sscanf(ptr, "ETag: %s", etag);
	if (retag != 0) {
		printf("INFO: %s\n", etag);
		size_t offset = header_buffer->allocated - header_buffer->left;
		retag = sprintf(header_buffer->ptr + offset, "Content-Md5#%s#", etag);
		header_buffer->left -= retag;
	}
	
	rtype = sscanf(ptr, "Content-Type: %s", type);
	if (rtype != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rtype = sprintf(header_buffer->ptr + offset, "Content-Type#%s#", type);
		header_buffer->left -= rtype;
	}
	
	rlength = sscanf(ptr, "Content-Length: %s", length);
	if (rlength != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rlength = sprintf(header_buffer->ptr + offset, "Content-Length#%s#", length);
		header_buffer->left -= rlength;
	}

	rlastmodified = sscanf(ptr, "Last-Modified: %s %s %s %s %s %s", week, day, mon, year, time, gmt);
	if (rlastmodified != 0) {
		size_t offset = header_buffer->allocated - header_buffer->left;
		rlastmodified = sprintf(header_buffer->ptr + offset, "Last-Modified#%s %s %s %s %s %s#", week,
				day, mon, year, time, gmt);
		header_buffer->left -= rlastmodified;
	}
	return size * nmemb;

}

