/*
 * =============================================================================
 *
 *       Filename:  oss_generate_presigned_url_request.h
 *
 *    Description:  oss_generate_presigned_url_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_GENERATE_PRESIGNED_URL_REQUEST_H
# error Never include <ossc/modules/oss_generate_presigned_url_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#define OSS_GENERATE_PRESIGNED_URL_REQUEST_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include "ossc/modules/oss_response_header_overrides.h"
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

#include <ossc/util/oss_map.h>

/**
 * @defgroup oss_generate_presigned_url_request_t oss_generate_presigned_url_request_t
 * @{
 */

/**
 * struct oss_generate_presigned_url_request_s 内部使用 \n
 * oss_generate_presigned_url_request_t 开发者使用
 */
typedef struct oss_generate_presigned_url_request_s oss_generate_presigned_url_request_t;

/**
 * 生成带有签名信息的URL的请求
 */
struct oss_generate_presigned_url_request_s {
	char *bucket_name;      /**< Bucket名称 */
	char *expiration;       /**< 过期时间 */
	char *key;              /**< Object名称 */
	char *method;           /**< HTTP方法 */
	oss_response_header_overrides_t *response_headers;   /**< 要重载的返回请求头 */
	oss_map_t *user_metadata;      /**< 用户自定义的元数据 */

	/**
	 * 增加用户自定义的元数据
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param key [in] 元数据的key
	 * @param value [in] 元数据的value
	 * @retval void
	 */
	void (*add_user_metadata)(oss_generate_presigned_url_request_t *request,
			const char *key, const char *value);

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_generate_presigned_url_request_t *request, const char *bucket_name);
	
	/**
	 * 获得过期时间
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @return 过期时间
	 * @retval const char *
	 */
	const char * (*get_expiration)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置过期时间
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param expiration [in] 过期时间
	 * @retval void
	 */
	void (*set_expiration)(oss_generate_presigned_url_request_t *request, const char *expiration);

	/**
	 * 获得Object名称
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置Object名称
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_generate_presigned_url_request_t *request, const char *key);

	/**
	 * 获得HTTP方法
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @return HTTP方法
	 * @retval const char *
	 */
	const char * (*get_method)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置HTTP方法
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param method [in] HTTP方法
	 * @retval void
	 */
	void (*set_method)(oss_generate_presigned_url_request_t *request, const char *method);

	/**
	 * 获得要重载的返回请求头
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @return 要重载的返回请求头
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_response_header_overrides_t * (*get_response_headers)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置要重载的返回请求头
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param response_headers [in] 要重载的返回请求头
	 * @retval void
	 */
	void (*set_response_headers)(oss_generate_presigned_url_request_t *request, oss_response_header_overrides_t *response_headers);

	/**
	 * 获得用户自定义的元数据
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @return 用户自定义的元数据
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_map_t * (*get_user_metadata)(oss_generate_presigned_url_request_t *request);

	/**
	 * 设置用户自定义的元数据
	 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
	 * @param user_metadata [in] 用户自定义的元数据
	 * @retval void
	 */
	void (*set_user_metadata)(oss_generate_presigned_url_request_t *request,
			oss_map_t *user_metadata);


};

/**
 * oss_generate_presigned_url_request_t 构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] Object名称
 * @return 返回一个oss_generate_presigned_url_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize(const char *bucket_name, const char *key);

/**
 * oss_generate_presigned_url_request_t带http方法的构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] Object名称
 * @param method [in] http方法
 * @return 返回一个oss_generate_presigned_url_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_generate_presigned_url_request_t *
generate_presigned_url_request_initialize_with_method(const char *bucket_name, const char *key, const char *method);

/**
 * oss_generate_presigned_url_request_t析构函数
 * @param request [in] 标识一个oss_generate_presigned_url_request_t结构指针
 * @retval void
 * @pre request 必须使用delete_multiple_object_request_initialize的返回值
 */
extern void 
generate_presigned_url_request_finalize(oss_generate_presigned_url_request_t *request);

/**@}*/
#endif
