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
#include <lib/md5.h>
#include <lib/base64.h>
extern long
oss_get_file_size(FILE *fp);

extern const char *
oss_compute_md5_digest(void *ptr, size_t len);

#endif // OSS_COMMON_H
