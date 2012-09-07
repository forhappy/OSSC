/*
 * =============================================================================
 *
 *       Filename:  generate_authentication.h
 *
 *    Description:  generate authentication
 *
 *        Created:  09/06/2012 06:09:49 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef GENERATE_AUTHENTICATION_H
#define GENERATE_AUTHENTICATION_H

extern const char *
generate_authentication(const char *access_key, const char *method,
		oss_map_t *default_headers, oss_map_t *user_headers,
		const char *resource, unsigned int *sign_len);

#endif // GENERATE_AUTHENTICATION_H
