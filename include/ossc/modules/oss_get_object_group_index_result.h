/*
 * =============================================================================
 *
 *       Filename:  oss_get_object_group_index_result.h
 *
 *    Description:  oss get_object_group_index_result
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
# error Never include <ossc/modules/oss_get_object_group_index_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#define OSS_GET_OBJECT_GROUP_INDEX_RESULT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_MULTIPART_OBJECT_GROUP_H
#include <ossc/modules/oss_multipart_object_group.h>
#undef _OSS_MULTIPART_OBJECT_GROUP_H

/**
 * @defgroup oss_get_object_group_index_result_t oss_get_object_group_index_result_t
 * @{
 */

/**
 * struct oss_get_object_group_index_result_s 内部使用 \n
 * oss_get_object_group_index_result_t 开发者使用
 */
typedef struct oss_get_object_group_index_result_s oss_get_object_group_index_result_t;

/**
 * 用于返回 Object Group 中的 Object List 信息
 */
struct oss_get_object_group_index_result_s {
	char *bucket_name;               /**< Bucket名称 */
	char *key;                       /**< Object名称 */
	char *etag;                      /**< Object的MD5值 */
	oss_multipart_object_group_t **group;  /**< object group中的多个object列表 */
	int part_number;                 /**< object group中的多个object列表的个数 */
	size_t file_length;              /**< 文件长度 */

	/**
	 * 获得Bucket名称
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_get_object_group_index_result_t *result);

	/**
	 * 设置Bucket名称
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_get_object_group_index_result_t *result, const char *bucket_name);

	/**
	 * 获得Object名称
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_get_object_group_index_result_t *result);

	/**
	 * 设置Object名称
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_get_object_group_index_result_t *result, const char *key);

	/**
	 * 获得Object的MD5值
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @return Object的MD5值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_get_object_group_index_result_t *result);

	/**
	 * 设置Object的MD5值
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @param etag [in] Object的MD5值
	 * @retval void
	 */
	void (*set_etag)(oss_get_object_group_index_result_t *result, const char *etag);

	/**
	 * 获得object group中的多个object列表
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @param part_number [out] object group中的多个object列表的个数
	 * @return object group中的多个object列表
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_multipart_object_group_t ** (*get_group)(oss_get_object_group_index_result_t *result, int *part_number);

	/**
	 * 设置object group中的多个object列表
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @param group [in] object group中的多个object列表
	 * @param part_number [in] object group中的多个object列表的个数
	 * @retval void
	 */
	void (*set_group)(oss_get_object_group_index_result_t *result,
			oss_multipart_object_group_t **group, int part_number);
	
	/**
	 * 获得Object的文件长度
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @return Object的文件长度
	 * @retval size_t
	 */
	size_t (*get_file_length)(oss_get_object_group_index_result_t *result);

	/**
	 * 设置Object的文件长度
	 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
	 * @param file_length [in] Object的文件长度
	 * @retval void
	 */
	void (*set_file_length)(oss_get_object_group_index_result_t *result, size_t file_length);

};

/**
 * oss_get_object_group_index_result_t构造函数
 * @return 返回一个oss_get_object_group_index_result_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_get_object_group_index_result_t * get_object_group_index_result_initialize();

/**
 * oss_get_object_group_index_result_t析构函数
 * @param result [in] 标识一个oss_get_object_group_index_result_t结构指针
 * @retval void
 * @pre result 必须使用get_object_group_index_result_initialize的返回值
 */
extern void get_object_group_index_result_finalize(oss_get_object_group_index_result_t *result);
/**@}*/
#endif // OSS_GET_OBJECT_GROUP_INDEX_RESULT_H

