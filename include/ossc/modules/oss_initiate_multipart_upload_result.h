/*
 * =============================================================================
 *
 *       Filename:  oss_initiate_multipart_upload_result.h
 *
 *    Description:  initiate multipart upload result implementation.
 *
 *        Created:  09/05/2012 12:09:28 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
# error Never include <ossc/modules/oss_initiate_multipart_upload_result.h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H 
#define OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include "ossc-config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @defgroup oss_initiate_multipart_upload_result_t oss_initiate_multipart_upload_result_t
 * @{
 */

/**
 * struct oss_initiate_multipart_upload_result_s 内部使用 \n
 * oss_initiate_multipart_upload_result_t 开发者使用
 */
typedef struct oss_initiate_multipart_upload_result_s oss_initiate_multipart_upload_result_t;

/**
 * 包含初始化一个Multipart上传事件的结果参数
 */
struct oss_initiate_multipart_upload_result_s {
	char *bucket_name;        /**< Bucket名称 */
	char *key;                /**< Object名称 */
	char *upload_id;          /** 标识Multipart上传事件的Upload ID */

	/**
	 * 获得Bucket名称
	 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_initiate_multipart_upload_result_t *result);

	/**
	 * 获得Object名称
	 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_initiate_multipart_upload_result_t *result);

	/**
	 * 获得Upload ID
	 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
	 * @return Upload ID
	 * @retval const char *
	 */
	const char * (*get_upload_id)(oss_initiate_multipart_upload_result_t *result);

	/**
	 * 设置Bucket名称
	 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_initiate_multipart_upload_result_t *result,
			const char *bucket_name);

	/**
	 * 设置Object名称
	 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_initiate_multipart_upload_result_t *result,
			const char *key);

	/**
	 * 设置Upload ID
	 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
	 * @param upload_id [in] Upload ID
	 * @retval void
	 */
	void (*set_upload_id)(oss_initiate_multipart_upload_result_t *result,
			const char *upload_id);
};

/**
 * oss_initiate_multipart_upload_result_t构造函数
 * @return 返回一个oss_initiate_multipart_upload_result_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_initiate_multipart_upload_result_t *
initiate_multipart_upload_result_initialize(void);

/**
 * oss_initiate_multipart_upload_result_t析构函数
 * @param result [in] 标识一个oss_initiate_multipart_upload_result_t结构指针
 * @retval void
 * @pre result 必须使用initiate_multipart_upload_result_initialize的返回值
 */
extern void
initiate_multipart_upload_result_finalize(
		oss_initiate_multipart_upload_result_t *result);
/**@}*/
#endif // OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
