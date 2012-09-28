/*
 * =============================================================================
 *
 *       Filename:  oss_common.h
 *
 *    Description:  oss common routines.
 *
 *        Created:  09/15/2012 10:23:16 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef OSS_COMMON_H
#define OSS_COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define MD5_BUFFER_SIZE (4 * 1024 * 1024)

extern long
oss_get_file_size(FILE *fp);

extern char *
oss_get_file_md5_digest(const char *file);

extern char *
oss_get_file_md5_digest_2nd(const char *file);

extern char *
oss_get_buffer_md5_digest(void *ptr, unsigned int len);

extern char *
oss_compute_md5_digest(void *ptr, unsigned int len);

#endif // OSS_COMMON_H
