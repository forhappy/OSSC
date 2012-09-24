/*
 * =============================================================================
 *
 *       Filename:  oss_post_object_group_request.h
 *
 *    Description:  oss post object group request.
 *
 *        Created:  09/07/2012 03:22:02 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_POST_OBJECT_GROUP_REQUEST_H
# error Never include <ossc/modules/oss_post_object_group_request.h> directly,
# error use <ossc/client.h> instead.
#endif

#ifndef OSS_POST_OBJECT_GROUP_REQUEST_H 
#define OSS_POST_OBJECT_GROUP_REQUEST_H 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _OSS_OBJECT_GROUP_ITEM_H
#include "oss_object_group_item.h"
#undef _OSS_OBJECT_GROUP_ITEM_H 

/**
 * @defgroup oss_post_object_group_request_t oss_post_object_group_request_t
 * @{
 */

/**
 * struct oss_post_object_group_request_s 内部使用 \n
 * oss_post_object_group_request_t 开发者使用
 */
typedef struct oss_post_object_group_request_s oss_post_object_group_request_t;

/**
 * Object Group 是由一个或多个 Object 组成的松散的集合,它的数据长度是其
 * 所属 Object 的数据长度的总和。用户可以指定在同一个 Bucket 下的一个或多个Object,
 * 将其在 OSS 服务器上创建出一个 Object Group。一旦 Object Group 创建成功,
 * 用户可以像操作普通 Object 一样去操作 Object Group,这些操作包括 get,range query,
 * delete,list,但 Object Group 的数据和所包含 Object 的数据不是紧密联系的。
 * 通过 Object Group,用户可以实现 Object 的软连接,超大文件的多线程操作,以及断点续传等功能。
 *
 */

/**
 * Post Object Group 操作将根据用户提供的 Object 信息,在 OSS 服务器端创
 * 建出一个新的 Object Group。用户需要将创建该 Object Group 所需的 Object Name,
 * E-tag 以及标识该 Object 在整个 Group 中相对位置的 Part ID 按照规定的 XML 格式发给 OSS。
 *
 */
struct oss_post_object_group_request_s {
	char *bucket_name;      /**< Bucket 名称*/
	char *key;              /**< Oject 名称*/
	oss_object_group_item_t **items;  /**< 一个或多个 Object 组成的松散的集合*/
	size_t itemnums;        /**< 一个或多个 Object 组成的松散的集合的数目*/

	/**
	 * 获得 Bucket 名称
	 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
	 * @return Bucket 名称
	 * @retval const char *
	 */
	const char * (*get_bucket_name)(oss_post_object_group_request_t *request);

	/**
	 * 设置 Bucket 名称
	 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
	 * @param bucket_name [in] Bucket 名称
	 * @retval void
	 */
	void (*set_bucket_name)(oss_post_object_group_request_t *request,
			const char *bucket_name);

	/**
	 * 获得 Object 名称
	 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
	 * @return Object 名称
	 * @retval const char *
	 */
	const char * (*get_key)(oss_post_object_group_request_t *request);

	/**
	 * 设置Object 名称
	 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
	 * @param key [in] Object 名称
	 * @retval void
	 */
	void (*set_key)(oss_post_object_group_request_t *request,
			const char *key);

	/**
	 * 获得一个或多个 Object 组成的松散的集合
	 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
	 * @param itemnums [out]  Object 组成的松散的集合的个数
	 * @return 返回一个oss_object_group_item_t结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	oss_object_group_item_t ** (*get_items)(oss_post_object_group_request_t *request,
			int *itemnums);

	/**
	 * 设置一个或多个 Object 组成的松散的集合
	 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
	 * @param item  [in] 一个或多个 Object 组成的松散的集合
	 * @param itemnums [in] 一个或多个 Object 组成的松散的集合中条目的个数
	 */
	void (*set_items)(oss_post_object_group_request_t *request,
			oss_object_group_item_t **item, int itemnums);
	
};

/**
 * oss_post_object_group_request_t 构造函数
 * @return oss_post_object_group_request_t *指针
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 */
extern oss_post_object_group_request_t *
post_object_group_request_initialize(const char *bucket_name, const char *key,
		oss_object_group_item_t **items, int itemnums);

/**
 * oss_post_object_group_request_t 析构函数
 * @param request [in] 标识一个oss_post_object_group_request_t结构指针
 * @retval void
 * @pre request 必须使用post_object_group_request_initialize的返回值
 */
extern void post_object_group_request_finalize(oss_post_object_group_request_t *request);
/**@}*/
#endif // OSS_POST_OBJECT_GROUP_REQUEST_H

