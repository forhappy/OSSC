/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_request.h
 *
 *    Description:  oss_get_object_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_GET_OBJECT_REQUEST_H
# error Never include <ossc/modules/oss_get_object_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GET_OBJECT_REQUEST_H
#define OSS_GET_OBJECT_REQUEST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_RESPONSE_HEADER_OVERRIDES_H
#include <ossc/modules/oss_response_header_overrides.h>
#undef _OSS_RESPONSE_HEADER_OVERRIDES_H

/**
 * @defgroup oss_get_object_request_t oss_get_object_request_t
 * @{
 */

/**
 * struct oss_get_object_request_s 内部使用 \n
 * oss_get_object_request_t 开发者使用
 */
typedef struct oss_get_object_request_s oss_get_object_request_t;

/**
 * 指定从OSS下载Object的请求参数
 */
struct oss_get_object_request_s {
	char *bucket_name;                   /**< Bucket名称 */
	char *key;                           /**< Object名称 */
	char *modified_since_constraint;     /**< 一个时间，如果源Object自从该时间以后被修改过，则执行拷贝操作； 否则抛出异常 */
	long start;                         /**< Object内容的起始位置 */
	long length;                        /**< Object内容的长度 */
	oss_response_header_overrides_t *response_headers;  /**< 要重载的返回请求头 */
	char *unmodified_since_constraint;    /**< 一个时间，如果该时间等于或者晚于文件实际修改时间，则正常传输文件； 否则抛出异常 */
	char **matching_etag_constraints;     /**< ETag限定值的列表 */
	char **no_matching_etag_constraints;  /**< 限定Object的ETag限定必须不匹配给定值的列表 */

	unsigned int _counts_matching_etag_constraints;  /**< ETag限定值的列表的个数 */
	unsigned int _counts_no_matching_etag_constraints; /**< 限定Object的ETag限定必须不匹配给定值的列表的个数 */

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_get_object_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_get_object_request_t *request, const char *bucket_name);

	/**
	 * 获得Object名称
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_get_object_request_t *request);

	/**
	 * 设置Object名称
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_get_object_request_t *request, const char *key);

	/**
	 * 获得modified_since_constraint时间
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @return modified_since_constraint时间
	 * @retval const char *
	 */
	const char * (*get_modified_since_constraint)(oss_get_object_request_t *request);

	/**
	 * 设置modified_since_constraint时间
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param modified_since_constraint [in] modified_since_constraint时间
	 * @retval void
	 */
	void (*set_modified_since_constraint)(oss_get_object_request_t *request, const char *modified_since_constraint);

	/**
	 * 获得Object内容的start和length
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param start [out] Object内容的起始位置
	 * @param length [out] Object内容的长度
	 * @retval void
	 */
	void (*get_range)(oss_get_object_request_t *request, long *start, long *length);

	/**
	 * 设置Object内容的start和length
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param start [in] Object内容的起始位置
	 * @param length [in] Object内容的长度
	 * @retval void
	 */
	void (*set_range)(oss_get_object_request_t *request, long start, long length);

	/**
	 * 获得要重载的返回请求头
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @return 要重载的返回请求头
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_response_header_overrides_t * (*get_response_headers)(oss_get_object_request_t *request);

	/**
	 * 设置要重载的返回请求头
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param response_headers [in] 要重载的返回请求头
	 * @retval void
	 */
	void (*set_response_headers)(oss_get_object_request_t *request, oss_response_header_overrides_t *response_headers);

	/**
	 * 获得unmodified_since_constraint时间
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @return unmodified_since_constraint时间
	 * @retval const char *
	 */
	const char * (*get_unmodified_since_constraint)(oss_get_object_request_t *request);

	/**
	 * 设置unmodified_since_constraint时间
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param unmodified_since_constraint[in] unmodified_since_constraint时间
	 * @retval void
	 */
	void (*set_unmodified_since_constraint)(oss_get_object_request_t *request, const char *unmodified_since_constraint);

	/**
	 * 获得ETag限定值的列表
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param counts [out] ETag限定值的列表的个数
	 * @return ETag限定值的列表
	 * @retval const char **
	 */
	const char ** (*get_matching_etag_constraints)(oss_get_object_request_t *object,
			unsigned int *counts);

	/**
	 * 设置ETag限定值的列表
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param matching_etag_constraints [in] 设置ETag限定值的列表
	 * @param counts [in] ETag限定值的列表的个数
	 * @retval void
	 */
	void (*set_matching_etag_constraints)(oss_get_object_request_t *object,
			const char **matching_etag_constraints, unsigned int counts);

	/**
	 * 获得限定Object的ETag限定必须不匹配给定值的列表
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param counts [out] 限定Object的ETag限定必须不匹配给定值的列表的个数
	 * @return 限定Object的ETag限定必须不匹配给定值的列表
	 * @retval const char **
	 */
	const char ** (*get_no_matching_etag_constraints)(oss_get_object_request_t *object,
			unsigned int *counts);

	/**
	 * 设置限定Object的ETag限定必须不匹配给定值的列表
	 * @param request [in] 标识一个oss_get_object_request_t结构指针
	 * @param no_matching_etag_constraints [in] 限定Object的ETag限定必须不匹配给定值的列表
	 * @param counts [in] 限定Object的ETag限定必须不匹配给定值的列表的个数
	 * @retval void
	 */
	void (*set_no_matching_etag_constraints)(oss_get_object_request_t *object,
			const char **no_matching_etag_constraints, unsigned counts);
};

/**
 * oss_get_object_request_t构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] Object名称
 * @return 返回oss_get_object_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_get_object_request_t *
get_object_request_initialize(const char *bucket_name, const char *key);

/**
 * oss_get_object_request_t析构函数
 * @param request [in] 标识一个oss_get_object_request_t结构指针
 * @retval void
 * @pre request 必须使用get_object_request_initialize的返回值
 */
extern void 
get_object_request_finalize(oss_get_object_request_t *request);
/**@}*/
#endif
