/*
 * =============================================================================
 *
 *       Filename:  oss_upload_part_result.h
 *
 *    Description:  oss_upload_part_result.
 *
 *        Company:  ICT ( Institute Of Computing Technuprogy, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_UPLOAD_PART_RESULT_H
# error Never include <ossc/modules/oss_upload_part_result.h> directly,
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_UPLOAD_PART_RESULT_H
#define OSS_UPLOAD_PART_RESULT_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_PART_ETAG_H
#include <ossc/modules/oss_part_etag.h>
#undef _OSS_PART_ETAG_H

/**
 * @addtogroup oss_upload_part_result_t oss_upload_part_result_t
 * @{
 */

/**
 * oss_upload_part_result_t 包含向OSS上传 Multipart 分块（Part）的结果信息
 */

typedef struct oss_upload_part_result_s oss_upload_part_result_t;

struct oss_upload_part_result_s {
	char *etag; /**< OSS生成的ETag值*/
	int part_number; /**< 包含Part标识号码和ETag值的PartETag对象*/

	/**
	 * 返回OSS生成的ETag值
	 * @param result [in] oss_upload_part_result_t 指针
	 * @retval const char *
	 * @return OSS生成的ETag值
	 */
	const char * (*get_etag)(oss_upload_part_result_t *result);

	/**
	 * 设置OSS生成的ETag值
	 * @param result oss_upload_part_result_t 指针
	 * @param etag 设置OSS生成的ETag值
	 * @retval void
	 */
	void (*set_etag)(oss_upload_part_result_t *result, const char *etag);

	/**
	 * 返回分块（Part）标识号
	 * @param result oss_upload_part_result_t 指针
	 * @return 分块（Part）标识号
	 * @retval int
	 */
	int(*get_part_number)(oss_upload_part_result_t *result);

	/**
	 * 设置分块（Part）标识号
	 * @param result oss_upload_part_result_t 指针
	 * @param part_number 分块号
	 * @retval void
	 */
	void (*set_part_number)(oss_upload_part_result_t *result, int part_number);

	/**
	 * 返回包含Part标识号码和ETag值的PartETag对象
	 * @param result oss_upload_part_result_t 指针
	 * @return 包含Part标识号码和ETag值的PartETag对象
	 * @retval oss_part_etag_t *
	 */
	oss_part_etag_t * (*get_part_etag)(oss_upload_part_result_t * result);
};

/**
 * oss_upload_part_result_t 构造函数
 * @return oss_upload_part_result_t *指针
 * @retval 非空 表示成功
 * @retal NULL 表示失败
 */
extern oss_upload_part_result_t *
upload_part_result_initialize();

/**
 * oss_upload_part_result_t 析构函数
 * @param result oss_upload_part_result_t *指针
 * 该指针需由upload_part_result_initialize返回
 */
extern void 
upload_part_result_finalize(oss_upload_part_result_t *result);

/** @}*/
#endif
