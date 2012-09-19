/*
 * =============================================================================
 *
 *       Filename:  oss_object.h
 *
 *    Description:  oss object
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef _OSS_OBJECT_H
# error Never include <ossc/modules/oss_object.h> directly, use <ossc/client.h> instead.
#endif
#ifndef OSS_OBJECT_H
#define OSS_OBJECT_H
#include "ossc-config.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_OBJECT_METADATA_H
#include <ossc/modules/oss_object_metadata.h>
#undef _OSS_OBJECT_METADATA_H

/**
 * @defgroup oss_object_t oss_object_t
 * @{
 */

/**
 * struct oss_object_s 内部使用 \n
 * oss_object_t 开发者使用
 */
typedef struct oss_object_s oss_object_t;

/**
 * 表示OSS中的Object\n
 *
 * 在 OSS 中，用户的每个文件都是一个 Object，每个文件需小于 5G。 Object包含key、data和user meta。其中，key是Object 的名字； data是Object 的数据；user meta是用户对该object的描述。
 *
 * Object 命名规范:
 *
 * - 使用UTF-8编码
 * - 规则长度必须在 1-1023字节之间
 */
struct oss_object_s {
	char *bucket_name;                       /**< Object的所在Bucket的名称 */
	char *key;                               /**< Object的名称 */
	char *object_content;                    /**< Object的内容 */
	oss_object_metadata_t *object_metadata;  /**< Object的元数据 */
	size_t object_content_len;               /**< Object的内容长度 */

	/**
	 * 获得Object的所在Bucket的名称
	 * @param object [in]  标识一个oss_object_t结构指针
	 * @return Object的所在Bucket的名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_object_t *object);

	/**
	 * 获得Object的名称
	 * @param object [in]  标识一个oss_object_t结构指针
	 * @return Object的名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_object_t *object);

	/**
	 * 获得Object的内容
	 * @param object [in]  标识一个oss_object_t结构指针
	 * @param object_content_len [out] Object的内容长度
	 * @return Object的内容
	 * @retval const char *
	 */
	const char * (*get_object_content)(oss_object_t *object, size_t *object_content_len);

	/**
	 * 获得Object的元数据
	 * @param object [in]  标识一个oss_object_t结构指针
	 * @return 返回一个oss_object_metadata_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_object_metadata_t * (*get_object_metadata)(oss_object_t *object);

	/**
	 * 设置Object的所在Bucket的名称
	 * @param object [in] 标识一个oss_object_t结构指针
	 * @param bucket_name [in] Object的所在Bucket的名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_object_t *object, const char *bucket_name);

	/**
	 * 设置Object的名称
	 * @param object [in] 标识一个oss_object_t结构指针
	 * @param key [in] Object的名称
	 * @retval void
	 */
	void (*set_key)(oss_object_t *object, const char *key);

	/**
	 * 设置Object的内容
	 * @param object [in] 标识一个oss_object_t结构指针
	 * @param object_content [in] Object的内容
	 * @param object_content_len [in] Object的内容长度
	 * @retval void
	 */
	void (*set_object_content)(oss_object_t *object,
			const char *object_content,
			size_t object_content_len);

	/**
	 * 设置Object的元数据
	 * @param object [in] 标识一个oss_object_t结构指针
	 * @param object_metadata [in] Object的元数据
	 * @retval void
	 */
	void (*set_object_metadata)(oss_object_t *object,
			oss_object_metadata_t *object_metadata);
};

/**
 * oss_object_t构造函数
 * @return 返回一个oss_object_t结构指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_object_t * object_initialize();

/**
 * oss_object_t析构函数
 * @param object [in] 标识一个oss_object_t结构指针
 * @retval void
 * @pre object 必须使用object_initialize的返回值
 */
extern void object_finalize(oss_object_t *object);
/**@}*/
#endif // OSS_OBJECT_H

