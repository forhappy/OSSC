/*
 * =============================================================================
 *
 *       Filename:  oss_initiate_multipart_upload_request.h
 *
 *    Description:  initiate multipart upload request.
 *
 *        Created:  09/05/2012 02:32:01 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
# error Never include <ossc/modules/oss_initiate_multipart_upload_request.h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H 
#define OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include "ossc-config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _OSS_OBJECT_METADATA_H
#include <ossc/modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

/**
 * @defgroup oss_initiate_multipart_upload_request_t oss_initiate_multipart_upload_request_t
 * @{
 */

/**
 * struct oss_initiate_multipart_upload_request_s 内部使用 \n
 * oss_initiate_multipart_upload_request_t 开发者使用
 */
typedef struct oss_initiate_multipart_upload_request_s oss_initiate_multipart_upload_request_t;

/**
 * 包含初始化一个Multipart上传事件的请求参数
 */
struct oss_initiate_multipart_upload_request_s {
	char *bucket_name;       /**< Bucket名称 */
	char *key;               /**< Object名称 */
	oss_object_metadata_t *object_metadata;     /**< Object的元信息 */

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_initiate_multipart_upload_request_t *request);

	/**
	 * 获得Object名称
	 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_initiate_multipart_upload_request_t *request);

	/**
	 * 获得Object的元信息
	 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
	 * @return Object的元信息
	 * @retval 非空 表示成功
	 * @retval NULL 标识失败
	 */
	oss_object_metadata_t * (*get_object_metadata)(oss_initiate_multipart_upload_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_initiate_multipart_upload_request_t *request,
			const char *bucket_name);

	/**
	 * 设置Object名称
	 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
	 * @param key [in] Object名称
	 * @retval void
	 */
	void (*set_key)(oss_initiate_multipart_upload_request_t *request,
			const char *key);

	/**
	 * 设置Object的元信息
	 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
	 * @param object_metadata [in] Object的元信息
	 * @retval void
	 */
	void (*set_object_metadata)(oss_initiate_multipart_upload_request_t *request,
			oss_object_metadata_t *object_metadata);
};


/**
 * oss_initiate_multipart_upload_request_t构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] Object名称
 * @return 返回一个oss_initiate_multipart_upload_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_initiate_multipart_upload_request_t *
initiate_multipart_upload_request_initialize(const char *bucket_name,
		const char *key);

/**
 * oss_initiate_multipart_upload_request_t带元信息的构造函数
 * @param bucket_name [in] Bucket名称
 * @param key [in] Object名称
 * @param object_metadata [in] Object的元信息
 * @return 返回一个oss_initiate_multipart_upload_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_initiate_multipart_upload_request_t *
initiate_multipart_upload_request_initialize_with_metadata(const char *bucket_name,
		const char *key,
		oss_object_metadata_t *object_metadata);

/**
 * oss_initiate_multipart_upload_request_t析构函数
 * @param request [in] 标识一个oss_initiate_multipart_upload_request_t结构指针
 * @retval void
 * @pre request 必须使用initiate_multipart_upload_request_initialize的返回值
 */
extern void
initiate_multipart_upload_request_finalize(
		oss_initiate_multipart_upload_request_t *request);
/**@}*/
#endif // OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

