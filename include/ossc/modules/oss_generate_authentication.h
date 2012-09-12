/*
 * =============================================================================
 *
 *       Filename:  generate_authentication.h
 *
 *    Description:  generate authentication
 *
 *        Created:  09/06/2012 06:09:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef GENERATE_AUTHENTICATION_H
#define GENERATE_AUTHENTICATION_H
#include "ossc-config.h"

/* *
 * 生成签名值，传入参数分别为：
 * @access_key：      用户的
 * @method：          请求方法
 * @default_headers： 默认请求头部
 * @user_headers：    用户自定义头部
 * @resource：        请求的资源
 * 
 * 该函数返回两个值，第一个返回值为签名结果，由函数直接返回，
 * 第二个返回值为 sign_len，由最后一个参数 sign_len 返回。
 * */
extern const char *
generate_authentication(const char *access_key, const char *method,
		oss_map_t *default_headers, oss_map_t *user_headers,
		const char *resource, unsigned int *sign_len);

#endif // GENERATE_AUTHENTICATION_H
