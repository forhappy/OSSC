/*
 * =============================================================================
 *
 *       Filename:  oss_helper.h
 *
 *    Description:  oss helper routines.
 *
 *        Created:  09/16/2012 09:42:36 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_HELPER_H
#define OSS_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ossc/util/oss_ttxml.h>
extern void
oss_free_partial_user_data(curl_request_param_t *user_data);

extern void
oss_free_partial_user_data_2nd(curl_request_param_t *user_data);

extern void
oss_free_user_data(curl_request_param_t *user_data);

extern const char *
oss_get_error_message_from_retcode(unsigned short retcode);

extern const char *
oss_why(unsigned short retcode);

extern unsigned short
oss_get_retcode_from_response(const char *response);

extern unsigned short
oss_get_retcode_from_retinfo(const char *retinfo);


#endif // OSS_HELPER_H
