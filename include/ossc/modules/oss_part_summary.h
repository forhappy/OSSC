/*
 * =============================================================================
 *
 *       Filename:  oss_part_summary.h
 *
 *    Description:  oss_part_summary.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_PART_SUMMARY_H
# error Never include <ossc/modules/oss_part_summary.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_PART_SUMMARY_H
#define OSS_PART_SUMMARY_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @defgroup oss_part_summary_t oss_part_summary_t
 * @{
 */

/**
 * struct oss_part_summary_s 内部使用 \n
 * oss_part_summary_t 开发者使用
 */
typedef struct oss_part_summary_s oss_part_summary_t;

/**
 * 包含通过Multipart上传模式上传的Part的摘要信息
 */
struct oss_part_summary_s {
	char *etag;                   /**< Part的etag值 */
	char *last_modified;          /**< Part上次修改的时间 */
	int part_number;              /**< Part的序号number */
	long size;                    /**< Part的大小 */

	/**
	 * 获得Part的etag值
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @return Part的etag值
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_part_summary_t *summary);

	/**
	 * 设置Part的etag值
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @param etag [in] Part的etag值
	 * @retval void
	 */
	void (*set_etag)(oss_part_summary_t *summary, const char *etag);

	/**
	 * 获得Part上次修改的时间
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @return Part上次修改的时间
	 * @retval const char *
	 */
	const char * (*get_last_modified)(oss_part_summary_t *summary);

	/**
	 * 设置Part上次修改的时间
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @param lastmodified [in] Part上次修改的时间
	 * @retval void
	 */
	void (*set_last_modified)(oss_part_summary_t *summary, const char *lastmodified);

	/**
	 * 获得Part的序号number
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @return Part的序号number
	 * @retval int
	 */
	int (*get_part_number)(oss_part_summary_t *summary);

	/**
	 * 设置Part的序号number
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @param part_number [in] Part的序号number
	 * @retval void
	 */
	void (*set_part_number)(oss_part_summary_t *summary, int part_number);

	/**
	 * 获得Part的大小
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @return Part的大小
	 * @retval long
	 */
	long (*get_size)(oss_part_summary_t *summary);

	/**
	 * 设置Part的大小
	 * @param summary [in] 标识一个oss_part_summary_t结构指针
	 * @param size [in] Part的大小
	 * @retval void
	 */
	void (*set_size)(oss_part_summary_t *summary, long size);
};

/**
 * oss_part_summary_t构造函数
 * @return 返回一个oss_part_summary_t 结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_part_summary_t *
part_summary_initialize(void);

/**
 * oss_part_summary_t析构函数
 * @param summary [in] 标识一个oss_part_summary_t结构指针
 * @retval void
 * @pre summary 必须使用part_summary_initialize的返回值
 */
extern void 
part_summary_finalize(oss_part_summary_t *summary);
/**@}*/
#endif
