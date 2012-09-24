/*
 * =============================================================================
 *
 *       Filename:  oss_upload_part_request.h
 *
 *    Description:  oss_upload_part_request.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_UPLOAD_PART_REQUEST_H
# error Never include <ossc/modules/oss_upload_part_request.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_UPLOAD_PART_REQUEST_H
#define OSS_UPLOAD_PART_REQUEST_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_upload_part_request_t oss_upload_part_request_t
 * @{
 */

/**
 * struct oss_upload_part_request_s 内部使用 \n
 * oss_upload_part_request_t 开发者使用
 */
typedef struct oss_upload_part_request_s oss_upload_part_request_t;

/**
 * 包含上传Multipart分块（Part）参数
 */
struct oss_upload_part_request_s {

	char *bucket_name; /**< Bucket名称*/
	char *input_stream; /**< 包含上传分块内容的数据流*/
	int input_stream_len; /**< 包含上传分块内容的数据流的长度*/
	char *key; /**< OSSObject key*/
	char *md5_digest; /**< 分块（Part）数据的MD5校验值*/
	int part_number; /**< 上传分块（Part）的标识号码（Part Number）*/
	long part_size; /**< 分块（Part）数据的字节数*/
	char *upload_id; /**< 标识Multipart上传事件的Upload ID*/

	/**
	 * 获得Bucket名称
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @return 返回Bucket名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_upload_part_request_t *request);

	/**
	 * 设置Bucket名称
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param bucket_name [in] Bucket名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_upload_part_request_t *request,
			const char *bucket_name);

	/**
	 * 获得OSSObject key
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @return OSSObject key
	 * @retval const char *
	 */
	const char * (*get_key)(oss_upload_part_request_t *request);

	/**
	 * 设置OSSObject key
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param key [in] OSSObject key
	 * @retval void
	 */
	void (*set_key)(oss_upload_part_request_t *request, const char *key);

	/**
	 * 获得分块（Part）数据的MD5校验值
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @return  分块（Part）数据的MD5校验值
	 * @retval const char *
	 */
	const char * (*get_md5_digest)(oss_upload_part_request_t *request);

	/**
	 * 设置分块（Part）数据的MD5校验值
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param md5_digest [in] 分块（Part）数据的MD5校验值
	 * @retval void
	 */
	void (*set_md5_digest)(oss_upload_part_request_t *request, const char *md5_digest);

	/**
	 * 获得包含上传分块内容的数据流
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param input_stream_len [out] 上传分块内容的数据流的长度
	 * @return 包含上传分块内容的数据流
	 * @retval const char *
	 */
	const char * (*get_input_stream)(oss_upload_part_request_t *request, int *input_stream_len);

	/**
	 * 设置包含上传分块内容的数据流
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param input_stream [in] 包含上传分块内容的数据流
	 * @param input_stream_len [in] 包含上传分块内容的数据流的长度
	 * @retval void
	 */
	void (*set_input_stream)(oss_upload_part_request_t *request,
			const char *input_stream, size_t input_stream_len);

	/**
	 * 获得标识Multipart上传事件的Upload ID
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @return 标识Multipart上传事件的Upload ID
	 * @retval const char *
	 */
	const char * (*get_upload_id)(oss_upload_part_request_t *request);

	/**
	 * 设置标识Multipart上传事件的Upload ID
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param upload_id [in] 标识Multipart上传事件的Upload ID
	 * @retval void
	 */
	void (*set_upload_id)(oss_upload_part_request_t *request, const char *upload_id);

	/**
	 * 获得上传分块（Part）的标识号码（Part Number）
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @return 上传分块（Part）的标识号码（Part Number）
	 * @retval const char *
	 */
	int (*get_part_number)(oss_upload_part_request_t *request);

	/**
	 * 设置上传分块（Part）的标识号码（Part Number）
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param part_number [in] 上传分块（Part）的标识号码（Part Number）
	 * @retval void
	 */
	void (*set_part_number)(oss_upload_part_request_t *request, int part_number);

	/**
	 * 返回分块（Part）数据的字节数
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @return 分块（Part）数据的字节数
	 * @retval long
	 */
	long (*get_part_size)(oss_upload_part_request_t *request);

	/**
	 * 设置返回分块（Part）数据的字节数
	 * @param request [in] 标识一个oss_upload_part_request_t结构指针
	 * @param part_size [in] 分块（Part）数据的字节数
	 * @retval void
	 */
	void (*set_part_size)(oss_upload_part_request_t *request, long part_size);
};


/**
 * oss_upload_part_request_t 构造函数
 * @return 返回一个oss_upload_part_request_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_upload_part_request_t *
upload_part_request_initialize(void);

/**
 * oss_upload_part_request_t 析构函数
 * @param request [in] 标识一个oss_upload_part_request_t结构指针
 * @retval void
 * @pre request 必须使用upload_part_request_initialize的返回值
 */
extern void 
upload_part_request_finalize(oss_upload_part_request_t *request);

/**@}*/
#endif
