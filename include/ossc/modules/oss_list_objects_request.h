/*
 * =============================================================================
 *
 *       Filename:  oss_list_objects_request.h
 *
 *    Description:  list object request implementation. 
 *
 *        Created:  09/04/2012 10:54:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_LIST_OBJECTS_REQUEST_H
# error Never include <ossc/modules/oss_list_objects_request_h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_LIST_OBJECTS_REQUEST_H
#define OSS_LIST_OBJECTS_REQUEST_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @defgroup oss_list_objects_request_t oss_list_objects_request_t
 * @{
 */

/**
 * struct oss_list_objects_request_s 内部使用 \n
 * oss_list_objects_request_t 开发者使用
 */
typedef struct oss_list_objects_request_s oss_list_objects_request_t;

/**
 * 包含获取object列表的请求信息
 */
struct oss_list_objects_request_s {
	char *bucket_name;          /**< Bucket名称 */
	char *delimiter;            /**< 分隔符 */
	char *marker;               /**< 标识表示从哪里返回列表 */
	unsigned int max_keys;      /**< 限制的最大返回记录数 */
	char *prefix;               /**< 前缀 */

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_list_objects_request_t *request);

	/**
	 * 获得分隔符
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @return delimiter
	 * @retval const char *
	 */
	const char * (*get_delimiter)(oss_list_objects_request_t *request);

	/**
	 * 获得marker
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @return marker
	 * @retval const char *
	 */
	const char * (*get_marker)(oss_list_objects_request_t *request);

	/**
	 * 获得最大返回记录数
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @return max_keys
	 * @retval unsigned int
	 */
	unsigned int (*get_max_keys)(oss_list_objects_request_t *request);

	/**
	 * 获得前缀
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @return 前缀
	 * @retval const char *
	 */
	const char * (*get_prefix)(oss_list_objects_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_list_objects_request_t *request,
			const char *bucket_name);

	/**
	 * 设置分隔符
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @param delimiter [in] 分隔符
	 * @retval void
	 */
	void (*set_delimiter)(oss_list_objects_request_t *request,
			const char *delimiter);

	/**
	 * 设置marker
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @param marker [in] marker
	 * @retval void
	 */
	void (*set_marker)(oss_list_objects_request_t *request,
			const char *marker);

	/**
	 * 设置max_keys
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @param max_keys [in] max_keys
	 * @retval void
	 */
	void (*set_max_keys)(oss_list_objects_request_t *request,
			unsigned int max_keys);

	/**
	 * 设置前缀
	 * @param request [in] 标识一个oss_list_objects_request_t结构指针
	 * @param prefix [in] 前缀
	 * @retval void
	 */
	void (*set_prefix)(oss_list_objects_request_t *request,
			const char *prefix);
};

/**
 * oss_list_objects_request_t构造函数
 * @return 返回一个oss_list_objects_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_list_objects_request_t *
list_objects_request_initialize(void);

/**
 * oss_list_objects_request_t带bucket_name的构造函数
 * @param bucket_name [in] Bucket名称
 * @return 返回一个oss_list_objects_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_list_objects_request_t *
list_objects_request_initialize_with_bucket_name(const char *bucket_name);

/**
 * oss_list_objects_request_t带多参数的构造函数
 * @param bucket_name [in] Bucket名称
 * @param prefix [in] 前缀
 * @param marker [in] 标识表示从哪里返回列表
 * @param delimiter [in] 分隔符
 * @param max_keys [in] 限制的最大返回记录数
 * @return 返回一个oss_list_objects_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_list_objects_request_t *
list_objects_request_initialize_with_args(const char *bucket_name,
		const char *prefix,
		const char *marker,
		const char *delimiter,
		unsigned int max_keys);
/**
 * oss_list_objects_request_t析构函数
 * @param request [in] 标识一个oss_list_objects_request_t结构指针
 * @retval void
 * @pre request 必须使用list_objects_request_initialize的返回值
 */
extern void
list_objects_request_finalize(
		oss_list_objects_request_t *request);

/**@}*/
#endif // OSS_LIST_OBJECTS_REQUEST_H 

