/*
 * =============================================================================
 *
 *       Filename:  oss_complete_multipart_upload_result.h
 *
 *    Description:  oss_complete_multipart_upload_result.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
# error Never include <ossc/modules/oss_complete_multipart_upload_result.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#define OSS_COMPLETE_MULTIPART_UPLOAD_RESULT_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_complete_multipart_upload_result_t oss_complete_multipart_upload_result_t
 * @{
 */

/**
 * struct oss_complete_multipart_upload_result_s 内部使用 \n
 * oss_complete_multipart_upload_result_t 开发者使用
 */
typedef struct oss_complete_multipart_upload_result_s oss_complete_multipart_upload_result_t;


/**
 * 包含完成一个Multipart上传事件的返回结果
 */
struct oss_complete_multipart_upload_result_s {
	char *bucket_name;  /**< Bucket 名称 */
	char *etag;         /**< Object 的MD5值 */
	char *key;          /**< Object 名称 */
	char *location;     /**< Object 的URL地址 */

	/**
	 * 获得Bucket的名称
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @return Bucket的名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_complete_multipart_upload_result_t *result);

	/**
	 * 设置Bucket的名称
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @param bucket_name [in] Bucket的名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_complete_multipart_upload_result_t *result, const char *bucket_name);

	/**
	 * 获得Object的MD5值
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @return Object的MD5值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_complete_multipart_upload_result_t *result);

	/**
	 * 设置Object的MD5值
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @param etag [in] Object的MD5值
	 * @retval void
	 */
	void (*set_etag)(oss_complete_multipart_upload_result_t *result, const char * etag);

	/**
	 * 获得Object的名称
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @return Object的名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_complete_multipart_upload_result_t *result);

	/**
	 * 设置Object的名称
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @param key [in] Object的名称
	 * @retval void
	 */
	void (*set_key)(oss_complete_multipart_upload_result_t *result, const char *key);

	/**
	 * 获得Object 的URL地址
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @return Object 的URL地址
	 * @retval const char *
	 */
	const char * (*get_location)(oss_complete_multipart_upload_result_t *result);

	/**
	 * 设置Object 的URL地址
	 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
	 * @param location [in] Object 的URL地址
	 * @retval void
	 */
	void (*set_location)(oss_complete_multipart_upload_result_t *result, const char *location);
};

/**
 * oss_complete_multipart_upload_result_t 构造函数
 * @return 返回一个oss_complete_multipart_upload_result_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_complete_multipart_upload_result_t *
complete_multipart_upload_result_initialize(void);


/**
 * oss_complete_multipart_upload_result_t 析构函数
 * @param result [in] 标识一个oss_complete_multipart_upload_result_t的结构指针
 * @retval void
 * @pre result 必须使用complete_multipart_upload_result_initialize的返回值
 */
extern void 
complete_multipart_upload_result_finalize(oss_complete_multipart_upload_result_t *result);
/**@}*/
#endif
