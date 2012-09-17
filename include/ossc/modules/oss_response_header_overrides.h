/*
 * =============================================================================
 *
 *       Filename:  oss_response_header_overrides.h
 *
 *    Description:  oss_response_header_overrides.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
//#ifndef _OSS_RESPONSE_HEADER_OVERRIDES_H
//# error Never include <modules/oss_response_header_overrides.h> directly, use <ossc/client.h> instead.
//#endif

#ifndef OSS_RESPONSE_HEADER_OVERRIDES_H
#define OSS_RESPONSE_HEADER_OVERRIDES_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_CONSTANTS_H
#include "ossc/oss_constants.h"
#undef _OSS_CONSTANTS_H

/**
 * @defgroup oss_response_header_overrides_t oss_response_header_overrides_t
 * @{
 */

/**
 * oss_response_header_overrides_t 包含了在发送OSS GET请求时可以重载的返回请求头
 */
typedef struct oss_response_header_overrides_s oss_response_header_overrides_t;

/**
 * oss_response_header_overrides_t 包含了在发送OSS GET请求时可以重载的返回请求头
 */
struct oss_response_header_overrides_s {
	char *cache_control; /**< 重载的cacheControl返回请求头*/
	char *content_disposition; /**< 重载的contentDisposition返回请求头*/
	char *content_encoding; /**< 重载的contentEncoding返回请求头*/
	char *content_language; /**< 重载的content language返回请求头*/
	char *content_type; /**< 重载的content type返回请求头*/
	char * expires; /**< 重载的expires返回请求头*/

	/**
	 * 返回重载的cacheControl返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @return 返回重载的cacheControl返回请求头
	 */
	const char * (*get_cache_control)(oss_response_header_overrides_t *overrides);

	/**
	 * 设置重载的cacheControl返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @param cache_control 重载的cacheControl返回请求头
	 */
	void (*set_cache_control)(oss_response_header_overrides_t *overrides,
			const char *cache_control);

	/**
	 * 返回重载的contentDisposition返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @return 返回重载的contentDisposition返回请求头
	 */
	const char * (*get_content_disposition)(oss_response_header_overrides_t *overrides);

	/**
	 * 设置重载的contentDisposition返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @param content_disposition 重载的contentDisposition返回请求头
	 */
	void (*set_content_disposition)(oss_response_header_overrides_t *overrides,
			const char *content_disposition);

	/**
	 * 返回重载的contentEncoding返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @return 返回重载的contentEncoding返回请求头
	 */
	const char * (*get_content_encoding)(oss_response_header_overrides_t *overrides);

	/**
	 * 设置重载的contentEncoding返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @param content_encoding 重载的contentEncoding返回请求头
	 */
	void (*set_content_encoding)(oss_response_header_overrides_t *overrides,
			const char *content_encoding);

	/**
	 * 返回重载的content language返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @return 返回重载的content language返回请求头
	 */
	const char * (*get_content_language)(oss_response_header_overrides_t *overrides);

	/**
	 * 设置重载的content language返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @param content_language 重载的content language返回请求头
	 */
	void (*set_content_language)(oss_response_header_overrides_t *overrides,
			const char *content_language);

	/**
	 * 返回重载的content type返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @return 返回重载的content type返回请求头
	 */
	const char * (*get_content_type)(oss_response_header_overrides_t *overrides);

	/**
	 * 设置重载的content type返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @param content_type 重载的content type返回请求头
	 */
	void (*set_content_type)(oss_response_header_overrides_t *overrides,
			const char *content_type);

	/**
	 * 返回重载的expires返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @return 返回重载的expires返回请求头
	 */
	const char * (*get_expires)(oss_response_header_overrides_t *overrides);

	/**
	 * 设置重载的expires返回请求头
	 * @param overrides 指向包含了在发送OSS GET请求时可以重载的返回请求头结构的指针
	 * @param expires 设置重载的expires返回请求头
	 */
	void (*set_expires)(oss_response_header_overrides_t *overrides,
			const char *expires);
};

/**
 * oss_response_header_overrides_t 构造函数
 * @return oss_response_header_overrides_t *指针
 * @retval 非空 表示成功
 * @retal NULL 表示失败
 */
extern oss_response_header_overrides_t *
response_header_overrides_initialize(void);

/**
 * oss_response_header_overrides_t 析构函数
 * @param overrides oss_response_header_overrides_t *指针
 * 该指针需由upload_part_request_initialize返回
 */
extern void 
response_header_overrides_finalize(oss_response_header_overrides_t *overrides);

/**@}*/
#endif
