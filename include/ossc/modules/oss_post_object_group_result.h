/*
 * =============================================================================
 *
 *       Filename:  oss_post_object_group_result.h
 *
 *    Description:  oss post_object_group_result
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_POST_OBJECT_GROUP_RESULT_H
# error Never include <ossc/modules/oss_post_object_group_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_POST_OBJECT_GROUP_RESULT_H
#define OSS_POST_OBJECT_GROUP_RESULT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 * @defgroup oss_post_object_group_result_t oss_post_object_group_result_t
 * @{
 */

/**
 * struct oss_post_object_group_result_s 内部使用 \n
 * oss_post_object_group_result_t 开发者使用
 */
typedef struct oss_post_object_group_result_s oss_post_object_group_result_t;

/**
 * 创建对象组（Object Group）返回的结果
 */
struct oss_post_object_group_result_s {
	char *bucket_name;      /**< Bucket名称 */
	char *key;              /**< Object名称*/
	char *etag;             /**< Object的etag值*/
	size_t size;            /**< Object的大小*/

	/**
	 * 获得Bucket名称
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_post_object_group_result_t *result);

	/**
	 * 设置Bucket名称
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_post_object_group_result_t *result,
			const char *bucket_name);

	/**
	 * 获得Object名称
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_post_object_group_result_t *result);

	/**
	 * 设置Object名称
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_post_object_group_result_t *result,
			const char *key);

	/**
	 * 获得Object的etag值
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @return Object的etag值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_post_object_group_result_t *result);

	/**
	 * 设置Object的etag值
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @param etag [in] Object的etag值
	 * @retval void
	 */
	void (*set_etag)(oss_post_object_group_result_t *result, const char *etag);

	/**
	 * 获得Object的大小
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @return Object的大小
	 * @retval size_t
	 */
	size_t (*get_size)(oss_post_object_group_result_t *result);

	/**
	 * 设置Object的大小
	 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
	 * @param size [in] Object的大小
	 * @retval void
	 */
	void (*set_size)(oss_post_object_group_result_t *result, size_t size);

};


/**
 * oss_post_object_group_result_t构造函数
 * @return 返回一个oss_post_object_group_result_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_post_object_group_result_t *
post_object_group_result_initialize();

/**
 * oss_post_object_group_result_t析构函数
 * @param result [in] 标识一个oss_post_object_group_result_t结构指针
 * @retval void
 * @pre result 必须使用post_object_group_result_initialize的返回值
 */
extern void
post_object_group_result_finalize(oss_post_object_group_result_t *result);

/**@}*/
#endif // OSS_POST_OBJECT_GROUP_RESULT_H

