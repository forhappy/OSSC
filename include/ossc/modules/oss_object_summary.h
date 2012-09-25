/*
 * =============================================================================
 *
 *       Filename:  oss_object_summary.h
 *
 *    Description:  oss_object_summary.
 *
 *        Created:  09/04/2012 08:57:48 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
#ifndef _OSS_OBJECT_SUMMARY_H
# error Never include <ossc/modules/oss_object_summary.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OBJECT_SUMMARY_H
#define OSS_OBJECT_SUMMARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include  <ossc/modules/oss_owner.h>
#undef _OSS_OWNER_H

/**
 * @defgroup oss_object_summary_t oss_object_summary_t
 * @{
 */

/**
 * struct oss_object_summary_s 内部使用 \n
 * oss_object_summary_t 开发者使用
 */
typedef struct oss_object_summary_s oss_object_summary_t;

/**
 * OSSObject摘要信息
 */
struct oss_object_summary_s {
	char *type;               /**< Object的类别 */
	char *etag;               /**< Object的etag */
	char *key;                /**< Object的名称 */
	char *last_modified;      /**< Object上次修改的时间 */
	oss_owner_t *owner;       /**< Object的所有者 */
	long size;                /**< Object的大小 */
	char *storage_class;      /**< Object的存储类别 */

	/**
	 * 获得Object的类别
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return Object的类别
	 * @retval const char *
	 */
	const char * (*get_type)(oss_object_summary_t *summary);

	/**
	 * 设置Object的类别
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param type [in] Object的类别
	 * @retval void
	 */
	void (*set_type)(oss_object_summary_t *summary, const char *type);

	/**
	 * 获得Object的etag
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return Object的etag
	 * @retval const char *
	 */
	const char * (*get_etag)(oss_object_summary_t *summary);

	/**
	 * 设置Object的etag
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param etag [in] Object的etag
	 * @retval void
	 */
	void (*set_etag)(oss_object_summary_t *summary, const char * etag);

	/**
	 * 获得Object的名称
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return Object的名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_object_summary_t *summary);

	/**
	 * 设置Object的名称
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param key [in] Object的名称
	 * @retval void
	 */
	void (*set_key)(oss_object_summary_t *summary, const char *key);

	/**
	 * 获得Object上次修改的时间
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return Object上次修改的时间
	 * @retval const char *
	 */
	const char * (*get_last_modified)(oss_object_summary_t *summary);

	/**
	 * 设置Object上次修改的时间
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param last_modified [in] bject上次修改的时间
	 * @retval void
	 */
	void (*set_last_modified)(oss_object_summary_t *summary, const char *last_modified);

	/**
	 * 获得Object的所有者
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return 返回一个oss_owner_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_owner_t * (*get_owner)(oss_object_summary_t *summary);

	/**
	 * 设置Object的所有者
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param owner [in] Object的所有者
	 * @retval void
	 */
	void (*set_owner)(oss_object_summary_t *summary, oss_owner_t *owner);
	
	/**
	 * 获得Object的大小
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return Object的大小
	 * @retval long
	 */
	long  (*get_size)(oss_object_summary_t *summary);

	/**
	 * 设置Object的大小
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param size [in] Object的大小
	 * @retval void
	 */
	void (*set_size)(oss_object_summary_t *summary, long size);

	/**
	 * 获得Object的存储类别
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @return Object的存储类别
	 * @retval const char *
	 */
	const char * (*get_storage_class)(oss_object_summary_t *summary);

	/**
	 * 设置Object的存储类别
	 * @param summary [in] 标识一个oss_object_summary_t结构指针
	 * @param storage_class [in] bject的存储类别
	 * @retval void
	 */
	void (*set_storage_class)(oss_object_summary_t *summary, const char *storage_class);
};

/**
 * oss_object_summary_t构造函数
 * @return 返回一个oss_object_summary_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_object_summary_t *
object_summary_initialize(void);

/**
 * oss_object_summary_t析构函数
 * @param summary [in] 标识一个oss_object_summary_t结构指针
 * @retval void
 * @pre summary 必须使用object_summary_initialize的返回值
 */
extern void 
object_summary_finalize(oss_object_summary_t *summary);
/**@}*/
#endif
