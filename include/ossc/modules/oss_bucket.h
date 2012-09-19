/*
 * =============================================================================
 *
 *       Filename:  oss_bucket.h
 *
 *    Description:  oss_bucket.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_BUCKET_H
# error Never include <ossc/modules/oss_bucket.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_BUCKET_H
#define OSS_BUCKET_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include  "ossc/modules/oss_owner.h"
#undef _OSS_OWNER_H

/**
 * @defgroup oss_bucket_t oss_bucket_t
 * @{
 */

/**
 * struct oss_bucket_s 内部使用 \n
 * oss_bucket_t 开发者使用
 */
typedef struct oss_bucket_s oss_bucket_t;

/**
 * Bucket是OSS上的命名空间 \n
 * Bucket名在整个 OSS 中具有全局唯一性,且不能修改；
 * 存储在OSS上的每个Object必须都包含在某个Bucket中。
 * 一个应用，例如图片分享网站，可以对应一个或多个 Bucket。
 * 一个用户最多可创建 10 个Bucket, 但每个Bucket中存放的Object的数量和大小总和没有限制,
 * 用户不需要考虑数据的可扩展性。\n
 * Bucket 命名规范
 * - 只能包括小写字母，数字，下划线（_）和短横线（-）
 * - 必须以小写字母或者数字开头
 * - 长度必须在 3-255 字节之间
 */
struct oss_bucket_s {
	char *create_date;  /**< Bucket的创建时间 */
	char *name;         /**< Bucket的名称 */
	oss_owner_t *owner; /**< Bucket的所有者 */

	/**
	 * 返回Bucket的创建时间
	 * @param bucket [in] oss_bucket_t 指针
	 * @retval const char *
	 * @return Bucket的创建时间
	 */
	const char * (*get_create_date)(oss_bucket_t *bucket);

	/**
	 * 设置Bucket的创建时间
	 * @param bucket [in] oss_bucket_t 指针
	 * @param create_date [in] Bucket的创建时间
	 * @retval void
	 */
	void (*set_create_date)(oss_bucket_t *bucket, const char *create_date);

	/**
	 * 返回Bucket的名称
	 * @param bucket [in] oss_bucket_t 指针
	 * @retval const char *
	 * @return Bucket的名称
	 */
	const char * (*get_name)(oss_bucket_t *bucket);

	/**
	 * 设置Bucket的名称
	 * @param bucket [in] oss_bucket_t 指针
	 * @param name [in] Bucket的名称
	 * @retval void
	 */
	void (*set_name)(oss_bucket_t *bucket, const char * name);

	/**
	 * 返回Bucket的所有者
	 * @param bucket [in] oss_bucket_t 指针
	 * @retval oss_owner_t *
	 * @return Bucket的所有者
	 */
	oss_owner_t * (*get_owner)(oss_bucket_t *bucket);

	/**
	 * 设置Bucket的所有者
	 * @param bucket [in] oss_bucket_t 指针
	 * @param owner [in] Bucket的所有者
	 * @retval void
	 */
	void (*set_owner)(oss_bucket_t *bucket, oss_owner_t *owner);
};

/**
 * oss_bucket_t 构造函数
 * @return 返回一个标识Bucket的结构指针
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_bucket_t *
bucket_initialize(void);

/**
 * oss_bucket_t 带bucket_name参数的构造函数
 * @return 返回一个标识Bucket的结构指针
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 */
extern oss_bucket_t *
bucket_initialize_with_name(const char *name);

/**
 * oss_bucket_t 析构函数
 * @param bucket [in] 标识一个ACL的结构指针
 * @retval void
 * @pre bucket 必须使用bucket_initialize的返回值
 */
extern void 
bucket_finalize(oss_bucket_t *bucket);

/**@}*/

#endif
