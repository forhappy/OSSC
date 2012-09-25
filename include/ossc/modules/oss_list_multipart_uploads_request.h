/*
 * =============================================================================
 *
 *       Filename:  oss_list_multipart_uploads_request.h
 *
 *    Description:  list multipart uploads request implementation.
 *
 *        Created:  09/05/2012 11:31:02 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
# error Never include <ossc/modules/oss_list_multipart_uploads_request_h> directly
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#define OSS_LIST_MULTIPART_UPLOADS_REQUEST_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @defgroup oss_list_multipart_uploads_request_t oss_list_multipart_uploads_request_t
 * @{
 */

/**
 * struct oss_list_multipart_uploads_request_s 内部使用 \n
 * oss_list_multipart_uploads_request_t 开发者使用
 */
typedef struct oss_list_multipart_uploads_request_s oss_list_multipart_uploads_request_t;

/**
 * 包含列出所有执行中Multipart上传事件的请求参数
 */
struct oss_list_multipart_uploads_request_s {
	char *bucket_name;            /**< Bucket名称 */
	char *delimiter;              /**< 分隔符 */
	char *key_marker;             /**< 标识表示从哪里返回列表 */
	unsigned int max_uploads;     /**< 限制的最大返回记录数 */
	char *prefix;                 /**< 前缀 */
	char *upload_id_marker;       /**< 标识表示从哪里返回列表 */

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @return Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_list_multipart_uploads_request_t *request);

	/**
	 * 获得分隔符
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @return delimiter
	 * @retval const char *
	 */
	const char * (*get_delimiter)(oss_list_multipart_uploads_request_t *request);

	/**
	 * 获得key_marker
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @return key_marker
	 * @retval const char *
	 */
	const char * (*get_key_marker)(oss_list_multipart_uploads_request_t *request);

	/**
	 * 获得最大返回记录数
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @return 最大返回记录数
	 * @retval unsigned int
	 */
	unsigned int (*get_max_uploads)(oss_list_multipart_uploads_request_t *request);

	/**
	 * 获得前缀
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @return 前缀
	 * @retval const char *
	 */
	const char * (*get_prefix)(oss_list_multipart_uploads_request_t *request);

	/**
	 * 获得upload_id_marker
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @return upload_id_marker
	 * @retval const char *
	 */
	const char * (*get_upload_id_marker)(oss_list_multipart_uploads_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_list_multipart_uploads_request_t *request,
			const char *bucket_name);

	/**
	 * 设置分隔符
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @param delimiter [in] 分隔符
	 * @retval void
	 */
	void (*set_delimiter)(oss_list_multipart_uploads_request_t *request,
			const char *delimiter);

	/**
	 * 设置key_marker
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @param key_marker [in] 标识表示从哪里返回列表
	 * @retval void
	 */
	void (*set_key_marker)(oss_list_multipart_uploads_request_t *request,
			const char *key_marker);

	/**
	 * 设置最大返回记录数
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @param max_uploads [in] 最大返回记录数
	 * @retval void
	 */
	void (*set_max_uploads)(oss_list_multipart_uploads_request_t *request,
			unsigned int max_uploads);

	/**
	 * 设置前缀
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @param prefix [in] 前缀
	 * @retval void
	 */
	void (*set_prefix)(oss_list_multipart_uploads_request_t *request,
			const char *prefix);

	/**
	 * 设置 upload_id_marker
	 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
	 * @param upload_id_marker [in] 标识表示从哪里返回列表
	 * @retval void
	 */
	void (*set_upload_id_marker)(oss_list_multipart_uploads_request_t *request,
			const char *upload_id_marker);
};

/**
 * oss_list_multipart_uploads_request_t构造函数
 * @param bucket_name [in] Bucket名称
 * @return 返回一个oss_list_multipart_uploads_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_list_multipart_uploads_request_t *
list_multipart_uploads_request_initialize(const char *bucket_name);

/**
 * oss_list_multipart_uploads_request_t析构函数
 * @param request [in] 标识一个oss_list_multipart_uploads_request_t结构指针
 * @retval void
 * @pre request 必须使用list_multipart_uploads_request_initialize的返回值
 */
extern void
list_multipart_uploads_request_finalize(
		oss_list_multipart_uploads_request_t *request);
/**@}*/

#endif // OSS_LIST_MULTIPART_UPLOADS_REQUEST_H 


