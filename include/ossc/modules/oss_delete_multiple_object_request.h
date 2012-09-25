/*
 * =============================================================================
 *
 *       Filename:  oss_delete_multiple_object.h
 *
 *    Description:  oss_delete_multiple_object.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
# error Never include <ossc/modules/oss_delete_multiple_object.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H
#define OSS_DELETE_MULTIPLE_OBJECT_REQUEST_H 

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_delete_multiple_object_request_t oss_delete_multiple_object_request_t
 * @{
 */

/**
 * struct oss_delete_multiple_object_request_s 内部使用 \n
 * oss_delete_multiple_object_request_t 开发者使用
 */
typedef struct oss_delete_multiple_object_request_s oss_delete_multiple_object_request_t;

/**
 * 删除指定Bucket的多个Object
 */
struct oss_delete_multiple_object_request_s{
	char *bucket_name;   /**< Bucket名称 */
	char **keys;         /**< 多个Object的名称 */
	bool mode;           /**< 设置为 true 表示 quiet 模式，false 标识 verbose 模式 */
	int keynums;         /**< 要删除object的个数 */

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_delete_multiple_object_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_delete_multiple_object_request_t *request, const char *bucket_name);

	/**
	 * 获得多个Object的名称
	 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
	 * @param keynums [out] 要删除object的个数
	 * @return 多个Object的名称
	 * @retval const char **
	 */
	const char ** (*get_keys)(oss_delete_multiple_object_request_t *request, int *keynums);

	/**
	 * 设置多个Object的名称
	 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
	 * @param keys [in] 多个Object的名称
	 * @param keynums [in] 要删除object的个数
	 * @retval void
	 */
	void (*set_keys)(oss_delete_multiple_object_request_t *request, const char **keys, int keynums);

	/**
	 * 获得模式
	 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
	 * @return 模式
	 * @retval bool
	 */
	bool  (*get_mode)(oss_delete_multiple_object_request_t *request);

	/**
	 * 设置模式
	 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
	 * @param mode [in] 模式
	 * @retval void
	 */
	void (*set_mode)(oss_delete_multiple_object_request_t *request, bool mode);
};

/**
 * oss_delete_multiple_object_request_t 构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] 多个Object的名称
 * @param keynums [in] 要删除object的个数
 * @param mode [in] 模式
 * @return 返回一个oss_delete_multiple_object_request_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_delete_multiple_object_request_t *
delete_multiple_object_request_initialize(
		const char *bucket_name,
		const char **key,
		int keynums,
		bool mode);

/**
 * oss_delete_multiple_object_request_t 析构函数
 * @param request [in] 标识一个oss_delete_multiple_object_request_t结构指针
 * @retval void
 * @pre request 必须使用delete_multiple_object_request_initialize的返回值
 */
extern void 
delete_multiple_object_request_finalize(
		oss_delete_multiple_object_request_t *request);
/**@}*/
#endif
