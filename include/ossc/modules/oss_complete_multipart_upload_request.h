/*
 * =============================================================================
 *
 *       Filename:  oss_complete_multipart_upload_request.h
 *
 *    Description:  oss_complete_multipart_upload_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
# error Never include <ossc/modules/oss_complete_multipart_upload_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H
#define OSS_COMPLETE_MULTIPART_UPLOAD_REQUEST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_PART_ETAG_H
#include <ossc/modules/oss_part_etag.h>
#undef _OSS_PART_ETAG_H
/**
 * @defgroup oss_complete_multipart_upload_request_t oss_complete_multipart_upload_request_t
 * @{
 */

/**
 * struct oss_complete_multipart_upload_request_s 内部使用 \n
 * oss_complete_multipart_upload_request_t 开发者使用
 */
typedef struct oss_complete_multipart_upload_request_s oss_complete_multipart_upload_request_t;

/**
 * 包含完成一个Multipart上传事件的请求参数
 */
struct oss_complete_multipart_upload_request_s {
	char *bucket_name;      /**< Bucket 名称 */
	char *key;              /**< Object 名称 */
	oss_part_etag_t **part_etags;     /**< 标识上传Part结果的PartETag列表 */
	char *upload_id;         /** 标识Multipart上传事件的Upload ID */
	int part_etags_number;    /** part_etags* 的个数 */

	/**
	 * 获得Bucket的名称
	 * @param [in] request 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @return Bucket的名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_complete_multipart_upload_request_t *request);

	/**
	 * 设置Bucket的名称
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @param bucket_name [in] Bucket的名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_complete_multipart_upload_request_t *request, const char *bucket_name);

	/**
	 * 获得Object的名称
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @return Object的名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_complete_multipart_upload_request_t *request);

	/**
	 * 设置Object的名称
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @param key [in] Object的名称
	 * @retval void
	 */
	void (*set_key)(oss_complete_multipart_upload_request_t *request, const char *key);

	/**
	 * 获得PartETag列表内容
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @param part_etags_number [in] part_etags* 的个数
	 * @return 返回一个oss_part_etag_t的双重指针，可通过part_etags_number来遍历
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_part_etag_t ** (*get_part_etags)(oss_complete_multipart_upload_request_t *request, int *part_etags_number);

	/**
	 * 设置PartETag列表内容
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @param part_etags [in] 标识一个oss_part_etag_t的双重指针
	 * @param part_etags_number [in] part_etags* 的个数
	 * retval void
	 */
	void (*set_part_etags)(oss_complete_multipart_upload_request_t *request, oss_part_etag_t **part_etags, int part_etags_number);

	/**
	 * 获得Upload ID的内容
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @return Upload ID的内容
	 * @retval const char *
	 */
	const char * (*get_upload_id)(oss_complete_multipart_upload_request_t *request);

	/**
	 * 设置Upload ID的内容
	 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
	 * @param upload_id [in] Upload ID的内容
	 * @retval void
	 */
	void (*set_upload_id)(oss_complete_multipart_upload_request_t *request, const char *upload_id);
};


/**
 * oss_complete_multipart_upload_request_t 构造函数
 * @param bucket_name [in] Bucket的名称
 * @param key [in] Object的名称
 * @param upload_id [in] Upload ID的内容
 * @param part_etags [in] 标识一个oss_part_etag_t的双重指针
 * @param part_etags_number [in] part_etags* 的个数
 * @return 返回一个oss_complete_multipart_upload_request_t的结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_complete_multipart_upload_request_t *
complete_multipart_upload_request_initialize(const char *bucket_name,
		const char *key,
		const char *upload_id,
		oss_part_etag_t **part_etags,
		int part_etags_number);

/**
 * oss_complete_multipart_upload_request_t 析构函数
 * @param request [in] 标识一个oss_complete_multipart_upload_request_t的结构指针
 * @retval void
 * @pre request 必须使用complete_multipart_upload_request_initialize的返回值
 */
extern void 
complete_multipart_upload_request_finalize(oss_complete_multipart_upload_request_t *request);
/**@}*/
#endif
