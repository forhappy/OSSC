/*
 * =============================================================================
 *
 *       Filename:  oss_abort_multipart_upload_request.h
 *
 *    Description:  abort multipart upload request structure.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/* 内部使用的头文件，用户开发应包含<ossc/client.h> */
#ifndef _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
# error Never include <ossc/modules/oss_abort_multipart_upload_request.h> directly
# error use <ossc/client.h> instead.
#endif

/* 头文件保护 */
#ifndef OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H 
#define OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * @defgroup oss_abort_multipart_upload_request_t oss_abort_multipart_upload_request_t
 * @{
 */

/** 
 *struct oss_abort_multipart_upload_request_s 内部使用 \n
 *oss_abort_multipart_upload_request_t 开发者使用
 */
typedef struct oss_abort_multipart_upload_request_s oss_abort_multipart_upload_request_t;


/**
 *oss_abort_multipart_upload_request_t 包含终止一个 Multipart 上传
 *事件的请求参数
 */
struct oss_abort_multipart_upload_request_s {
	char *bucket_name;       /**< Bucket 名称 */
	char *key;               /**< Object 名称 */
	char *upload_id;         /**< 标识 Multipart 上传事件的Upload ID */

	/**
	 * 返回 Bucket 名称
	 * @param request [in] 标识一个ss_abort_multipart_upload_request_t 指针
	 * @retval const char *
	 * @return Bucket 名称
	 */
	const char * (*get_bucket_name)(oss_abort_multipart_upload_request_t
			*request);

	/**
	 * 返回 Object 名称
	 * @param request [in] 标识一个oss_abort_multipart_upload_request_t 指针
	 * @retval const char *
	 * @return Object 名称
	 */
	const char * (*get_key)(oss_abort_multipart_upload_request_t
			*request);

	/**
	 * 返回 标识 Multipart 上传事件的Upload ID
	 * @param request [in] 标识一个oss_abort_multipart_upload_request_t 指针
	 * @retval const char *
	 * @return Upload ID
	 */
	const char * (*get_upload_id)(oss_abort_multipart_upload_request_t
			*request);
	/**
	 * 设置 Bucket 名称
	 * @param request [in] 标识一个oss_abort_multipart_upload_request_t 指针
	 * @param bucket_name [in] Bucket 名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_abort_multipart_upload_request_t *request,
			const char *bucket_name);

	/**
	 * 设置 Object 名称
	 * @param request [in] 标识一个oss_abort_multipart_upload_request_t 指针
	 * @param key [in] Object 名称
	 * @retval void
	 */
	void (*set_key)(oss_abort_multipart_upload_request_t *request,
			const char *key);

	/**
	 * 设置 标识 Multipart 上传事件的Upload ID
	 * @param request [in] 标识一个oss_abort_multipart_upload_request_t 指针
	 * @param upload_id [in] Upload ID 内容
	 * @retval void
	 */
	void (*set_upload_id)(oss_abort_multipart_upload_request_t *request,
			const char *upload_id);
};

/**
 * abort_multipart_upload_request_t 构造函数
 * @param [in] bucket_name Bucket名称
 * @param [in] key object名称
 * @param [in] upload_id 标识 Multipart 上传事件的Upload ID
 * @return 返回一个oss_abort_multipart_upload_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_abort_multipart_upload_request_t *
abort_multipart_upload_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id);

/**
 * abort_multipart_upload_request_t 析构函数
 * @param request [in] 标识一个oss_abort_multipart_upload_request_t结构指针
 * @retval void
 * @pre request 必须使用abort_multipart_upload_request_initialize的返回值
 */
extern void
abort_multipart_upload_request_finalize(
		oss_abort_multipart_upload_request_t *request);


/** @}*/


#endif // OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
