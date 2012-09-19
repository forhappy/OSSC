/*
 * =============================================================================
 *
 *       Filename:  oss_multipart_upload.h
 *
 *    Description:  oss_multipart_upload.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_MULTIPART_UPLOAD_H
# error Never include <ossc/modules/oss_multipart_upload.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_MULTIPART_UPLOAD_H
#define OSS_MULTIPART_UPLOAD_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
/**
 * @defgroup oss_multipart_upload_t oss_multipart_upload_t
 * @{
 */

/**
 * struct oss_multipart_upload_s 内部使用 \n
 * oss_multipart_upload_t 开发者使用
 */
typedef struct oss_multipart_upload_s oss_multipart_upload_t;

/**
 * multipart upload的信息
 */
struct oss_multipart_upload_s {
	char *initiated;           /**< 初始化的时间 */
	char *storage_class;       /**< Object的类型 */
	char *upload_id;           /**< Upload ID */
	char *key;                 /**< Object名称 */

	/**
	 * 获得初始化的时间
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @return 初始化的时间
	 * @retval const char *
	 */
	const char * (*get_initiated)(oss_multipart_upload_t *uploads);

	/**
	 * 设置初始化的时间
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @param initiated [in] 初始化的时间
	 * @retval void
	 */
	void (*set_initiated)(oss_multipart_upload_t *uploads, const char *initiated);

	/**
	 * 获得Object的类型
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @return Object的类型
	 * @retval const char *
	 */
	const char * (*get_storage_class)(oss_multipart_upload_t *uploads);

	/**
	 * 设置Object的类型
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @param storage_class [in] Object的类型
	 * @retval void
	 */
	void (*set_storage_class)(oss_multipart_upload_t *uploads, const char *storage_class);

	/**
	 * 获得Upload ID
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @return Upload ID
	 * @retval const char *
	 */
	const char * (*get_upload_id)(oss_multipart_upload_t *uploads);

	/**
	 * 设置Upload ID
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @param upload_id [in]
	 * @retval void
	 */
	void (*set_upload_id)(oss_multipart_upload_t *uploads, const char *upload_id);

	/**
	 * 获得Object名称
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @return Object名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_multipart_upload_t *uploads);

	/**
	 * 设置Object名称
	 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
	 * @param key Object名称
	 * @retval void
	 */
	void (*set_key)(oss_multipart_upload_t *uploads, const char *key);
};

/**
 * oss_multipart_upload_t构造函数
 * @return 返回一个oss_multipart_upload_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_multipart_upload_t *
multipart_upload_initialize(void);

/**
 * oss_multipart_upload_t析构函数
 * @param uploads [in] 标识一个oss_multipart_upload_t结构指针
 * @retval void
 * @pre uploads 必须使用multipart_upload_initialize的返回值
 */
extern void 
multipart_upload_finalize(oss_multipart_upload_t *uploads);
/**@}*/
#endif
