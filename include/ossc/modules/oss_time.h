/*
 * =============================================================================
 *
 *       Filename:  oss_time.h
 *
 *    Description:  oss time utility.
 *
 *        Created:  09/09/2012 12:13:50 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef oss_time_h
#define oss_time_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * 获取ANSI C格式时间
 */
extern const char * oss_get_asctime();

/**
 * 获取符合 HTTP/1.1 格式时间
 */
extern const char * oss_get_gmt_time();

#endif
