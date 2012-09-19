/*
 * =============================================================================
 *
 *       Filename:  oss_part_etag.h
 *
 *    Description:  oss_part_etag.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PART_ETAG_H
# error Never include <ossc/modules/oss_part_etag.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PART_ETAG_H
#define OSS_PART_ETAG_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_part_etag_t oss_part_etag_t
 * @{
 */

/**
 * struct oss_part_etag_s 内部使用 \n
 * oss_part_etag_t 开发者使用
 */
typedef struct oss_part_etag_s oss_part_etag_t;

/**
 * 包含Multipart上传的Part的返回结果信息
 */
struct oss_part_etag_s {
	char *etag;             /**< Part的ETag值 */
	int part_number;        /**< Part标识号码 */

	/**
	 * 获得Part的ETag值
	 * @param etag [in] 标识一个oss_part_etag_t结构指针
	 * @return Part的ETag值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_part_etag_t *etag);

	/**
	 * 设置Part的ETag值
	 * @param pe [in] 标识一个oss_part_etag_t结构指针
	 * @param etag [in] Part的ETag值
	 * @retval void
	 */
	void (*set_etag)(oss_part_etag_t *pe, const char *etag);

	/**
	 * 获得Part标识号码
	 * @param etag [in] 标识一个oss_part_etag_t结构指针
	 * @return Part标识号码
	 * @retval int
	 */
	int (*get_part_number)(oss_part_etag_t *etag);

	/**
	 * 设置Part标识号码
	 * @param etag [in] 标识一个oss_part_etag_t结构指针
	 * @param part_number [in] Part标识号码
	 * @retval void
	 */
	void (*set_part_number)(oss_part_etag_t *etag, int part_number);
};

/**
 * oss_part_etag_t构造函数
 * @param part_number [in] Part标识号码
 * @param etag [in] Part的ETag值
 * @return 返回一个oss_part_etag_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_part_etag_t *
part_etag_initialize(int part_number, const char *etag);

/**
 * oss_part_etag_t析构函数
 * @param etag [in] 标识一个oss_part_etag_t结构指针
 * @retval void
 * @pre etag 必须使用part_etag_initialize的返回值
 */
extern void 
part_etag_finalize(oss_part_etag_t *etag);
/**@}*/
#endif
